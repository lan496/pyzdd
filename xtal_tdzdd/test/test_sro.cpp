#include <cassert>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include <gtest/gtest.h>
#include <tdzdd/DdSpec.hpp>
#include <tdzdd/DdStructure.hpp>

#include "graph.hpp"
#include "permutation.hpp"
#include "short_range_order.hpp"
#include "type.hpp"
#include "utils.hpp"

using namespace pyzdd;
using namespace pyzdd::permutation;
using namespace pyzdd::graph;
using namespace pyzdd::derivative_structure;

void check(int num_sites, int num_types, const tdzdd::DdStructure<2>& dd,
           const std::vector<Vertex>& vertex_order,
           const std::string cardinality_expect,
           const std::vector<std::vector<int>>& enumerated_expect) {
    auto cardinality_actual = dd.zddCardinality();
    std::cerr << "# of structures: " << cardinality_actual << std::endl;
    if (cardinality_actual != cardinality_expect) {
        std::cerr << "The cardinality is wrong: (actual, expect) = ("
                  << cardinality_actual << ", " << cardinality_expect << ")"
                  << std::endl;
        exit(1);
    }

    std::unordered_set<std::vector<int>, VectorHash<int>> uset_expect;
    for (auto labeling : enumerated_expect) {
        uset_expect.insert(labeling);
    }

    std::unordered_set<std::vector<int>, VectorHash<int>> uset_actual;
    auto converter = VertexConverter(num_sites, vertex_order);
    for (auto itr = dd.begin(), end = dd.end(); itr != end; ++itr) {
        auto labeling = converter.retrieve_vertices(*itr);
        uset_actual.insert(labeling);
    }
    assert(uset_actual == uset_expect);
}

TEST(SroTest, BinaryTest) {
    int num_sites = 4;
    int num_types = 2;
    auto c4 = Permutation(std::vector<Element>{1, 2, 3, 0});
    auto m = Permutation(std::vector<Element>{3, 2, 1, 0});
    auto automorphism = generate_group(std::vector<Permutation>{c4, m});
    assert(automorphism.size() == 8);
    auto translations = generate_group(std::vector<Permutation>{c4});
    assert(translations.size() == 4);

    int num_variables = num_sites;

    // composition constraints
    {
        tdzdd::DdStructure<2> dd;

        std::vector<std::pair<std::vector<int>, int>> composition_constraints =
            {std::make_pair(std::vector<int>{0, 1, 2, 3}, 2)};

        Graph cluster_graph(num_variables);
        add_undirected_edge(cluster_graph, 0, 1, 2);
        add_undirected_edge(cluster_graph, 1, 2, 2);
        add_undirected_edge(cluster_graph, 2, 3, 2);
        add_undirected_edge(cluster_graph, 3, 0, 2);
        auto vertex_order = get_vertex_order_by_bfs(cluster_graph);

        std::vector<Graph> graphs = {
            cluster_graph,
        };
        std::vector<Weight> targets = {2};

        construct_derivative_structures_with_sro(
            dd, num_sites, num_types, vertex_order, automorphism, translations,
            composition_constraints, graphs, targets,
            true,  // remove superperiodic
            false  // not useMP
        );

        std::string cardinality_expect = "1";
        std::vector<std::vector<int>> enumerated_expect = {
            {0, 0, 1, 1},
        };
        check(num_sites, num_types, dd, vertex_order, cardinality_expect,
              enumerated_expect);
    }
}

int ravel(int site, int specie, int num_types) {
    return site * num_types + specie;
}

Permutation ravel_permutation(const Permutation& perm, int num_types) {
    size_t num_sites = perm.get_size();
    std::vector<Element> sigma(num_sites * num_types);
    for (int specie = 0; specie < num_types; ++specie) {
        for (size_t site = 0; site < num_sites; ++site) {
            sigma[ravel(site, specie, num_types)] =
                ravel(perm.permute(site), specie, num_types);
        }
    }
    auto ret = Permutation(sigma);
    return ret;
}

TEST(SroTest, TernaryTest) {
    int num_sites = 8;
    int num_types = 3;

    // symmetry
    auto t1 = Permutation(std::vector<Element>{1, 2, 3, 0, 5, 6, 7, 4});
    auto t2 = Permutation(std::vector<Element>{4, 5, 6, 7, 0, 1, 2, 3});
    auto m = Permutation(std::vector<Element>{0, 3, 2, 1, 4, 7, 6, 5});

    // one-hot encoding
    auto t1_aug = ravel_permutation(t1, num_types);
    auto t2_aug = ravel_permutation(t2, num_types);
    auto m_aug = ravel_permutation(m, num_types);
    auto automorphism =
        generate_group(std::vector<Permutation>{t1_aug, t2_aug, m_aug});
    assert(automorphism.size() == 16);
    auto translations =
        generate_group(std::vector<Permutation>{t1_aug, t2_aug});
    assert(translations.size() == 8);

    // composition constraints
    std::vector<int> composition = {4, 2, 2};
    std::vector<std::pair<std::vector<int>, int>> composition_constraints;
    for (int specie = 0; specie < num_types; ++specie) {
        std::vector<int> group;
        for (int site = 0; site < num_sites; ++site) {
            group.emplace_back(ravel(site, specie, num_types));
        }
        composition_constraints.emplace_back(
            std::make_pair(group, composition[specie]));
    }

    // SRO constraint
    Graph g(num_sites);
    add_undirected_edge(g, 0, 1, 1);
    add_undirected_edge(g, 0, 4, 2);
    add_undirected_edge(g, 1, 2, 1);
    add_undirected_edge(g, 1, 5, 2);
    add_undirected_edge(g, 2, 3, 1);
    add_undirected_edge(g, 2, 6, 2);
    add_undirected_edge(g, 3, 0, 1);
    add_undirected_edge(g, 3, 7, 2);
    add_undirected_edge(g, 4, 5, 1);
    add_undirected_edge(g, 5, 6, 1);
    add_undirected_edge(g, 6, 7, 1);
    add_undirected_edge(g, 7, 4, 1);

    int num_variables = num_sites * num_types;
    std::vector<Weight> targets = {4, 4, 2};
    std::vector<Graph> cluster_graphs;
    for (int specie0 = 0; specie0 < num_types; ++specie0) {
        for (int specie1 = specie0 + 1; specie1 < num_types; ++specie1) {
            Graph cg(num_variables);
            for (int u = 0; u < num_sites; ++u) {
                for (Edge e : g[u]) {
                    cg[ravel(u, specie0, num_types)].emplace_back(
                        Edge(ravel(u, specie0, num_types),
                             ravel(e.dst, specie1, num_types), e.weight));
                    cg[ravel(u, specie1, num_types)].emplace_back(
                        Edge(ravel(u, specie1, num_types),
                             ravel(e.dst, specie0, num_types), e.weight));
                }
            }
            cluster_graphs.emplace_back(cg);
        }
    }

    auto vertex_order = get_vertex_order_by_bfs(cluster_graphs[0]);

    tdzdd::DdStructure<2> dd;

    construct_derivative_structures_with_sro(
        dd, num_sites, num_types, vertex_order, automorphism, translations,
        composition_constraints, cluster_graphs, targets,
        true,  // remove superperiodic
        false);

    // return labelings
    auto converter = VertexConverter(num_variables, vertex_order);
    // 0 0 1 0 0 1 1 0 0 1 0 0 1 0 0 0 1 0 0 1 0 1 0 0
    // 2     2     0     0     0     1     1     0
    // 0 0 1 0 0 1 0 1 0 0 1 0 1 0 0 1 0 0 1 0 0 1 0 0
    // 2     2     1     1     0     0     0     0
    for (auto itr = dd.begin(), end = dd.end(); itr != end; ++itr) {
        std::vector<int> labeling =
            convert_to_binary_labeling_with_graph(itr, converter);
    }
    assert(std::stoi(dd.zddCardinality()) >= 1);
}
