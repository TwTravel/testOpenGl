# aiphy
## Requirements
The following packages are needed to install aiphy.

* Python 3
* gym
* mujoco_py
* numpy

## Install aiphy
To install aiphy, please go to the folder with setup.py in the command console and input
```
pip install -e .
```

## Demo
To play with the ReachEnv, you can run the following commands.
```
cd aiphy/test 
python3 test_reach_t0.py
```

## Define your own task
If you want to define your own task, you need:
1. Decide which physical model you want to use, e.g. reach.xml(reach_env.py). If no proper physical model exist in current envs, you can define your own .xml model or contact us.
2. Write your owm task environment. The basic example is: 'aiphy/test/template_for_new_env.py', 'aiphy/test/run_new_env.py'. You can put these two files in any folder you like. If you want to use HER baseline, please read the note in 'aiphy/envs/mujoco/reach_goal_env.py' 

Current environments are good examples, for example
* 'aiphy/envs/mujoco/reach_env_t0.py' defined a reach task with dense reward.
* 'aiphy/envs/mujoco/reach_goal_env_t0.py' defined a reach task with sparse reward for HER baseline.
* 'aiphy/envs/mujoco/reach_goal_env_t1.py' defined a reach task with sparse reward for HER baseline, and it fix all actuators in the hand. So the DOF is 6, and we can only move the arm.

