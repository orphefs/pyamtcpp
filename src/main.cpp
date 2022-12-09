#include <math.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <functional>
#include <iostream>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

py::array_t<uint16_t> compute_energy(const py::array_t<float_t> &image,
                                     const int hop_len, const int win_len) {
  std::cout << "Hello World!" << std::endl;
  return image;
}

PYBIND11_MODULE(pyoniip, m) {
  m.doc() = R"pbdoc(
        pyoniip
        -----------------------
        .. currentmodule:: pyoniip
        .. autosummary::
           :toctree: _generate
           compute_energy
    )pbdoc";

  m.def("compute_energy", &compute_energy, R"pbdoc(
        Pixel-wise mean imputation of an image

        This function takes as input two numpy arrays: 
         image[][]: uint16
         calibrationImage[][]: float

         of the same dimensions.
         The algorithm traverses the calibrationImage on a 
         pixel-by-pixel basis, looking for negative values. 
         A value average of nearest-neighbour (valid) pixels relative
         to the query pixel image[i,j] is then computed and imputed into
         image[i,j]. calibrationImage[i,j] is then set to 0.0 to indicate
         a corrected measurement. Array operations are not in-place 
         for the purpose of being explicit. 
    )pbdoc");

#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "dev";
#endif
}
