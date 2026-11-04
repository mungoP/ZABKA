# Motor Driver Board: Wiring, Cost, and Parts

**Status:** Design estimate. This has not been built or tested.

**Target:** One 12S (50.4 V full) motor board using the STSPIN32G4, AS5047P encoder, and CAN.

**Assumptions:** 10 boards, JLCPCB Economic PCBA, single-sided SMT assembly, about 60 x 60 mm, 4 layers, and 2 oz outer copper. Prices are estimates in USD.

---

## 1. Simple wiring

### Power

```text
12S+ --> XT30 in/out --> VM bus --> high-side MOSFET drains
                               \--> STSPIN32G4 VM --> SW --> 18 uH --> VCC (8 V at boot, then 12 V)
                                                                        |
                                                                        +--> Gate drivers
                                                                        |
                                                                        +--> 33 ohm --> REGIN --> LDO --> 3.3 V
                                                                                                         |
                                                                                                         +--> MCU
                                                                                                         +--> AS5047P
                                                                                                         +--> CAN transceiver

12S- --> PGND --> shunts --> low-side MOSFET sources
```

### One motor phase (repeat 3 times)

```text
VM bus --> high-side MOSFET --> switch node --> low-side MOSFET --> 1 mohm shunt --> PGND
                                    |
                                    +--> Motor phase wire
                                    +--> STSPIN32G4 OUTx
                                    +--> Bootstrap capacitor --> BOOTx

GHSx --> 22 ohm --> high-side gate
GLSx --> 22 ohm --> low-side gate
47 kohm pulldown from each gate to its source
```

### Signal connections

| Function                | STSPIN32G4 pin              | Connects to             | Notes                      |
| ----------------------- | --------------------------- | ----------------------- | -------------------------- |
| Phase U current         | PA1 (+), PA3 (-), PA2 (out) | Phase U shunt network   | Use Kelvin routing         |
| Phase V current         | PA7 (+), PC5 (-), PA6 (out) | Phase V shunt network   | Use Kelvin routing         |
| Phase W current         | PB0 (+), PB2 (-), PB1 (out) | Phase W shunt network   | Use Kelvin routing         |
| Encoder clock           | PB3                         | AS5047P pin 2, CLK      | SPI3                       |
| Encoder data in         | PB4                         | AS5047P pin 3, MISO     | SPI3                       |
| Encoder data out        | PB5                         | AS5047P pin 4, MOSI     | SPI3                       |
| Encoder select          | PA15                        | AS5047P pin 1, CSn      | Chip select                |
| CAN transmit            | PA12                        | SN65HVD230 D            | FDCAN1 TX                  |
| CAN receive             | PA11                        | SN65HVD230 R            | FDCAN1 RX                  |
| Bus voltage             | PA0                         | 72.3 k / 3.01 k divider | About 82 V full scale      |
| Temperature             | PC4                         | NTC divider             | Place NTC near the MOSFETs |
| Status LED              | PD2                         | LED and 1 k resistor    | Optional firmware status   |
| Debug                   | PA13, PA14, PG10            | SWD header              | SWDIO, SWCLK, NRST         |
| Boot pin                | PB8                         | 10 k to ground          | Boots from flash           |
| Optional output encoder | PA8, PA9                    | I2C header              | Leave unpopulated at first |

### Current-sense network (repeat 3 times)

```text
Shunt high side --> 1.5 k --> OPP
Shunt low side  --> 1.5 k --> OPN
OPO --> 49.9 k --> OPN
OPP --> 100 k --> 3.3 V
OPP --> 100 k --> Signal ground
```

Gain is about 33, giving a range of about +/-50 A with a 1 mohm shunt.

### Layout notes

- Join PGND and signal ground at one point near the shunts.
- Keep the half-bridge and gate-drive loops short.
- Keep the AS5047P away from phase current paths.
- Put all SMT parts on the motor-facing side to stay on Economic PCBA.
- Mount the XT30s and bulk capacitor through the board from the back.

### Firmware startup

1. Write VCC = 12 V over the internal I2C bus before enabling PWM.
2. Start with a current-limited bench supply at about 20 V and 1-2 A.
3. Use about 500 ns dead time, then tune it on a scope.
4. Set a firmware regen voltage limit around 56 V.

---

## 2. Estimated cost per board

| Cost category                         | Per board  | Basis                                     |
| ------------------------------------- | ---------- | ----------------------------------------- |
| STSPIN32G4                            | $5.36      | LCSC 10+ price snapshot                   |
| 6 x HYG053N10NS1C2 MOSFETs            | $3.30      | Upper JLCPCB price estimate               |
| AS5047P encoder                       | $3.20      | LCSC 10+ price snapshot                   |
| SN65HVD230DR CAN transceiver          | $0.42      | LCSC 10+ price snapshot                   |
| Shunts, passives, and connectors      | $6-10      | Rough estimate                            |
| **Components subtotal**               | **$18-22** |                                           |
| 4-layer PCB, 2 oz outer copper        | $3-6       | Rough estimate, confirm with a quote      |
| JLCPCB Economic assembly              | $8-10      | Includes an allowance for inspection fees |
| **Total before shipping and tariffs** | **$30-38** | 10-board order                            |
| Shipping                              | $2-4       | Rough estimate, varies by method          |

### Assembly estimate for 10 boards

| Fee                    | Total      | Assumption                                |
| ---------------------- | ---------- | ----------------------------------------- |
| Setup and stencil      | $9.71      | $8.18 setup + $1.53 stencil               |
| Extended part loading  | $61.40     | 20 extended parts x $3.07                 |
| SMT joints             | $4.40      | 275 joints per board x $0.0016            |
| Through-hole soldering | $4.56      | About 6 joints per board plus $3.58 labor |
| Placement confirmation | $0.45      | One order                                 |
| **Assembly subtotal**  | **$80.52** | **About $8.05 per board**                 |

A 5-board pilot would cost about $40-45 per board because the same loading fees are split across fewer boards.

---

## 3. Detailed part list

### Main parts

| Ref   | Part           | Qty | Value or notes                             | Est. cost per board |
| ----- | -------------- | --- | ------------------------------------------ | ------------------- |
| U1    | STSPIN32G4     | 1   | Gate driver and STM32G431 MCU              | $5.36               |
| Q1-Q6 | HYG053N10NS1C2 | 6   | 100 V, 5.3 mOhm, 73 nC, PDFN5x6            | $3.30               |
| U2    | AS5047P-ATSM   | 1   | 14-bit magnetic encoder, 3.3 V mode        | $3.20               |
| U3    | SN65HVD230DR   | 1   | 3.3 V CAN, 1 Mbps classic CAN              | $0.42               |
| M1    | Encoder magnet | 1   | 8 x 3 mm diametric, about 2 mm from sensor | Not included        |

### Power stage

| Ref     | Part                    | Qty | Value or notes                                            |
| ------- | ----------------------- | --- | --------------------------------------------------------- |
| R1-R3   | Current shunt           | 3   | 1 mOhm, 3 W, 2512, four-terminal Kelvin routing           |
| R4-R9   | Gate resistor           | 6   | 22 ohm, 0603, tune on scope                               |
| R10-R15 | Gate pulldown           | 6   | 47 kOhm, 0402                                             |
| C1-C3   | Bootstrap capacitor     | 3   | 1 uF, 50 V, X7R                                           |
| C4-C9   | Bus ceramic capacitor   | 6   | 4.7 uF, 100 V, 1210, two per half-bridge                  |
| C10     | Bulk capacitor          | 1   | 220 uF, 63 V, low ESR, through-hole from back if possible |
| C11     | VM decoupling capacitor | 1   | 220 nF, 100 V                                             |
| D1      | TVS diode               | 1   | SMBJ54A, doesn't handle sustained regen                   |

### Power supply

| Ref | Part                       | Qty | Value or notes             |
| --- | -------------------------- | --- | -------------------------- |
| L1  | Buck inductor              | 1   | 18 uH, 1 A                 |
| D2  | Buck diode                 | 1   | STPS1H100A, 100 V Schottky |
| C12 | Buck output capacitor      | 1   | 47 uF, 25 V                |
| R16 | REGIN resistor             | 1   | 33 ohm, 1 W, 2512          |
| C13 | REGIN capacitor            | 1   | 4.7 uF, 25 V               |
| C14 | 3.3 V bulk capacitor       | 1   | 10 uF                      |
| C15 | 3.3 V decoupling capacitor | 1   | 100 nF                     |
| C16 | VDDA capacitor             | 1   | 1 uF                       |
| C17 | VDDA capacitor             | 1   | 100 nF                     |
| C18 | VREF+ capacitor            | 1   | 1 uF                       |
| C19 | VREF+ capacitor            | 1   | 100 nF                     |
| C20 | VBAT capacitor             | 1   | 100 nF                     |

Estimated 3.3 V load is about 92 mA, against a 150 mA regulator limit. Measure this on the first board.

### Current sensing and monitoring

| Ref     | Part                    | Qty | Value or notes                           |
| ------- | ----------------------- | --- | ---------------------------------------- |
| R17-R22 | Sense input resistor    | 6   | 1.5 kOhm, 1%                             |
| R23-R25 | Sense feedback resistor | 3   | 49.9 kOhm, 1%                            |
| R26-R31 | Sense bias resistor     | 6   | 100 kOhm, 1%, two per op-amp             |
| R32     | SCREF divider           | 1   | 22 kOhm, top resistor                    |
| R33     | SCREF divider           | 1   | 10 kOhm, bottom resistor, sets about 1 V |
| R34     | Bus sense divider       | 1   | 72.3 kOhm, top resistor                  |
| R35     | Bus sense divider       | 1   | 3.01 kOhm, bottom resistor               |
| C21     | Bus sense filter        | 1   | 33 nF                                    |
| TH1     | NTC thermistor          | 1   | 10 kOhm, place near MOSFETs              |
| R36     | NTC pull-up             | 1   | 10 kOhm                                  |
| C22     | NTC filter              | 1   | 33 nF                                    |

### MCU support

| Ref     | Part                    | Qty | Value or notes                          |
| ------- | ----------------------- | --- | --------------------------------------- |
| Y1      | Crystal                 | 1   | 24 MHz on PF0/PF1                       |
| C23-C24 | Crystal load capacitors | 2   | 6.8 pF, check against crystal datasheet |
| R37     | Reset pull-up           | 1   | 100 kOhm                                |
| C25     | Reset capacitor         | 1   | 100 nF                                  |
| R38     | Boot pulldown           | 1   | 10 kOhm on PB8                          |
| LED1    | Status LED              | 1   | Connected to PD2                        |
| R39     | LED resistor            | 1   | 1 kOhm                                  |

### Encoder and CAN support

| Ref | Part               | Qty | Value or notes                   |
| --- | ------------------ | --- | -------------------------------- |
| C26 | Encoder capacitor  | 1   | 1 uF, close to U2                |
| C27 | Encoder capacitor  | 1   | 100 nF, close to U2              |
| C28 | CAN capacitor      | 1   | 100 nF, close to U3              |
| R40 | CAN termination    | 1   | 120 ohm, use only at bus ends    |
| JP1 | Termination jumper | 1   | Solder jumper in series with R40 |

Tie AS5047P VDD and VDD3V3 together in 3.3 V mode, connect TEST to ground, and leave unused A, B, I, U, V, and W outputs open.

For the SN65HVD230, confirm the pinout in its datasheet. From memory, Rs goes to ground for full-speed operation.

### Connectors

| Ref     | Part                  | Qty | Value or notes                       |
| ------- | --------------------- | --- | ------------------------------------ |
| J1-J2   | XT30                  | 2   | Power input and pass-through         |
| J3-J4   | 4-pin JST GH          | 2   | CANH, CANL, GND, spare               |
| J5      | SWD header            | 1   | 3V3, GND, SWDIO, SWCLK, NRST         |
| J6      | Output encoder header | 1   | Optional, leave unpopulated at first |
| R41-R42 | I2C pull-ups          | 2   | 4.7 kOhm, optional with J6           |
| P1-P3   | Motor phase pads      | 3   | Direct solder connections            |

### Board specification

| Item                   | Specification                                            |
| ---------------------- | -------------------------------------------------------- |
| Size                   | About 60 x 60 mm                                         |
| Layers                 | 4                                                        |
| Stackup                | Signal / solid ground / power and phase routing / signal |
| Outer copper           | 2 oz                                                     |
| Inner copper           | 0.5-1 oz                                                 |
| Assembly               | Single-sided JLCPCB Economic PCBA                        |
| Unique populated parts | About 36                                                 |
| Parts placed per board | About 88                                                 |

## Tradeoffs and checks

- The STSPIN32G4 is limited to 75 V, or 78 V absolute max, so the firmware regen limit matters.
- Test the HYG053N10NS1C2 on one board under load before ordering all 10.
- The SN65HVD230 is limited to 1 Mbps classic CAN and is rated to 85 C.
- Check that the motor shaft is exposed behind the motor.
- Add a homing routine or output encoder, because the motor encoder alone can't identify the joint angle at startup through a 20:1 gearbox.
- Confirm JLCPCB stock, and whether each part is basic or extended, before ordering.
