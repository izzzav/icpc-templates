/**
 * Author: Lucian Bicsi
 * Date: 2017-10-31
 * License: CC0
 * Source: folklore
 * Description: Дерево отрезков снизу вверх, без рекурсии. Индексы с 0.
 * update(pos, val): a[pos] = val. query(l, r): f на [l, r).
 * Сейчас считает максимум. Под другую операцию меняй T, unit и f:
 * f ассоциативна, unit нейтрален: f(unit, x) = f(x, unit) = x.
 * Сумма: unit = 0, a + b. Минимум: unit = INT_MAX, min(a, b).
 * Time: O(\log N)
 * Status: stress-tested
 */
#pragma once

struct Tree {
	typedef int T;
	static constexpr T unit = INT_MIN;
	T f(T a, T b) { return max(a, b); }

	// Лист a[i] лежит в s[n + i]. Вершина v < n хранит f(s[2v], s[2v + 1]), корень — s[1].
	vector<T> s;
	int n;

	// def — начальное значение всех a[i]. Внутренние вершины тоже заполняются def,
	// поэтому с суммой def должен быть 0, иначе заполни через update.
	Tree(int n = 0, T def = unit) : s(2 * n, def), n(n) {}

	void update(int pos, T val) {
		pos += n;
		s[pos] = val;
		for (pos /= 2; pos >= 1; pos /= 2)
			s[pos] = f(s[2 * pos], s[2 * pos + 1]);
	}

	T query(int l, int r) {
		T resL = unit, resR = unit;
		for (l += n, r += n; l < r; l /= 2, r /= 2) {
			if (l % 2 == 1) resL = f(resL, s[l++]);
			if (r % 2 == 1) resR = f(s[--r], resR);
		}
		// левую и правую части копим отдельно, чтобы не перепутать порядок
		return f(resL, resR);
	}
};
