// Код из KACTL использует эти определения. Если в твоём шаблоне их нет,
// а вставляешь файл из ext/kactl — добавь эти строки над вставленным кодом.
// При вставке удали из KACTL-файла строки "#pragma once" и "#include "...""
// (а то, что они подключали, вставь выше — например Point.h для геометрии).
#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
