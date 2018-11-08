import pickle
import numpy as np
from aiphy.envs.mujoco.utils import compute_ik as ik

_FLOAT_EPS = np.finfo(np.float64).eps

class PlayModel(object):
    def __init__(self, policy_file):
        with open(policy_file, 'rb') as f:
            self.policy = pickle.load(f)

        self.center = np.array([0.75, 0, 0.95])
        self.size = np.array([0.15, 0.2, 0.15])
        self.rel_size = 0.05
        self.rel_rpy_size = 0.05

    # action for mujoco
    def get_action(self, obs):
        o = obs['observation']
        ag = obs['achieved_goal']
        g = obs['desired_goal']
        policy_output = self.policy.get_actions(
                o, ag, g,
                compute_Q=False,
                noise_eps=0.0,
                random_eps=0.0,
                use_target_net=False)
        return policy_output
    
    # action for the real robot
    def get_action_robot(self, obs):
        u = self.get_action(obs)
        assert len(u) == 6

        o = obs['observation']
        ag = obs['achieved_goal']
        g = obs['desired_goal']

        cur_gripper_pos = (o[:3] - self.center) / self.size
        pos_action = cur_gripper_pos + u[0:3] * self.rel_size / self.size

        cur_gripper_rpy = o[3:]
        if cur_gripper_rpy[0] > 0:
            cur_gripper_rpy[0] -= 2 * np.pi

        cur_gripper_rpy = 3.0 * (cur_gripper_rpy - np.asarray([-np.pi, 0.0, 0.0])) / np.pi
        rpy_action = cur_gripper_rpy + 3.0 * u[3:6] * self.rel_rpy_size / np.pi

        action = np.concatenate([pos_action, rpy_action])
        joint_pos_target = self._action_project(action)
        return joint_pos_target

    def _action_project(self, action):
        action = np.minimum(action, 1)
        action = np.maximum(action, -1)
        act = list(action[0:3] * self.size + self.center)
        pose = np.asarray(action[3:6])
        pose_base = np.asarray([-np.pi, 0.0, 0.0])
        rpy = pose_base + pose * np.pi / 3.0
        quat = list(self.euler2quat(rpy))
        act.extend(quat)
        act = np.asarray(act)
        joint_pos = self._angele_move_rule(act)
        return joint_pos

    def euler2quat(self, euler):
        """ Convert Euler Angles to Quaternions.  See rotation.py for notes """
        euler = np.asarray(euler, dtype=np.float64)
        assert euler.shape[-1] == 3, "Invalid shape euler {}".format(euler)

        ai, aj, ak = euler[..., 2] / 2, -euler[..., 1] / 2, euler[..., 0] / 2
        si, sj, sk = np.sin(ai), np.sin(aj), np.sin(ak)
        ci, cj, ck = np.cos(ai), np.cos(aj), np.cos(ak)
        cc, cs = ci * ck, ci * sk
        sc, ss = si * ck, si * sk

        quat = np.empty(euler.shape[:-1] + (4,), dtype=np.float64)
        quat[..., 0] = cj * cc + sj * ss
        quat[..., 3] = cj * sc - sj * cs
        quat[..., 2] = -(cj * ss + sj * cc)
        quat[..., 1] = cj * cs - sj * sc
        return quat

    def quat2mat(self, quat):
        """ Convert Quaternion to Euler Angles.  See rotation.py for notes """
        quat = np.asarray(quat, dtype=np.float64)
        assert quat.shape[-1] == 4, "Invalid shape quat {}".format(quat)

        w, x, y, z = quat[..., 0], quat[..., 1], quat[..., 2], quat[..., 3]
        Nq = np.sum(quat * quat, axis=-1)
        s = 2.0 / Nq
        X, Y, Z = x * s, y * s, z * s
        wX, wY, wZ = w * X, w * Y, w * Z
        xX, xY, xZ = x * X, x * Y, x * Z
        yY, yZ, zZ = y * Y, y * Z, z * Z

        mat = np.empty(quat.shape[:-1] + (3, 3), dtype=np.float64)
        mat[..., 0, 0] = 1.0 - (yY + zZ)
        mat[..., 0, 1] = xY - wZ
        mat[..., 0, 2] = xZ + wY
        mat[..., 1, 0] = xY + wZ
        mat[..., 1, 1] = 1.0 - (xX + zZ)
        mat[..., 1, 2] = yZ - wX
        mat[..., 2, 0] = xZ - wY
        mat[..., 2, 1] = yZ + wX
        mat[..., 2, 2] = 1.0 - (xX + yY)
        return np.where((Nq > _FLOAT_EPS)[..., np.newaxis, np.newaxis], mat, np.eye(3))

    def _angele_move_rule(self, act):
        assert act.shape == (7,)
        target_pos = act[:3]
        target_rot = self.quat2mat(act[3:])

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
        # if self.data.qpos is not None and self.model.joint_names:
        #     names = [n for n in self.model.joint_names if n.startswith('ur10')]
        #     return (
        #         np.array([self.data.get_joint_qpos(name) for name in names]),
        #         np.array([self.data.get_joint_qvel(name) for name in names]),
        #     )
        return np.zeros(6), np.zeros(0)
