/**
 * Author: someone on Codeforces
 * Date: 2017-03-14
 * Source: folklore
 * Description: Декартово дерево (treap) по неявному ключу: массив, который можно
 * резать и склеивать за O(log N). По позиции это дерево поиска (позиция вершины =
 * сколько элементов левее неё), по случайному приоритету y — куча. Из-за случайных
 * приоритетов глубина в среднем O(log N).
 * Чтобы хранить что-то про поддерево (сумму, минимум, флаг разворота), добавь поле
 * в Node и пересчитывай его в recalc.
 * Time: $O(\log N)$
 * Status: stress-tested
 */
#pragma once

struct Node {
	Node *l = 0, *r = 0;
	int val;   // сам элемент
	int y;     // приоритет: у родителя больше, чем у детей
	int c = 1; // размер поддерева
	Node(int val) : val(val), y(rand()) {}
	void recalc();
};

int cnt(Node* n) { return n ? n->c : 0; }

// Пересчитать данные поддерева по детям. Звать после любого изменения l или r.
void Node::recalc() { c = cnt(l) + cnt(r) + 1; }

// Обойти элементы по порядку: each(t, [&](int x) { ... });
template<class F> void each(Node* n, F f) {
	if (!n) return;
	each(n->l, f);
	f(n->val);
	each(n->r, f);
}

// Разрезать на первые k элементов и все остальные.
// Чтобы резать по значению (как lower_bound(k)): условие "n->val >= k",
// а в правую ветку передавать просто k.
pair<Node*, Node*> split(Node* n, int k) {
	if (!n) return {};
	if (cnt(n->l) >= k) {
		// корень и правое поддерево целиком справа от разреза, режем левое
		auto [L, R] = split(n->l, k);
		n->l = R;
		n->recalc();
		return {L, n};
	} else {
		// левое поддерево и корень целиком слева, режем правое
		auto [L, R] = split(n->r, k - cnt(n->l) - 1);
		n->r = L;
		n->recalc();
		return {n, R};
	}
}

// Склеить: все элементы l идут перед всеми элементами r.
// Корнем становится вершина с большим приоритетом.
Node* merge(Node* l, Node* r) {
	if (!l) return r;
	if (!r) return l;
	if (l->y > r->y) {
		l->r = merge(l->r, r);
		l->recalc();
		return l;
	} else {
		r->l = merge(l, r->l);
		r->recalc();
		return r;
	}
}

// Вставить вершину n так, чтобы перед ней стояло pos элементов.
Node* ins(Node* t, Node* n, int pos) {
	auto [l, r] = split(t, pos);
	return merge(merge(l, n), r);
}

// Пример: вырезать отрезок [l, r) и вставить его перед элементом,
// который сейчас стоит на позиции k (k <= l или k >= r).
void move(Node*& t, int l, int r, int k) {
	Node *a, *b, *c;
	tie(a, b) = split(t, l);
	tie(b, c) = split(b, r - l);
	if (k <= l) t = merge(ins(a, b, k), c);
	else t = merge(a, ins(c, b, k - r));
}
