import gym
import numpy as np
from gym.envs.registration import register
from gym.envs.robotics import rotations
# from aiphy.envs.mujoco.ur10_rg6.grip_and_place import GripPlaceEnv
import aiphy.envs.mujoco.utils.compute_ik as ik
import time

register(
    id='InsertBlocksEnv-v1',
    entry_point='aiphy.envs.mujoco.ur10_rg6.insert_blocks_v1:InsertBlocksEnvV1',
    max_episode_steps=100,
    kwargs={"action_type": "abs"}
)


if __name__ == '__main__':
    env = gym.make('InsertBlocksEnv-v1')

    for j in range(10):
        env.reset()
        env.render()

        for i in range(100):
            print(np.zeros_like(env.action_space.sample()))
            env.step(np.zeros_like(env.action_space.sample()))
            env.render()
