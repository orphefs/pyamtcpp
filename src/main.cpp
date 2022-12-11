#include <math.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <functional>
#include <iostream>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

struct meanOfSlice
{
  float_t channelOne;
  float_t channelTwo;
};

meanOfSlice computeSliceMean(const py::array_t<float_t> &srcAudio,
                             const py::ssize_t start, const py::ssize_t stop)
{
  auto data = srcAudio.unchecked<2>();

  float_t channelOne = 0.0;
  float_t channelTwo = 0.0;

  for (py::ssize_t i = start; i < stop; i++)
  {
    channelOne = channelOne + data(0, i);
    channelTwo = channelTwo + data(1, i);
  }

  channelOne = channelOne / (stop - start);
  channelTwo = channelTwo / (stop - start);

  return meanOfSlice{channelOne, channelTwo};
}

py::array_t<float_t> compute_energy(const py::array_t<float_t> &audio,
                                    const int hop_len, const int win_len)
{
  auto srcAudio =
      audio.unchecked<2>(); // x must have ndim = 2; can be non-writeable

  ssize_t windowLength = static_cast<ssize_t>(win_len);
  ssize_t hopLength = static_cast<ssize_t>(hop_len);

  // init size of new array
  ssize_t arrayLength = ceil(static_cast<float>(srcAudio.shape(1) - hopLength) /
                             static_cast<float>(hopLength)) +
                        1;
  std::cout << arrayLength << std::endl;

  // init new array for destination
  py::array_t<float_t> result({srcAudio.shape(0), arrayLength});
  auto destAudio = result.mutable_unchecked<2>();

  // init destAudio with 0's
  for (py::ssize_t i = 0; i < destAudio.shape(0); i++)
  {
    destAudio(0, i) = 0.0;
    destAudio(1, i) = 0.0;
  }

  ssize_t z = 0;
  meanOfSlice meanOfSlice;

  // TODO: fix loop bounds, as right array edge has funky numbers

  for (py::ssize_t j = 0; j < arrayLength; j += hopLength)
  {
    meanOfSlice = computeSliceMean(audio, j, j + windowLength);

    // destAudio should have size np.ceil((23420-8)/8) + 1

    destAudio(0, z) = meanOfSlice.channelOne;
    destAudio(1, z) = meanOfSlice.channelTwo;
    z += 1;
    std::cout << "meanOfSlice.channelOne: " << meanOfSlice.channelOne << std::endl;
    std::cout << "meanOfSlice.channelTwo: " << meanOfSlice.channelTwo << std::endl;

    std::cout << "destAudio(0, "<< z << "): " << destAudio(0, z) << std::endl;
    std::cout << "destAudio(1, "<< z << "): "<< destAudio(1, z) << std::endl;



  }

  return result;
}

PYBIND11_MODULE(pyamtcpp, m)
{
  m.doc() = R"pbdoc(
        pyamtcpp
        -----------------------
        .. currentmodule:: pyamtcpp
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
