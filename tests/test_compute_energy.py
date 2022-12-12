import nptyping
import numpy as np
import pyamtcpp
import pytest
from nptyping import Shape, Float32

from utils.benchmarking import compute_energy, compute_energy_cpp

special_array = np.random.normal(0, 1, [2, 12000]).astype(np.float32)
special_array[1,100] = np.nan
@pytest.mark.parametrize("audio, win_len, hop_len",
                         [

                             (
                                     np.random.normal(0, 1, [2, 12000000]).astype(np.float32),
                                     106,
                                     80,
                             ),
                             (
                                     np.random.normal(0, 1, [2, 23452453]).astype(np.float32),
                                     19,
                                     5,
                             ),
                             (
                                     np.random.normal(0, 1, [2, 234245]).astype(np.float32),
                                     200,
                                     100,
                             ),
                             (
                                     np.random.normal(0, 1, [2, 58556434]).astype(np.float32),
                                     1000,
                                     500,
                             ),
                             # (
                             #         np.random.normal(0, 1, [1, 120000]).astype(np.float32),
                             #         106,
                             #         80,
                             # ),
                             (
                                     special_array,
                                     106,
                                     80,
                             )




                         ])
def test_compute_energy(audio: nptyping.NDArray[Shape["2, *"], Float32], win_len: int, hop_len:int):
    p = compute_energy(audio, win_len, hop_len)
    cpp = compute_energy_cpp(audio, win_len, hop_len)
    diff = np.mean(np.abs(p - cpp), axis=1)
    assert pyamtcpp.__version__ == '0.0.1'
    assert np.mean(diff) < 5e-7

