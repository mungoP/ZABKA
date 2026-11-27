# CAN protocol

All 10 motor controller boards and the ESP32-C3 bridge share one CAN bus. This document defines the physical layer, timing and message format. For how the bus fits into the rest of the robot, see [`system_architecture.md`](system_architecture.md).

## Physical layer

| Setting | Value |
| ------- | ----- |
| Standard | Classic CAN 2.0, 11-bit IDs. **Not CAN FD**: the ESP32's TWAI controller treats FD frames as errors. The motor boards' TCAN3414 handles classic CAN without any hardware change. |
| Bit rate | 1 Mbps |
| Connectors | JST-GH 4-pin, J4 and J5 on each motor board (wired in parallel, one in, one out) |
| Cable | Twisted CANH/CANL pair, pin 1 to pin 1 on every cable. Total length about 2–3 m, well under the roughly 40 m limit at 1 Mbps. |

### Connector pinout (J4, J5)

| Pin | Signal |
| --- | ------ |
| 1 | CANL |
| 2 | CANH |
| 3 | GND |
| 4 | Spare |

This matches the motor board schematic (`pcb/motor_board/CAN_bus.kicad_sch`). The bridge's GH connectors must use the same pinout.

Many "JST 1.25 mm" listings are actually Molex PicoBlade, which does not mate with JST-GH.

### Topology

```text
 L ankle [TERM] ─ L knee ─ L hip ─ L hip ─ L hip ─┐                 ┌─ R hip ─ R hip ─ R hip ─ R knee ─ R ankle [TERM]
                                                  │                 │
                                          GH ─────┴── ESP32 bridge ─┴───── GH
                                                (SN65HVD230, 120 Ω REMOVED)
```

Electrically this is one linear bus. The bridge sits in the pelvis with two GH connectors wired in parallel (CANH to CANH, CANL to CANL, GND to GND), the same pass-through scheme as the motor boards.

### Termination

- Bridge solder jumpers **JP1 and JP2 only on the two ankle boards**, which are the ends of the bus. Leave them open on every other board.
- Remove the 120 Ω resistor across CANH/CANL on the Waveshare SN65HVD230 board, because the bridge is in the middle of the bus.
- Check with the power off: CANH to CANL should measure about 60 Ω.

## Timing

The ESP32 runs a fixed CAN cycle: one command to each of the 10 boards, then 10 replies, 20 frames in total. At 1 Mbps with 8-byte frames, one frame takes 111 µs at best and 135 µs in the worst case (worst-case bit stuffing).

| CAN cycle rate | Worst-case bus load (one bus, 10 motors) |
| -------------- | ---------------------------------------- |
| 50 Hz | 14 % |
| 100 Hz | 27 % |
| **200 Hz** | **54 % (chosen)** |
| 250 Hz | 68 % |
| 300 Hz | 81 % |
| 370 Hz | 100 % (absolute ceiling) |

200 Hz is 4× the typical 50 Hz policy rate while leaving the bus about half free.

## Messages

### Command (bridge → motor board)

One 8-byte frame per board per cycle, MIT-style (the format used by the MIT Mini Cheetah actuators). Each command carries:

- position target
- velocity target
- position gain Kp
- velocity gain Kd
- feed-forward torque

The board computes `torque = Kp·(pos_target − pos) + Kd·(vel_target − vel) + torque_ff` in its own control loop, so the bridge only sends setpoints.

### Reply (motor board → bridge)

One frame per board per cycle with the measured position, velocity and current, plus fault and temperature status.

### Special commands

Enable, disable (damping) and zero-position commands will use reserved frames.

### Timeout

If a motor board receives no valid command for **50 ms**, it switches to damping (phase short) on its own. See [Safety and e-stop](system_architecture.md#safety-and-e-stop).

> **TBD:** CAN IDs per joint, the exact bit packing and scaling of each field, the reply layout and the special command codes. They will be defined in `firmware/common/` and this section will be updated to match.
