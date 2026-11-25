# Control, CAN and power BOM (off-the-shelf)

Last updated: 2026-11-24. Prices are Amazon.com listings on that date, before tax.

This is the cheapest set of parts I found that lets the Raspberry Pi 4 command all 10 motor boards over CAN fast enough for an RL walking policy. It uses an ESP32 as the CAN bridge, so there is no custom main board to build yet. Everything comes from Amazon except where noted.

## Summary

| Block            | Choice                                                     | Why                                                                                    |
| ---------------- | ---------------------------------------------------------- | -------------------------------------------------------------------------------------- |
| CAN bridge MCU   | **ESP32 (already owned)**                                  | Built-in hardware CAN controller (TWAI, classic CAN up to 1 Mbps). $0.                 |
| CAN transceiver  | Waveshare SN65HVD230 board                                 | 3.3 V, ESD protection, 177 reviews. Remove its 120 Ω resistor (see wiring).            |
| IMU              | LSM6DSV 6-DoF breakout                                     | Low noise, ±16 g, on-chip sensor fusion outputs a gravity vector. I²C and SPI.         |
| Pi link          | USB cable to the ESP32                                     | Power, 921.6 kbaud serial and reflashing the ESP32 from the Pi, all on one cable.      |
| BMS              | JBD-type smart BMS 7–14S, Li-ion, 20 A, Bluetooth + UART   | Auto-detects 12S. The ESP32 can read every cell voltage.                               |
| Charger          | 50.4 V 1.5 A, 12S                                          | Charges in the robot through the BMS.                                                  |
| Fuse             | 58 V DC ATO blade fuses (30 A main, 3 A logic)             | Normal ATO/ATC fuses are rated 32 V DC and can't safely break a 50.4 V arc.            |
| Pi power         | DROK 11–90 V to 5 V 6 A buck                               | 90 V input gives headroom over 50.4 V plus regen spikes.                               |
| Safety           | XT90-S anti-spark connector + 22 mm mushroom e-stop        | XT90-S is the hard power disconnect. The e-stop is a fail-safe stop handled by the ESP32. |
| Raspberry Pi Pico | Spare                                                     | Upgrade path for a second CAN bus (see below).                                          |

**Total: $199.64**. Not included:  the Pi 4 and the ESP32.

## Is it fast enough?

Yes, at 4× the usual RL policy rate.

The motor boards run FOC and a local PD loop (MIT-style: position, velocity, Kp, Kd and feed-forward torque in one 8-byte frame). The Pi therefore only needs to send targets at the policy rate (typically 50 Hz) and read joint state back. It does not have to close the current loop.

Classic CAN at 1 Mbps, 11-bit ID, 8-byte frames: 111 µs per frame best case and 135 µs worst case (worst-case bit stuffing). One cycle is 10 commands plus 10 replies, or 20 frames.

| CAN cycle rate | Worst-case bus load (one bus, all 10 motors) |
| -------------- | -------------------------------------------- |
| 50 Hz          | 14 %                                         |
| 100 Hz         | 27 %                                         |
| **200 Hz**     | **54 % (recommended)**                       |
| 250 Hz         | 68 %                                         |
| 300 Hz         | 81 %                                         |
| 370 Hz         | 100 % (absolute ceiling)                     |

The Pi-to-ESP32 serial link at 200 Hz carries about 86 bytes down (10 commands plus header) and 118 bytes up (10 motor states, gravity and gyro, battery status, header). That uses 19 % and 26 % of a 921,600 baud link.

**Why not connect CAN straight to the Pi** with an MCP2515 HAT? The MCP2515 has only 2 receive buffers. The 10 replies arrive back-to-back about 120 µs apart, and Linux interrupt latency will overrun it. The ESP32 bridge costs $0, gives deterministic timing, and can run a watchdog that stops the motors if the Pi crashes.

## Purchase list

| #  | Item                                                                     | Qty | Price  | Line total | Link                                              | Notes                                                                                                   |
| -- | ------------------------------------------------------------------------ | --: | -----: | ---------: | ------------------------------------------------- | ------------------------------------------------------------------------------------------------------- |
| 1  | ESP32 dev board                                                          |   1 |     $0 |         $0 | Already owned                                     | Any ESP32 except the ESP32-C2 (and not an ESP8266). All others have TWAI.                               |
| 2  | Waveshare SN65HVD230 CAN board                                           |   1 | $10.99 |     $10.99 | https://www.amazon.com/dp/B00KM6XMXO              | 4.6★ (177). Desolder the 120 Ω SMD resistor across CANH/CANL.                                           |
| 3  | LSM6DSV 6-DoF IMU breakout (I²C/SPI)                                     |   1 | $12.99 |     $12.99 | https://www.amazon.com/dp/B0H8WQQDLN              | New listing with no reviews, but sold through Amazon so returns are easy.                               |
| 4  | GH1.25 connectors + pre-crimped silicone wires kit                       |   1 | $18.99 |     $18.99 | https://www.amazon.com/dp/B07PBHN7TM              | Genuine JST-GH form factor, 4.4★ (289). Make every CAN cable pin 1 to pin 1. Many "JST 1.25" listings are Molex PicoBlade and won't fit. |
| 5  | Smart BMS 7–14S auto-detect, 20 A, Bluetooth + UART (JBD hardware)        |   1 | $21.00 |     $21.00 | https://www.amazon.com/dp/B0GLWR62NL              | Ships from China, arriving about Dec 17. See the AliExpress alternative below.                          |
| 6  | 50.4 V 1.5 A 12S Li-ion charger                                          |   1 | $19.99 |     $19.99 | https://www.amazon.com/dp/B076KJD4J5              | 4.4★ (72). DC 5.5×2.1 mm plug. About 3 h for a full charge.                                             |
| 7  | DC 5.5×2.1 mm panel jack, pre-wired, 10 A (pack)                          |   1 |  $7.99 |      $7.99 | https://www.amazon.com/dp/B0BW8N7XZ7              | 4.7★ (322). This is the robot's charge port.                                                           |
| 8  | 6S JST-XH balance extension leads (5 pcs)                                |   1 |  $8.99 |      $8.99 | https://www.amazon.com/dp/B08L5WCH82              | 4.6★ (158). Cut two to build the 13-wire BMS harness.                                                   |
| 9  | ATO 58 V DC blade fuse kit, 80 pcs, 2–40 A                               |   1 | $19.99 |     $19.99 | https://www.amazon.com/dp/B0HBNFKQSV              | 58 V DC, 1,000 A interrupt rating, ISO 8820-3. Fits standard ATO holders.                               |
| 10 | Inline ATO fuse holder, 12 AWG (2 pack)                                  |   1 |  $5.39 |      $5.39 | https://www.amazon.com/dp/B0D9JLN9M1              | 4.7★ (102). One for the 30 A main, one for the 3 A logic branch. Throw away the 32 V fuses it ships with. |
| 11 | Amass XT90-S anti-spark connectors (5 pairs)                             |   1 | $12.98 |     $12.98 | https://www.amazon.com/dp/B074PTHZ3M              | 4.8★ (1K). Main disconnect. Handles the inrush into 10 × 470 µF bulk caps.                              |
| 12 | 22 mm mushroom e-stop, latching, 2 NC (2 pack)                           |   1 | $11.99 |     $11.99 | https://www.amazon.com/dp/B07R8PTTDX              | 4.4★ (253). Put it on a 2–3 m tether for testing.                                                       |
| 13 | DROK waterproof buck, 11–90 V in, 5 V 6 A out                            |   1 | $17.99 |     $17.99 | https://www.amazon.com/dp/B0C4L66SZ9              | 4.8★ (30). Powers the Pi 4. The Pi powers the ESP32 over USB.                                           |
| 14 | USB-C male pigtail, 20 AWG, 5 A (2 pack)                                 |   1 |  $5.99 |      $5.99 | https://www.amazon.com/dp/B0HHK68YRS              | Feeds the buck output into the Pi's USB-C port, keeping its input protection.                           |
| 15 | XT30U pigtails, 16 AWG, 100 mm (5 pairs)                                 |   2 |  $7.19 |     $14.38 | https://www.amazon.com/dp/B0FDGN2M8F              | 10 female ends, one per motor board XT30.                                                               |
| 16 | 14 AWG silicone wire, 10 ft red + 10 ft black                            |   1 |  $9.99 |      $9.99 | https://www.amazon.com/dp/B0C5QY73WC              | Battery leads and the power trunk down each leg.                                                        |
|    | **Total**                                                                |     |        | **$199.64** |                                                   |                                                                                                         |

### Cheaper or faster alternatives (not Amazon)

| Item                     | Where                                                         | Price             | Why you might pick it                                                                        |
| ------------------------ | ------------------------------------------------------------- | ----------------: | -------------------------------------------------------------------------------------------- |
| JBD 7–14S 20 A smart BMS | https://www.aliexpress.us/item/3256811426295916.html          | $16.99 + $3.28 shipping | Same JBD board as #5, 2,000+ sold. Delivery Dec 2–9 is **faster than Amazon** for this one item. |
| Littelfuse 0997030.WXN, MINI 58 V 30 A | LCSC C207030 or DigiKey F1878-ND                | about $0.44–1.06 each | If you'd rather not buy an 80-piece kit. Fits standard MINI holders.                      |
| ICM-42688-P module (I²C only) | AliExpress, "ZY-ICM-42688" by LIZAO                       | $5.26             | Cheapest good IMU. I²C only and slow shipping.                                               |

## Wiring

### CAN bus (one bus, bridge in the middle)

```text
 L ankle [TERM] ─ L knee ─ L hip ─ L hip ─ L hip ─┐                ┌─ R hip ─ R hip ─ R hip ─ R knee ─ R ankle [TERM]
                                                  │                │
                                          GH ─────┴── ESP32 bridge ─┴───── GH
                                                (SN65HVD230, 120 Ω REMOVED)
```

- Physically this is one linear bus. The bridge sits in the pelvis with two GH connectors wired in parallel (CANH to CANH, CANL to CANL, GND to GND), the same pass-through scheme as the motor boards.
- **Termination:** bridge JP1 and JP2 only on the two ankle boards. Leave every other board open, and remove the 120 Ω resistor on the Waveshare board. Check with power off: about 60 Ω between CANH and CANL.
- Twist CANH and CANL together. Total length is about 2–3 m, far under the roughly 40 m limit at 1 Mbps.
- GH pinout matches the motor board: 1 CANH, 2 CANL, 3 GND, 4 spare.

### ESP32 pins (suggested)

| Function                           | ESP32 pin      | Goes to                                                    |
| ---------------------------------- | -------------- | ---------------------------------------------------------- |
| TWAI TX                            | GPIO 5         | SN65HVD230 `CTX`                             |
| TWAI RX                            | GPIO 4         | SN65HVD230 `CRX`                             |
| I²C SDA / SCL                      | GPIO 21 / 22   | LSM6DSV, 400 kHz to 1 MHz                                  |
| E-stop                             | GPIO 27        | NC contact to GND, internal pull-up. Pressed **or a broken wire** reads high, so it fails safe. |
| BMS UART RX / TX (optional)        | GPIO 16 / 17   | JBD UART port. **Measure its logic level first.** If it isn't 3.3 V, read the BMS over Bluetooth instead. |
| 3V3 / GND                          | 3V3 / GND      | Transceiver and IMU power                                  |
| USB                                | Micro-USB/USB-C | Pi 4 USB port (power and serial)                          |

### Power

```text
 Pack A (−) ──┬──────────────────────── BMS B−
              └──────────────────────── XT90-S (−) ── robot GND bus
 Pack A (+) ── series link ── Pack B (−)
 Pack B (+) ──┬── 30 A 58 V ATO fuse ── XT90-S (+) ─┬─ L leg trunk (14 AWG) ── 5 × XT30 taps (16 AWG)
              │                                     ├─ R leg trunk (14 AWG) ── 5 × XT30 taps (16 AWG)
              │                                     └─ 3 A 58 V ATO fuse ── DROK buck ── USB-C ── Pi 4 ── USB ── ESP32
              └──────────────────────── charge jack (+)
 BMS P− ────────────────────────────── charge jack (−)

 Balance: Pack A JST-XH B0..B6 → BMS B0..B6
          Pack B JST-XH B1..B6 → BMS B7..B12   (leave Pack B's balance black wire unconnected; it duplicates BMS B6)
```

- **The BMS is wired charge-only on purpose.** The robot's load bypasses the BMS's MOSFETs. A 20 A BMS would trip on walking current peaks and drop power mid-step, and opening the bus while the motors regenerate pumps the bus voltage toward the STSPIN32G4's 75 V limit. The BMS still stops charging when any cell reaches its limit, balances the cells, and reports cell voltages.
- **Over-discharge is handled in firmware.** The ESP32 reads cell voltages from the BMS at 1 Hz: below 3.5 V per cell, the robot sits down and alarms; below 3.3 V, the motors are disabled. Each motor board's VBUS sense is a second, pack-level check.
- In the xiaoxiang app, set the BMS to **Li-ion (4.2 V)**, cell overvoltage 4.20–4.25 V. Never use LiFePO4 thresholds, which would stop charging at about 3.65 V per cell.
- Use matched packs (same brand, capacity and age) in series. Charge only with the robot idle.

### E-stop behaviour

1. Pressing the e-stop (or a broken wire, or no packet from the Pi for 50 ms) makes the ESP32 command every motor to damping (phase short). The legs slump instead of flopping, and no regen energy is pumped into the bus. The stop latches until it is reset.
2. Each motor board also goes to damping if it gets no valid CAN command for 50 ms, which covers a dead ESP32.
3. The XT90-S is the hard power disconnect once the robot is stopped. Later upgrade: a 48 V DC contactor in the main lead, driven by the e-stop's second NC contact.

## Firmware implications

- **Motor boards:** run FDCAN in **classic CAN mode at 1 Mbps**, not FD. The ESP32's controller treats FD frames as errors. The TCAN3414 handles classic CAN fine, so no hardware change is needed. Keep the PD loop and the 50 ms CAN-timeout damping on the board.
- **ESP32:** fixed-rate 200 Hz CAN cycle (send 10 commands, collect 10 replies). IMU read at 400 Hz or more using the LSM6DSV's on-chip fusion gravity vector, or a Mahony filter on the ESP32. Also the Pi watchdog, the e-stop latch and a 1 Hz BMS poll. Stream one packet per cycle to the Pi.
- **Pi:** policy at 50 Hz reads the latest state packet and writes targets. It isn't real-time and doesn't need to be.

## Raspberry Pi Pico: the upgrade path

The Pico has no CAN controller, so it isn't needed now. Keep it for:

- **A second CAN bus** if you ever want a 500 Hz to 1 kHz loop. `can2040` runs CAN 2.0B at up to 1 Mbps on the RP2040's PIO. Split the bus into ESP32 for the left leg and Pico for the right leg. That doubles the bandwidth for one more $11 transceiver.
- **A fallback bridge** if your ESP32 turns out to be a variant without TWAI.

The longer-term option is a CAN-FD bridge (any STM32G4 board) running 1 Mbps arbitration and 5 Mbps data, using the TCAN3414's FD capability. That gives roughly 1.5 kHz on a single bus.

## Relation to `main_board_BOM.md`

This replaces the custom main board for now (STM32G474, BMI088, LM5146, 2 × SN65HVD230). That design needed Standard PCBA because of the BMI088, 9 extended parts, and a $16 buck IC. Note that LCSC now flags the BMI088 (C194919, about $10.22) as **"Not recommended for new designs"**. If the custom board is revived, swap it for an ICM-42688-P or an LSM6DSV.

## Check before ordering

- [ ] Confirm which ESP32 you have (`esptool.py chip_id`). Anything except the ESP32-C2 has TWAI.
- [ ] Confirm the 6S packs' main connector (XT60 or XT90) for the series link. Buy a second matched pack.
- [ ] Re-check the price and stock of every link. Amazon listings change often.

## Sources

- can2040 (RP2040 PIO CAN, up to 1 Mbps): https://github.com/KevinOConnor/can2040
- ESP32 TWAI is not FD-tolerant: https://docs.espressif.com/projects/esp-faq/en/latest/software-framework/peripherals/twai.html
- Littelfuse MINI 58 V (fits standard MINI sockets): https://www.littelfuse.com/products/fuses-overcurrent-protection/fuses/automotive-fuses/blade-fuses-shunt/mini/997/0997030-wxn
- LSM6DSV-family SFLP (game rotation vector and gravity vector): https://community.st.com/mems-and-sensors-62/how-lsm6dsv16x-enables-sensor-fusion-low-power-sflp-algorithm-120280
- Waveshare SN65HVD230 fixed 120 Ω terminator: https://core-electronics.com.au/sn65hvd230-can-bus-transceiver-board-3-3v.html
- BMI088 at LCSC: https://www.lcsc.com/product-detail/C194919.html
