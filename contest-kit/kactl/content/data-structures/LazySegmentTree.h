/**
 * Author: Simon Lindholm
 * Date: 2016-10-08
 * License: CC0
 * Source: me
 * Description: Дерево отрезков с отложенными операциями:
 * прибавить x на [L, R), присвоить x на [L, R), максимум на [L, R).
 * Можно переделать под другие операции (см. комментарий у push).
 * Вершины создаются только когда в них спускаются, поэтому подходит и для
 * огромного диапазона вроде [0, 1e9) без сжатия координат.
 * BumpAllocator ускоряет new и экономит память.
 * Time: O(\log N).
 * Usage: Node* tr = new Node(v, 0, sz(v));  // из массива v
 *   Node* tr = new Node(0, n);  // все значения -inf
 *   tr->add(l, r, x); tr->set(l, r, x); tr->query(l, r);
 * Status: stress-tested a bit
 */
#pragma once

#include "../various/BumpAllocator.h"

const int inf = 1e9;

struct Node {
	Node *l = 0, *r = 0; // дети, nullptr — ещё не созданы
	int lo, hi;          // вершина отвечает за [lo, hi)
	int val = -inf;      // максимум на [lo, hi)
	// Отложенная операция для детей: присвоить mset (если mset != inf), иначе прибавить madd.
	// Обе сразу не нужны: прибавление поверх присвоения просто меняет mset.
	int mset = inf, madd = 0;

	Node(int lo, int hi) : lo(lo), hi(hi) {}

	Node(vi& v, int lo, int hi) : lo(lo), hi(hi) {
		if (hi - lo == 1) {
			val = v[lo];
			return;
		}
		int mid = lo + (hi - lo) / 2;
		l = new Node(v, lo, mid);
		r = new Node(v, mid, hi);
		val = max(l->val, r->val);
	}

	int query(int L, int R) {
		if (R <= lo || hi <= L) return -inf;  // не пересекается с запросом
		if (L <= lo && hi <= R) return val;   // целиком внутри запроса
		push();
		return max(l->query(L, R), r->query(L, R));
	}

	void set(int L, int R, int x) {
		if (R <= lo || hi <= L) return;
		if (L <= lo && hi <= R) {
			// присвоение отменяет всё, что было отложено раньше
			val = x;
			mset = x;
			madd = 0;
			return;
		}
		push();
		l->set(L, R, x);
		r->set(L, R, x);
		val = max(l->val, r->val);
	}

	void add(int L, int R, int x) {
		if (R <= lo || hi <= L) return;
		if (L <= lo && hi <= R) {
			val += x;
			if (mset != inf) mset += x;
			else madd += x;
			return;
		}
		push();
		l->add(L, R, x);
		r->add(L, R, x);
		val = max(l->val, r->val);
	}

	// Передать отложенную операцию детям (создав их, если нужно). Зовётся перед
	// каждым спуском в детей. Своя операция: заведи для неё метку, в случае
	// «целиком внутри» обнови val и метку, а здесь передай метку детям и сбрось.
	void push() {
		if (!l) {
			int mid = lo + (hi - lo) / 2;
			l = new Node(lo, mid);
			r = new Node(mid, hi);
		}
		if (mset != inf) {
			l->set(lo, hi, mset);
			r->set(lo, hi, mset);
			mset = inf;
		} else if (madd) {
			l->add(lo, hi, madd);
			r->add(lo, hi, madd);
			madd = 0;
		}
	}
};
