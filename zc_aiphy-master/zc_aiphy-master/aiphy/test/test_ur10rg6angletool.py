import gym
import numpy as np
from gym.envs.registration import register
from gym.envs.robotics import rotations
# from aiphy.envs.mujoco.ur10_rg6.grip_and_place import GripPlaceEnv
import aiphy.envs.mujoco.utils.compute_ik as ik
import time

TARGET_POS_QUAT = np.asarray([[0.75, 0.0, 0.95, 0.0, -1.0, 0.0, 0.0],
                              [0.9, 0.2, 0.8, 0.0, -1.0, 0.0, 0.0],
                              [0.6, 0.2, 0.8, 0.0, -1.0, 0.0, 0.0],
                              [0.6, -0.2, 0.8, 0.0, -1.0, 0.0, 0.0],
                              [0.9, -0.2, 0.8, 0.0, -1.0, 0.0, 0.0],
                              [0.9, 0.2, 1.1, 0.0, -1.0, 0.0, 0.0],
                              [0.6, 0.2, 1.1, 0.0, -1.0, 0.0, 0.0],
                              [0.6, -0.2, 1.1, 0.0, -1.0, 0.0, 0.0],
                              [0.9, -0.2, 1.1, 0.0, -1.0, 0.0, 0.0]])


def move_rule(act, q_init):
    assert act.shape == (7,)
    # act = np.asarray([0.98820294, -0.08337582, 1.04251417, -
    #                   0.00431598, -0.99052331, -0.07592707, 0.11436796])
    # act = np.asarray([0.95, -0.10631333, 1.06,
    #                   0.08309565, -0.86071728, 0.01984664, 0.50186352])

    target_pos = act[:3]

    target_rot = rotations.quat2mat(act[3:])

    arm_action = ik.choose_ik(
        *ik.calculate_inverse_kinematics(target_pos, target_rot, ik.UR10_RG6_Para), q_init=q_init)
    print(q_init)
    print(arm_action)
    arm_action += np.asarray([0, -1.5707963267948966,
                              0, -1.5707963267948966, 0.0, -1.5707963267948966])

    return arm_action


register(
    id='UR10RG6AngleToolEnv-v0',
    entry_point='aiphy.envs.mujoco.ur10_rg6.base_env:UR10RG6AngleToolEnv',
    max_episode_steps=200,
    kwargs={"max_speed": 0.2}
)


if __name__ == '__main__':
    env = gym.make('UR10RG6AngleToolEnv-v0')
    init = np.asarray([-0.19408803, -1.07770249, 1.80195949, -
                       2.29505332, -1.57079633, 2.94750463])

    env.reset()
    env.render()

    for target in TARGET_POS_QUAT:
        joint_pos_action = move_rule(target, init)
        init = joint_pos_action
        gripper_pos_action = np.asarray([0.0])
        action = np.concatenate(
            [joint_pos_action, gripper_pos_action])

        for _ in range(30):
            obs, rwd, done, info = env.step(action)
            env.render()
