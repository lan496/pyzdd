from typing import Any

import networkx as nx
from pyzdd_ import (
    Edge,
    Graph,
    GraphAuxiliary,
    VertexGraphFrontierManager,
    get_vertex_order_by_bfs,
)

__all__ = [
    "convert_to_raw_graph",
    "get_frontier_size",
    "Edge",
    "VertexGraphFrontierManager",
    "Graph",
    "GraphAuxiliary",
    "get_vertex_order_by_bfs",
]


def convert_to_raw_graph(graph: nx.Graph) -> tuple[list[list[Edge]], dict[Any, int]]:
    """
    convert nx.Graph to List[List[Edge]]

    Parameters
    ----------
    graph: nx.Graph
    mapping: map original nodes to 0-indexed relabeled ones.
    """
    mapping = {}
    for i, u in enumerate(sorted(graph.nodes)):
        mapping[u] = i
    relabeled = nx.relabel_nodes(graph, mapping)

    graph_pb = []
    for src in relabeled.nodes:
        # nx.Graph.neighbors is iterator on dict. the order of dict is fixed in python>=3.7.
        edges_pb = [
            Edge(src, dst, relabeled[src][dst].get("weight", 1))
            for dst in relabeled.neighbors(src)
        ]
        graph_pb.append(edges_pb)

    return graph_pb, mapping


def get_frontier_size(raw_graph: list[list[Edge]], vertex_order: list[int]):
    """
    return frontier size of DD for vertex-induced subgraphs
    """
    vgfm = VertexGraphFrontierManager(raw_graph, vertex_order)
    frontier_size = vgfm.get_max_frontier_size()
    return frontier_size
