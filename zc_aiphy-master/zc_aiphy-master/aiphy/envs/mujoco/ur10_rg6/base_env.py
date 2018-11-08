from aiphy.envs.mujoco import base_env
from aiphy.envs.mujoco.utils import mujoco_utils
import numpy as np
from gym import utils
from aiphy.envs.mujoco.utils import path_math as pm
from gym import spaces


DEFAULT_INITIAL_QPOS = {
    'ur10:shoulder_pan': -0.19408803,
    'ur10:shoulder_lift': -1.07770249,
    'ur10:elbow': 1.80195949,
    'ur10:wrist_1': -2.29505332,
    'ur10:wrist_2': -1.57079633,
    'ur10:wrist_3': 2.94750463,
    'rg6:left_out_up_joint': 0.75,
    'rg6:left_in_up_joint': 0.75,
    'rg6:left_in_down_joint': -0.75,
    'rg6:right_out_up_joint': -0.75,
    'rg6:right_in_up_joint': -0.75,
    'rg6:right_in_down_joint': 0.75
}


class UR10RG6BaseEnv(base_env.BaseEnv):
    def __init__(self, model_path, frame_skip, initial_pos_dict):

        base_env.BaseEnv.__init__(
            self, model_path, frame_skip, initial_pos_dict)

        self.joint_fix_status = np.zeros(self.model.nu)
        self.joint_fix_angle = np.zeros(self.model.nu)

    def _viewer_setup(self):
        lookat = self.data.get_body_xpos('bench')
        for idx, value in enumerate(lookat):
            self.viewer.cam.lookat[idx] = value
        self.viewer.cam.distance = 4.5
        self.viewer.cam.azimuth = 0
        self.viewer.cam.elevation = -45.

    def set_action(self, action, control_type=base_env.ControlType.Raw):

        assert action.shape == (self.model.nu,)

        ctrlrange = self.sim.model.actuator_ctrlrange
        actuation_range = (ctrlrange[:, 1] - ctrlrange[:, 0]) / 2.

        if control_type == base_env.ControlType.Raw:
            final_action = action
        elif control_type == base_env.ControlType.Normalized:
            actuation_center = (ctrlrange[:, 1] + ctrlrange[:, 0]) / 2.
            final_action = actuation_center + action * actuation_range
        elif control_type == base_env.ControlType.RelNormalized:
            actuation_center = np.zeros_like(action)
            for i in range(self.model.nu):
                actuation_center[i] = self.sim.data.get_joint_qpos(
                    self.sim.model.actuator_names[i].replace(':AP_', ':'))
            final_action = actuation_center + action * actuation_range
        else:
            assert 0

        # print(final_action)
        for i in range(self.model.nu):
            if self.joint_fix_status[i]:
                final_action[i] = self.joint_fix_angle[i]
        self.sim.data.ctrl[:] = final_action
        # print(final_action)
        self.sim.data.ctrl[:] = np.clip(
            self.sim.data.ctrl, ctrlrange[:, 0], ctrlrange[:, 1])

    def fix_a_joint(self, joint_id, active, angle):
        self.joint_fix_status[joint_id] = active
        self.joint_fix_angle[joint_id] = angle
        mujoco_utils.set_equality_constraint_by_id(
            self.sim, joint_id, active, angle)

    def fix_joint_by_name(self, joint_name, active, angle):
        joint_id = self.model.actuator_name2id(joint_name.replace(':', ':A_'))
        self.joint_fix_status[joint_id] = active
        self.joint_fix_angle[joint_id] = angle
        mujoco_utils.set_equality_constraint_by_id(
            self.sim, joint_id, active, angle)


class UR10RG6AngleToolEnv(UR10RG6BaseEnv, utils.EzPickle):
    def __init__(self, model_path='ur10_rg6.xml', frame_skip=40,
                 initial_robot_pos_dict=DEFAULT_INITIAL_QPOS, max_speed=0.5, ctrl_hz=6.25):

        UR10RG6BaseEnv.__init__(
            self, model_path, frame_skip, initial_robot_pos_dict)

        self.sim.forward()

        self.ctrl_hz = 6.25
        self.hz = 1.0 / (self.sim.nsubsteps * self.sim.model.opt.timestep)
        self.max_speed = max_speed
        self.joint_names = ('ur10:shoulder_pan', 'ur10:shoulder_lift',
                            'ur10:elbow', 'ur10:wrist_1', 'ur10:wrist_2', 'ur10:wrist_3')
        self.tool_name = 'rg6:right_in_up_joint'

        self.is_moving = False
        self.pos_last = np.asarray(
            [initial_robot_pos_dict[name] for name in self.joint_names])
        self.vel_last = np.zeros(6)

        utils.EzPickle.__init__(self)

    def _get_action_space(self):
        return spaces.Box(-np.inf, np.inf, shape=(7,), dtype='float32')

    def _get_observation_space(self):
        observation_space = spaces.Box(-np.inf,
                                       np.inf, shape=(12, 0), dtype='float32')
        return observation_space

    def _get_obs(self):

        obs = []

        for i in range(self.model.nu):
            actuator_name = self.sim.model.actuator_names[i]
            # if 'AP' in actuator_name:
            obs.append(self.sim.data.get_joint_qpos(
                actuator_name.replace(':AP_', ':')))
        for i in range(self.model.nu):
            actuator_name = self.sim.model.actuator_names[i]
            # if 'AP' in actuator_name:
            obs.append(self.sim.data.get_joint_qvel(
                actuator_name.replace(':AP_', ':')))

        return np.asarray(obs)

    def _reset_sim(self):

        self.sim.set_state(self.initial_state)
        self.sim.forward()

        return True

    def _angleToolMove(self, positions):

        ctrl_N = int(self.hz / self.ctrl_hz)

        if self.is_moving:
            pos0 = self.pos_last
            vel0 = self.vel_last
        else:
            pos0 = []
            vel0 = []
            for name in self.joint_names:
                pos0.append(self.sim.data.get_joint_qpos(name))
                vel0.append(self.sim.data.get_joint_qvel(name))
            pos0 = np.asarray(pos0)
            vel0 = np.asarray(vel0)
            self.is_moving = True

        pos1 = np.asarray(positions)
        vel1 = np.zeros(6)
        angel_diff = np.abs(pos0 - pos1)
        max_angel_diff = np.max(angel_diff)

        if max_angel_diff < 5e-3:
            pos_path = np.repeat(pos1.reshape(1, -1), ctrl_N, axis=0)
            vel_path = np.repeat(vel1.reshape(1, -1), ctrl_N, axis=0)
            self.is_moving = False
            return pos_path

        p1 = 0.2
        v0 = 0.05
        v1 = 0.3
        v_cmd = 0.5

        if max_angel_diff <= p1:
            v_cmd = v0 + max_angel_diff / p1 * (v1 - v0)

        if v_cmd > self.max_speed:
            v_cmd = self.max_speed

        time_span = max_angel_diff / v_cmd

        N = int(time_span * self.hz)
        time_stamps = np.linspace(v_cmd, time_span, num=N)

        pos_path = pm.interp_cubic(
            time_stamps, time_span, pos0, pos1, vel0, vel1)
        vel_path = pm.interp_cubic_vel(
            time_stamps, time_span, pos0, pos1, vel0, vel1)

        if N > ctrl_N:
            pos_path = pos_path[:, :ctrl_N]
            vel_path = vel_path[:, :ctrl_N]
            self.pos_last = pos_path[:, -1]
            self.vel_last = vel_path[:, -1]
        else:
            pos_path = np.concatenate([pos_path, np.repeat(
                pos1.reshape(-1, 1), ctrl_N - N, axis=1)], axis=1)
            vel_path = np.concatenate([vel_path, np.repeat(
                vel1.reshape(-1, 1), ctrl_N - N, axis=1)], axis=1)
            self.is_moving = False
        return pos_path.T

    def step(self, action):

        joint_pos_path = self._angleToolMove(action[:6])
        tool_action = np.arcsin(action[6] / 0.16) - 0.75

        for joint_pos in joint_pos_path:
            sub_action = np.concatenate([joint_pos, np.asarray([tool_action])])

            self.set_action(
                sub_action, control_type=base_env.ControlType.Raw)
            self.sim.step()

        obs = self._get_obs()
        done = False

        info = {}

        reward = -1.0

        return obs, reward, done, info
