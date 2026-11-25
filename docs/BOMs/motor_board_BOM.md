# Motor board BOM

This is the parts list for the ŻABKA motor controller, a 58 mm round, 4-layer board that mounts on the back of each MAD 5008 motor. The robot uses 10 of them, one per joint, all on a shared CAN bus.

Every part has an LCSC number, so it can be ordered from LCSC or assembled by JLCPCB. The KiCad project is in `pcb/motor_board/`, and the files I sent to JLCPCB are in `pcb/motor_board_jlc/`.

## Assembled by JLCPCB (top side)

| Designator                              | Part                                              | LCSC      |
| --------------------------------------- | ------------------------------------------------- | --------- |
| U1                                      | STSPIN32G4 motor controller (gate driver + STM32G431) | C7434864  |
| U2                                      | TCAN3414 CAN transceiver                          | C22396693 |
| U5                                      | AS5047P magnetic encoder                          | C962063   |
| Q101, Q102, Q201, Q202, Q301, Q302      | HYG053N10NS1C2 100 V MOSFET                       | C2986239  |
| R108, R208, R308                        | 1 mΩ 3 W current shunt, 2512                      | C500734   |
| L1                                      | 18 µH inductor                                    | C168016   |
| X1                                      | 25 MHz crystal                                    | C9006     |
| D1                                      | Red LED, 0603                                     | C2286     |
| D2                                      | SS210 Schottky diode                              | C14996    |
| D3                                      | 5.0SMDJ51A TVS diode                              | C2990373  |
| D4                                      | PESD1CAN CAN ESD protection                       | C15771    |
| FB1                                     | Ferrite bead, 0603                                | C1002     |
| TH1                                     | 10 kΩ NTC thermistor, 0603                        | C13564    |
| J3                                      | GH 1.25 mm 2-pin connector (motor temperature)    | C2845397  |
| J4, J5                                  | JST-GH 4-pin connector (CAN in / out)             | C189895   |
| C1, C2                                  | 15 pF capacitor, 0402                             | C1548     |
| C3, C4, C8, C11, C13, C21, C22          | 100 nF capacitor, 0402                            | C1525     |
| C5                                      | 10 µF capacitor, 0603                             | C19702    |
| C6, C7, C103, C203, C303                | 100 nF 100 V capacitor, 0805                      | C28233    |
| C9, C10                                 | 22 µF capacitor, 1206                             | C12891    |
| C12, C15, C23                           | 1 µF capacitor, 0402                              | C52923    |
| C14                                     | 4.7 µF capacitor, 0805                            | C1779     |
| C16, C18, C19                           | 33 nF capacitor, 0603                             | C21117    |
| C20                                     | 4.7 nF capacitor, 0402                            | C1538     |
| C101, C102, C201, C202, C301, C302      | 4.7 µF 100 V capacitor, 1210                      | C337978   |
| C104, C204, C304                        | 1 µF capacitor, 0603                              | C15849    |
| C105, C205, C305                        | 10 pF capacitor, 0402                             | C32949    |
| C106, C206, C306                        | 100 pF capacitor, 0402                            | C1546     |
| R1                                      | 1 kΩ resistor, 0402                               | C11702    |
| R2, R9, R10, R17                        | 10 kΩ resistor, 0402                              | C25744    |
| R3                                      | 22 kΩ resistor, 0402                              | C25768    |
| R4                                      | 3.3 kΩ resistor, 0402                             | C25890    |
| R5                                      | 100 kΩ resistor, 0402                             | C25741    |
| R6, R7, R8                              | 100 Ω resistor, 1206                              | C17901    |
| R11                                     | 75 kΩ resistor, 0603                              | C23242    |
| R12                                     | 3 kΩ resistor, 0603                               | C4211     |
| R13, R14, R15, R16                      | 120 Ω resistor, 0402                              | C25079    |
| R18, R19, R20                           | 33 Ω resistor, 0402                               | C25105    |
| R101, R103, R201, R203, R301, R303      | 10 Ω resistor, 0402                               | C25077    |
| R102, R104, R202, R204, R302, R304      | 47 kΩ resistor, 0402                              | C25792    |
| R105, R107, R205, R207, R305, R307      | 100 kΩ resistor, 0603                             | C25803    |
| R106, R109, R206, R209, R306, R309      | 1.5 kΩ resistor, 0603                             | C22843    |
| R110, R210, R310                        | 49.9 kΩ resistor, 0402                            | C25897    |

## Hand-soldered (back side)

| Designator | Part                                         | LCSC    |
| ---------- | -------------------------------------------- | ------- |
| J1         | JST-GH 5-pin connector (SWD programming)     | C189896 |
| J2         | XT30PW-M power connector                     | C431092 |
| C17        | 220 µF 100 V electrolytic capacitor, 12.5 × 20 mm | C350119 |

Also needed, but not on the board: a diametrically magnetized magnet (8 × 3 mm) glued to the end of the motor shaft for the encoder.

## How it's wired

```text
12S battery (36–50.4 V)
   │
   J2 XT30 ── VM ──┬── C17 bulk cap + D3 TVS (VM to PGND)
                   ├── 3 half-bridges (Q101–Q302) ── phase pads TP101 / TP201 / TP301 ── motor
                   └── U1 STSPIN32G4 ── buck (L1, D2) ── gate drive and 3.3 V logic

U1 ── SPI ──── U5 AS5047P encoder (reads the magnet on the motor shaft)
U1 ── CAN ──── U2 TCAN3414 ── J4 / J5 (CAN in and out, daisy-chained board to board)
U1 ── ADC ──── current shunts (R108 / R208 / R308), board temp (TH1), motor temp (J3), battery voltage (R11 / R12)
J1 ── SWD programming
```

Connector pinouts:

- **J2 (XT30):** 1 = PGND (−), 2 = VM (+)
- **J4, J5 (CAN):** 1 = CANL, 2 = CANH, 3 = GND, 4 = spare
- **J1 (SWD):** 1 = 3V3, 2 = GND, 3 = SWDIO, 4 = SWCLK, 5 = NRST
- **J3 (motor temp):** 1 = thermistor, 2 = GND

CAN termination: bridge solder jumpers JP1 and JP2 only on the last board at each end of the CAN chain.
