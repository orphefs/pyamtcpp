import argparse
import filecmp
import json
import os
from argparse import Namespace
from pathlib import Path
from typing import Union, Dict

import nptyping
import numpy as np
import pyamtcpp
import pytest
from nptyping import NDArray

from utils.benchmarking import compute_energy, compute_energy_cpp


@pytest.mark.parametrize("audio, win_len, hope_len",
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
                             (
                                     np.random.normal(0, 1, [2, 12000000]).astype(np.float32),
                                     106,
                                     80,
                             ),




                         ])
def test_compute_energy(audio: nptyping.NDArray[np.float32], win_len: int, hop_len:int):
    p = compute_energy(audio, win_len, hop_len)
    cpp = compute_energy_cpp(audio, win_len, hop_len)
    diff = np.mean(np.abs(p - cpp), axis=1)
    assert pyamtcpp.__version__ == '0.0.1'
    assert np.mean(diff) < 1e-7

