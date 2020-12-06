#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <cassert>

#include <tdzdd/DdSpec.hpp>
#include <tdzdd/DdStructure.hpp>

#include "type.hpp"
#include "permutation.hpp"
#include "graph.hpp"
#include "short_range_order.hpp"
#include "utils.hpp"
#include "structure_enumeration.hpp"

using namespace pyzdd;
using namespace pyzdd::permutation;
using namespace pyzdd::graph;
using namespace pyzdd::derivative_structure;

int main() {
    tdzdd::MessageHandler::showMessages(false);
    int num_sites = 6;
    int num_types = 2;
    auto t1 = Permutation(std::vector<Element>{2, 0, 1, 5, 3, 4});
    auto t2 = Permutation(std::vector<Element>{3, 4, 5, 0, 1, 2});
    auto m = Permutation(std::vector<Element>{0, 2, 1, 3, 5, 4});
    auto automorphism = generate_group(std::vector<Permutation>{t1, t2, m});
    auto translations = generate_group(std::vector<Permutation>{t1, t2});
    assert(automorphism.size() == 12);

    // composition constraints
    tdzdd::DdStructure<2> dd;
    /*
    pyzdd::derivative_structure::construct_derivative_structures(
        dd,
        num_sites,
        num_types,
        automorphism,
        translations,
        std::vector<int>(),
        std::vector<std::vector<permutation::Element>>(),
        false,
        false
    );
    dd.dumpDot(std::cerr);
    */

    std::vector<std::pair<std::vector<int>, int>> composition_constraints = {
        std::make_pair(std::vector<int>{0, 1, 2, 3, 4, 5}, 3)
    };

    int num_variables = num_sites;
    Graph cluster_graph(num_variables);
    add_undirected_edge(cluster_graph, 0, 1, 1);
    add_undirected_edge(cluster_graph, 0, 3, 1);
    add_undirected_edge(cluster_graph, 1, 2, 1);
    add_undirected_edge(cluster_graph, 1, 5, 1);
    add_undirected_edge(cluster_graph, 2, 0, 1);
    add_undirected_edge(cluster_graph, 2, 3, 1);
    // auto vertex_order = get_vertex_order_by_bfs(cluster_graph);
    std::vector<pyzdd::graph::Vertex> vertex_order = {0, 1, 2, 3, 4, 5};
    for (auto v: vertex_order) {
        std::cerr << " " << v;
    }
    std::cerr << std::endl;

    int target = 3;

    pyzdd::derivative_structure::prepare_derivative_structures_with_sro(
        dd,
        num_sites,
        num_types,
        vertex_order,
        automorphism,
        composition_constraints
    );

    std::ofstream os1("example.structure.dot");
    dd.dumpDot(os1);

    /*
    pyzdd::derivative_structure::restrict_pair_correlation(
        dd,
        num_sites,
        num_types,
        vertex_order,
        cluster_graph,
        target
    );
    */

    return 0;
}
