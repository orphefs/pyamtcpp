import pyamtcpp as m
import numpy as np
from scratch import compute_energy, compute_energy_cpp

audio = np.array(
    [
        [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12],
        [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]
    ], dtype=np.float32)
audio = np.random.random([2,1200000]).astype(np.float32)
win_len = 2
hop_len = 2

assert m.__version__ == '0.0.1'
assert np.all(compute_energy_cpp(audio, win_len, hop_len)
              == compute_energy(audio, win_len, hop_len))
