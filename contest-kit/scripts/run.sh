#!/usr/bin/env bash
# Компиляция с санитайзерами + отладочным STL и запуск.
#   ./scripts/run.sh a.cpp            — ввод с клавиатуры
#   ./scripts/run.sh a.cpp in.txt     — ввод из файла
# Ловит: выход за границы vector/массива, переполнение int (signed),
# use-after-free, обращение к пустому set/map и т.п.
# Работает медленнее обычного — для отладки, не для больших тестов.
# Другой компилятор: CXX=g++-14 ./scripts/run.sh a.cpp
set -e
src="$1"; inp="$2"
[ -z "$src" ] && { echo "использование: $0 файл.cpp [вход.txt]"; exit 1; }
CXX="${CXX:-g++}"
out="${TMPDIR:-/tmp}/$(basename "${src%.*}")_dbg"
"$CXX" -std=gnu++20 -O1 -g -DLOCAL \
  -Wall -Wextra -Wshadow \
  -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all \
  -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC \
  "$src" -o "$out"
if [ -n "$inp" ]; then "$out" < "$inp"; else "$out"; fi
