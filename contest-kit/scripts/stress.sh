#!/usr/bin/env bash
# Стресс-тест: решение vs наивное решение на случайных тестах.
#   ./scripts/stress.sh sol.cpp brute.cpp gen.cpp [кол-во тестов, по умолч. 1000]
# gen получает seed первым аргументом:  ./gen 17 > in   (образец: scripts/gen.cpp)
# С санитайзерами (медленнее, но ловит RE/UB):  SAN=1 ./scripts/stress.sh ...
# При расхождении печатает тест и ответы, тест сохраняется в failed_input.txt.
# Пробелы и переводы строк при сравнении не важны.
# Если у задачи несколько правильных ответов — diff не подходит, нужен чекер.
set -e
sol="$1"; brute="$2"; gen="$3"; n="${4:-1000}"
[ -z "$gen" ] && { echo "использование: $0 sol.cpp brute.cpp gen.cpp [N]"; exit 1; }
CXX="${CXX:-g++}"
FLAGS="-std=gnu++20 -O2"
[ "$SAN" = 1 ] && FLAGS="-std=gnu++20 -O1 -g -fsanitize=address,undefined -fno-sanitize-recover=all -D_GLIBCXX_DEBUG"
d="$(mktemp -d)"
echo "компилирую..."
$CXX $FLAGS "$sol"   -o "$d/sol"
$CXX $FLAGS "$brute" -o "$d/brute"
$CXX -std=gnu++20 -O2 "$gen" -o "$d/gen"
for ((i = 1; i <= n; i++)); do
  "$d/gen" "$i" > "$d/in"
  if ! "$d/sol" < "$d/in" > "$d/out1" 2> "$d/err"; then
    echo "RE в sol на seed $i"; echo "--- вход:"; cat "$d/in"; echo "--- stderr:"; head -30 "$d/err"
    cp "$d/in" ./failed_input.txt; exit 1
  fi
  "$d/brute" < "$d/in" > "$d/out2"
  if ! diff -q <(tr -s '[:space:]' '\n' < "$d/out1" | sed '/^$/d') \
               <(tr -s '[:space:]' '\n' < "$d/out2" | sed '/^$/d') > /dev/null; then
    echo "WA на seed $i"
    echo "--- вход:";  cat "$d/in"
    echo "--- sol:";   cat "$d/out1"
    echo "--- brute:"; cat "$d/out2"
    cp "$d/in" ./failed_input.txt; exit 1
  fi
  (( i % 100 == 0 )) && echo "  $i ок"
done
echo "OK: $n тестов совпали"
