#!/usr/bin/env bash
# Проверка, что скрипты работают на ЭТОМ ноутбуке. Запусти один раз:
#   bash scripts/selftest.sh
# Должно быть везде OK. Если где-то FAIL — пришли вывод целиком.
source "$(dirname "$0")/env.sh"
SH="${BASH:-bash}"
t="$(mktemp -d)"; cd "$t" || exit 1
pass=0; fail=0
ok()  { echo "OK   $1"; pass=$((pass + 1)); }
bad() { echo "FAIL $1"; fail=$((fail + 1)); }

echo "система: $(uname), bash $BASH_VERSION"
if [ "$IS_MAC" = 1 ]; then
  echo "clang: $("$CLANG" --version 2>/dev/null | head -1)"
  if [ -n "$GCC" ]; then echo "gcc:   $GCC"; else echo "gcc:   не найден (brew install gcc) — pb_ds и __gcd собираться не будут"; fi
else
  echo "gcc:   $("$GCC" --version 2>/dev/null | head -1)"
fi
echo

cat > hello.cpp <<'EOF'
#include <bits/stdc++.h>
using namespace std;
int main() { set<int> s{3, 1, 2}; cout << *s.begin() << '\n'; }
EOF
if [ "$(echo | "$SH" "$SCRIPTS_DIR/run.sh" hello.cpp 2>/dev/null)" = 1 ]; then ok "bits/stdc++.h + run.sh"; else bad "bits/stdc++.h + run.sh"; "$SH" "$SCRIPTS_DIR/run.sh" hello.cpp 2>&1 | head -5; fi

cat > oob.cpp <<'EOF'
#include <bits/stdc++.h>
using namespace std;
int main() { vector<int> a(3); int i; cin >> i; cout << a[i] << '\n'; }
EOF
if echo 5 | "$SH" "$SCRIPTS_DIR/run.sh" oob.cpp > out.txt 2> err.txt; then
  bad "выход за границы vector НЕ пойман"
else ok "выход за границы vector ловится"; fi

cat > ovf.cpp <<'EOF'
#include <bits/stdc++.h>
using namespace std;
int main() { int x; cin >> x; x *= x; cout << x << '\n'; }
EOF
if echo 2000000000 | "$SH" "$SCRIPTS_DIR/run.sh" ovf.cpp > out.txt 2> err.txt; then
  bad "переполнение int НЕ поймано (санитайзеры не работают?)"
else ok "переполнение int ловится"; fi

cp "$SCRIPTS_DIR/gen.cpp" gen.cpp
cat > brute.cpp <<'EOF'
#include <bits/stdc++.h>
using namespace std;
int main() { int n; cin >> n; vector<int> a(n); for (auto& x : a) cin >> x; cout << *max_element(a.begin(), a.end()) << '\n'; }
EOF
cat > wrong.cpp <<'EOF'
#include <bits/stdc++.h>
using namespace std;
int main() { int n; cin >> n; int mx = 0; for (int i = 0; i < n; i++) { int x; cin >> x; mx = max(mx, x); } cout << mx << '\n'; }
EOF
if "$SH" "$SCRIPTS_DIR/stress.sh" wrong.cpp brute.cpp gen.cpp 300 > out.txt 2>&1; then
  bad "stress НЕ нашёл ошибку"
elif grep -q "WA на seed" out.txt && [ -f failed_input.txt ]; then ok "stress находит WA"
else bad "stress упал не так, как ожидалось:"; head -10 out.txt; fi

if "$SH" "$SCRIPTS_DIR/stress.sh" brute.cpp brute.cpp gen.cpp 200 > out.txt 2>&1 && grep -q "^OK" out.txt; then
  ok "stress проходит на верном решении"
else bad "stress на верном решении:"; head -10 out.txt; fi

if SAN=1 "$SH" "$SCRIPTS_DIR/stress.sh" brute.cpp brute.cpp gen.cpp 50 > out.txt 2>&1 && grep -q "^OK" out.txt; then
  ok "stress с SAN=1"
else bad "stress с SAN=1:"; head -10 out.txt; fi

mkdir -p my/x && cp hello.cpp my/x/good.cpp && echo 'int main() { nope(); }' > my/x/broken.cpp
"$SH" "$SCRIPTS_DIR/check_all.sh" my > out.txt 2>&1
if grep -q "FAIL: my/x/broken.cpp" out.txt && grep -q "компилируется: 1, сломано: 1" out.txt; then
  ok "check_all.sh"
else bad "check_all.sh:"; head -10 out.txt; fi

if [ -d "$KACTL_DIR" ]; then
  expect="$(printf '1 0\n1 0')"
  if [ "$("$SH" "$SCRIPTS_DIR/run.sh" "$SCRIPTS_DIR/example_kactl.cpp" 2>/dev/null)" = "$expect" ]; then
    ok "KACTL через #include собирается локально"
  else bad "KACTL через #include:"; "$SH" "$SCRIPTS_DIR/run.sh" "$SCRIPTS_DIR/example_kactl.cpp" 2>&1 | head -8; fi

  if "$SH" "$SCRIPTS_DIR/bundle.sh" "$SCRIPTS_DIR/example_kactl.cpp" > send.cpp 2> err.txt &&
     ! grep -q '#include "' send.cpp &&
     [ "$(grep -c 'struct Point' send.cpp)" = 1 ] &&
     compile_fast send.cpp send_bin 2>/dev/null &&
     [ "$(./send_bin)" = "$expect" ]; then
    ok "bundle.sh склеивает в один файл, и он работает"
  else bad "bundle.sh:"; cat err.txt | head -5; fi
else
  echo "SKIP KACTL не найден в ext/kactl — сначала git clone (bundle.sh не проверен)"
fi

echo
echo "итого: OK $pass, FAIL $fail"
rm -rf "$t"
[ "$fail" -eq 0 ]
