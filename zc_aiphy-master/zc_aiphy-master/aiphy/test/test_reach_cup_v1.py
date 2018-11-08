import gym
import numpy as np
from gym.envs.registration import register
from gym.envs.robotics import rotations
# from aiphy.envs.mujoco.ur10_rg6.grip_and_place import GripPlaceEnv
import aiphy.envs.mujoco.utils.compute_ik as ik
import time

register(
    id='ReachCupEnv-v1',
    entry_point='aiphy.envs.mujoco.ur10_rg6.reach_cup_v1:ReachCupEnvV1',
    max_episode_steps=100,
    kwargs={"action_type": "rel"}
)


def distance(achieved_goal, desired_goal):
    pos_distance = achieved_goal[0:3] - desired_goal[0:3]
    rad_distance = achieved_goal[3:6] - desired_goal[3:6]

    return np.concatenate([pos_distance, rad_distance])

    # pos_bool =
    # print("-------")
    # print(rotations.quat2euler(achieved_goal[3:7]))
    # print(rotations.quat2euler(desired_goal[3:7]))
    # print(pos_distance)
    # print(rad_distance)


if __name__ == '__main__':
    env = gym.make('ReachCupEnv-v1')
    print(env.action_space)
    print(env.observation_space)
    ctrl_range = np.array(env.unwrapped.sim.model.actuator_ctrlrange)
    l_range = ctrl_range[:6, 0]
    u_range = ctrl_range[:6, 1]
    print(l_range)
    print(u_range)
    thresh_array = np.asarray([0.05] * 3 + [0.05] * 3)

    for j in range(20):

        obs = env.reset()
        env.render()
        # goal = obs['desired_goal']
        # print(obs['desired_goal'])
        # print(obs['achieved_goal'])
        diff = distance(obs['achieved_goal'], obs['desired_goal'])
        while not (np.abs(diff) < thresh_array).all():
            action = np.zeros(6)
            print(np.abs(diff) < thresh_array)
            print(diff)
            for i in range(6):
                if abs(diff[i]) <= thresh_array[i]:
                    action[i] = - diff[i] / thresh_array[i]
                elif diff[i] > 0.05:
                    action[i] = -1
                else:
                    action[i] = 1

            print(action)

            obs, rwd, done, info = env.step(action)
            print(rwd)
            print("+++++")
            print(obs['achieved_goal'])
            print(obs['desired_goal'])
            env.render()

            diff = distance(obs['achieved_goal'], obs['desired_goal'])

        for _ in range(10):

            action = - diff / thresh_array
            obs, rwd, done, info = env.step(action)
            print(rwd)
            print("+++++-------------+++++")
            print(obs['achieved_goal'])
            print(obs['desired_goal'])
            env.render()
            diff = distance(obs['achieved_goal'], obs['desired_goal'])

        # while not distance_bool(obs['achieved_goal'], obs['desired_goal'], 0.05, 0.2).all():
        #     action = np.zeros(6)

        #     for i in range(3):
        #         if abs(obs['achieved_goal'][i] - obs['desired_goal'][i]) <= 0.05:
        #             action[i] = (obs['achieved_goal'][i] -
        #                          obs['desired_goal'][i]) / 0.05
        #         elif (obs['achieved_goal'][i] - obs['desired_goal'][i]) > 0.05:
        #             action[i] = 1
        #         else:
        #             action[i] = -1

        #     for i in range(3):
        #         if abs(obs['achieved_goal'][i] - obs['desired_goal'][i]) <= 0.05:
        #             action[i] = (obs['achieved_goal'][i] -
        #                          obs['desired_goal'][i]) / 0.05
        #         elif (obs['achieved_goal'][i] - obs['desired_goal'][i]) > 0.05:
        #             action[i] = 1
        #         else:
        #             action[i] = -1

        # time.sleep(1)

        # while not (np.abs(obs['achieved_goal'][0:3] - obs['desired_goal'][0:3]) < 0.05).all() or (np.abs(obs['achieved_goal'][0:3] - obs['desired_goal'][0:3]) < 0.05).all():
        #     action = np.zeros(6)
        #     for i in range(3):
        #         if abs(obs['achieved_goal'][i] - obs['desired_goal'][i]) <= 0.05:
        #             action[i] = (obs['achieved_goal'][i] -
        #                          obs['desired_goal'][i]) / 0.05
        #         elif (obs['achieved_goal'][i] - obs['desired_goal'][i]) > 0.05:
        #             action[i] = 1
        #         else:
        #             action[i] = -1
        #     print(action)
        #     obs, rwd, done, info = env.step(action)
        #     print(obs['desired_goal'])
        #     print(obs['achieved_goal'])
        #     env.render()

        # action = np.zeros(6)

        # for i in range(3):
        #     action[i] = (obs['achieved_goal'][i] -
        #                  obs['desired_goal'][i]) / 0.05
        # obs, rwd, done, info = env.step(action)
        # env.render()

        # for i in range(20):
        #     obs, r, done, info = env.step(
        #         np.array([-1.0, 0.0, 0.0, 1, 0.0, 0]))
        #     print(obs['achieved_goal'][0:3])
        #     print(- np.pi * 2 / 3)
        #     print(rotations.quat2euler(obs['achieved_goal'][3:7]))
        #     env.render()

        # for i in range(10):
        #     obs, r, done, info = env.step(
        #         np.array([0.0, 0.0, 0.0, -0.1, 0, 0]))
        #     # print(obs)
        #     env.render()

        # for i in range(10):
        #     obs, r, done, info = env.step(
        #         np.array([0.0, 0.0, 0.0, 0.0, 0.1, 0]))
        #     # print(obs)
        #     env.render()

        # for i in range(10):
        #     obs, r, done, info = env.step(np.array([0.0, 0.0, 0.0, 0.0, -0.1, 0]))
        #     # print(obs)
        #     env.render()

        # for i in range(10):
        #     obs, r, done, info = env.step(
        #         np.array([0.0, 0.0, 0.0, 0.0, 0.0, 0.1]))
        #     # print(obs)
        #     env.render()

        # for i in range(10):
        #     obs, r, done, info = env.step(np.array([0.0, 0.0, 0.0, 0.0, 0.0, -0.1]))
        #     # print(obs)
        #     env.render()

        # while not (np.abs(obs['achieved_goal'] - obs['observation'][0:3]) < 0.05).all():
        #     action = np.zeros(4)
        #     action[-1] = 1
        #     for i in range(3):
        #         if abs(obs['achieved_goal'][i] - obs['observation'][0:3][i]) <= 0.05:
        #             action[i] = (obs['achieved_goal'][i] -
        #                          obs['observation'][0:3][i]) / 0.05
        #         elif (obs['achieved_goal'][i] - obs['observation'][0:3][i]) > 0.05:
        #             action[i] = 1
        #         else:
        #             action[i] = -1

        #     obs, rwd, done, info = env.step(action)
        #     env.render()

        # action = np.zeros(4)

        # for i in range(3):
        #     action[i] = (obs['achieved_goal'][i] -
        #                  obs['observation'][0:3][i]) / 0.05
        # obs, rwd, done, info = env.step(action)
        # env.render()

        # obs, rwd, done, info = env.step(np.array([0, 0, 0, -1]))
        # env.render()

        # while not (np.abs(obs['desired_goal'] - obs['achieved_goal']) < 0.05).all():
        #     action = np.zeros(4)
        #     for i in range(3):
        #         if abs(obs['desired_goal'][i] - obs['achieved_goal'][i]) <= 0.05:
        #             action[i] = (obs['desired_goal'][i] -
        #                          obs['achieved_goal'][i]) / 0.05
        #         elif (obs['desired_goal'][i] - obs['achieved_goal'][i]) > 0.05:
        #             action[i] = 1
        #         else:
        #             action[i] = -1

        #         action[3] = -1

        #     obs, rwd, done, info = env.step(action)
        #     env.render()

        # action = np.zeros(4)

        # for i in range(3):
        #     action[i] = (obs['desired_goal'][i] -
        #                  obs['achieved_goal'][i]) / 0.05
        # action[3] = -1
        # obs, rwd, done, info = env.step(action)
        # env.render()
