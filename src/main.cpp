#include <math.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <functional>
#include <iostream>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

struct meanOfSlice {
  float_t channelOne;
  float_t channelTwo;
};

meanOfSlice computeSliceMean(const py::array_t<float_t> &srcAudio,
                             const py::ssize_t start, const py::ssize_t stop) {
  auto data = srcAudio.unchecked<2>();

  float_t channelOne = 0.0;
  float_t channelTwo = 0.0;

  for (py::ssize_t i = start; i < stop; i++) {
    channelOne = channelOne + data(0, i);
    channelTwo = channelTwo + data(1, i);
  }

  std::cout << channelOne / (stop - start) << std::endl;
  std::cout << channelTwo / (stop - start) << std::endl;

  return meanOfSlice{channelOne, channelTwo};
}

py::array_t<float_t> compute_energy(const py::array_t<float_t> &audio,
                                    const int hop_len, const int win_len) {
  auto srcAudio =
      audio.unchecked<2>();  // x must have ndim = 2; can be non-writeable

  // init new array for destination
  py::array_t<float_t> result({srcAudio.shape(0), srcAudio.shape(1)});
  auto destAudio = result.mutable_unchecked<2>();

  meanOfSlice something;
  for (py::ssize_t j = 0; j < srcAudio.shape(1) - 2; j++) {
    something = computeSliceMean(audio, j, j + 2);
  }

  // One liner for testing in python:
  // python3 -c 'import pyoniip; import numpy as np; arr =
  // np.array([[0,1],[2,3]], dtype=np.float32); b =
  // pyoniip.compute_energy(arr,1, 2); print(b)'
  return audio;
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
