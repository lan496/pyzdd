#include <gtest/gtest.h>

#include <cassert>
#include <iostream>
#include <string>
#include <tdzdd/DdSpec.hpp>
#include <unordered_set>
#include <vector>
// #include <tdzdd/DdStructure.hpp>

#include "permutation.hpp"
#include "structure_enumeration.hpp"
#include "type.hpp"

using namespace pyzdd;
using namespace pyzdd::permutation;
using namespace pyzdd::derivative_structure;

// https://stackoverflow.com/questions/29855908/c-unordered-set-of-vectors
template <typename T>
struct VectorHash {
    size_t operator()(const std::vector<T>& v) const {
        std::hash<T> hasher;
        size_t seed = 0;
        for (auto i : v) {
            seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

void check(int num_sites, int num_types, const tdzdd::DdStructure<2>& dd,
           const std::string cardinality_expect,
           const std::vector<std::vector<Element>>& enumerated_expect) {
    auto cardinality_actual = dd.zddCardinality();
    std::cerr << "# of nonequivalent structures: " << cardinality_actual
              << std::endl;
    if (cardinality_actual != cardinality_expect) {
        std::cerr << "The cardinality is wrong: (actual, expect) = ("
                  << cardinality_actual << ", " << cardinality_expect << ")"
                  << std::endl;
        exit(1);
    }

    std::unordered_set<std::vector<Element>, VectorHash<Element>> uset_expect;
    for (auto labeling : enumerated_expect) {
        uset_expect.insert(labeling);
    }
    std::unordered_set<std::vector<Element>, VectorHash<Element>> uset_actual;
    for (auto itr = dd.begin(), end = dd.end(); itr != end; ++itr) {
        auto labeling = convert_to_labeling(itr, num_sites, num_types);
        uset_actual.insert(labeling);
    }
    assert(uset_actual == uset_expect);
}

TEST(StructureEnumerationTest, BinaryTest) {
    // reproduce Fig.2
    int num_sites = 4;
    int num_types = 2;
    auto c4 = Permutation(std::vector<Element>{1, 2, 3, 0});
    auto m = Permutation(std::vector<Element>{3, 2, 1, 0});
    auto automorphism = generate_group(std::vector<Permutation>{c4, m});
    assert(automorphism.size() == 8);
    auto translations = generate_group(std::vector<Permutation>{c4});
    assert(translations.size() == 4);

    // no option
    {
        tdzdd::DdStructure<2> dd;

        std::vector<int> composition_constraints;
        std::vector<std::vector<permutation::Element>> site_constraints;
        bool remove_incomplete = false;
        bool remove_superperiodic = false;
        construct_derivative_structures(dd, num_sites, num_types, automorphism,
                                        translations, composition_constraints,
                                        site_constraints, remove_incomplete,
                                        remove_superperiodic);

        std::string cardinality_expect = "6";
        std::vector<std::vector<Element>> enumerated_expect = {
            {0, 0, 0, 0}, {0, 0, 0, 1}, {0, 1, 0, 1},
            {0, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 1},
        };
        check(num_sites, num_types, dd, cardinality_expect, enumerated_expect);
    }

    // remove_incomplete
    {
        tdzdd::DdStructure<2> dd;

        std::vector<int> composition_constraints;
        std::vector<std::vector<permutation::Element>> site_constraints;
        bool remove_incomplete = true;
        bool remove_superperiodic = false;
        construct_derivative_structures(dd, num_sites, num_types, automorphism,
                                        translations, composition_constraints,
                                        site_constraints, remove_incomplete,
                                        remove_superperiodic);
        std::string cardinality_expect = "4";
        std::vector<std::vector<Element>> enumerated_expect = {
            {0, 0, 0, 1},
            {0, 1, 0, 1},
            {0, 0, 1, 1},
            {0, 1, 1, 1},
        };
        check(num_sites, num_types, dd, cardinality_expect, enumerated_expect);
    }

    // remove superperiodic
    {
        tdzdd::DdStructure<2> dd;

        std::vector<int> composition_constraints;
        std::vector<std::vector<permutation::Element>> site_constraints;
        bool remove_incomplete = false;
        bool remove_superperiodic = true;
        construct_derivative_structures(dd, num_sites, num_types, automorphism,
                                        translations, composition_constraints,
                                        site_constraints, remove_incomplete,
                                        remove_superperiodic);

        std::string cardinality_expect = "3";
        std::vector<std::vector<Element>> enumerated_expect = {
            {0, 0, 0, 1},
            {0, 0, 1, 1},
            {0, 1, 1, 1},
        };
        check(num_sites, num_types, dd, cardinality_expect, enumerated_expect);
    }

    // remove incomplete and superperiodic
    {
        tdzdd::DdStructure<2> dd;

        std::vector<int> composition_constraints;
        std::vector<std::vector<permutation::Element>> site_constraints;
        bool remove_incomplete = true;
        bool remove_superperiodic = true;
        construct_derivative_structures(dd, num_sites, num_types, automorphism,
                                        translations, composition_constraints,
                                        site_constraints, remove_incomplete,
                                        remove_superperiodic);

        std::string cardinality_expect = "3";
        std::vector<std::vector<Element>> enumerated_expect = {
            {0, 0, 0, 1},
            {0, 0, 1, 1},
            {0, 1, 1, 1},
        };
        check(num_sites, num_types, dd, cardinality_expect, enumerated_expect);
    }

    // composition constraints
    {
        tdzdd::DdStructure<2> dd;

        std::vector<int> composition_constraints = {3, 1};
        std::vector<std::vector<permutation::Element>> site_constraints;
        bool remove_incomplete = false;
        bool remove_superperiodic = false;
        construct_derivative_structures(dd, num_sites, num_types, automorphism,
                                        translations, composition_constraints,
                                        site_constraints, remove_incomplete,
                                        remove_superperiodic);

        std::string cardinality_expect = "1";
        std::vector<std::vector<Element>> enumerated_expect = {
            {0, 0, 0, 1},
        };
        check(num_sites, num_types, dd, cardinality_expect, enumerated_expect);
    }

    // site constraints 0
    {
        tdzdd::DdStructure<2> dd;

        std::vector<int> composition_constraints;
        std::vector<std::vector<permutation::Element>> site_constraints = {
            {0},
            {0},
            {0},
            {0},
        };
        bool remove_incomplete = false;
        bool remove_superperiodic = false;
        construct_derivative_structures(dd, num_sites, num_types, automorphism,
                                        translations, composition_constraints,
                                        site_constraints, remove_incomplete,
                                        remove_superperiodic);

        std::string cardinality_expect = "1";
        std::vector<std::vector<Element>> enumerated_expect = {
            {1, 1, 1, 1},
        };
        check(num_sites, num_types, dd, cardinality_expect, enumerated_expect);
    }

    // site constraints 1
    {
        tdzdd::DdStructure<2> dd;

        std::vector<int> composition_constraints;
        std::vector<std::vector<permutation::Element>> site_constraints = {
            {1},
            {1},
            {1},
            {1},
        };
        bool remove_incomplete = false;
        bool remove_superperiodic = false;
        construct_derivative_structures(dd, num_sites, num_types, automorphism,
                                        translations, composition_constraints,
                                        site_constraints, remove_incomplete,
                                        remove_superperiodic);

        std::string cardinality_expect = "1";
        std::vector<std::vector<Element>> enumerated_expect = {
            {0, 0, 0, 0},
        };
        check(num_sites, num_types, dd, cardinality_expect, enumerated_expect);
    }
}

TEST(StructureEnumerationTest, TernaryTest) {
    // reproduce Fig.5(b)
    int num_sites = 4;
    int num_types = 3;
    auto c4 = Permutation(std::vector<Element>{1, 2, 3, 0});
    auto m = Permutation(std::vector<Element>{3, 2, 1, 0});
    auto automorphism = generate_group(std::vector<Permutation>{c4, m});
    assert(automorphism.size() == 8);
    auto translations = generate_group(std::vector<Permutation>{c4});
    assert(translations.size() == 4);

    // no option
    {
        tdzdd::DdStructure<2> dd;

        std::vector<int> composition_constraints;
        std::vector<std::vector<permutation::Element>> site_constraints;
        bool remove_incomplete = false;
        bool remove_superperiodic = false;
        construct_derivative_structures(dd, num_sites, num_types, automorphism,
                                        translations, composition_constraints,
                                        site_constraints, remove_incomplete,
                                        remove_superperiodic);

        std::string cardinality_expect = "21";
        std::vector<std::vector<Element>> enumerated_expect = {
            {0, 0, 0, 0},
            {1, 1, 1, 1},
            {2, 2, 2, 2},
            //
            {1, 1, 1, 0},
            {1, 1, 0, 0},
            {1, 0, 1, 0},
            {1, 0, 0, 0},
            //
            {2, 2, 2, 0},
            {2, 2, 0, 0},
            {2, 0, 2, 0},
            {2, 0, 0, 0},
            //
            {2, 2, 2, 1},
            {2, 2, 1, 1},
            {2, 1, 2, 1},
            {2, 1, 1, 1},
            //
            {2, 2, 1, 0},  // (2, 2, 0, 1)
            {2, 1, 2, 0},  // (2, 0, 2, 1)
            {2, 1, 1, 0},  // (2, 0, 1, 1)
            {2, 1, 0, 1},
            {2, 0, 1, 0},
            {2, 1, 0, 0},  // (2, 0, 0, 1)
        };
        check(num_sites, num_types, dd, cardinality_expect, enumerated_expect);
    }

    // remove incomplete
    {
        tdzdd::DdStructure<2> dd;

        std::vector<int> composition_constraints;
        std::vector<std::vector<permutation::Element>> site_constraints;
        bool remove_incomplete = true;
        bool remove_superperiodic = false;
        construct_derivative_structures(dd, num_sites, num_types, automorphism,
                                        translations, composition_constraints,
                                        site_constraints, remove_incomplete,
                                        remove_superperiodic);

        std::string cardinality_expect = "6";
        std::vector<std::vector<Element>> enumerated_expect = {
            {2, 2, 1, 0},                              // (2, 2, 0, 1)
            {2, 1, 2, 0},                              // (2, 0, 2, 1)
            {2, 1, 1, 0},                              // (2, 0, 1, 1)
            {2, 1, 0, 1}, {2, 0, 1, 0}, {2, 1, 0, 0},  // (2, 0, 0, 1)
        };
        check(num_sites, num_types, dd, cardinality_expect, enumerated_expect);
    }

    // remove superperiodic
    {
        tdzdd::DdStructure<2> dd;

        std::vector<int> composition_constraints;
        std::vector<std::vector<permutation::Element>> site_constraints;
        bool remove_incomplete = false;
        bool remove_superperiodic = true;
        construct_derivative_structures(dd, num_sites, num_types, automorphism,
                                        translations, composition_constraints,
                                        site_constraints, remove_incomplete,
                                        remove_superperiodic);

        std::string cardinality_expect = "15";
        std::vector<std::vector<Element>> enumerated_expect = {
            {1, 1, 1, 0},
            {1, 1, 0, 0},
            {1, 0, 0, 0},
            //
            {2, 2, 2, 0},
            {2, 2, 0, 0},
            {2, 0, 0, 0},
            //
            {2, 2, 2, 1},
            {2, 2, 1, 1},
            {2, 1, 1, 1},
            //
            {2, 2, 1, 0},  // (2, 2, 0, 1)
            {2, 1, 2, 0},  // (2, 0, 2, 1)
            {2, 1, 1, 0},  // (2, 0, 1, 1)
            {2, 1, 0, 1},
            {2, 0, 1, 0},
            {2, 1, 0, 0},  // (2, 0, 0, 1)
        };
        check(num_sites, num_types, dd, cardinality_expect, enumerated_expect);
    }

    // remove incomplete and superperiodic
    {
        tdzdd::DdStructure<2> dd;

        std::vector<int> composition_constraints;
        std::vector<std::vector<permutation::Element>> site_constraints;
        bool remove_incomplete = true;
        bool remove_superperiodic = true;
        construct_derivative_structures(dd, num_sites, num_types, automorphism,
                                        translations, composition_constraints,
                                        site_constraints, remove_incomplete,
                                        remove_superperiodic);

        std::string cardinality_expect = "6";
        std::vector<std::vector<Element>> enumerated_expect = {
            {2, 2, 1, 0},                              // (2, 2, 0, 1)
            {2, 1, 2, 0},                              // (2, 0, 2, 1)
            {2, 1, 1, 0},                              // (2, 0, 1, 1)
            {2, 1, 0, 1}, {2, 0, 1, 0}, {2, 1, 0, 0},  // (2, 0, 0, 1)
        };
        check(num_sites, num_types, dd, cardinality_expect, enumerated_expect);
    }

    // remove superperiodic
    {
        tdzdd::DdStructure<2> dd;

        std::vector<int> composition_constraints;
        std::vector<std::vector<permutation::Element>> site_constraints;
        bool remove_incomplete = false;
        bool remove_superperiodic = true;
        construct_derivative_structures(dd, num_sites, num_types, automorphism,
                                        translations, composition_constraints,
                                        site_constraints, remove_incomplete,
                                        remove_superperiodic);

        std::string cardinality_expect = "15";
        std::vector<std::vector<Element>> enumerated_expect = {
            {1, 1, 1, 0},
            {1, 1, 0, 0},
            {1, 0, 0, 0},
            //
            {2, 2, 2, 0},
            {2, 2, 0, 0},
            {2, 0, 0, 0},
            //
            {2, 2, 2, 1},
            {2, 2, 1, 1},
            {2, 1, 1, 1},
            //
            {2, 2, 1, 0},  // (2, 2, 0, 1)
            {2, 1, 2, 0},  // (2, 0, 2, 1)
            {2, 1, 1, 0},  // (2, 0, 1, 1)
            {2, 1, 0, 1},
            {2, 0, 1, 0},
            {2, 1, 0, 0},  // (2, 0, 0, 1)
        };
        check(num_sites, num_types, dd, cardinality_expect, enumerated_expect);
    }
    // composition constraints
    {
        tdzdd::DdStructure<2> dd;

        std::vector<int> composition_constraints = {1, 1, 2};
        std::vector<std::vector<permutation::Element>> site_constraints;
        bool remove_incomplete = false;
        bool remove_superperiodic = false;
        construct_derivative_structures(dd, num_sites, num_types, automorphism,
                                        translations, composition_constraints,
                                        site_constraints, remove_incomplete,
                                        remove_superperiodic);

        std::string cardinality_expect = "2";
        std::vector<std::vector<Element>> enumerated_expect = {
            {2, 2, 1, 0},  // (2, 2, 0, 1)
            {2, 1, 2, 0},  // (2, 0, 2, 1)
        };
        check(num_sites, num_types, dd, cardinality_expect, enumerated_expect);
    }

    // site constraints
    {
        tdzdd::DdStructure<2> dd;

        std::vector<int> composition_constraints;
        std::vector<std::vector<permutation::Element>> site_constraints = {
            {1},
            {1},
            {1},
            {1},
        };
        bool remove_incomplete = false;
        bool remove_superperiodic = false;
        construct_derivative_structures(dd, num_sites, num_types, automorphism,
                                        translations, composition_constraints,
                                        site_constraints, remove_incomplete,
                                        remove_superperiodic);

        std::string cardinality_expect = "6";
        std::vector<std::vector<Element>> enumerated_expect = {
            {0, 0, 0, 0},
            {2, 2, 2, 2},
            //
            {2, 2, 2, 0},
            {2, 2, 0, 0},
            {2, 0, 2, 0},
            {2, 0, 0, 0},
        };
        check(num_sites, num_types, dd, cardinality_expect, enumerated_expect);
    }
}
