# PCB

KiCad 10 project for the ŻABKA motor controller board. One board mounts on the back of each joint motor, 10 in total.

## Motor controller board

|                 |                                                                      |
| --------------- | -------------------------------------------------------------------- |
| **Size**        | 58 mm round (72.97 × 69.84 mm overall due to the mounting tabs)      |
| **Stackup**     | 4 layers, 1.6 mm FR-4, 1 oz outer / 0.5 oz inner copper, ENIG        |
| **Controller**  | STSPIN32G4 (STM32G431 + three-phase gate driver + buck)              |
| **Power stage** | 6 × HYG053N10NS1C2 100 V MOSFETs, three 1 mΩ shunts, 36–50.4 V input |
| **Sensing**     | AS5047P magnetic encoder, bus voltage, board and motor temperature   |
| **Interface**   | CAN (TCAN3414) on two JST-GH connectors, SWD on JST-GH               |

The design is explained in [`docs/system_architecture.md`](../docs/system_architecture.md#motor-controller-board), and the CAN wiring and termination in [`docs/can_protocol.md`](../docs/can_protocol.md).

## Folder layout

| Path                                           | Contents                                                                                                                                                                                                                             |
| ---------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| [`motor_board/`](motor_board/)                 | KiCad project: `motor_board.kicad_pro`, the top-level schematic and its sheets (`power_input`, `half_bridge`, `controller`, `encoder`, `monitoring`, `CAN_bus`), and the layout `motor_board.kicad_pcb`                              |
| [`motor_board/JLC_fab/`](motor_board/JLC_fab/) | Files sent to JLCPCB: Gerbers and drill files (`gerbers/` and `motor_board_gerbers.zip`), the assembly BOM (`motor_board_BOM_JLC.csv`), the placement file (`motor_board_CPL_JLC.csv`) and the order settings (`ORDER_SETTINGS.txt`) |

All SMT parts are assembled by JLCPCB (Economic PCBA, top side only). Three through-hole or back-side parts are hand-soldered: C17 (220 µF bulk capacitor), J2 (XT30 power) and J1 (SWD). They are listed in [`docs/BOMs/actuator_BOM.csv`](../docs/BOMs/actuator_BOM.csv).

## Libraries

To open the project, install:

- **Basic parts:** the [JLCPCB KiCad Library](https://github.com/CDFER/JLCPCB-Kicad-Library) by CDFER, from KiCad's Plugin and Content Manager. It provides the symbols, footprints and 3D models for the JLCPCB basic parts (passives, crystal, diodes, inductor).
- **Extended parts:** imported from LCSC/EasyEDA with the [easyeda2kicad](https://github.com/uPesy/easyeda2kicad.py) CLI tool and stored in this repo under [`lib/kicad/`](../lib/kicad/). That folder holds `zabka.kicad_sym` (symbols), `zabka.pretty/` (footprints) and `zabka.3dshapes/` (3D models). The project's `sym-lib-table` and `fp-lib-table` already point to it, so no setup is needed.
