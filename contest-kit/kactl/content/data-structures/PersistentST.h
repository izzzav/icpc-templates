/**
 * Description: Персистентное дерево отрезков на позициях [0, n).
 * Версия = номер корня (int). Версия 0 — пустое дерево (везде unit).
 * upd не трогает старую версию, а возвращает номер новой.
 * Как в SegmentTree.h, меняй T, unit и f (ассоциативная, f(unit, x) = x).
 * upd делает a[pos] = f(a[pos], val): для суммы это +=, для max — max=.
 * kth — только для f = сумма счётчиков: k-й (с 0) по возрастанию элемент
 * среди добавленных между версиями a и b.
 * upds — сколько будет upd (только чтобы заранее выделить память, можно 0).
 * Usage: k-я порядковая на [l, r): сожми значения в [0, m), потом
 *   PST t(m, n); vi root{0};
 *   rep(i,0,n) root.push_back(t.upd(root[i], a[i], 1));
 *   t.kth(root[l], root[r], k);                        // сжатое значение
 *   t.query(root[r], 0, x) - t.query(root[l], 0, x);  // сколько < x на [l, r)
 * Time: O(\log N) на операцию, память O(\log N) на upd.
 * Status: stress-tested
 */
#pragma once

struct PST {
	typedef int T;
	static constexpr T unit = 0;
	T f(T a, T b) { return a + b; }
	struct Node { int l, r; T v; };
	vector<Node> t{{0, 0, unit}};
	int n;
	PST(int N, int upds = 0) : n(N) { t.reserve((upds + 1) * (bit_width((unsigned)N) + 1)); }
	int upd(int x, int pos, T val) { return upd(x, pos, val, 0, n); }
	int upd(int x, int pos, T val, int lo, int hi) {
		Node c = t[x];
		if (hi - lo == 1) c.v = f(c.v, val);
		else {
			int mid = (lo + hi) / 2;
			if (pos < mid) c.l = upd(c.l, pos, val, lo, mid);
			else c.r = upd(c.r, pos, val, mid, hi);
			c.v = f(t[c.l].v, t[c.r].v);
		}
		t.push_back(c);
		return sz(t) - 1;
	}
	T query(int x, int L, int R) { return query(x, L, R, 0, n); } // [L, R)
	T query(int x, int L, int R, int lo, int hi) {
		if (!x || R <= lo || hi <= L) return unit;
		if (L <= lo && hi <= R) return t[x].v;
		int mid = (lo + hi) / 2;
		return f(query(t[x].l, L, R, lo, mid), query(t[x].r, L, R, mid, hi));
	}
	int kth(int a, int b, T k) {
		int lo = 0, hi = n;
		while (hi - lo > 1) {
			int mid = (lo + hi) / 2;
			T cnt = t[t[b].l].v - t[t[a].l].v;
			if (k < cnt) a = t[a].l, b = t[b].l, hi = mid;
			else k -= cnt, a = t[a].r, b = t[b].r, lo = mid;
		}
		return lo;
	}
};
