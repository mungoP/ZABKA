# Bills of materials

| File                                         | Covers                                                                                   |
| -------------------------------------------- | ---------------------------------------------------------------------------------------- |
| [`actuator_BOM.csv`](actuator_BOM.csv)       | Joint motors, motor controller boards, hand-soldered board parts, bearings and fasteners |
| [`electronics_BOM.csv`](electronics_BOM.csv) | Battery, compute, CAN bridge, IMU, BMS, charging, fusing, e-stop and wiring              |
| [`general_BOM.csv`](general_BOM.csv)         | 3D printing filament                                                                     |

The root [`BOM.csv`](../../BOM.csv) combines all three into one list with a running total.

The full component list for the motor controller board (every SMT part with its LCSC number), along with the placement file, is in [`pcb/motor_board/JLC_fab/`](../../pcb/motor_board/JLC_fab/). That is the BOM JLCPCB used to assemble the boards.

These files are exported from the [project Google Sheet](https://docs.google.com/spreadsheets/d/1IvTHRRW_kaQCn1X5jmZqQL5CsmA0eXGoDoHb6eFF2dk/edit?gid=0#gid=0), which keeps the cell formulas.
