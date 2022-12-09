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

  channelOne = channelOne / (stop - start);
  channelTwo = channelTwo / (stop - start);

  return meanOfSlice{channelOne, channelTwo};
}

py::array_t<float_t> compute_energy(const py::array_t<float_t> &audio,
                                    const int hop_len, const int win_len) {
  auto srcAudio =
      audio.unchecked<2>();  // x must have ndim = 2; can be non-writeable

  // init new array for destination
  py::array_t<float_t> result({srcAudio.shape(0), srcAudio.shape(1)});
  auto destAudio = result.mutable_unchecked<2>();

  ssize_t windowLength = static_cast<ssize_t>(win_len);
  ssize_t hopLength = static_cast<ssize_t>(hop_len);
  ssize_t z = 0;
  meanOfSlice meanOfSlice;
  for (py::ssize_t j = 0; j < srcAudio.shape(1) - windowLength + 1;
       j += hopLength) {
    meanOfSlice = computeSliceMean(audio, j, j + windowLength);
    destAudio(0, z) = meanOfSlice.channelOne;
    destAudio(1, z) = meanOfSlice.channelTwo;
    z += 1;
  }

  return result;
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
