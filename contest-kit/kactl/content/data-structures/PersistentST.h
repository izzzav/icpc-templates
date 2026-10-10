/**
 * Description: Персистентное дерево отрезков на позициях [0, n).
 * При изменении копируются только вершины на пути от корня до листа (их ~log n),
 * остальные поддеревья общие со старой версией. Поэтому старые версии не портятся,
 * а каждая версия — это просто номер её корня.
 * Версия 0 — пустое дерево (везде unit). update возвращает номер новой версии.
 * Операция как в SegmentTree.h: меняй T, unit и f (ассоциативна, f(unit, x) = f(x, unit) = x).
 * update делает a[pos] = f(a[pos], val): для суммы это +=, для max — max=.
 * Usage: k-я порядковая на [l, r): сожми значения в [0, m), потом
 *   PST t(m, n); vi root{0};
 *   rep(i,0,n) root.push_back(t.update(root[i], a[i], 1));
 *   t.kth(root[l], root[r], k);                        // сжатое значение
 *   t.query(root[r], 0, x) - t.query(root[l], 0, x);  // сколько < x на [l, r)
 * Time: O(\log N) на операцию, память O(\log N) на update.
 * Status: stress-tested
 */
#pragma once

struct PST {
	typedef int T;
	static constexpr T unit = 0;
	T f(T a, T b) { return a + b; }

	struct Node {
		int left, right; // номера детей в nodes
		T val;           // f на отрезке вершины
	};
	// Все вершины всех версий. nodes[0] — пустая вершина: её дети — она сама,
	// значение unit. Поэтому версия 0 — массив из unit, и строить его не нужно.
	vector<Node> nodes{{0, 0, unit}};
	int n;

	// upds — сколько будет update, чтобы выделить память сразу (можно не указывать)
	PST(int n, int upds = 0) : n(n) {
		nodes.reserve((upds + 1) * (bit_width((unsigned)n) + 1));
	}

	// a[pos] = f(a[pos], val) в версии root, возвращает корень новой версии
	int update(int root, int pos, T val) { return update(root, pos, val, 0, n); }

	// f на [l, r) в версии root
	T query(int root, int l, int r) { return query(root, l, r, 0, n); }

	// Только для f = сумма счётчиков: k-й (с 0) по возрастанию элемент среди
	// добавленных между версиями a и b (b новее). Спускаемся по обеим версиям сразу:
	// разность в левых детях — сколько элементов попало в левую половину.
	int kth(int a, int b, T k) {
		int lo = 0, hi = n;
		while (hi - lo > 1) {
			int mid = (lo + hi) / 2;
			T inLeft = nodes[nodes[b].left].val - nodes[nodes[a].left].val;
			if (k < inLeft) {
				a = nodes[a].left;
				b = nodes[b].left;
				hi = mid;
			} else {
				k -= inLeft;
				a = nodes[a].right;
				b = nodes[b].right;
				lo = mid;
			}
		}
		return lo;
	}

	// Рекурсивные версии: v — вершина, отвечающая за [lo, hi).
	int update(int v, int pos, T val, int lo, int hi) {
		Node copy = nodes[v]; // именно копия: push_back ниже может переложить вектор
		if (hi - lo == 1) {
			copy.val = f(copy.val, val);
		} else {
			int mid = (lo + hi) / 2;
			if (pos < mid) copy.left = update(copy.left, pos, val, lo, mid);
			else copy.right = update(copy.right, pos, val, mid, hi);
			copy.val = f(nodes[copy.left].val, nodes[copy.right].val);
		}
		nodes.push_back(copy);
		return (int)nodes.size() - 1;
	}

	T query(int v, int l, int r, int lo, int hi) {
		if (v == 0 || r <= lo || hi <= l) return unit; // пусто или мимо запроса
		if (l <= lo && hi <= r) return nodes[v].val;   // целиком внутри запроса
		int mid = (lo + hi) / 2;
		return f(query(nodes[v].left, l, r, lo, mid), query(nodes[v].right, l, r, mid, hi));
	}
};
