# Шаблоны — оглавление

KACTL: `kactl/content/<путь>`. Всё там протестировано, в начале каждого
файла есть Description / Usage / Time — как вызывать, читать там
Весь KACTL в одном файле с оглавлением: `kactl/kactl.pdf`.

Поиск: `Ctrl+Shift+F` (на маке `Cmd+Shift+F`) в IDE по папке `kactl/content`.

## Перед вставкой KACTL-кода

1. Удали из файла `#pragma once` и строки `#include "..."`. То, что они подключали,
   вставь выше (например, для геометрии сначала `geometry/Point.h`).
2. Если в основном шаблоне нет `rep`, `all`, `sz`, `ll`, `pii`, `vi`, возьми эти строки
   из `00_solutions/template.cpp`.
3. Посмотри колонку «Индексы»: в KACTL почти везде полуинтервалы `[l, r)` и нумерация с 0.

macOS: `brew install gcc`, иначе не соберётся код с pb_ds (`OrderStatisticTree.h`,
`HashMap.h`) и `__gcd`. На Windows MinGW — это и есть GCC, всё соберётся.

## Графы

| Что / ключевые слова | Путь | Индексы, подвохи |
|---|---|---|
| топсорт, topological sort | `graph/TopoSort.h` | если есть цикл, вернёт меньше n вершин |
| мосты, точки сочленения, двусвязность, bridges, articulation | `graph/BiconnectedComponents.h` | мост = компонента из одного ребра; точка сочленения = вершина в >1 компоненте |
| компоненты сильной связности, SCC, конденсация | `graph/SCC.h` | |
| 2-SAT | `graph/2sat.h` | отрицание переменной x — это `~x` |
| Форд-Беллман, отрицательный цикл | `graph/BellmanFord.h` | недостижимые = inf, вершины, задетые отрицательным циклом, = -inf |
| Флойд, все пары | `graph/FloydWarshall.h` | |
| Дейкстра | **нет в KACTL** — пишется по памяти (BFS + `priority_queue`) | |
| LCA | `graph/LCA.h` | корень = 0 |
| двоичные подъёмы, k-й предок | `graph/BinaryLifting.h` | |
| HLD, запросы на пути в дереве | `graph/HLD.h` | |
| link-cut tree, LCT | `graph/LinkCutTree.h` | |
| эйлеров путь/цикл | `graph/EulerWalk.h` | |
| паросочетание, Кун | `graph/DFSMatching.h` | |
| паросочетание быстрое, Хопкрофт-Карп | `graph/HopcroftKarp.h` | |
| венгерский алгоритм, назначение, assignment | `graph/WeightedMatching.h` | минимизирует сумму |
| поток, Диниц, flow | `graph/Dinic.h` | |
| поток мин. стоимости, MCMF | `graph/MinCostMaxFlow.h` | отрицательные стоимости → `setpi` перед `maxflow` |
| мин. разрез, min cut | `graph/MinCut.h`, `graph/GlobalMinCut.h` | |

## Структуры данных

| Что / ключевые слова | Путь | Индексы, подвохи |
|---|---|---|
| СНМ, DSU, Крускал | `data-structures/UnionFind.h` | |
| дерево отрезков, segment tree | `data-structures/SegmentTree.h` | с 0, запрос `[l, r)`, по умолчанию **максимум**: поменяй `unit` и `f` |
| ДО с отложенными операциями, lazy, присвоение/прибавление на отрезке | `data-structures/LazySegmentTree.h` | `[l, r)`, максимум + add/set |
| Фенвик, BIT | `data-structures/FenwickTree.h` | `query(pos)` = сумма `[0, pos)` |
| Фенвик 2D | `data-structures/FenwickTree2d.h` | |
| разреженная таблица, RMQ, sparse table | `data-structures/RMQ.h` | `query(a, b)` = min на `[a, b)` |
| декартово дерево, treap, неявный ключ, split/merge | `data-structures/CartesianTree.h` | |
| Мо, корневая | `data-structures/Mo.h` | |
| ordered set, k-й элемент, order of key | `data-structures/OrderStatisticTree.h` | set, не multiset. Нужен GCC |
| быстрый хеш-мап | `data-structures/HashMap.h` | нужен GCC |
| CHT, максимум прямых, convex hull trick | `data-structures/LineContainer.h` | ищет **максимум**; для минимума меняй знаки |
| матрица, возведение в степень | `data-structures/Matrix.h` | |
| персистентное ДО, k-я порядковая на отрезке | `data-structures/PersistentST.h` (наш, не из KACTL) | |

## Строки

| Что / ключевые слова | Путь | Индексы, подвохи |
|---|---|---|
| префикс-функция, KMP | `strings/KMP.h` | |
| Z-функция | `strings/Zfunc.h` | `z[0] = 0` |
| Ахо-Корасик, бор | `strings/AhoCorasick.h` | |
| хеши строк | `strings/Hashing.h` | |
| суффиксный массив, LCP | `strings/SuffixArray.h` | |
| палиндромы, Манакер | `strings/Manacher.h` | |

## Теория чисел и алгебра

| Что / ключевые слова | Путь | Индексы, подвохи |
|---|---|---|
| арифметика по модулю, Mod | `number-theory/ModularArithmetic.h` | |
| быстрое возведение в степень, binpow | `number-theory/ModPow.h` | |
| обратные по модулю для всех чисел до LIM | `number-theory/ModInverse.h` | модуль простой |
| расширенный Евклид, ax+by=gcd | `number-theory/euclid.h` | |
| КТО, CRT | `number-theory/ChineseTheory.h` | |
| решето Эратосфена | `number-theory/Eratosthenes.h` | |
| проверка на простоту до 7e18, Миллер-Рабин | `number-theory/MillerRabin.h` | |
| факторизация больших чисел, Поллард | `number-theory/Factor.h` | |
| дискретный логарифм, a^x = b mod m | `number-theory/ModLog.h` | |
| функция Эйлера | `number-theory/phiFunction.h` | |
| NTT, умножение многочленов mod 998244353 | `numerical/NumberTheoreticTransform.h` | |
| FFT по произвольному модулю | `numerical/FastFourierTransformMod.h` | |
| Гаусс | `numerical/SolveLinear.h` | |
| Гаусс по модулю 2, xor | `numerical/SolveLinearBinary.h` | |
| определитель по модулю | `numerical/IntDeterminant.h` | |
| линейная рекуррента, Берлекэмп-Мэсси | `numerical/BerlekampMassey.h` | |

## Геометрия (сначала вставь `geometry/Point.h`)

| Что / ключевые слова | Путь | Индексы, подвохи |
|---|---|---|
| точка, вектор, cross, dot | `geometry/Point.h` | T = long long или double, не int |
| площадь многоугольника | `geometry/PolygonArea.h` | возвращает **удвоенную** площадь со знаком |
| точка в многоугольнике | `geometry/InsidePolygon.h` | |
| выпуклая оболочка | `geometry/ConvexHull.h` | |
| пересечение отрезков | `geometry/SegmentIntersection.h` | |
| пересечение прямых | `geometry/lineIntersection.h` | |
| диаметр, самые далёкие точки | `geometry/HullDiameter.h` | |
| ближайшая пара точек | `geometry/ClosestPair.h` | |
| мин. охватывающая окружность | `geometry/MinimumEnclosingCircle.h` | |
| пересечение окружностей | `geometry/CircleIntersection.h` | |
