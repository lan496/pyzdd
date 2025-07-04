from collections.abc import Generator

from pyzdd_ import (
    BinaryVertexConverter,
    construct_derivative_structures,
    construct_derivative_structures_with_sro,
    convert_to_binary_labeling_with_graph,
    convert_to_labeling,
    prepare_derivative_structures_with_sro,
    remove_superperiodic_structures,
    restrict_pair_correlation,
)

__all__ = [
    "BinaryVertexConverter",
    "construct_derivative_structures",
    "construct_derivative_structures_with_sro",
    "convert_to_binary_labeling_with_graph",
    "convert_to_labeling",
    "prepare_derivative_structures_with_sro",
    "remove_superperiodic_structures",
    "restrict_pair_correlation",
    "enumerate_labelings",
    "enumerate_binary_labelings_with_graph",
]

from pyzdd import Universe


def enumerate_labelings(dd: Universe, num_sites, num_types) -> Generator[list[int], None, None]:
    """
    yield labeling of derivative structure

    Parameters
    ----------
    dd: DD for enumerating derivative structures
    num_sites: the number of sites in supercell
    num_types: the number of kinds of species

    Returns
    -------
    labeling
    """
    if not isinstance(dd, Universe):
        TypeError(f"Given type is not Universe but {type(dd)}")

    itr = dd.begin()
    end = dd.end()
    while itr != end:
        labeling = convert_to_labeling(itr, num_sites, num_types)
        yield labeling
        itr.next()


def enumerate_binary_labelings_with_graph(
    dd: Universe, num_sites: int, vertex_order: list[int]
) -> Generator[list[int], None, None]:
    """
    yield labeling of derivative structure with a cluster graph

    Parameters
    ----------
    dd: DD for enumerating derivative structures
    num_sites: the number of sites in supercell
    vertex_order

    Returns
    -------
    labeling
    """
    if not isinstance(dd, Universe):
        TypeError(f"Given type is not Universe but {type(dd)}")

    itr = dd.begin()
    end = dd.end()

    num_variables = len(vertex_order)

    converter = BinaryVertexConverter(num_variables, vertex_order)
    while itr != end:
        labeling = convert_to_binary_labeling_with_graph(itr, converter)
        yield labeling
        itr.next()
