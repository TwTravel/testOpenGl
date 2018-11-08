from os import path
import copy
import numpy as np
from enum import Enum

from gym import error, spaces
from gym.utils import seeding
import gym

try:
    import mujoco_py
except ImportError as e:
    raise error.DependencyNotInstalled(
        "{}. (HINT: you need to install mujoco_py, and also perform the setup instructions here: https://github.com/openai/mujoco-py/.)".format(e))


class ControlType(Enum):
    Raw = 0
    Normalized = 1
    RelNormalized = 2


class BaseEnv(gym.Env):
    """Superclass for all MuJoCo environments.
    """

    def __init__(self, model_path, frame_skip, initial_pos_dict):
        if model_path.startswith("/"):
            fullpath = model_path
        else:
            fullpath = path.join(path.dirname(
                __file__), "xml", model_path)
        if not path.exists(fullpath):
            raise IOError("File %s does not exist" % fullpath)
        self.model = mujoco_py.load_model_from_path(fullpath)
        self.sim = mujoco_py.MjSim(self.model, nsubsteps=frame_skip)
        self.data = self.sim.data
        self.viewer = None

        self.metadata = {
            'render.modes': ['human', 'rgb_array'],
            'video.frames_per_second': int(np.round(1.0 / self.dt))
        }

        self.action_space = self._get_action_space()
        self.observation_space = self._get_observation_space()

        self.seed()

        self.set_pos_by_dict(initial_pos_dict)
        self.initial_state = copy.deepcopy(self.sim.get_state())

    @property
    def dt(self):
        return self.model.opt.timestep * self.sim.nsubsteps

    def seed(self, seed=None):
        self.np_random, seed = seeding.np_random(seed)
        return [seed]

    def render(self, mode='human'):
        self._render_callback()
        if mode == 'server':
            width, height = 640, 480
            images = []
            depth_images = []
            for i in range(3):
                data = self.sim.render(
                    width, height, depth=True, camera_name='cam{}'.format(i))
                image = data[0]
                depth_image = data[1]
                image = image[::-1, :, :]
                depth_image = depth_image[::-1, :]
                images.append(image)
                depth_images.append(depth_image)
            return images, depth_images

        elif mode == 'rgb_array':
            self._get_viewer().render()
            # window size used for old mujoco-py:
            width, height = 640, 480
            data = self._get_viewer().read_pixels(width, height, depth=False)
            # original image is upside-down, so flip it
            return data[::-1, :, :]
        elif mode == 'human':
            self._get_viewer().render()

    def close(self):
        if self.viewer is not None:
            self.viewer.finish()
            self.viewer = None

    def reset(self):
        # Attempt to reset the simulator. Since we randomize initial conditions, it
        # is possible to get into a state with numerical issues (e.g. due to penetration or
        # Gimbel lock) or we may not achieve an initial condition (e.g. an object is within the hand).
        # In this case, we just keep randomizing until we eventually achieve a valid initial
        # configuration.
        did_reset_sim = False
        while not did_reset_sim:
            did_reset_sim = self._reset_sim()
        obs = self._get_obs()
        return obs

    def set_state(self, qpos, qvel):
        assert qpos.shape == (
            self.model.nq,) and qvel.shape == (self.model.nv,)
        old_state = self.sim.get_state()
        new_state = mujoco_py.MjSimState(old_state.time, qpos, qvel,
                                         old_state.act, old_state.udd_state)
        self.sim.set_state(new_state)
        self.sim.forward()

    def set_pos_by_dict(self, pos_dict):
        for name, value in pos_dict.items():
            self.data.set_joint_qpos(name, value)
        self.sim.forward()

    # methods to override:
    # ----------------------------

    def _get_viewer(self):
        if self.viewer is None:
            self.viewer = mujoco_py.MjViewer(self.sim)
            self._viewer_setup()
        return self.viewer

    def _get_action_space(self):
        """Get the action spaces for for the envrionment.
        """
        raise NotImplementedError

    def _get_observation_space(self):
        """Get the observation spaces for for the envrionment.
        """
        raise NotImplementedError

    def _get_obs(self):
        """Get observation
        """
        raise NotImplementedError

    def _reset_sim(self):
        """Resets a simulation and indicates whether or not it was successful.
        If a reset was unsuccessful (e.g. if a randomized state caused an error in the
        simulation), this method should indicate such a failure by returning False.
        In such a case, this method will be called again to attempt a the reset again.
        """
        raise NotImplementedError

    def _viewer_setup(self):
        """Initial configuration of the viewer. Can be used to set the camera position,
        for example.
        """
        pass

    def _render_callback(self):
        """A custom callback that is called before rendering. Can be used
        to implement custom visualizations.
        """
        pass

    # -----------------------------
