# Lib

Custom KiCad library for ŻABKA, used by the motor controller board in [`pcb/`](../pcb/).

| Path                    | Contents                 |
| ----------------------- | ------------------------ |
| `kicad/zabka.kicad_sym` | Symbols                  |
| `kicad/zabka.pretty/`   | Footprints               |
| `kicad/zabka.3dshapes/` | 3D models (STEP and WRL) |

These are the JLCPCB extended parts (STSPIN32G4, MOSFETs, AS5047P, TCAN3414, connectors, shunts and others). They were imported from LCSC/EasyEDA with the [easyeda2kicad](https://github.com/uPesy/easyeda2kicad.py) CLI tool, for example:

```sh
easyeda2kicad --full --lcsc_id=C7434864 --output lib/kicad/zabka
```

Footprints reference their 3D models as `${KIPRJMOD}/../../lib/kicad/zabka.3dshapes/...`, so the paths work for any project two levels below the repo root, such as `pcb/motor_board/`.

JLCPCB basic parts are not stored here. They come from the [JLCPCB KiCad Library](https://github.com/CDFER/JLCPCB-Kicad-Library) plugin.
