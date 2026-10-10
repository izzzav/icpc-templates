// Образец генератора для stress.sh. Запуск: ./gen SEED > in
// Делай тесты МАЛЕНЬКИМИ (n до 5-10): упавший тест надо прочитать глазами.
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    mt19937 rng(atoi(argv[1]));
    auto rnd = [&](long long l, long long r) {
        return uniform_int_distribution<long long>(l, r)(rng);
    };

    int n = rnd(1, 8);
    cout << n << '\n';
    for (int i = 0; i < n; i++) cout << rnd(-10, 10) << ' ';
    cout << '\n';
}
