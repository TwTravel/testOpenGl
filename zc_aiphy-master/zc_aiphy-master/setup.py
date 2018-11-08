from setuptools import setup

setup(name='aiphy',
      version='0.0.1',
      description='A reinforcement learning environment for shadow hand and ur10, based on openai/gym and mujoco',
      author='Cheng, Weitao',
      author_email='chengzhmike@gmail.com, xiweitao@gmail.com',
      install_requires=['gym', 'numpy', 'mujoco_py']
      )
