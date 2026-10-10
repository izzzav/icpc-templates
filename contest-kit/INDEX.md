# Шаблоны — оглавление

Поиск: в IDE `Ctrl+Shift+F` (mac: `Cmd+Shift+F`) по всей папке, или в терминале
`grep -rli "мост" my ext docs`. В строках ниже есть ключевые слова на русском и
английском — ищи по ним.

## Шапка в начале КАЖДОГО своего файла (5 строк, код ниже не трогать)

```
// ЧТО: дерево отрезков: сумма на отрезке + прибавление на отрезке
// СЛОЖНОСТЬ: O(log n) на запрос
// КАК ЗВАТЬ: build(...); update(l, r, x); get(l, r)
// ИНДЕКСАЦИЯ: с 0 / с 1? отрезки [l, r] или [l, r)?
// ПРОВЕРЕНО: весна ДЗ 9, задача B — OK
```

## Скрипты (`scripts/`)

| Что | Команда |
|---|---|
| Запуск с санитайзерами | `./scripts/run.sh a.cpp in.txt` |
| Стресс-тест | `./scripts/stress.sh sol.cpp brute.cpp gen.cpp 1000` (с санитайзерами: `SAN=1 ...`) |
| Образец генератора | `scripts/gen.cpp` |
| Все ли шаблоны компилируются | `./scripts/check_all.sh` |
| Чего не хватает для вставки KACTL | `scripts/kactl_prefix.cpp` |

## Свои шаблоны (`my/`) — из домашек

Колонку «проверено» заполняй только если решение было сдано (OK в системе).

| Что / ключевые слова | Файл | Откуда | Проверено |
|---|---|---|---|
| мосты, bridges | `my/graphs/bridges.cpp` | весна ДЗ 2 | |
| точки сочленения, cutpoints, articulation | `my/graphs/cutpoints.cpp` | весна ДЗ 2 | |
| Дейкстра, dijkstra, кратчайший путь | `my/graphs/dijkstra.cpp` | весна ДЗ 4 | |
| Форд-Беллман, отрицательный цикл, bellman, negative cycle | `my/graphs/bellman.cpp` | весна ДЗ 5 | |
| СНМ, DSU, Крускал, остов, MST | `my/ds/dsu.cpp` | весна ДЗ 6 | |
| декартово дерево, treap, неявный ключ, split merge, reverse | `my/ds/treap.cpp` | весна ДЗ 8 | |
| дерево отрезков, segment tree, ленивое, lazy | `my/ds/segtree.cpp` | весна ДЗ 9 | |
| Фенвик, fenwick, BIT | `my/ds/fenwick.cpp` | весна ДЗ 10 | |
| разреженная таблица, sparse table, RMQ | `my/ds/sparse.cpp` | весна ДЗ 10 | |
| LCA, двоичные подъёмы, binary lifting | `my/graphs/lca.cpp` | весна ДЗ 11 | |
| НОД, обратный по модулю, решето, КТО, gcd, inverse, sieve, CRT, binpow | `my/math/nt.cpp` | весна ДЗ 12 | |
| Мо, корневая, sqrt, mo | `my/ds/mo.cpp` | весна ДЗ 15 | |
| префикс-функция, Z-функция, KMP, prefix function | `my/strings/prefix.cpp` | осень ДЗ 1 | |
| Ахо-Корасик, aho corasick, бор, trie | `my/strings/aho.cpp` | осень ДЗ 2 | |
| персистентное ДО, persistent, k-я порядковая на отрезке | `my/ds/persistent.cpp` | осень ДЗ 6 | |
| паросочетание, Кун, kuhn, matching | `my/graphs/kuhn.cpp` | осень ДЗ 7 | |
| поток, Диниц, dinic, flow, mincost | `my/graphs/flow.cpp` | осень ДЗ 8 | |
| геометрия, точка, вектор, векторное произведение | `my/geometry/base.cpp` | осень ДЗ 9 | |

## KACTL (`ext/kactl/content/...`) — то, чего нет в домашках

Всё в KACTL протестировано. Перед вставкой посмотри `scripts/kactl_prefix.cpp`.

| Что / ключевые слова | Путь |
|---|---|
| компоненты сильной связности, SCC, конденсация | `graph/SCC.h` |
| 2-SAT | `graph/2sat.h` |
| компоненты вершинной двусвязности | `graph/BiconnectedComponents.h` |
| эйлеров путь/цикл, euler | `graph/EulerWalk.h` |
| HLD, тяжёлые пути, запросы на пути в дереве | `graph/HLD.h` |
| link-cut tree, LCT, динамическое дерево | `graph/LinkCutTree.h` |
| Хопкрофт-Карп, быстрое паросочетание | `graph/HopcroftKarp.h` |
| венгерский алгоритм, назначение, assignment, взвешенное паросочетание | `graph/WeightedMatching.h` |
| Диниц (если своего нет) | `graph/Dinic.h` |
| поток мин. стоимости, MCMF, min cost max flow | `graph/MinCostMaxFlow.h` |
| ordered set, k-й элемент в set, pb_ds | `data-structures/OrderStatisticTree.h` |
| convex hull trick, CHT, максимум прямых | `data-structures/LineContainer.h` |
| матрица, возведение матрицы в степень | `data-structures/Matrix.h` |
| быстрый хеш-мап, gp_hash_table | `data-structures/HashMap.h` |
| хеши строк, hashing | `strings/Hashing.h` |
| суффиксный массив, LCP, suffix array | `strings/SuffixArray.h` |
| палиндромы, Манакер, manacher | `strings/Manacher.h` |
| проверка на простоту до 7e18, Миллер-Рабин | `number-theory/MillerRabin.h` |
| факторизация больших чисел, Поллард, pollard rho | `number-theory/Factor.h` |
| дискретный логарифм, a^x = b mod m | `number-theory/ModLog.h` |
| NTT, умножение многочленов по модулю 998244353 | `numerical/NumberTheoreticTransform.h` |
| FFT по произвольному модулю | `numerical/FastFourierTransformMod.h` |
| Гаусс, система уравнений | `numerical/SolveLinear.h` |
| Гаусс по модулю 2, xor | `numerical/SolveLinearBinary.h` |
| определитель по модулю | `numerical/IntDeterminant.h` |
| Берлекэмп-Мэсси, линейная рекуррента | `numerical/BerlekampMassey.h` |
| точка, Point (база для всей геометрии KACTL) | `geometry/Point.h` |
| выпуклая оболочка, convex hull | `geometry/ConvexHull.h` |
| пересечение отрезков | `geometry/SegmentIntersection.h` |
| пересечение прямых | `geometry/lineIntersection.h` |
| площадь многоугольника | `geometry/PolygonArea.h` |
| точка в многоугольнике | `geometry/InsidePolygon.h` |
| самые далёкие точки, диаметр, вращающиеся калиперы | `geometry/HullDiameter.h` |
| ближайшая пара точек | `geometry/ClosestPair.h` |
| минимальная охватывающая окружность | `geometry/MinimumEnclosingCircle.h` |
| пересечение окружностей | `geometry/CircleIntersection.h` |

Всё остальное: `ext/kactl/kactl.pdf` — там оглавление на первой странице.

## Документация

| Что | Где |
|---|---|
| Статьи по алгоритмам (cp-algorithms, бывш. e-maxx) | `docs/cp-algorithms/src/` — искать поиском по папке |
| KACTL целиком в PDF | `ext/kactl/kactl.pdf` |
| C++ (set, map, lower_bound, ...) | Zeal / Dash / офлайн-архив cppreference |
