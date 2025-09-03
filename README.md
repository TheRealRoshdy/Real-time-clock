# Real-Time Clock (RTC) with DS1307, LCD, and Keypad

An embedded systems project implementing a **real-time clock** using the **DS1307 RTC module**, with time displayed on a **16x2 Character LCD (CLCD)** and configured through a **4x4 keypad**.  
This project is written in **C** for **AVR microcontrollers** (tested on ATmega32 @ 8 MHz), and demonstrates modular driver development for I²C (TWI), LCD, keypad, and RTC devices.

---

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Installation & Build](#installation--build)

---

## Overview
The project demonstrates:
- **Real-time timekeeping** with persistence using the DS1307 RTC module.
- **Time display** on a 16x2 character LCD.
- **Interactive configuration** via a 4x4 keypad to set hours, minutes, and seconds.
- **Modular driver-based design**, separating hardware abstraction (LCD, keypad, I²C) from application logic.

This project is well-suited for:
- Students learning **embedded C programming**.
- Demonstrations of **I²C (TWI) communication** with external peripherals.
- Educational labs in **real-time systems and microcontroller interfacing**.

---

## Features
- DS1307 RTC integration via I²C (TWI).
- 16x2 CLCD interface for displaying time.
- Keypad-based interactive time setting.
- Persistent timekeeping during power loss (via DS1307 battery backup).
- Modular reusable drivers for:
  - DS1307 RTC
  - TWI (I²C)
  - CLCD (character LCD)
  - KPD (keypad)
- Utility headers (`BIT_MATH.h`, `STD_TYPES.h`) for portability and readability.

---

## Hardware Requirements
- **Microcontroller**: ATmega32 (or compatible AVR MCU, 8 MHz clock recommended).
- **RTC Module**: DS1307 with backup coin cell battery.
- **LCD**: 16x2 Character LCD (HD44780 compatible).
- **Keypad**: 4x4 matrix keypad.
- **Power Supply**: 5 V regulated supply.
- Jumper wires, breadboard or PCB.

---

## Software Requirements
- **AVR-GCC toolchain** (for compiling C code).
- **AVRDUDE** (for flashing binaries).
- Alternatively, you can use **Atmel Studio / Microchip Studio** for build + flash.
- `make` (if using the included Makefile).

---

## Installation & Build

Clone the repository:
```bash
git clone https://github.com/TheRealRoshdy/Real-time-clock.git
cd Real-time-clock
