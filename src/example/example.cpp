#include <cassert>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include <tdzdd/DdSpec.hpp>
#include <tdzdd/DdStructure.hpp>

#include "graph.hpp"
#include "permutation.hpp"
#include "short_range_order.hpp"
#include "spec/combination.hpp"
#include "structure_enumeration.hpp"
#include "type.hpp"
#include "utils.hpp"

using namespace pyzdd;
using namespace pyzdd::permutation;
using namespace pyzdd::graph;
using namespace pyzdd::combination;
using namespace pyzdd::derivative_structure;

int main() {
    tdzdd::MessageHandler::showMessages(false);

    // system
    int num_sites = 6;
    int num_types = 2;
    auto t1 = Permutation(std::vector<Element>{2, 0, 1, 5, 3, 4});
    auto t2 = Permutation(std::vector<Element>{3, 4, 5, 0, 1, 2});
    auto m = Permutation(std::vector<Element>{0, 2, 1, 3, 5, 4});
    auto automorphism = generate_group(std::vector<Permutation>{t1, t2, m});
    assert(automorphism.size() == 12);

    // cluster graph
    int num_variables = num_sites;
    Graph cluster_graph(num_variables);
    add_undirected_edge(cluster_graph, 0, 1, 1);
    add_undirected_edge(cluster_graph, 0, 3, 2);
    add_undirected_edge(cluster_graph, 1, 2, 1);
    add_undirected_edge(cluster_graph, 1, 4, 2);
    add_undirected_edge(cluster_graph, 2, 0, 1);
    add_undirected_edge(cluster_graph, 2, 5, 2);
    add_undirected_edge(cluster_graph, 3, 4, 1);
    add_undirected_edge(cluster_graph, 3, 5, 1);
    add_undirected_edge(cluster_graph, 4, 5, 1);

    // not use BFS
    // auto vertex_order = get_vertex_order_by_bfs(cluster_graph);
    std::vector<Vertex> vertex_order = {0, 1, 2, 3, 4, 5};
    for (auto v : vertex_order) {
        std::cerr << " " << v;
    }
    std::cerr << std::endl;

    int target = 3;

    // full structures
    {
        VertexConverter converter(num_sites, vertex_order);
        std::vector<Permutation> automorphism_aug;
        for (const auto& perm : automorphism) {
            auto aug = converter.reorder_premutation(perm);
            automorphism_aug.emplace_back(aug);
        }

        tdzdd::DdStructure<2> dd0;
        std::vector<Permutation> translations_aug;
        pyzdd::derivative_structure::construct_derivative_structures(
            dd0, num_sites, num_types, automorphism_aug,
            std::vector<Permutation>(), std::vector<int>(),
            std::vector<std::vector<permutation::Element>>(),
            false,  // incomplete
            false   // superperiodic
        );

        std::ofstream os("full.raw.dot");
        dd0.dumpDot(os);
    }

    // AB composition with duplicates
    {
        Combination spec(6, 3);
        tdzdd::DdStructure<2> dd(spec);
        dd.zddReduce();
        std::ofstream os("AB_duplicates.raw.dot");
        dd.dumpDot(os);
    }

    // composition constraints
    tdzdd::DdStructure<2> dd;
    std::vector<std::pair<std::vector<int>, int>> composition_constraints = {
        std::make_pair(std::vector<int>{0, 1, 2, 3, 4, 5}, 3)};

    pyzdd::derivative_structure::prepare_derivative_structures_with_sro(
        dd, num_sites, num_types, vertex_order, automorphism,
        composition_constraints, false);
    std::ofstream os1("AB.raw.dot");
    dd.dumpDot(os1);

    pyzdd::derivative_structure::restrict_pair_correlation(
        dd, num_sites, num_types, vertex_order, cluster_graph, target);
    std::ofstream os2("AB_1stNN.raw.dot");
    dd.dumpDot(os2);

    return 0;
}
