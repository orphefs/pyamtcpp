pyamtcpp
==============

|      CI              | status |
|----------------------|--------|
| pip builds           | [![Pip Actions Status][actions-pip-badge]][actions-pip-link] |

[actions-pip-link]:        https://github.com/orphefs/pyamtcpp/actions?query=workflow%3A%22Pip
[actions-pip-badge]:       https://github.com/orphefs/pyamtcpp/workflows/Pip/badge.svg

A [pybind11](https://github.com/pybind/pybind11) module built with a
CMake-based build system, based on [this](https://github.com/pybind/cmake_example) template.

Installation
------------

Just clone this repository and pip install. Note the `--recursive` option which is
needed for the pybind11 submodule:

```bash
git clone --recursive https://github.com/orphefs/pyamtcpp.git
pip install ./pyamtcpp
```

With the `setup.py` file included in this example, the `pip install` command will
invoke CMake and build the pybind11 module as specified in `CMakeLists.txt`.

CI Examples
------------

There are examples for CI in `.github/workflows`. A simple way to produces
binary "wheels" for all platforms is illustrated in the "wheels.yml" file,
using [`cibuildwheel`][]. You can also see a basic recipe for building and
testing in `pip.yml`, and `conda.yml` has an example of a conda recipe build.

Building the documentation
------------

Documentation for the example project is generated using Sphinx. Sphinx has the
ability to automatically inspect the signatures and documentation strings in
the extension module to generate beautiful documentation in a variety formats.
The following command generates HTML-based reference documentation; for other
formats please refer to the Sphinx manual:

- `cd pyamtcpp/docs`
- `make html`

License
-------

pyamtcpp is provided under a BSD-style license that can be found in the LICENSE
file. By using, distributing, or contributing to this project, you agree to the
terms and conditions of this license.

Tests
---------

First, instatiate a local `venv` environment:

```bash
python -m venv venv
source venv/bin/activate
pip install -r unpinned_requirements.txt
```

Then you can run the tests:

```bash
python tests/test.py
```

## Usage

You can import and use the module like so:

```python
import pyamtcpp
energy = pyamtcpp.compute_energy(audio, win_len, hop_len)
```

where

```python
energy: numpy.typing.NDArray[np.float32] of dimensions 2 x N
win_len: int
hop_len: int
```

[`cibuildwheel`]:          https://cibuildwheel.readthedocs.io
