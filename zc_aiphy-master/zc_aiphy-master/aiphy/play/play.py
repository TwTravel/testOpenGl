import click
import numpy as np
import gym
from gym.envs.registration import register

from play_model import PlayModel

register(
    id='ReachCupEnv-v1',
    entry_point='aiphy.envs.mujoco.ur10_rg6.reach_cup_v1:ReachCupEnvV1',
    max_episode_steps=100,
    kwargs={"action_type": "rel"}
)


@click.command()
@click.argument('policy_file', type=str)
@click.option('--seed', type=int, default=0)
def main(policy_file, seed,):
    m = PlayModel(policy_file)
    game = 'ReachCupEnv-v1'
    env = gym.make(game)
    for _ in range(10):
        obs = env.reset()
        env.render()
        for i in range(100):
            action = m.get_action(obs)
            robot_action = m.get_action_robot(obs)
            print(robot_action)
            new_obs, r, done, info = env.step(action)
            env.render()
            obs = new_obs
            # done is always false now.
            if done:
                print('done')
                break

if __name__ == '__main__':
    main()
