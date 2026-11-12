# Motor Driver Board: Wiring, Cost, and Parts

**Status:** Schematic complete (all 8 sheets), not yet laid out, built or tested.

**Last updated:** 2026-11-12, to match the KiCad schematic.

**Target:** One 12S (36–50.4 V) motor board using the STSPIN32G4, AS5047P encoder, and CAN FD.

**Assumptions:** 10 boards, JLCPCB Economic PCBA, single-sided SMT assembly, 58 mm round, 4 layers, 2 oz outer copper. Prices are estimates in USD.

**Related file:** `pcb/motorboard/motor_board.csv` (from this `doc/` folder: `../pcb/motorboard/motor_board.csv`).

**Refs:** the refs below are the ones annotated in KiCad. The half-bridge sheet is placed three times and annotated with "first free after sheet number × 100", so phase U parts are 1xx, phase V parts 2xx and phase W parts 3xx.

---

## 1. Simple wiring

### Schematic sheets

| Sheet                   | File                         | Contents                                                                         |
| ----------------------- | ---------------------------- | -------------------------------------------------------------------------------- |
| Root                    | `motor_board.kicad_sch`      | Sheet symbols and inter-sheet wiring only                                        |
| Controller              | `controller.kicad_sch`       | U1 STSPIN32G4 and all of its support parts, GND/PGND net tie                     |
| Power and monitoring    | `power_input.kicad_sch`      | XT30, bulk cap, TVS, bus voltage sense, MOSFET and motor temperature             |
| Half bridge – U / V / W | `half_bridge.kicad_sch` (×3) | 2 MOSFETs, gate drive, bootstrap, bus ceramics, shunt, op-amp network, phase pad |
| CAN bus                 | `CAN_bus.kicad_sch`          | TCAN3414 transceiver, split termination, ESD, two pass-through connectors        |
| Encoder                 | `encoder.kicad_sch`          | AS5047P with SPI series resistors and decoupling                                 |

### Power

```text
12S+ --> XT30 (J2) --> VM bus --> high-side MOSFET drains (Q101/Q201/Q301)
                          |
                          +--> STSPIN32G4 VM --> SW --> L1 18 uH --> VCC (8 V at boot, firmware sets 12 V)
                                                                      |
                                                                      +--> Gate drivers + bootstrap caps
                                                                      |
                                                                      +--> 3 x 100 ohm (R6-R8) --> REGIN --> LDO --> +3V3
                                                                                                                  |
                                                                                                                  +--> MCU (+ FB1 --> VDDA/VREF+)
                                                                                                                  +--> AS5047P
                                                                                                                  +--> TCAN3414

12S- --> XT30 (J2) --> PGND --> shunts --> low-side MOSFET sources
                         |
                         +--> NT1 net tie --> GND (single point, place next to U1 PGND pin / EPAD)
```

### One motor phase (sheet placed 3 times)

```text
VM --> Q_HS --> OUT (phase pad TPx01, U1 OUTx, bootstrap cap) --> Q_LS --> SHUNT_TOP --> 1 mohm shunt --> PGND
                                                                                |                     |
                                                                         net tie --> 1.5 k --> OPP    net tie --> 1.5 k --> OPN

GHSx --> 22 ohm --> high-side gate, 47 k gate to OUT
GLSx --> 22 ohm --> low-side gate,  47 k gate to SHUNT_TOP
BOOTx --> 1 uF --> OUT
```

### Signal connections (U1 pins)

| Function                | U1 pin                                            | Connects to                    | Notes                                |
| ----------------------- | ------------------------------------------------- | ------------------------------ | ------------------------------------ |
| Phase U current         | 14 PA1 (OPP1), 16 PA3 (OPN1), 15 PA2 (OPO1)       | Half bridge – U                | Kelvin routing via net ties          |
| Phase V current         | 20 PA7 (OPP2), 22 PC5 (OPN2), 19 PA6 (OPO2)       | Half bridge – V                | Kelvin routing via net ties          |
| Phase W current         | 23 PB0 (OPP3), 25 PB2 (OPN3), 24 PB1 (OPO3)       | Half bridge – W                | Kelvin routing via net ties          |
| Gate drive U            | 43 GHS1, 29 GLS1, 42 OUT1, 41 BOOT1               | Half bridge – U                |                                      |
| Gate drive V            | 40 GHS2, 30 GLS2, 39 OUT2, 38 BOOT2               | Half bridge – V                |                                      |
| Gate drive W            | 37 GHS3, 31 GLS3, 36 OUT3, 35 BOOT3               | Half bridge – W                |                                      |
| Encoder clock           | 54 PB3 (ENC_SCK)                                  | U5 pin 2 CLK via R18 33 ohm    | SPI3                                 |
| Encoder data in         | 55 PB4 (ENC_MISO)                                 | U5 pin 3 MISO via R19 33 ohm   | SPI3                                 |
| Encoder data out        | 56 PB5 (ENC_MOSI)                                 | U5 pin 4 MOSI via R20 33 ohm   | SPI3                                 |
| Encoder select          | 51 PA15 (ENC_CS)                                  | U5 pin 1 CSn, R17 10 k pull-up | Chip select                          |
| CAN transmit            | 48 PA12 (CAN_TX)                                  | U2 pin 1 TXD                   | FDCAN1 TX                            |
| CAN receive             | 47 PA11 (CAN_RX)                                  | U2 pin 4 RXD                   | FDCAN1 RX                            |
| Bus voltage             | 13 PA0 (VBUS_SENSE)                               | R11 75 k / R12 3 k divider     | Vbus = Vadc x 26.0, ~86 V full scale |
| MOSFET temperature      | 21 PC4 (MOSFET_TEMP)                              | TH1 NTC divider                | Place TH1 between the MOSFETs        |
| Motor temperature       | 9 PC0 (MOTOR_TEMP)                                | J3 external NTC probe          | Probe likely B 3950                  |
| Short-circuit threshold | 52 SCREF                                          | R3 22 k / R4 10 k              | ~1.03 V, VDS trip ~1 V               |
| Status LED              | 53 PD2 (STATUS_LED)                               | R1 1 k, D1 red LED             | Firmware status                      |
| Debug                   | 49 PA13, 50 PA14, 8 PG10                          | J1 SWD header                  | SWDIO, SWCLK, NRST                   |
| Boot pin                | 59 PB8-BOOT0                                      | R2 10 k to GND                 | Boots user code from flash           |
| Crystal                 | 6 PF0, 7 PF1                                      | X1 25 MHz                      | Firmware HSE = 25 MHz                |
| Unused                  | 3–5, 10–12, 17, 18, 28, 33, 34, 44–46, 57, 58, 60 | No-connect                     | PA8/PA9 freed (no output encoder)    |

### Current-sense network (per phase)

```text
Shunt top    --> net tie --> 1.5 k (Rx06) --> OPP
Shunt bottom --> net tie --> 1.5 k (Rx09) --> OPN
OPO --> 49.9 k (Rx10) --> OPN
OPP --> 100 k (Rx05) --> +3V3
OPP --> 100 k (Rx07) --> GND
```

Gain = 49.9 k / 1.5 k ≈ 33. Output rests at 1.65 V, giving about ±50 A full scale with a 1 mohm shunt. The two 100 k bias resistors look like 50 k from OPP, which matches the 49.9 k feedback so the difference amp stays balanced. Change the feedback and bias resistors together if the gain changes.

### Layout notes

- PGND and GND join only at NT1. Place it next to U1's PGND pin (32) and exposed pad.
- Keep each hot loop tight: Q_HS drain → Q_LS → shunt → PGND → C101/C102/C103 back to VM.
- Route each shunt's sense pair from the net ties as a pair, away from OUT and gate traces. Put the op-amp resistors next to U1, not next to the shunt.
- Bootstrap caps (C104/C204/C304) go at U1's BOOTx/OUTx pins.
- AS5047P at the exact board center over the motor shaft. Keep phase, VM, L1 and steel hardware out of a ~10 mm radius.
- TH1 sits between the MOSFETs, with thermal vias under each PDFN drain pad.
- R11 (75 k) sees ~48 V, so it must be 0603 or larger.
- All SMT parts on the motor-facing side to stay on Economic PCBA.
- J2 (XT30) and C17 (bulk cap) are through-hole on the back side. C17 is laid flat and glued/RTV'd for vibration.

### Firmware startup

1. Write VCC = 12 V (VCC_VAL = 10) over the internal I2C bus before enabling PWM. The MOSFETs have Vgs(th) up to 4 V and are only rated at 10 V gate drive, so the 8 V default is too low.
2. Clear the power-up fault (all INx low for 160 us, or write the CLEAR register).
3. Turn on the low sides first to charge the bootstrap caps before running the high sides. Keep PWM duty a bit under 100% so the bootstrap caps recharge every cycle.
4. Read each current-sense ADC with the motor idle and store it as zero amps.
5. Start on a current-limited bench supply at about 20 V and 1–2 A.
6. Use about 500 ns dead time (the driver's 150 ns minimum is not added on top), then tune it on a scope.
7. Set a firmware regen voltage limit around 56 V. The STSPIN32G4 is rated to 75 V.
8. SPI3 at ~5.3 MHz (170 MHz / 32) for the AS5047P.
9. Run a homing routine at startup to find the joint angle, since the motor encoder has 20 possible output positions through the 20:1 gearbox.

---

## 2. Estimated cost per board

Prices are snapshots from earlier sessions. Lines marked "re-quote" changed since the first estimate and need a fresh JLC quote.

| Cost category                             | Per board  | Basis                                |
| ----------------------------------------- | ---------- | ------------------------------------ |
| STSPIN32G4                                | $5.36      | LCSC 10+ price snapshot              |
| 6 x HYG053N10NS1C2 MOSFETs                | $3.30      | Upper JLCPCB price estimate          |
| AS5047P encoder                           | $4.58      | LCSC 10+ price snapshot              |
| TCAN3414DR CAN transceiver                | re-quote   | Replaced the SN65HVD230              |
| 3 x 1 mohm shunts                         | ~$0.18     | LCSC snapshot, $0.0585 each          |
| 6 x 4.7 uF 100 V 1210                     | ~$1.71     | LCSC snapshot, $0.2842 each          |
| Other passives, protection and connectors | $4-7       | Rough estimate                       |
| **Components subtotal**                   | **$20-25** |                                      |
| 4-layer PCB, 2 oz outer copper            | $3-6       | Rough estimate, confirm with a quote |
| JLCPCB Economic assembly                  | $5-7       | See below                            |
| **Total before shipping and tariffs**     | **$28-38** | 10-board order                       |

### Assembly estimate for 10 boards

| Fee                    | Total       | Assumption                                                |
| ---------------------- | ----------- | --------------------------------------------------------- |
| Setup and stencil      | $9.71       | $8.18 setup + $1.53 stencil                               |
| Extended part loading  | ~$30.70     | 10 extended lines x $3.07 (more if FB1 or J1 is extended) |
| SMT joints             | re-quote    | About 103 SMT parts per board                             |
| Through-hole soldering | $0          | J2 and C17 hand-soldered                                  |
| **Assembly subtotal**  | **~$45-55** | **About $5 per board**                                    |

### Extended parts (loading fee each)

| Part                        | LCSC      | Used as        |
| --------------------------- | --------- | -------------- |
| STSPIN32G4                  | C7434864  | U1             |
| HYG053N10NS1C2              | C2986239  | Qx01, Qx02     |
| AS5047P-ATSM                | C141793   | U5             |
| TCAN3414DR                  | C22396693 | U2             |
| PESD1CAN,215                | C15771    | D4             |
| SM04B-GHS-TB                | C189895   | CAN connectors |
| FNR6020S180MT 18 uH         | C168016   | L1             |
| NCP18XH103F03RB NTC         | C13564    | TH1            |
| C1210X475K101T 4.7 uF 100 V | C337978   | Cx01, Cx02     |
| LR2512D-3W-1mR-1% shunt     | C500734   | Rx08           |

Everything else was chosen from JLC Basic parts. C17 (C371297) is through-hole and hand-soldered, so it adds no loading fee.

---

## 3. Detailed part list

LCSC numbers marked "verify" are the JLC Basic part matching the value and voltage on the schematic. Check the package against the footprint field of each symbol before ordering. The final JLC BOM should be exported from KiCad so the LCSC fields come straight from the symbols. See also `pcb/motorboard/motor_board.csv`.

### Controller sheet

| Ref        | Part              | Qty | Value / package                  | LCSC                  | Notes                                              |
| ---------- | ----------------- | --- | -------------------------------- | --------------------- | -------------------------------------------------- |
| U1         | STSPIN32G4        | 1   | VFQFPN-64 9x9                    | C7434864              | Gate driver + STM32G431                            |
| L1         | Buck inductor     | 1   | 18 uH, FNR6020S180MT             | C168016               | SW → VCC, extended                                 |
| D2         | Buck diode        | 1   | SS210, 100 V Schottky            | C14996                | Cathode SW, anode PGND                             |
| C9, C10    | Buck output       | 2   | 22 uF, 25 V, 1206                | C12891                | VCC → PGND                                         |
| R6, R7, R8 | REGIN resistors   | 3   | 100 ohm, 1206, parallel ≈ 33 ohm | C17901                | VCC → REGIN, cuts LDO heat at VCC = 12 V (Fig. 10) |
| C14        | REGIN cap         | 1   | 4.7 uF, 25 V                     | C1779 (0805, verify)  | REGIN → GND                                        |
| C6, C7     | VM decoupling     | 2   | 100 nF, 100 V, 0805              | C28233                | VM → PGND at pin 61                                |
| C4         | REG3V3 decoupling | 1   | 100 nF, 16 V, 0402               | C1525                 | Pin 1                                              |
| C5         | REG3V3 bulk       | 1   | 10 uF, 10 V                      | C19702 (0603, verify) | Pin 1                                              |
| C3         | VBAT cap          | 1   | 100 nF, 16 V, 0402               | C1525                 | Pin 2                                              |
| FB1        | Ferrite bead      | 1   | 200 mA                           | From symbol           | +3V3 → VDDA/VREF+                                  |
| C11, C13   | VDDA / VREF+ HF   | 2   | 100 nF, 16 V, 0402               | C1525                 | Pins 26, 27                                        |
| C12, C15   | VDDA / VREF+ bulk | 2   | 1 uF, 25 V, 0402                 | C52923                | Pins 26, 27                                        |
| R3         | SCREF top         | 1   | 22 k                             | C25768 (0402, verify) | +3V3 → SCREF                                       |
| R4         | SCREF bottom      | 1   | 10 k, 0402                       | C25744                | SCREF → GND, sets ~1.03 V                          |
| R5         | NRST pull-up      | 1   | 100 k, 0402                      | C25741                |                                                    |
| C8         | NRST cap          | 1   | 100 nF, 16 V, 0402               | C1525                 |                                                    |
| R2         | BOOT0 pull-down   | 1   | 10 k, 0402                       | C25744                | Boot from main flash                               |
| X1         | Crystal           | 1   | 25 MHz, X322525MOB4SI            | C9006                 | PF0/PF1, pins 2/4 GND                              |
| C1, C2     | Crystal load caps | 2   | 15 pF, 50 V, 0402                | C1548                 |                                                    |
| R1         | LED resistor      | 1   | 1 k                              | C11702 (0402, verify) |                                                    |
| D1         | Status LED        | 1   | Red                              | From symbol           | PD2                                                |
| J1         | SWD header        | 1   | 1x05, 3V3/GND/SWDIO/SWCLK/NRST   | Hand-fit or exclude   |                                                    |
| NT1        | Net tie           | 1   | NetTie_2                         | None                  | Single PGND–GND join                               |

### Power and monitoring sheet

| Ref      | Part                | Qty | Value / package                             | LCSC        | Notes                                                        |
| -------- | ------------------- | --- | ------------------------------------------- | ----------- | ------------------------------------------------------------ |
| J2       | XT30PW-M            | 1   | Through-hole, back side                     | Hand-solder | Pin 1 (square) PGND, pin 2 (round) VM                        |
| C17      | Bulk cap            | 1   | 470 uF, 63 V, Samyoung NXA63V470M12.5*25 LO | C371297     | 41 mohm ESR, 1.9 A ripple. Laid flat, glued. Check pad 1 = + |
| D3       | TVS                 | 1   | SMCJ54CA, bidirectional, 1.5 kW             | C41411761   | VM → PGND. Not for sustained regen                           |
| TP1, TP2 | Test points         | 2   | Pads                                        | None        | VM, PGND                                                     |
| R11      | Bus divider top     | 1   | 75 k, 0603                                  | C23242      | Must be 0603 or larger for voltage                           |
| R12      | Bus divider bottom  | 1   | 3 k, 0603                                   | C4211       |                                                              |
| C19      | Bus sense filter    | 1   | 33 nF, 50 V, 0603                           | C21117      |                                                              |
| TH1      | MOSFET NTC          | 1   | 10 k, B 3380, NCP18XH103F03RB               | C13564      | Extended, place between MOSFETs                              |
| R9       | NTC pull-up         | 1   | 10 k, 0402                                  | C25744      |                                                              |
| C16      | NTC filter          | 1   | 33 nF, 50 V, 0603                           | C21117      |                                                              |
| J3       | Motor NTC connector | 1   | 2-pin                                       | From symbol | Pin 1 MOTOR_TEMP, pin 2 GND                                  |
| R10      | Motor NTC pull-up   | 1   | 10 k, 0402                                  | C25744      |                                                              |
| C18      | Motor NTC filter    | 1   | 33 nF, 50 V, 0603                           | C21117      |                                                              |

### Half-bridge sheet (x3: refs x = 1, 2, 3 for U, V, W)

| Ref          | Part             | Qty per board | Value / package          | LCSC     | Notes                                              |
| ------------ | ---------------- | ------------- | ------------------------ | -------- | -------------------------------------------------- |
| Qx01         | High-side MOSFET | 3             | HYG053N10NS1C2, PDFN5x6  | C2986239 | D (5–8, 9) VM, S OUT                               |
| Qx02         | Low-side MOSFET  | 3             | HYG053N10NS1C2, PDFN5x6  | C2986239 | D OUT, S SHUNT_TOP                                 |
| Rx01, Rx03   | Gate resistors   | 6             | 22 ohm, 0603             | C23345   | ~0.43 A peak, ~1 V/ns edges. Tune on scope         |
| Rx02, Rx04   | Gate pull-downs  | 6             | 47 k, 0402               | C25792   | Each to its own FET's source                       |
| Cx04         | Bootstrap        | 3             | 1 uF, 50 V, X7R, 0603    | C15849   | BOOT → OUT. Internal bootstrap diode               |
| Cx01, Cx02   | Bus ceramics     | 6             | 4.7 uF, 100 V, X7R, 1210 | C337978  | VM → PGND, extended                                |
| Cx03         | Bus HF bypass    | 3             | 100 nF, 100 V, 0805      | C28233   | VM → PGND, closest to FETs                         |
| Rx08         | Current shunt    | 3             | 1 mohm, 1%, 3 W, 2512    | C500734  | SHUNT_TOP → PGND, extended                         |
| NTx01, NTx02 | Net ties         | 6             | NetTie_2                 | None     | Kelvin taps at shunt pads                          |
| Rx06, Rx09   | Op-amp inputs    | 6             | 1.5 k, 1%, 0603          | C22843   |                                                    |
| Rx10         | Op-amp feedback  | 3             | 49.9 k, 1%, 0603         | C23184   | OPO → OPN. 0603 because 49.9 k is extended in 0402 |
| Rx05, Rx07   | Op-amp bias      | 6             | 100 k, 1%, 0603          | C25803   | OPP to +3V3 and GND                                |
| TPx01        | Phase pad        | 3             | Solder pad               | None     | Size for the MAD 5008 phase wire                   |

### CAN bus sheet

| Ref      | Part                   | Qty | Value / package            | LCSC                  | Notes                                          |
| -------- | ---------------------- | --- | -------------------------- | --------------------- | ---------------------------------------------- |
| U2       | CAN FD transceiver     | 1   | TCAN3414DR, SOIC-8         | C22396693             | STB (8) and SHDN (5) to GND = normal mode      |
| C21      | Transceiver decoupling | 1   | 100 nF, 16 V, 0402         | C1525                 | At pin 3                                       |
| R13–R16  | Split termination      | 4   | 120 ohm                    | C25079 (0402, verify) | Two 60 ohm halves                              |
| C20      | Termination mid-point  | 1   | 4.7 nF, 50 V               | C1538 (0402, verify)  | Common-mode filter                             |
| JP1, JP2 | Termination jumpers    | 2   | Solder jumper, open        | None                  | Bridge both only on the last board of each leg |
| D4       | CAN ESD                | 1   | PESD1CAN,215, SOT-23       | C15771                | Extended                                       |
| J4, J5   | CAN connectors         | 2   | JST GH 4-pin, SM04B-GHS-TB | C189895               | Pin 1 CANH, 2 CANL, 3 GND, 4 spare             |

### Encoder sheet

| Ref           | Part             | Qty | Value / package           | LCSC       | Notes                                        |
| ------------- | ---------------- | --- | ------------------------- | ---------- | -------------------------------------------- |
| U5            | Magnetic encoder | 1   | AS5047P-ATSM, TSSOP-14    | C141793    | 3.3 V mode: VDD and VDD3V3 tied, TEST to GND |
| R17           | CSn pull-up      | 1   | 10 k, 0402                | C25744     | Keeps encoder deselected at boot             |
| R18, R19, R20 | SPI series       | 3   | 33 ohm, 0402              | C25105     | CLK/MOSI near U1, MISO near U5               |
| C22           | Decoupling       | 1   | 100 nF, 16 V, 0402        | C1525      |                                              |
| C23           | Decoupling       | 1   | 1 uF, 25 V, 0402          | C52923     | Extra, for distance from the LDO             |
| M1            | Magnet           | 1   | N35H, 8 x 3 mm, diametric | Not on BOM | ~2 mm from the sensor                        |

### Board specification

| Item                | Specification                                            |
| ------------------- | -------------------------------------------------------- |
| Outline             | Round, 58 mm diameter, 1.6 mm thick                      |
| Mounting            | M3 on a 51 mm bolt circle, angles TBD                    |
| Layers              | 4                                                        |
| Stackup             | Signal / solid ground / power and phase routing / signal |
| Outer copper        | 2 oz (confirm Economic PCBA allows it)                   |
| Inner copper        | 0.5–1 oz                                                 |
| Assembly            | Single-sided JLCPCB Economic PCBA, motor-facing side     |
| SMT parts per board | About 103                                                |
| Hand-soldered       | J2 XT30, C17 bulk cap (J1 and J3 if not JLC-assembled)   |

## Changes from the first estimate

- CAN transceiver: SN65HVD230 → TCAN3414DR (CAN FD), with split termination, PESD1CAN ESD and two JST GH pass-through connectors.
- Power input: one XT30 only (no pass-through), 470 uF 63 V bulk cap, SMCJ54CA TVS (Basic) instead of SMBJ54A.
- Bus divider: 75 k / 3 k instead of 72.3 k / 3.01 k (both Basic).
- Added motor winding temperature input (J3 → PC0).
- Output encoder header and I2C pull-ups removed. Joint angle comes from homing. PA8/PA9 now no-connect.
- Crystal: 25 MHz instead of 24 MHz, with 15 pF load caps.
- Buck: SS210 instead of STPS1H100A, 2 x 22 uF instead of 47 uF.
- REGIN: 3 x 100 ohm 1206 in parallel instead of one 33 ohm 2512.
- VM decoupling: 2 x 100 nF 100 V instead of one 220 nF.
- Added FB1 ferrite bead on VDDA/VREF+.
- Half-bridge: net ties for Kelvin sensing, op-amp network all 0603, shunt and 4.7 uF part numbers chosen.
- Added NT1 so PGND and GND join at exactly one point.

## Tradeoffs and checks

- The STSPIN32G4 is limited to 75 V, or 78 V absolute max, so the firmware regen limit matters.
- Test the HYG053N10NS1C2 on one board under load before ordering all 10.
- Check the MAD 5008's real peak phase current. If it stays under ~33 A, a higher op-amp gain gives finer current resolution.
- Confirm C17's footprint pad 1 is + and the XT30 footprint's square pad is −.
- Check that the GND symbol under Rx07 attaches to the resistor and doesn't touch the OPN wire.
- CAN connector C189895 showed 0 stock on 2026-11-11. Re-check before ordering.
- Confirm JLCPCB stock and Basic/Extended status for every part before ordering. Classifications change.
- Check that the motor shaft is exposed behind the motor for the encoder magnet.
