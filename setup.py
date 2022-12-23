import os
from pathlib import Path
import subprocess
from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
import sys


with open(Path(__file__).resolve().parent / "README.md") as f:
    long_description = "\n" + f.read()


# Adapted from https://github.com/pybind/cmake_example

# A CMakeExtension needs a sourcedir instead of a file list.
# The name must be the _single_ output extension from the CMake build.
# If you need multiple extensions, see scikit-build.
class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))

        # required for auto-detection of auxiliary "native" libs
        if not extdir.endswith(os.path.sep):
            extdir += os.path.sep

        cfg = "Debug" if self.debug else "Release"

        # Set Python_EXECUTABLE instead if you use PYBIND11_FINDPYTHON
        # EXAMPLE_VERSION_INFO shows you how to pass a value into the C++ code
        # from Python.
        cmake_args = [
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}",
            f"-DPYTHON_EXECUTABLE={sys.executable}",
            f"-DCMAKE_BUILD_TYPE={cfg}",  # not used on MSVC, but no harm
        ]
        build_args = []

        # Set CMAKE_BUILD_PARALLEL_LEVEL to control the parallel build level
        # across all generators.
        if "CMAKE_BUILD_PARALLEL_LEVEL" not in os.environ:
            # self.parallel is a Python 3 only way to set parallel jobs by hand
            # using -j in the build_ext call, not supported by pip or PyPA-build.
            if hasattr(self, "parallel") and self.parallel:
                # CMake 3.12+ only.
                build_args += ["-j{}".format(self.parallel)]

        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)

        subprocess.check_call(["cmake", ext.sourcedir] + cmake_args, cwd=self.build_temp)
        subprocess.check_call(["cmake", "--build", "."] + build_args, cwd=self.build_temp)


setup(
    name="pyzdd",
    license="MIT",
    description="Python wrapper to TdZdd",
    long_description=long_description,
    long_description_content_type="text/markdown",
    author="Kohei Shinohara",
    author_email="kohei19950508@gmail.com",
    package_dir={"": "src"},
    packages=find_packages("src"),
    python_requires=">=3.8",
    setup_requires=["setuptools_scm"],
    install_requires=[
        "setuptools",
        "setuptools_scm",
        "wheel",
        "networkx>=2.0",
    ],
    extras_require={
        "dev": [
            "cibuildwheel",
            "ipython",
            "pre-commit",
            "black",
            "versioneer",
            "pytest",
            "pytest-cov",
            "flake8",
            "mypy",
            "isort",
            "pyupgrade",
        ],
    },
    tests_require=["pytest"],
    ext_modules=[CMakeExtension("pyzdd_", sourcedir="./")],
    cmdclass={"build_ext": CMakeBuild},
    include_package_data=True,
    zip_safe=False,
    use_scm_version=True,
    classifiers=[
        # Trove classifiers
        # Full list: https://pypi.python.org/pypi?%3Aaction=list_classifiers
        "Development Status :: 2 - Pre-Alpha",
        "Intended Audience :: Science/Research",
        "License :: OSI Approved :: MIT License",
        "Operating System :: POSIX :: Linux",
        "Operating System :: MacOS",
        "Programming Language :: Python",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Topic :: Scientific/Engineering",
    ],
    url="https://github.com/lan496/pyzdd",
)
