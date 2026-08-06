# Encoder Modbus RTU Firmware

## 1. Project Overview

This project is an embedded firmware for absolute encoder control based on dsPIC33CK MCU.

The firmware supports encoder data acquisition, position management, speed calculation and Modbus RTU communication.

---

## 2. Main Features

- Absolute encoder position reading
- Single-turn and multi-turn position processing
- Encoder zero position setting
- Position offset storage
- RPM calculation
- Rotation direction detection
- Modbus RTU communication
- UART error recovery
- EEPROM/DEE parameter storage
- Configurable communication parameters

---

## 3. Hardware Platform

| Item | Description |
|---|---|
| Sensor Chip | IC-MU + IC-PVL |
| MCU | dsPIC33CK256MP502 |
| Clock Source | External Crystal | 8MHz |
| Toolchain | MPLAB XC16 |
| Protocol | FreeModBus |
| Interface | RS485 |
| Encoder Interface | Absolute SSI |

---

## 4. Supported Encoder Data

### Position Data

| Parameter | Description |
|---|---|
| Single Turn | Encoder single revolution position |
| Multi Turn | Encoder rotation count |
| Total Position | Combined absolute position |

Position calculation:Total_Position =(MultiTurn_Data << SingleTurn_Bit)+SingleTurn_Data

---


---

## 5. Modbus RTU Communication

### Supported Function Codes

| Function Code | Description |
|---|---|
| 03H | Read Holding Register |
| 06H | Write Single Register |
| 10H | Write Multiple Registers |

---

### Communication Parameters

Configurable parameters:

- Slave ID
- Baud Rate
- Parity
- Protocol Mode
- Report Interval

Supported baud rates: 1200|2400|4800|9600|19200|38400|57600|115200

---


---

## 6. Encoder Control Functions

### Zero Setting

The current encoder position can be saved as zero position.

Command: Write Register 0x0005 = 0xFF

---


---

### Position Preset

Encoder position can be preset by writing: 0x0032 : Position Low Word
                                           0x0033 : Position High Word

---


---

## 7. Speed Calculation

The firmware calculates:

- Rotation speed (RPM)
- Rotation direction


Speed update period: Update Period = Register Value × 10ms

---


---

## 8. Parameter Storage

The following parameters are stored in internal DEE:

- Encoder zero offset
- Baud rate configuration
- Encoder configuration parameters

Parameters are automatically restored after power-on.

---

## 9. UART Error Handling

The firmware includes UART error recovery:

Supported errors:

- RX Overrun Error (OERR)
- Framing Error (FERR)
- Parity Error (PERR)

Recovery functions:

- Clear UART error flags
- Flush RX FIFO
- Restart UART receiver

This prevents communication lock-up caused by invalid frames or incorrect communication settings.

---

## 10. Documentation

Related documents:  doc/
                    └── DEE_Direction_Define.md
                    └── Modbus_Register_Map.md

---