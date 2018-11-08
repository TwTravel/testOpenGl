import numpy as np
from aiphy.envs.mujoco.utils import angle_math
# inverse kinematic is based on "ROS-Industrial/universal_robot/ur_kinematics"
# the position calculated in the file is the position of "ur10:ee_link" in aiphy model with some shift

# UR10_Para = (0.1273,
#              -0.612,
#              -0.5723,
#              0.163941,
#              0.1157,
#              0.0922)
# d6 = 0.0922 + 0.32 for the center of hand
UR10_Para = (0.1273,
             -0.612,
             -0.5723,
             0.163941,
             0.1157,
             0.0922 + 0.32)

UR10_RG6_Para = (0.1273, -0.612, -0.5723, 0.163941, 0.1157, 0.0922 + 0.23)


def forward(q, robot_para=UR10_Para):
    d1 = robot_para[0]
    a2 = robot_para[1]
    a3 = robot_para[2]
    d4 = robot_para[3]
    d5 = robot_para[4]
    d6 = robot_para[5]

    s1 = np.sin(q[0])
    c1 = np.cos(q[0])
    q234 = q[1]
    s2 = np.sin(q[1])
    c2 = np.cos(q[1])
    s3 = np.sin(q[2])
    c3 = np.cos(q[2])
    q234 += q[2]
    q234 += q[3]
    s5 = np.sin(q[4])
    c5 = np.cos(q[4])
    s6 = np.sin(q[5])
    c6 = np.cos(q[5])
    s234 = np.sin(q234)
    c234 = np.cos(q234)
    pos = np.zeros((4,4), dtype=float)
    pos[0, 0] = ((c1*c234-s1*s234)*s5)/2.0 - c5*s1 + ((c1*c234+s1*s234)*s5)/2.0
    pos[0, 1] = (c6*(s1*s5 + ((c1*c234-s1*s234)*c5)/2.0 + ((c1*c234+s1*s234)*c5)/2.0) -
          (s6*((s1*c234+c1*s234) - (s1*c234-c1*s234)))/2.0)
    pos[0, 2] = (-(c6*((s1*c234+c1*s234) - (s1*c234-c1*s234)))/2.0 -
          s6*(s1*s5 + ((c1*c234-s1*s234)*c5)/2.0 + ((c1*c234+s1*s234)*c5)/2.0))
    pos[0, 3] = ((d5*(s1*c234-c1*s234))/2.0 - (d5*(s1*c234+c1*s234))/2.0 -
          d4*s1 + (d6*(c1*c234-s1*s234)*s5)/2.0 + (d6*(c1*c234+s1*s234)*s5)/2.0 -
          a2*c1*c2 - d6*c5*s1 - a3*c1*c2*c3 + a3*c1*s2*s3)
    pos[1, 0] = c1*c5 + ((s1*c234+c1*s234)*s5)/2.0 + ((s1*c234-c1*s234)*s5)/2.0
    pos[1, 1] = (c6*(((s1*c234+c1*s234)*c5)/2.0 - c1*s5 + ((s1*c234-c1*s234)*c5)/2.0) +
          s6*((c1*c234-s1*s234)/2.0 - (c1*c234+s1*s234)/2.0))
    pos[1, 2] = (c6*((c1*c234-s1*s234)/2.0 - (c1*c234+s1*s234)/2.0) -
          s6*(((s1*c234+c1*s234)*c5)/2.0 - c1*s5 + ((s1*c234-c1*s234)*c5)/2.0))
    pos[1, 3] = ((d5*(c1*c234-s1*s234))/2.0 - (d5*(c1*c234+s1*s234))/2.0 + d4*c1 +
          (d6*(s1*c234+c1*s234)*s5)/2.0 + (d6*(s1*c234-c1*s234)*s5)/2.0 + d6*c1*c5 -
          a2*c2*s1 - a3*c2*c3*s1 + a3*s1*s2*s3)
    pos[2, 0] = ((c234*c5-s234*s5)/2.0 - (c234*c5+s234*s5)/2.0)
    pos[2, 1] = ((s234*c6-c234*s6)/2.0 - (s234*c6+c234*s6)/2.0 - s234*c5*c6)
    pos[2, 2] = (s234*c5*s6 - (c234*c6+s234*s6)/2.0 - (c234*c6-s234*s6)/2.0)
    pos[2, 3] = (d1 + (d6*(c234*c5-s234*s5))/2.0 + a3*(s2*c3+c2*s3) + a2*s2 -
          (d6*(c234*c5+s234*s5))/2.0 - d5*c234)
    pos[3, 0] = 0.0
    pos[3, 1] = 0.0
    pos[3,2] = 0.0
    pos[3,3] = 1.0
    return pos


def inverse(T, q6_des=0, robot_para=UR10_Para):
    d1 = robot_para[0]
    a2 = robot_para[1]
    a3 = robot_para[2]
    d4 = robot_para[3]
    d5 = robot_para[4]
    d6 = robot_para[5]

    q_sols = np.zeros((8,6), dtype=float)
    num_sols = 0
    T02 = -T[0,0]
    T00 = T[0,1]
    T01 = T[0,2]
    T03 = -T[0,3]
    T12 = -T[1,0]
    T10 =  T[1,1]
    T11 =  T[1,2]
    T13 = -T[1,3]
    T22 =  T[2,0]
    T20 = -T[2,1]
    T21 = -T[2,2]
    T23 =  T[2,3]

    ZERO_THRESH = 0.00000001;

    # ////////////////////////////// shoulder rotate joint (q1) //////////////////////////////
    q1 = np.zeros(2, dtype=float)

    A = d6*T12 - T13
    B = d6*T02 - T03
    R = A*A + B*B
    if np.abs(A) < ZERO_THRESH:
        if np.abs(np.abs(d4) - np.abs(B)) < ZERO_THRESH:
            div = -np.sign(d4)*np.sign(B)
        else:
            div = -d4/B
        arcsin = np.arcsin(div);
        if np.abs(arcsin) < ZERO_THRESH:
            arcsin = 0.0
        if arcsin < 0.0:
            q1[0] = arcsin + 2.0*np.pi;
        else:
            q1[0] = arcsin
        q1[1] = np.pi - arcsin
    elif np.abs(B) < ZERO_THRESH:
        if np.abs(np.abs(d4) - np.abs(A)) < ZERO_THRESH:
            div = np.sign(d4)*np.sign(A)
        else:
            div = d4/A
        arccos = np.arccos(div)
        q1[0] = arccos
        q1[1] = 2.0*np.pi - arccos
    elif d4*d4 > R:
        return num_sols, q_sols
    else:
        arccos = np.arccos(d4 / np.sqrt(R))
        arctan = np.arctan2(-B, A)
        pos = arccos + arctan
        neg = -arccos + arctan
        if np.abs(pos) < ZERO_THRESH:
            pos = 0.0
        if np.abs(neg) < ZERO_THRESH:
            neg = 0.0
        if pos >= 0.0:
            q1[0] = pos
        else:
            q1[0] = 2.0*np.pi + pos
        if neg >= 0.0:
            q1[1] = neg
        else:
            q1[1] = 2.0*np.pi + neg
    # ////////////////////////////////////////////////////////////////////////////////
    #
    # ////////////////////////////// wrist 2 joint (q5) //////////////////////////////
    # double q5[2][2];
    q5 = np.zeros((2,2), dtype=float)
    for i in range(2):
        numer = (T03*np.sin(q1[i]) - T13*np.cos(q1[i])-d4)
        if np.abs(np.abs(numer) - np.abs(d6)) < ZERO_THRESH:
            div = np.sign(numer) * np.sign(d6)
        else:
            div = numer / d6
        arccos = np.arccos(div)
        q5[i][0] = arccos
        q5[i][1] = 2.0*np.pi - arccos

    # ////////////////////////////////////////////////////////////////////////////////

    for i in range(2):
        for j in range(2):
            c1 = np.cos(q1[i])
            s1 = np.sin(q1[i])
            c5 = np.cos(q5[i][j])
            s5 = np.sin(q5[i][j])
            # ////////////////////////////// wrist 3 joint (q6) //////////////////////////////
            if np.abs(s5) < ZERO_THRESH:
                q6 = q6_des
            else:
                q6 = np.arctan2(np.sign(s5)*-(T01*s1 - T11*c1),
                           np.sign(s5)*(T00*s1 - T10*c1))
                if np.abs(q6) < ZERO_THRESH:
                    q6 = 0.0
                if q6 < 0.0:
                    q6 += 2.0*np.pi
            # ////////////////////////////////////////////////////////////////////////////////

            q2 = np.zeros(2, dtype=float)
            q3 = np.zeros(2, dtype=float)
            q4 = np.zeros(2, dtype=float)
            # ///////////////////////////// RRR joints (q2,q3,q4) ////////////////////////////
            c6 = np.cos(q6)
            s6 = np.sin(q6)
            x04x = -s5*(T02*c1 + T12*s1) - c5*(s6*(T01*c1 + T11*s1) - c6*(T00*c1 + T10*s1))
            x04y = c5*(T20*c6 - T21*s6) - T22*s5
            p13x = d5*(s6*(T00*c1 + T10*s1) + c6*(T01*c1 + T11*s1)) - d6*(T02*c1 + T12*s1) + T03*c1 + T13*s1
            p13y = T23 - d1 - d6*T22 + d5*(T21*c6 + T20*s6)

            c3 = (p13x*p13x + p13y*p13y - a2*a2 - a3*a3) / (2.0*a2*a3)
            if np.abs(np.abs(c3) - 1.0) < ZERO_THRESH:
                c3 = np.sign(c3)
            elif(np.abs(c3) > 1.0):
                # // TODO NO SOLUTION
                # pass
                continue
            arccos = np.arccos(c3)
            q3[0] = arccos
            q3[1] = 2.0*np.pi - arccos
            denom = a2*a2 + a3*a3 + 2*a2*a3*c3
            s3 = np.sin(arccos)
            A = (a2 + a3*c3)
            B = a3*s3
            q2[0] = np.arctan2((A*p13y - B*p13x) / denom, (A*p13x + B*p13y) / denom)
            q2[1] = np.arctan2((A*p13y + B*p13x) / denom, (A*p13x - B*p13y) / denom)
            c23_0 = np.cos(q2[0]+q3[0])
            s23_0 = np.sin(q2[0]+q3[0])
            c23_1 = np.cos(q2[1]+q3[1])
            s23_1 = np.sin(q2[1]+q3[1])
            q4[0] = np.arctan2(c23_0*x04y - s23_0*x04x, x04x*c23_0 + x04y*s23_0)
            q4[1] = np.arctan2(c23_1*x04y - s23_1*x04x, x04x*c23_1 + x04y*s23_1)
            # ////////////////////////////////////////////////////////////////////////////////
            for k in range(2):
                if np.abs(q2[k]) < ZERO_THRESH:
                    q2[k] = 0.0
                elif q2[k] < 0.0:
                    q2[k] += 2.0*np.pi
                if np.abs(q4[k]) < ZERO_THRESH:
                    q4[k] = 0.0
                elif q4[k] < 0.0:
                    q4[k] += 2.0*np.pi
                q_sols[num_sols, 0] = q1[i]
                q_sols[num_sols, 1] = q2[k]
                q_sols[num_sols, 2] = q3[k]
                q_sols[num_sols, 3] = q4[k]
                q_sols[num_sols, 4] = q5[i][j]
                q_sols[num_sols, 5] = q6
                num_sols += 1

    return num_sols, q_sols


def pos_trans(position):
    # transform from aiphy coordinate to ik coordinate
    return np.array([position[0], position[1], position[2] - 0.985])


def mat_trans(matrix):
    # transform from aiphy coordinate to ik coordinate
    matrix_trans = np.array([[matrix[0, 2], -matrix[0, 0], -matrix[0, 1]],
                             [matrix[1, 2], -matrix[1, 0], -matrix[1, 1]],
                             [matrix[2, 2], -matrix[2, 0], -matrix[2, 1]]])
    return matrix_trans


def to_transformation_matrix(position, matrix):
    transformation = np.array([np.hstack((matrix[0, :], position[0])),
                               np.hstack((matrix[1, :], position[1])),
                               np.hstack((matrix[2, :], position[2])),
                               [0, 0, 0, 1]])
    return transformation


def q_shift(q_sols):
    # from ik coordinate to aiphy coordinate
    for i in range(8):
        q_sols[i, 1] = q_sols[i, 1] + np.pi/2.0
        q_sols[i, 3] = q_sols[i, 3] + np.pi/2.0
    return q_sols


def calculate_inverse_kinematics(pos, rotation, robot_para=UR10_Para):
    transform_matrix = to_transformation_matrix(pos_trans(pos), mat_trans(rotation))
    num_sols, q_sols_raw = inverse(transform_matrix, 0, robot_para=robot_para)

    q_sols = q_shift(q_sols_raw)
    for i in range(8):
        for j in range(6):
            q_sols[i, j] = angle_math.angle_trailer(q_sols[i, j])
    return num_sols, q_sols


def choose_proper_ik(num_sols, q_sols):
    flag = 1
    for i in range(num_sols):
        if q_sols[i, 2] >= 0 and -2 <= q_sols[i, 3] <= 2:
        # if q_sols[i, 2] >= 0 and -2 <= q_sols[i, 3]:
            return q_sols[i, :], flag
    # print("no proper solution")
    flag = 0
    return q_sols[0, :], flag

# def choose_proper_ik(num_sols, q_sols):
#     flag = 1
#     for i in range(num_sols):
#         if q_sols[i, 2] >= 0 and -2 <= q_sols[i, 3] <= 2:
#         # if q_sols[i, 2] >= 0 and -2 <= q_sols[i, 3]:
#             return q_sols[i, :], flag
#     # print("no proper solution")
#     flag = 0
#     return q_sols[0, :], flag


def choose_ik(num_sols, q_sols, q_init):
    q_res = None
    if num_sols == 8:
        q_res = q_sols[2, :]
    elif num_sols == 6:
        q_res = q_sols[0, :]
    elif num_sols == 4:
        q_res = q_sols[0, :]
    else:
        print(num_sols)
        print("no proper solution.")
        assert (q_init <= 2.0 * np.pi).all() and (q_init >= -2.0 * np.pi).all()
        return q_init

    for i in range(6):
        if q_res[i] - q_init[i] > np.pi:
            print('too big move for joint {}'.format(i))
            q_res[i] -= 2 * np.pi
        elif q_res[i] - q_init[i] < -np.pi:
            q_res[i] += 2 * np.pi

    assert (q_res <= 2.0 * np.pi).all() and (q_res >= -2.0 * np.pi).all()
    return q_res


def choose_close_proper_ik(num_sols, q_sols, q_init):
    if num_sols == 0:
        return q_init
    else:
        avg = []
        for i in range(num_sols):
            avg.append(np.average(np.abs(q_sols[i, :] - q_init)))

        i_argsort = np.argsort(avg)
        for i in i_argsort:
            if q_sols[i, 2] >= 0 and abs(q_sols[i, 3]) <= 2:
                # print('-------')
                # print(avg[i])
                # print(q_init[0])
                # print(q_sols[i, 0])
                return q_sols[i, :]
        print("+++++++++++")
        return q_init



    # if q_sols[i_min, 2] >= 0 and -2 <= q_sols[i_min, 3] <= 2:
    #     return q_sols[i_min, :], flag
    # # print("no proper solution")
    # flag = 0
    # return q_sols[0, :], flag


if __name__ == '__main__':
    q_init = np.array([0.0, -np.pi/2.0, 0, -np.pi/2, 0.0, 0.0], dtype=float)
    q_pos = np.array([0, 0, np.pi/3.0, 0, np.pi/3.0, np.pi/3.0]) + q_init
    # q_pos = np.array([np.pi/3.0, np.pi/3.0, np.pi/3.0, np.pi/3.0, np.pi/3.0, np.pi/3.0]) + q_init
    pos = forward(q_pos)
    print(pos)
    # q_num, q_list = inverse(pos)
    # print(q_num)
    # print(q_list)
