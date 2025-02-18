# Embedded Systems Projects

This repository contains two main projects developed as part of the **Embedded Systems** course at the University of Genova. These projects focus on real-time data acquisition, sensor integration, and autonomous control systems.

## Projects Overview

1. **Magnetometer Data Collection and Processing (Assignment 1)**
2. **Autonomous Car Control System (Final Project)**

---

## 1. Magnetometer Data Collection and Processing

### Objective
The goal of this project is to interface with a **magnetometer sensor** via **SPI communication**, collect real-time data, process it to determine the **magnetic north direction**, and transmit the processed data using **UART**.

### Features
- **SPI Communication**: Configure SPI to communicate with the magnetometer.
- **Data Acquisition**: Read X, Y, and Z magnetic field values at **25 Hz**.
- **Data Processing**:
  - Compute the average of the last **five** measurements.
  - Calculate the **yaw angle** relative to magnetic north using `atan2(y_avg, x_avg)`.
- **UART Transmission**:
  - Send raw averaged data at **5 Hz** using `$MAG,x,y,z*`.
  - Send the calculated yaw angle at **5 Hz** using `$YAW,x*`.
- **Real-Time Constraints**:
  - Simulate an **algorithm execution time** of **7 ms** within a **100 Hz loop**.
  - Utilize **interrupts and circular buffers** for efficient UART communication.

### How to Run
1. Initialize SPI and UART interfaces.
2. Configure the **magnetometer data rate** by setting register `0x4C` to `0b110`.
3. Collect magnetometer data at **25 Hz**, process it, and compute the yaw angle.
4. Transmit processed data via UART at **5 Hz**.

### Files
- **`tools.h` / `tools.c`**: Implements SPI and UART communication, data collection, and processing functions.
- **`main.c`**: Controls the execution flow, manages timing constraints, and handles real-time data processing.

---

## 2. Autonomous Car Control System

### Objective
The objective of this project is to design an **embedded control system** for an autonomous car that can receive movement commands via **UART**, process them, execute corresponding motor actions, and provide feedback.

### Features
- **Command Handling**:
  - Receive movement commands in the format `$PCCMD,x,t*`, where:
    - `x` represents the action: 
      - `1 = Forward`
      - `2 = Counterclockwise Rotation`
      - `3 = Clockwise Rotation`
      - `4 = Backward`
    - `t` specifies the duration in milliseconds.
  - Maintain a **FIFO queue (max 10 commands)** for command execution.
  - Send acknowledgment messages `$MACK,1*` (success) or `$MACK,0*` (queue full).

- **Motor Control**:
  - Four PWM signals control the **left and right wheels** at **10 kHz**.
  - Commands map to PWM signals to adjust speed and direction.

- **State Management**:
  - **Waiting State**:
    - Motors are idle.
    - LED blinks at **1 Hz**.
    - Commands are stored in FIFO for later execution.
  - **Execution State**:
    - Processes commands in FIFO.
    - Executes motor movements and monitors distance sensors.
    - Stops movement if an obstacle is detected within **20 cm**.
    - LED blinks at **1 Hz**.

- **Sensor Integration**:
  - **Battery Monitoring**: Read battery voltage from **AN11** and transmit `$MBATT,v_batt*` at **1 Hz**.
  - **Distance Sensing**: Read IR sensor data from **AN14/AN15**, transmit `$MDIST,distance*` at **10 Hz**.
  - **Obstacle Avoidance**: Stop motion if an obstacle is detected within **20 cm**.

### How to Run
1. Power on the autonomous car system.
2. The system starts in **Waiting State**. Press **RE8** to switch to **Execution State**.
3. Send movement commands via UART.
4. Monitor responses and sensor data through UART output.
5. Press **RE8** again to reset back to **Waiting State**.

### Files
- **`tools.h` / `tools.c`**: Implements UART communication, command parsing, motor control, and sensor handling.
- **`main.c`**: Manages system states, executes commands, and handles real-time constraints.

---

## Authors
- **Ouassim Milous**
- **Younes Hebik**
- **Mohamed Aimen Boulala**
