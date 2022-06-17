# pyzdd
[![testing](https://github.com/lan496/pyzdd/actions/workflows/testing.yml/badge.svg)](https://github.com/lan496/pyzdd/actions/workflows/testing.yml)
[![pre-commit.ci status](https://results.pre-commit.ci/badge/github/lan496/pyzdd/main.svg?badge_token=MU26PgVHQe-LRTPsqN6olg)](https://results.pre-commit.ci/latest/github/lan496/pyzdd/main?badge_token=MU26PgVHQe-LRTPsqN6olg)
[![MIT License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)
![PyPI - Python Version](https://img.shields.io/pypi/pyversions/pyzdd)
[![PyPI version](https://badge.fury.io/py/pyzdd.svg)](https://badge.fury.io/py/pyzdd)
![PyPI - Downloads](https://img.shields.io/pypi/dm/pyzdd)

Python wrapper to TdZdd

- Github: https://github.com/lan496/pyzdd
- PyPI: https://pypi.org/project/pyzdd/

## Installation

pyzdd works with Python3.8+ and can be installed via PyPI:

```shell
pip install pyzdd
```

or in local:

```shell
git clone git@github.com:lan496/pyzdd.git
cd pyzdd
pip install -e .
```

## How to cite pyzdd

If you use this package in your research, please cite TdZDD as follows.

```
@techreport{Iwashita13,
  author = {Hiroaki Iwashita and Shinichi Minato},
  memo = {Efficient Top-Down {ZDD} Construction Techniques Using Recursive Specifications},
  year = {2013},
  number = {TCS-TRA-1369,
  INSTITUTION = {Graduate School of Information Science and Technology, Hokkaido University}
}
```

The citation for the isomorphism-elimination DD is as follows.

```
@inproceedings{Horiyama2018,
  memo ={Isomorphism Elimination by Zero-Suppressed Binary Decision Diagrams},
  author={Takashi Horiyama and Masahiro Miyasaka and Riku Sasaki},
  booktitle={the Canadian Conference on Computational Geometry},
  pages={360--366},
  address={Winnipeg, Manitoba, Canada}
  year={2018},
  url={http://www.cs.umanitoba.ca/~cccg2018/papers/session7B-p2.pdf}
}
```

## Development

### Installation

```shell
./clean.sh && pip install -e ".[dev]"
pre-commit install
```

### Testing

```shell
cd xtal_tdzdd/test
cmake -S . -B build
cmake --build build -j 32
cd build && ctest -vv
```

### Write Custom Specification
1. Write a TdZdd-specification in `src/spec/*.hpp`
2. Let the new specification class be `A`, wrap the following classes and methods in `src/wrapper.cpp`
    - `tdzdd::DdSpecBase<A, 2>`
    - `tdzdd::DdSpec<A, T, 2>`
    - `A`
    - `tdzdd::DdStructure<2>::zddSubset<A>`
3. import `_pyzdd.A` in `pyzdd/__init__.py`

## References
- https://github.com/kunisura/TdZdd
- https://github.com/junkawahara/frontier_basic_tdzdd
