import pyamtcpp
import numpy as np
import nptyping
import time


def st_time(func):
    """
        st decorator to calculate the total time of a func
    """

    def st_func(*args, **kwargs):
        t1 = time.time()
        r = func(*args, **kwargs)
        t2 = time.time()
        print("Function=%s, Time=%s" % (func.__name__, t2 - t1))
        return r

    return st_func


@st_time
def compute_energy(audio: nptyping.NDArray, hop_len: int, win_len: int):
    audio_len = audio.shape[1]
    energy = np.array(
        [np.mean(audio[:, i:i + win_len] ** 2, axis=1) for i in range(0, audio_len, hop_len)]).T
    return energy


@st_time
def compute_energy_cpp(audio: nptyping.NDArray, hop_len: int, win_len: int):
    energy = pyamtcpp.compute_energy(audio, hop_len, win_len)
    return energy


def main():
    # define input arguments
    audio = np.random.normal(0, 1, [2, 12000000]).astype(np.float32)
    win_len = 106
    hop_len = 80

    print("python: {}".format(compute_energy(audio, hop_len, win_len)))
    print("cpp: {}".format(compute_energy_cpp(audio, hop_len, win_len)))

    energy = compute_energy(audio, hop_len, win_len)
    energy_cpp = compute_energy_cpp(audio, hop_len, win_len)

    print(energy.shape)
    print(energy_cpp.shape)


if __name__ == '__main__':
    main()
