import pyamtcpp as m
import numpy as np
from scratch import compute_energy, compute_energy_cpp

audio = np.array(
    [
        [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17],
        [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17]
    ], dtype=np.float32)
audio = np.random.normal(0, 1, [2,12000000]).astype(np.float32)
win_len = 106
hop_len = 80

p = compute_energy(audio, win_len, hop_len)
cpp = compute_energy_cpp(audio, win_len, hop_len)

# print(p)
# print(cpp)
print(p-cpp)
diff = np.mean(np.abs(p - cpp), axis=1)


assert m.__version__ == '0.0.1'
assert np.mean(diff) < 1e-7
