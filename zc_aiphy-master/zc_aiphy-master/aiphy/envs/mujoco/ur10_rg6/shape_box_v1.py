import numpy as np
from gym import spaces
from aiphy.envs.mujoco import base_env
from aiphy.envs.mujoco.utils import compute_ik as ik
from gym import utils
from gym.envs.robotics import rotations
import random

from aiphy.envs.mujoco.ur10_rg6.base_env import UR10RG6AngleToolEnv, DEFAULT_INITIAL_QPOS

# DEFAULT_INITIAL_QPOS = {
#     'ur10:shoulder_pan': 0,
#     'ur10:shoulder_lift': np.pi / 2,
#     'ur10:elbow': 0,
#     'ur10:wrist_1': np.pi / 2,
#     'ur10:wrist_2': 0.0,
#     'ur10:wrist_3': np.pi / 2,
#     'rg6:left_out_up_joint': 0.0,
#     'rg6:left_in_up_joint': 0.0,
#     'rg6:left_in_down_joint': 0.0,
#     'rg6:right_out_up_joint': 0.0,
#     'rg6:right_in_up_joint': 0.0,
#     'rg6:right_in_down_joint': 0.0
# }


# DEFAULT_INITIAL_QPOS = {
#     'ur10:shoulder_pan': -0.19408803,
#     'ur10:shoulder_lift': 0.49309383,
#     'ur10:elbow': 1.80195949,
#     'ur10:wrist_1': -0.72425699,
#     'ur10:wrist_2': 4.71238898,
#     'ur10:wrist_3': 4.51830095,
#     'rg6:left_out_up_joint': 0.75,
#     'rg6:left_in_up_joint': 0.75,
#     'rg6:left_in_down_joint': -0.75,
#     'rg6:right_out_up_joint': -0.75,
#     'rg6:right_in_up_joint': -0.75,
#     'rg6:right_in_down_joint': 0.75
# }


class ShapeBoxEnvV1(UR10RG6AngleToolEnv, utils.EzPickle):
    def __init__(self, model_path='ur10_rg6_shape_box.xml', frame_skip=40, max_speed=0.5, ctrl_hz=6.25,
                 initial_robot_pos_dict=DEFAULT_INITIAL_QPOS,
                 object_range=0.15,
                 target_range=0.15,
                 target_offset=0.0,
                 distance_threshold=0.05, reward_type='sparse', action_type='rel',
                 center_offset=[0.75, 0, 0.95], center_range=[0.05, 0.05, 0.05], step_length=0.1, radius=0.07, move_obj_first=False):

        self.distance_threshold = distance_threshold
        self.obj_range = object_range
        self.target_range = target_range
        self.target_offset = target_offset
        self.height_offset = 0.795
        self.reward_type = reward_type
        self.action_type = action_type
        # self.middle_render = middle_render
        self.move_obj_first = move_obj_first

        self.center = np.array([0.75, 0, 0.95])
        self.size = np.array([0.15, 0.2, 0.15])
        self.rel_size = 0.05
        self.rel_rpy_size = 0.05

        self.center_offset = np.asarray(center_offset)
        self.center_range = np.asarray(center_range)
        self.radius = radius
        self.step_length = step_length

        self.circle_center = self.center_offset + \
            np.random.uniform(-self.center_range, self.center_range)
        self.angle = np.random.random_sample() * 2 * np.pi

        self.initial_gripper_xpos = self.center
        self.goal = self._sample_goal()

        UR10RG6AngleToolEnv.__init__(
            self, model_path, frame_skip, initial_robot_pos_dict, max_speed, ctrl_hz)

        self.sim.forward()
        self.success = False

        utils.EzPickle.__init__(self)

    def _get_action_space(self):

        return spaces.Box(-1.0, 1.0, shape=(6,), dtype='float32')

    def _get_observation_space(self):

        obs = self._get_obs()
        observation_space = spaces.Dict(dict(
            desired_goal=spaces.Box(-np.inf, np.inf,
                                    shape=obs['achieved_goal'].shape, dtype='float32'),
            achieved_goal=spaces.Box(-np.inf, np.inf,
                                     shape=obs['achieved_goal'].shape, dtype='float32'),
            observation=spaces.Box(-np.inf, np.inf, shape=obs['observation'].shape, dtype='float32')))

        return observation_space

    def _get_obs(self):
        # dt = self.sim.nsubsteps * self.sim.model.opt.timestep

        grip_pos, grip_rpy = self._get_gripper_pos_rpy()

        # grip_velp = self.sim.data.get_body_xvelp('gripper:target').copy() * dt

        achieved_goal = self._get_achieved_goal()
        desired_goal = self.goal

        obs = np.concatenate([
            grip_pos, grip_rpy
        ])

        observation = {
            'observation': obs.copy(),
            'achieved_goal': achieved_goal.copy(),
            'desired_goal': desired_goal.copy()
        }

        return observation

    def _get_achieved_goal(self):
        gripper_pos, gripper_rpy = self._get_gripper_pos_rpy()
        achieved_goal = np.concatenate([gripper_pos, gripper_rpy])

        return achieved_goal

    def _reset_sim(self):

        self.sim.set_state(self.initial_state)
        self.sim.forward()
        self.goal = self._sample_goal()
        target_body_id = self.sim.model.body_name2id('target')
        target_point_body_id = self.sim.model.body_name2id("target_point")

        target_body_xquat = rotations.quat_mul(rotations.euler2quat(self.goal[3:6]), rotations.quat_conjugate(
            self.sim.model.body_quat[target_point_body_id]))

        target_body_xpos = self.goal[0:3] - rotations.quat_rot_vec(
            target_body_xquat, self.sim.model.body_pos[target_point_body_id])

        self.sim.model.body_pos[target_body_id] = target_body_xpos
        self.sim.model.body_quat[target_body_id] = target_body_xquat

        self.sim.forward()

        self.success = False

        return True

    def _get_gripper_pos_rpy(self):
        gripper_pos = self.data.get_body_xpos(
            'gripper:target').copy()
        gripper_quat = self.data.get_body_xquat(
            'gripper:target').copy()
        if np.sum(np.abs(gripper_quat)) == 0:
            gripper_rpy = [.0, .0, .0]
        else:
            gripper_rpy = rotations.quat2euler(gripper_quat)
        if gripper_rpy[0] > 0:
            gripper_rpy[0] -= 2 * np.pi

        return gripper_pos, gripper_rpy

    def step(self, action):

        if self.action_type == "rel":
            cur_gripper_pos = (self.data.get_body_xpos(
                'gripper:target').copy() - self.center) / self.size
            pos_action = cur_gripper_pos + \
                action[0:3] * self.rel_size / self.size

            cur_gripper_rpy = rotations.quat2euler(self.data.get_body_xquat(
                'gripper:target').copy())
            if cur_gripper_rpy[0] > 0:
                cur_gripper_rpy[0] -= 2 * np.pi

            cur_gripper_rpy = 3.0 * \
                (cur_gripper_rpy - np.asarray([-np.pi, 0.0, 0.0])) / np.pi
            rpy_action = cur_gripper_rpy + 3.0 * \
                action[3:6] * self.rel_rpy_size / np.pi

            action = np.concatenate([pos_action, rpy_action])
        joint_pos_target = self._action_project(action)

        joint_pos_path = self._angleToolMove(joint_pos_target)
        tool_action = -0.75
        for joint_pos in joint_pos_path:
            sub_action = np.concatenate([joint_pos, np.asarray([tool_action])])

            self.set_action(
                sub_action, control_type=base_env.ControlType.Raw)
            self.sim.step()

        obs = self._get_obs()
        done = False

        info = {
            'is_success': self._is_success(obs['achieved_goal'], self.goal),
        }

        reward = self.compute_reward(obs['achieved_goal'], self.goal, info)

        return obs, reward, done, info

    def _action_project(self, action):
        action = np.minimum(action, 1)
        action = np.maximum(action, -1)
        assert (self.action_space.contains(action))
        act = list(action[0:3] * self.size + self.center)
        pose = np.asarray(action[3:6])
        pose_base = np.asarray([-np.pi, 0.0, 0.0])
        rpy = pose_base + pose * np.pi / 3.0
        quat = list(rotations.euler2quat(rpy))
        act.extend(quat)
        act = np.asarray(act)
        joint_pos = self._angele_move_rule(act)
        return joint_pos

    def _angele_move_rule(self, act):
        assert act.shape == (7,)

        target_pos = act[:3]

        target_rot = rotations.quat2mat(act[3:])

        num_sols, q_sols = ik.calculate_inverse_kinematics(
            target_pos, target_rot, ik.UR10_RG6_Para)

        if num_sols == 0 or num_sols == 2:
            print(act)

        arm_action = ik.choose_ik(
            num_sols, q_sols, q_init=self.robot_get_obs()[0])
        arm_action += np.asarray([0, -1.5707963267948966,
                                  0, -1.5707963267948966, 0.0, -1.5707963267948966])

        return arm_action

    def robot_get_obs(self):
        """Returns all joint positions and velocities associated with
        a robot.
        """
        if self.data.qpos is not None and self.model.joint_names:
            names = [n for n in self.model.joint_names if n.startswith('ur10')]
            return (
                np.array([self.data.get_joint_qpos(name) for name in names]),
                np.array([self.data.get_joint_qvel(name) for name in names]),
            )
        return np.zeros(0), np.zeros(0)

    def _is_success(self, achieved_goal, desired_goal):
        d = np.linalg.norm(achieved_goal - desired_goal, axis=-1)
        return (d < self.distance_threshold).astype(np.float32)

    def compute_reward(self, achieved_goal, goal, info):
        # Compute distance between goal and the achieved goal
        assert achieved_goal.shape == goal.shape
        d = np.linalg.norm(achieved_goal - goal, axis=-1)

        if self.reward_type == 'sparse':
            return -(d > self.distance_threshold).astype(np.float32)
        else:
            return -d

    def _move_goal(self):
        # print("**env", self.goal, self.direction * self.step_length)
        self.angle += self.step_length
        self.goal[0] = self.circle_center[0] + self.radius * np.sin(self.angle)
        self.goal[1] = self.circle_center[1] + self.radius * np.cos(self.angle)
        target_site_id = self.sim.model.site_name2id('target_site')
        target_body_id = self.sim.model.body_name2id('target')
        self.sim.model.body_pos[target_body_id] = self.goal - \
            self.sim.model.site_pos[target_site_id]

    def _sample_goal(self):
        posi = self.initial_gripper_xpos[:3] + \
            np.random.uniform(-self.target_range, self.target_range, size=3)
        posi += self.target_offset
        posi[2] = np.random.uniform(
            self.height_offset + 0.1, self.height_offset + 0.3)

        rpy = np.random.uniform(
            [-7 * np.pi / 6, -np.pi / 6, -np.pi / 6], [-5 * np.pi / 6, np.pi / 6, np.pi / 6])

        goal = np.concatenate([posi, rpy])

        return goal

    def _sample_achieved_goal(self):
        object_xpos = self.initial_gripper_xpos[:2]
        while np.linalg.norm(object_xpos - self.initial_gripper_xpos[:2]) < 0.1:
            object_xpos = self.initial_gripper_xpos[:2] + \
                np.random.uniform(-self.obj_range, self.obj_range, size=2)

        return np.concatenate([object_xpos, [0.795]])
