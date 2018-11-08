import numpy as np

def angle_trailer(angle):
    while angle > np.pi:
        angle = angle - 2.0 * np.pi
    while angle <= -np.pi:
        angle = angle + 2.0 * np.pi
    return angle


def quat_to_rotation_matrix(quat):
    sqw = quat[0] * quat[0]
    sqx = quat[1] * quat[1]
    sqy = quat[2] * quat[2]
    sqz = quat[3] * quat[3]

    rot_mat = np.zeros([3, 3])
    # invs(inverse square length) is only required if quaternion is not already normalised
    invs = 1.0 / (sqx + sqy + sqz + sqw)
    rot_mat[0, 0] = (sqx - sqy - sqz + sqw) * invs  # since sqw + sqx + sqy + sqz = 1 / invs * invs
    rot_mat[1, 1] = (-sqx + sqy - sqz + sqw) * invs
    rot_mat[2, 2] = (-sqx - sqy + sqz + sqw) * invs

    tmp1 = quat[1] * quat[2]
    tmp2 = quat[3] * quat[0]
    rot_mat[1, 0] = 2.0 * (tmp1 + tmp2) * invs
    rot_mat[0, 1] = 2.0 * (tmp1 - tmp2) * invs

    tmp1 = quat[1] * quat[3]
    tmp2 = quat[2] * quat[0]
    rot_mat[2, 0] = 2.0 * (tmp1 - tmp2) * invs
    rot_mat[0, 2] = 2.0 * (tmp1 + tmp2) * invs
    tmp1 = quat[2] * quat[3]
    tmp2 = quat[1] * quat[0]
    rot_mat[2, 1] = 2.0 * (tmp1 + tmp2) * invs
    rot_mat[1, 2] = 2.0 * (tmp1 - tmp2) * invs
    return rot_mat


def rotation_matrix_to_quat(rotation):
    tr = rotation[0,0] + rotation[1,1] + rotation[2,2]

    if tr > 0:
        S = np.sqrt(tr+1.0) * 2
        qw = 0.25 * S
        qx = (rotation[2,1] - rotation[1,2]) / S
        qy = (rotation[0,2] - rotation[2,0]) / S
        qz = (rotation[1,0] - rotation[0,1]) / S
    # elif rotation[0,0]>rotation[1,1] or rotation[0,0]>rotation[2,2]:
    elif rotation[0,0]>rotation[1,1] and rotation[0,0]>rotation[2,2]:
        S = np.sqrt(1 + rotation[0,0] - rotation[1,1] - rotation[2,2]) * 2
        qw = (rotation[2,1] - rotation[1,2]) / S
        qx = 0.25 * S
        qy = (rotation[0,1] + rotation[1,0]) / S
        qz = (rotation[0,2] + rotation[2,0]) / S
    elif rotation[1,1] > rotation[2,2]:
        S = np.sqrt(1 + rotation[1,1] - rotation[0,0] - rotation[2,2]) * 2
        qw = (rotation[0,2] - rotation[2,0]) / S
        qx = (rotation[0,1] + rotation[1,0]) / S
        qy = 0.25 * S
        qz = (rotation[1,2] + rotation[2,1]) / S
    else:
        S = np.sqrt(1 + rotation[2,2] - rotation[0,0] - rotation[1,1]) * 2
        qw = (rotation[1,0] - rotation[0,1]) / S
        qx = (rotation[0,2] + rotation[2,0]) / S
        qy = (rotation[1,2] + rotation[2,1]) / S
        qz = 0.25 * S

    quaternion = np.array([qw, qx, qy, qz])
    return quaternion


def rotate_z_matrix(angle):
    rotation_matrix = np.array([[np.cos(angle), -np.sin(angle), 0],
                                [np.sin(angle),  np.cos(angle), 0],
                                [0, 0, 1]])
    return rotation_matrix


def rotate_x_matrix(angle):
    rotation_matrix = np.array([[1, 0, 0],
                                [0, np.cos(angle), -np.sin(angle)],
                                [0, np.sin(angle), np.cos(angle)]])
    return rotation_matrix


def rotate_y_matrix(angle):
    rotation_matrix = np.array([[np.cos(angle), 0, np.sin(angle)],
                                [0, 1, 0],
                                [-np.sin(angle), 0, np.cos(angle)]])
    return rotation_matrix


def local_pos_to_global(frame_pos, frame_rot, local_pos):
    global_pos = frame_pos + frame_rot.dot(local_pos)
    return global_pos


def local_rot_to_global(frame_rot, local_rot):
    return frame_rot.dot(local_rot)


def uniform_sample_quat():
    x = np.random.uniform(size=3)
    quat = np.array([np.sqrt(1 - x[0]) * np.sin(2 * np.pi * x[1]),
                     np.sqrt(1 - x[0]) * np.cos(2 * np.pi * x[1]),
                     np.sqrt(x[0]) * np.sin(2 * np.pi * x[2]),
                     np.sqrt(x[0]) * np.cos(2 * np.pi * x[2])])
    return quat


if __name__ == "__main__":
    rot = np.array([[0.0, 0.0, 1.0],
                    [0.0, -1.0, 0.0],
                    [1.0, 0.0, 0.0]])
    pos = np.array([1, 1, 1])
    local_pos = np.array([0, 0, 1])
    print(local_pos_to_global(pos, rot, local_pos))


