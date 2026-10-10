# Общие настройки компиляторов. Подключается из run.sh / stress.sh / check_all.sh,
# сам по себе не запускается. Работает и на Linux, и на macOS (bash 3.2).
#
# Linux: всё через g++.
# macOS: "g++" там на самом деле Apple clang — у него нет bits/stdc++.h и pb_ds.
#   - санитайзеры (run.sh): Apple clang + свой bits/stdc++.h из scripts/include;
#     если clang не собрал (pb_ds, __gcd...) — GCC из Homebrew с _GLIBCXX_DEBUG.
#   - обычная сборка (stress.sh, check_all.sh): GCC из Homebrew (как в тестирующей
#     системе), если не собрал или его нет — Apple clang.
#   GCC из Homebrew ставится так: brew install gcc   (появится g++-15 / g++-16 ...)
#
# Свой компилятор принудительно: CXX=g++-16 ./scripts/run.sh a.cpp

SCRIPTS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_DIR="$(cd "$SCRIPTS_DIR/.." && pwd)"
# чтобы локально собиралось решение с #include "graph/LCA.h" (для отправки — bundle.sh)
KACTL_DIR="$REPO_DIR/ext/kactl/content"
# в олимпиадном коде память обычно не освобождают — отчёты об утечках не нужны
export ASAN_OPTIONS="detect_leaks=0${ASAN_OPTIONS:+:$ASAN_OPTIONS}"
SAN_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all"
WARN_FLAGS="-Wall -Wextra -Wshadow"

if [ "$(uname)" = Darwin ]; then
  IS_MAC=1
  CLANG="${CLANG:-clang++}"
  if [ -n "$CXX" ]; then
    GCC="$CXX"
  else
    GCC="$(ls /opt/homebrew/bin/g++-[0-9]* /usr/local/bin/g++-[0-9]* 2>/dev/null | sort -t- -k2 -n | tail -1)"
  fi
else
  IS_MAC=0
  GCC="${CXX:-g++}"
fi

# compile_dbg SRC OUT — сборка для отладки (санитайзеры + проверки STL)
compile_dbg() {
  local src="$1" out="$2"
  if [ "$IS_MAC" = 0 ]; then
    "$GCC" -std=gnu++20 -O1 -g -DLOCAL $WARN_FLAGS $SAN_FLAGS \
      -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -I "$KACTL_DIR" -I "$REPO_DIR" "$src" -o "$out"
    return
  fi
  # macOS: сначала Apple clang с санитайзерами и проверками libc++
  if "$CLANG" -std=gnu++20 -O1 -g -DLOCAL $WARN_FLAGS $SAN_FLAGS \
       -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_DEBUG \
       -I "$SCRIPTS_DIR/include" -I "$KACTL_DIR" -I "$REPO_DIR" "$src" -o "$out" 2> "$out.log" ||
     "$CLANG" -std=gnu++20 -O1 -g -DLOCAL $WARN_FLAGS $SAN_FLAGS \
       -I "$SCRIPTS_DIR/include" -I "$KACTL_DIR" -I "$REPO_DIR" "$src" -o "$out" 2> "$out.log"; then
    cat "$out.log" >&2
    return 0
  fi
  if [ -n "$GCC" ]; then
    echo "[clang не собрал — собираю через $GCC: без санитайзеров, но с _GLIBCXX_DEBUG]" >&2
    "$GCC" -std=gnu++20 -O1 -g -DLOCAL $WARN_FLAGS \
      -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -I "$KACTL_DIR" -I "$REPO_DIR" "$src" -o "$out"
    return
  fi
  cat "$out.log" >&2
  return 1
}

# compile_fast SRC OUT — обычная сборка -O2
compile_fast() {
  local src="$1" out="$2"
  if [ -n "$GCC" ] && "$GCC" -std=gnu++20 -O2 -I "$KACTL_DIR" -I "$REPO_DIR" "$src" -o "$out" 2> "$out.log"; then
    return 0
  fi
  if [ "$IS_MAC" = 1 ] &&
     "$CLANG" -std=gnu++20 -O2 -I "$SCRIPTS_DIR/include" -I "$KACTL_DIR" -I "$REPO_DIR" "$src" -o "$out" 2> "$out.log"; then
    return 0
  fi
  cat "$out.log" >&2
  return 1
}

# compile_check SRC — только проверка, что компилируется (без сборки)
compile_check() {
  local src="$1"
  if [ -n "$GCC" ] && "$GCC" -std=gnu++20 -fsyntax-only -I "$KACTL_DIR" -I "$REPO_DIR" "$src" 2> /dev/null; then
    return 0
  fi
  if [ "$IS_MAC" = 1 ] &&
     "$CLANG" -std=gnu++20 -fsyntax-only -I "$SCRIPTS_DIR/include" -I "$KACTL_DIR" -I "$REPO_DIR" "$src" 2> /dev/null; then
    return 0
  fi
  # повторить с выводом ошибки
  if [ -n "$GCC" ]; then "$GCC" -std=gnu++20 -fsyntax-only -I "$KACTL_DIR" -I "$REPO_DIR" "$src"
  else "$CLANG" -std=gnu++20 -fsyntax-only -I "$SCRIPTS_DIR/include" -I "$KACTL_DIR" -I "$REPO_DIR" "$src"; fi
  return 1
}
