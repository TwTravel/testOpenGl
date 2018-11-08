import numpy as np


def interp_cubic(t, T, p0_pos, p1_pos, p0_vel, p1_vel):
    a = p0_pos.reshape(-1, 1)
    b = p0_vel
    c = (-3.0 * p0_pos + 3.0 * p1_pos - 2.0 *
         T * p0_vel - T * p1_vel) / (T ** 2)
    d = (2.0 * p0_pos - 2.0 * p1_pos + T * p0_vel + T * p1_vel) / (T ** 3)
    positions = a + np.outer(b, t) + np.outer(c, t ** 2) + np.outer(d, t ** 3)
    return positions


def interp_cubic_vel(t, T, p0_pos, p1_pos, p0_vel, p1_vel):
    b = p0_vel.reshape(-1, 1)
    c = (-3.0 * p0_pos + 3.0 * p1_pos - 2.0 *
         T * p0_vel - T * p1_vel) / (T ** 2)
    d = (2.0 * p0_pos - 2.0 * p1_pos + T * p0_vel + T * p1_vel) / (T ** 3)
    vel = b + np.outer(c * 2.0, t) + np.outer(d * 3.0, t ** 2)
    return vel


if __name__ == "__main__":
    T = 1
    t = np.arange(0, 1.2, 0.2)
    p0_pos = np.zeros(6)
    p1_pos = np.array([2.0] * 6)
    p0_vel = np.zeros(6)
    p1_vel = np.array([0.2] * 6)

    print(interp_cubic(t, T, p0_pos, p1_pos, p0_vel, p1_vel))
    print(interp_cubic_vel(t, T, p0_pos, p1_pos, p0_vel, p1_vel))
