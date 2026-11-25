# Main board BOM

Last updated: 2026-11-06

Parts list for the main board described in `main-board.md`. Same format as the motor board list.

- **Add from CDFER** = symbol from the CDFER JLCPCB KiCad library (2025.07.18 package). Search the symbol name as written.
- **Add from Import** = not in the CDFER library. Import with `importlcsc <LCSC#>` (easyeda2kicad).
- **Add from KiCad** = stock KiCad symbol, hand-soldered or through-hole, excluded from the JLC BOM.
- **Status** = Basic (no loading fee) or Extended (about $3 loading fee per unique part per order). Basic/Extended status was checked against the JLC basic list scraped for the CDFER parts database; LCSC numbers for imported parts were checked on LCSC/JLC pages on 2026-11-06.

## MCU (STM32G474CBT6)

| Ref          | Qty | Value                  | Add from               | Symbol name or import                     | LCSC       | Status      | Notes                                                                                                         |
| ------------ | --: | ---------------------- | ---------------------- | ----------------------------------------- | ---------- | ----------- | ------------------------------------------------------------------------------------------------------------- |
| **U1**       |   1 | STM32G474CBT6, LQFP-48 | Import                 | STM32G474CBT6                             | `C730123`  | Extended    | Backup: STM32G474CCT6, same footprint. I couldn't find its LCSC number; search JLC before ordering.           |
| **Y1**       |   1 | 25 MHz, 3225           | CDFER                  | `Crystal, 25MHz, 11pF`                    | `C9006`    | Basic       | Same crystal as the motor board. Firmware HSE = 25 MHz. Needed for CAN timing.                                |
| **C1, C2**   |   2 | 15 pF, 0402            | CDFER                  | `0402,15pF`                               | `C1548`    | Basic       | Crystal load caps. Same datasheet check as motor board (11 vs 12 pF CL).                                      |
| **C3–C8**    |   6 | 100 nF, 16 V, 0402     | CDFER                  | `0402,100nF`                              | `C1525`    | Basic       | One per VDD pin (3), VBAT, VDDA, VREF+                                                                        |
| **C9**       |   1 | 4.7 µF, 16 V, 0603     | CDFER                  | `0603,4.7uF`                              | `C19666`   | Basic       | VDD bulk                                                                                                      |
| **C10, C11** |   2 | 1 µF, 25 V, 0402       | CDFER                  | `0402,1uF`                                | `C52923`   | Basic       | VDDA and VREF+                                                                                                |
| **C12**      |   1 | 100 nF, 16 V, 0402     | CDFER                  | `0402,100nF`                              | `C1525`    | Basic       | NRST to GND                                                                                                   |
| **R1**       |   1 | 10 kΩ, 0402            | CDFER                  | `0402,10kΩ`                               | `C25744`   | Basic       | BOOT0 (PB8) pulldown                                                                                          |
| **SW1, SW2** |   2 | Tactile button, 4 mm   | CDFER                  | `Tactile Button, 160gf, 12V, 50mA, 4.0mm` | `C720477`  | Basic       | SW1 BOOT0 to 3.3 V, SW2 NRST to GND                                                                           |
| **LED1**     |   1 | Red LED, 0603          | CDFER                  | `LED,0603,Red`                            | `C2286`    | Basic       | Firmware status                                                                                               |
| **LED2**     |   1 | Green LED, 0805        | CDFER                  | `LED,0805,Green`                          | `C2297`    | Basic       | 3.3 V power indicator                                                                                         |
| **R2, R3**   |   2 | 1 kΩ, 0402             | CDFER                  | `0402,1kΩ`                                | `C11702`   | Basic       | LED resistors                                                                                                 |
| **J1**       |   1 | 5-pin 2.54 mm header   | KiCad                  | `Connector:Conn_01x05_Pin`                | None       | Hand-solder | SWD, same pinout as the motor board J5 so one cable works for both                                            |
| **J2**       |   1 | USB-C, 16-pin          | CDFER (Extended sheet) | `Connector, USB-TYPE-C-16P`               | `C2927039` | Extended    | **Optional.** Only needed for USB DFU flashing. Leave unpopulated to save a loading fee; SWD covers flashing. |
| **R4, R5**   |   2 | 5.1 kΩ, 0402           | CDFER                  | `0402,5.1kΩ`                              | `C25905`   | Basic       | USB-C CC pulldowns. Only if J2 fitted.                                                                        |

## IMU (BMI088)

| Ref          | Qty | Value                 | Add from | Symbol name or import | LCSC      | Status   | Notes                                                         |
| ------------ | --: | --------------------- | -------- | --------------------- | --------- | -------- | ------------------------------------------------------------- |
| **U2**       |   1 | BMI088, LGA-16        | Import   | BMI088                | `C194919` | Extended | **Standard PCBA only** (not Economic). PS pin to GND for SPI. |
| **C13, C14** |   2 | 100 nF, 16 V, 0402    | CDFER    | `0402,100nF`          | `C1525`   | Basic    | VDD and VDDIO                                                 |
| **C15**      |   1 | 10 µF, 10 V, 0603     | CDFER    | `0603,10uF`           | `C19702`  | Basic    | IMU supply after ferrite                                      |
| **FB1**      |   1 | 600 Ω @ 100 MHz, 0603 | CDFER    | `Ferrite,0603`        | `C1002`   | Basic    | Isolates IMU supply from 3.3 V rail                           |

## CAN (2 buses)

| Ref          | Qty | Value                | Add from | Symbol name or import               | LCSC     | Status      | Notes                                                                    |
| ------------ | --: | -------------------- | -------- | ----------------------------------- | -------- | ----------- | ------------------------------------------------------------------------ |
| **U3, U4**   |   2 | SN65HVD230DR, SOIC-8 | CDFER    | `Transceiver, CAN, SN65HVD230DR`    | `C12084` | Extended    | Same part as the motor boards. Classic CAN, 1 Mbps max. See note below.  |
| **C16, C17** |   2 | 100 nF, 16 V, 0402   | CDFER    | `0402,100nF`                        | `C1525`  | Basic       | One per transceiver                                                      |
| **R6, R7**   |   2 | 120 Ω, 0402          | CDFER    | `0402,120Ω`                         | `C25079` | Basic       | Bus termination, in series with a solder jumper so it can be disabled    |
| **J3, J4**   |   2 | CAN connector        | KiCad    | Match the motor board CAN connector | None     | Hand-solder | Use the same connector as the motor boards so cables are interchangeable |

**CAN-FD note:** `main-board.md` says CAN-FD transceivers, but the motor boards use the SN65HVD230, which is classic CAN only (1 Mbps). The bus can only run as fast as its slowest transceiver, so I matched the motor board here. If you later want CAN-FD data rates, both boards need an FD transceiver such as the TCAN332G.

## Power input and protection

| Ref          | Qty | Value                              | Add from | Symbol name or import | LCSC       | Status      | Notes                                                                                         |
| ------------ | --: | ---------------------------------- | -------- | --------------------- | ---------- | ----------- | --------------------------------------------------------------------------------------------- |
| **J5**       |   1 | XT30                               | KiCad    | XT30 footprint        | None       | Hand-solder | Same connector as motor boards. Tap before the motor e-stop.                                  |
| **F1**       |   1 | 2 A, 63 V, 1206 fuse               | Import   | 1206T2A63V            | `C354895`  | Extended    | Not in the basic library. Board draws under 1 A from the pack.                                |
| **D1**       |   1 | SS210, 100 V 2 A, SMA              | CDFER    | `Schottky,SS210`      | `C14996`   | Basic       | Reverse polarity, in series after F1                                                          |
| **D2**       |   1 | SMBJ54A TVS, unidirectional        | Import   | SMBJ54A (PAKER)       | `C3012530` | Extended    | 54 V standoff, ~87 V clamp, under the buck's 100 V limit. No basic TVS in this voltage range. |
| **C18, C19** |   2 | 2.2 µF, 100 V, 1210, X7R           | Import   | CL32B225KCJSNNE       | `C55151`   | Extended    | Buck input ceramics. No basic MLCC above 1 µF is rated 100 V.                                 |
| **C20, C21** |   2 | 100 nF, 100 V, 0805                | CDFER    | `0805,100nF`          | `C28233`   | Basic       | High-frequency input decoupling, one at the FETs and one at the VIN pin                       |
| **C22**      |   1 | 47 µF, 100 V electrolytic, 12.5 mm | Import   | UCD2A470MNQ1MS        | `C340746`  | Extended    | **Optional.** Damps input ringing on hot-plug. Fit if you see overshoot on the scope.         |

## 5 V buck (LM5146, 5.1 V / 5 A)

| Ref                   | Qty | Value                              | Add from | Symbol name or import | LCSC       | Status   | Notes                                                                                                                                                                                         |
| --------------------- | --: | ---------------------------------- | -------- | --------------------- | ---------- | -------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **U5**                |   1 | LM5146QRGYRQ1, VQFN-20             | Import   | LM5146QRGYRQ1         | `C1850329` | Extended | ~2,460 in stock but ~$16 at qty 1. The non-Q1 LM5146RGYR (`C3188679`) and the 75 V LM5145RGYR (`C485912`) are cheaper and pin-compatible but out of stock today. Check again before ordering. |
| **Q1, Q2**            |   2 | CSD19537Q3T, 100 V, 12.1 mΩ, 21 nC | Import   | CSD19537Q3T           | `C134143`  | Extended | High and low side. Low gate charge keeps the LM5146's internal VCC regulator cool at 50 V input; the motor board MOSFET (73 nC) would be too much.                                            |
| **L1**                |   1 | 10 µH, 7.8 A, 11.5 × 10 mm         | Import   | FXL1040-100-M         | `C475915`  | Extended | Check saturation current in the datasheet (listing shows 8.5 A). Peak current at 5 A out is about 5.8 A.                                                                                      |
| **C23–C26**           |   4 | 22 µF, 25 V, 1206                  | CDFER    | `1206,22uF,(2)`       | `C12891`   | Basic    | Output caps. Same part as motor board.                                                                                                                                                        |
| **C27**               |   1 | 1 µF, 50 V, 0603                   | CDFER    | `0603,1uF`            | `C15849`   | Basic    | VCC pin                                                                                                                                                                                       |
| **C28**               |   1 | 100 nF, 50 V, 0402                 | CDFER    | `0402,100nF,(2)`      | `C307331`  | Basic    | Bootstrap, BST to SW                                                                                                                                                                          |
| **C29**               |   1 | 47 nF, 0603                        | CDFER    | `0603,47nF`           | `C1622`    | Basic    | Soft-start, about 4 ms                                                                                                                                                                        |
| **R8**                |   1 | 33 kΩ, 0603                        | CDFER    | `0603,33kΩ`           | `C4216`    | Basic    | Feedback top                                                                                                                                                                                  |
| **R9**                |   1 | 6.2 kΩ, 0603                       | CDFER    | `0603,6.2kΩ`          | `C4260`    | Basic    | Feedback bottom. 0.8 V × (1 + 33/6.2) = **5.06 V**                                                                                                                                            |
| **R10**               |   1 | RT, TBD                            | CDFER    | Nearest basic value   | TBD        | Basic    | Sets 400 kHz. Value from the LM5146 quickstart calculator.                                                                                                                                    |
| **R11**               |   1 | ILIM, TBD                          | CDFER    | Nearest basic value   | TBD        | Basic    | Current limit via low-side RDS(on). Value from the calculator using Q2's RDS(on).                                                                                                             |
| **R12, R13, C30–C32** |   5 | Type III compensation, TBD         | CDFER    | Nearest basic values  | TBD        | Basic    | Values from the calculator. All needed values exist in the basic 0402/0603 range.                                                                                                             |
| **D3**                |   1 | SMBJ6.5CA TVS, SMB                 | CDFER    | `TVS-Bi,SMBJ6.5CA`    | `C7420377` | Basic    | 5 V rail to Pi. Protects the Pi since back-feeding pins 2/4 bypasses its input protection.                                                                                                    |

EN ties directly to VIN. Add a UVLO divider later if you want the buck to shut off below ~30 V.

## 3.3 V LDO and battery sense

| Ref          | Qty | Value                | Add from | Symbol name or import | LCSC     | Status | Notes                                                                                                                                |
| ------------ | --: | -------------------- | -------- | --------------------- | -------- | ------ | ------------------------------------------------------------------------------------------------------------------------------------ |
| **U6**       |   1 | AMS1117-3.3, SOT-223 | CDFER    | `LDO, 3.3V, 1A`       | `C6186`  | Basic  | 5 V to 3.3 V                                                                                                                         |
| **C33, C34** |   2 | 10 µF, 25 V, 0805    | CDFER    | `0805,10uF`           | `C15850` | Basic  | LDO in and out. AMS1117 can oscillate with very low-ESR ceramics; if it does, swap the output to the 10 µF tantalum `C7171` (basic). |
| **R14**      |   1 | 200 kΩ, 0603         | CDFER    | `0603,200kΩ`          | `C25811` | Basic  | Battery divider top. 0603, not 0402, because ~48 V sits across it.                                                                   |
| **R15**      |   1 | 12 kΩ, 0603          | CDFER    | `0603,12kΩ`           | `C22790` | Basic  | Battery divider bottom. 50.4 V gives 2.85 V at the ADC; 58 V still under 3.3 V.                                                      |
| **C35**      |   1 | 100 nF, 16 V, 0402   | CDFER    | `0402,100nF`          | `C1525`  | Basic  | ADC filter                                                                                                                           |

## Pi interface

| Ref    | Qty | Value                         | Add from | Symbol name or import                   | LCSC | Status      | Notes                                                                                                             |
| ------ | --: | ----------------------------- | -------- | --------------------------------------- | ---- | ----------- | ----------------------------------------------------------------------------------------------------------------- |
| **J6** |   1 | 2 × 20 female header, 2.54 mm | KiCad    | `Connector_Generic:Conn_02x20_Odd_Even` | None | Hand-solder | Pi 4 header. Carries SPI (Pi master), 5 V on pins 2/4, GND, and one data-ready GPIO. Or mount the board as a HAT. |

## Summary

| Measure                                              | Count                                                                                                             |
| ---------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------- |
| Unique parts on the JLC BOM (required, values fixed) | 36, plus up to ~5 basic values still TBD for RT, ILIM and compensation                                            |
| Basic                                                | 27                                                                                                                |
| Extended, required                                   | 9: STM32G474CBT6, BMI088, SN65HVD230DR, LM5146-Q1, CSD19537Q3T, FXL1040 inductor, 2.2 µF 100 V cap, SMBJ54A, fuse |
| Extended, optional                                   | 2: USB-C, 47 µF electrolytic                                                                                      |
| Hand-soldered / through-hole                         | SWD header, 2 CAN connectors, XT30, Pi header                                                                     |
| Loading fees                                         | About $28 per order for the 9 required extended parts                                                             |

## Check before layout

- **BMI088 needs Standard PCBA.** That makes the whole board Standard, not Economic. Budget for it, or place the IMU on a small daughter board.
- **LM5146 values:** run TI's LM5146 quickstart calculator for RT, ILIM and compensation with Vin 36–58 V, Vout 5.1 V, Iout 5 A, 400 kHz, L = 10 µH, Cout = 4 × 22 µF (derated for DC bias), Q = CSD19537Q3T. Then pick the nearest basic values.
- **22 µF output caps lose capacitance at 5 V bias** (X5R, 25 V). Put the derated value into the calculator, not 88 µF.
- **FDCAN pin check** in CubeMX: confirm FDCAN1 and FDCAN2 route alongside SPI1 (IMU), SPI2 (Pi), SWD and USB on the LQFP-48.
- **D1 polarity:** the SS210 symbol pins are unnamed, so make sure the bar faces the load.
