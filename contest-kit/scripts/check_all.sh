#!/usr/bin/env bash
# Проверяет, что КАЖДЫЙ .cpp в my/ компилируется. Запускать из корня репо:
#   ./scripts/check_all.sh
# Ничего не запускает — только компиляция (-fsyntax-only), поэтому быстро.
CXX="${CXX:-g++}"
ok=0; bad=0
while IFS= read -r f; do
  if err=$("$CXX" -std=gnu++20 -fsyntax-only "$f" 2>&1); then
    ok=$((ok + 1))
  else
    bad=$((bad + 1)); echo "FAIL: $f"; echo "$err" | head -5; echo
  fi
done < <(find my -name '*.cpp' | sort)
echo "компилируется: $ok, сломано: $bad"
[ "$bad" -eq 0 ]
