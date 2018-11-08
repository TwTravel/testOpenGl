import numpy as np

from gym import error
try:
    import mujoco_py
except ImportError as e:
    raise error.DependencyNotInstalled("{}. (HINT: you need to install mujoco_py, and also perform the setup instructions here: https://github.com/openai/mujoco-py/.)".format(e))


def set_vel_to_zero(sim):
    sim.data.qvel[:] = np.zeros_like(sim.data.qvel)
    sim.forward()


def set_equality_constraint_by_name(sim, joint_name, active, angle=0.0):
    sim.model.eq_active[sim.model.joint_name2id(joint_name)] = active
    if active is True:
        sim.model.qpos0[sim.model.joint_name2id(joint_name)] = angle


def set_equality_constraint_by_id(sim, joint_id, active, angle=0.0):
    # in this function, we assume all joints are hinge joint or slide joint
    sim.model.eq_active[joint_id] = active
    if active is True:
        sim.model.eq_data[joint_id, 0] = angle
        sim.data.qpos[joint_id] = angle
        sim.forward()


def set_camera_status(sim, cam_name, pos=None, quat=None, fovy=None):
    cam_id = sim.model.camera_name2id(cam_name)
    if pos is not None:
        sim.model.cam_pos[cam_id, :] = pos
    if quat is not None:
        sim.model.cam_quat[cam_id, :] = quat
    if fovy is not None:
        sim.model.cam_fovy[cam_id] = fovy
