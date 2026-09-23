# Robot_Arm
# 6-DOF Robotic Arm

Arduino Uno controlled 6-DOF robotic arm, driven by 6 servos and controlled via serial commands.

## Wiring

| Component | Pin |
|-----------|-----|
| Base | D3 |
| Arm1 | D5 |
| Arm2 | D6 |
| Arm3 | D9 |
| Wrist | D10 |
| Claw | D11 |

> Use external 5V supply for servos, share GND with Uno.

## Build

```bash
pio run            # build
pio run -t upload  # flash
pio device monitor # serial monitor (9600 baud)
