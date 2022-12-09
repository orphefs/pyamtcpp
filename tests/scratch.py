import pyoniip
import numpy as np
import nptyping


def compute_energy(audio: nptyping.NDArray, hop_len: int, win_len: int):
    audio_len = audio.shape[1]
    energy = np.array(
        [np.mean(audio[:, i:i + win_len], axis=1) for i in range(0, audio_len, hop_len)]).T
    return energy


def compute_energy_cpp(audio: nptyping.NDArray, hop_len: int, win_len: int):
    energy = pyoniip.compute_energy(audio, hop_len, win_len)
    return energy


def main():

    audio = np.array(
        [
            [1, 2, 3, 4, 5, 6, 7, 8, 9],
            [1, 2, 3, 4, 5, 6, 7, 8, 9]
        ], dtype=np.float32)
    win_len = 5
    hop_len = 2
    print("python: {}".format(compute_energy(audio, hop_len, win_len)))
    print("cpp: {}".format(compute_energy_cpp(audio, hop_len, win_len)))


if __name__ == '__main__':
    main()
