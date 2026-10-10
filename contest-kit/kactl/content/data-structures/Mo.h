/**
 * Author: Simon Lindholm
 * Date: 2019-12-28
 * License: CC0
 * Source: https://github.com/hoke-t/tamu-kactl/blob/master/content/data-structures/MoQueries.h
 * Description: Алгоритм Мо: офлайн-ответы на запросы на отрезках (mo) или на путях
 * в дереве (moTree). Запросы сортируются так, чтобы от одного к следующему границы
 * сдвигались мало, а текущий отрезок/путь меняется по одному элементу с краёв.
 * Под задачу пишешь только add, del и calc, остальное не трогаешь.
 * Значения на рёбрах дерева, а не в вершинах: в step добавляй/удаляй ребро (a, c)
 * и убери начальный add(0, 0) (in[0] = 1 оставь).
 * Time: O(N \sqrt Q)
 * Status: stress-tested
 */
#pragma once

void add(int ind, int end) { ... } // добавить a[ind]; end = 0 — слева, 1 — справа
void del(int ind, int end) { ... } // убрать a[ind]
int calc() { ... } // ответ для текущего отрезка

// Q[i] = {l, r} — запрос на [l, r). blk — размер блока, лучше всего ~N / sqrt(Q).
vi mo(vector<pii> Q, int blk = 350) {
	// Сортируем по блоку левой границы, внутри блока — по правой границе: в чётных
	// блоках по возрастанию, в нечётных по убыванию. Так правая граница ходит
	// «змейкой» и не возвращается каждый раз в начало.
	auto key = [&](pii q) {
		int block = q.first / blk;
		return pii(block, block % 2 == 0 ? q.second : -q.second);
	};
	vi order(sz(Q)), res(sz(Q));
	iota(all(order), 0);
	sort(all(order), [&](int i, int j) { return key(Q[i]) < key(Q[j]); });

	int L = 0, R = 0; // текущий отрезок [L, R)
	for (int qi : order) {
		auto [l, r] = Q[qi];
		// сначала расширяем, потом сужаем, чтобы не было L > R
		while (L > l) add(--L, 0);
		while (R < r) add(R++, 1);
		while (L < l) del(L++, 0);
		while (R > r) del(--R, 1);
		res[qi] = calc();
	}
	return res;
}

// Q[i] = {u, v} — запрос на пути u-v (концы включены). ed — списки смежности.
vi moTree(vector<array<int, 2>> Q, vector<vi>& ed, int root = 0, int blk = 350) {
	int n = sz(ed), timer = 0;
	vi par(n), num(n), L(n), R(n), in(n);
	// num — порядок, в котором сортируем: на нечётной глубине номер даём при входе,
	// на чётной — при выходе. Тогда вершины с соседними номерами близко в дереве
	// (не дальше 3 рёбер), и обход запросов в таком порядке короткий.
	// [L[x], R[x]) — отрезок номеров поддерева x: y предок x <=> L[y] <= L[x] && R[x] <= R[y].
	auto dfs = [&](int x, int p, int odd, auto& self) -> void {
		par[x] = p;
		L[x] = timer;
		if (odd) num[x] = timer++;
		for (int y : ed[x]) if (y != p) self(y, x, !odd, self);
		if (!odd) num[x] = timer++;
		R[x] = timer;
	};
	dfs(root, -1, 0, dfs);

	auto key = [&](array<int, 2> q) {
		int block = num[q[0]] / blk;
		return pii(block, block % 2 == 0 ? num[q[1]] : -num[q[1]]);
	};
	vi order(sz(Q)), res(sz(Q));
	iota(all(order), 0);
	sort(all(order), [&](int i, int j) { return key(Q[i]) < key(Q[j]); });

	// Текущий путь — от pos[0] до pos[1], in[x] = 1, если x на нём. Начинаем с вершины 0.
	int pos[2] = {0, 0};
	add(0, 0);
	in[0] = 1;
	vi path;
	for (int qi : order) {
		for (int end = 0; end < 2; end++) {
			int& a = pos[end];
			int b = Q[qi][end];
			// сдвинуть конец пути a в соседнюю вершину c
			auto step = [&](int c) {
				if (in[c]) { // идём назад по пути: a с пути уходит
					del(a, end);
					in[a] = 0;
				} else { // путь удлиняется
					add(c, end);
					in[c] = 1;
				}
				a = c;
			};
			// Ведём a в b: поднимаем b до предка a (запоминая, откуда шли),
			// поднимаем a до того же места и спускаемся по запомненному пути.
			path.clear();
			while (!(L[b] <= L[a] && R[a] <= R[b])) {
				path.push_back(b);
				b = par[b];
			}
			while (a != b) step(par[a]);
			for (int i = sz(path) - 1; i >= 0; i--) step(path[i]);
		}
		res[qi] = calc();
	}
	return res;
}
