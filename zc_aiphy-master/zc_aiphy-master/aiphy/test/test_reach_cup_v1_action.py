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


if __name__ == '__main__':
    env = gym.make('ReachCupEnv-v1')

    for j in range(300):
        env.reset()
        env.render()

        for i in range(100):
            env.step(env.action_space.sample())
            env.render()
