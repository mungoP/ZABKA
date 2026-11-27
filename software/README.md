# Software

Simulation, policy training and the code that runs on the Raspberry Pi. Work in progress: the folders below are the planned layout.

## Walking approach

ŻABKA will walk using a reinforcement learning (RL) policy:

1. **Simulate.** A model of the robot (exported from the Onshape CAD, with measured masses and motor limits) is trained in [NVIDIA Isaac Sim](https://developer.nvidia.com/isaac/sim) using [Isaac Lab](https://github.com/isaac-sim/IsaacLab). Thousands of copies of the robot learn to walk in parallel.
2. **Randomize.** Masses, friction, motor strength, sensor noise and control latency are varied during training (domain randomization), so the policy still works on the real robot despite differences from the simulation.
3. **Deploy.** The trained policy runs on the Raspberry Pi 4 at 50 Hz. Each step it reads the joint positions and velocities, the IMU's gravity vector and angular rate, and the walking command. It outputs a position target for each of the 10 joints.
4. **Track.** The ESP32 bridge sends those targets over CAN, and each motor board tracks them with its own PD and FOC loops at much higher rates. See [`docs/system_architecture.md`](../docs/system_architecture.md).

## Planned folders

| Folder      | Contents                                                                       |
| ----------- | ------------------------------------------------------------------------------ |
| `sim/`      | Robot description (URDF/USD) and Isaac Lab environment                         |
| `training/` | Training configs, reward terms and scripts                                     |
| `deploy/`   | Code that runs on the Pi: loads the policy, talks to the ESP32 over USB serial |
| `tools/`    | Calibration, logging and test utilities                                        |
