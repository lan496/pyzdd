# pyzdd
Python wrapper to TdZdd

## Installation

```script
git clone --recursive git@github.com:lan496/pyzdd.git
```

## Development

### Installation
```
./clean.sh && pip install -e ".[dev]"
pre-commit install
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
