from math import factorial

from pyzdd import Choice, Combination, Universe, enumerate_sets


def test_combination():
    n = 4
    k = 2
    spec = Combination(n, k)
    universe = Universe(n)
    universe.zddSubset(spec)
    universe.zddReduce()

    # test items
    count = 0
    items_expect = [
        [False, False, True, True],
        [False, True, False, True],
        [False, True, True, False],
        [True, False, False, True],
        [True, False, True, False],
        [True, True, False, False],
    ]
    for items, expect in zip(enumerate_sets(universe, n), items_expect):
        assert items == expect
        count += 1

    # test count
    count_expect = factorial(n) // factorial(k) // factorial(n - k)
    assert count == count_expect
    assert universe.cardinality() == str(count_expect)


def test_choice():
    n = 4
    k = 2
    v = [0, 2, 3]

    spec = Choice(n, k, v, False)
    universe = Universe(n)
    universe.zddSubset(spec)
    universe.zddReduce()

    # test items
    count = 0
    items_expect = [
        [False, False, True, True],
        [False, True, True, True],
        [True, False, False, True],
        [True, False, True, False],
        [True, True, False, True],
        [True, True, True, False],
    ]
    for items, expect in zip(enumerate_sets(universe, n), items_expect):
        assert items == expect
        count += 1

    # test count
    count_expect = factorial(len(v)) // factorial(k) // factorial(len(v) - k) * (2 ** (n - len(v)))
    assert count == count_expect
    assert universe.cardinality() == str(count_expect)


def test_dd_copy():
    n = 10
    k = 4
    spec = Combination(n, k)
    dd = Universe(n)
    dd.zddSubset(spec)
    dd.zddReduce()

    dd2 = Universe(dd)
    assert dd2 == dd
