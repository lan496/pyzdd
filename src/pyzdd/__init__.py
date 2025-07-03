from __future__ import annotations

from collections.abc import Generator

from pyzdd_ import (
    Choice,
    Combination,
    Permutation,
    Universe,
    generate_permutation_group,
    variable_choice,
)

from pyzdd._version import __version__

__all__ = [
    "__version__",
    "Choice",
    "Combination",
    "Permutation",
    "Universe",
    "generate_permutation_group",
    "variable_choice",
    "enumerate_sets",
]


def enumerate_sets(universe: Universe, n: int) -> Generator[set[int], None, None]:
    """
    yield combinations

    Parameters
    ----------
    universe
    n: the number of variables

    Returns
    -------
    items: set of selected levels(1-indexed)
    """
    if not isinstance(universe, Universe):
        TypeError(f"Given type is not Universe but {type(universe)}")

    itr = universe.begin()
    end = universe.end()
    while itr != end:
        choice = variable_choice(itr, n)
        yield choice
        itr.next()
