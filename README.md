# Robot_Arm
# 6-DOF Robotic Arm

Arduino Uno controlled 6-DOF robotic arm, driven by 6 servos and controlled via serial commands.
Inspired by TaiChi-Maker and MakerBuying

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

Serial port config:

- Baud rate: **9600**
- Line ending: **No line ending**

## Serial Commands

Two operation modes: **Instruction Mode (default, absolute angle control)** and **Handle Mode (relative jog control)**.

### Instruction Mode (Power‑on Default)

Send command character followed by number, e.g. `b90`

chart

| Command | Description |
| --- | --- |
| `b<angle>` | Set Base to absolute angle |
| `1<angle>` | Set Arm1 to absolute angle |
| `2<angle>` | Set Arm2 to absolute angle |
| `3<angle>` | Set Arm3 to absolute angle |
| `w<angle>` | Set Wrist to absolute angle |
| `c<angle>` | Set Claw to absolute angle |
| `m` | Switch to Handle Mode |
| `v` | Print all servo positions status |
| `n` | Return all joints to home position (90°) |
| `x` | Run pre‑programmed grab demo sequence |

### Handle Mode (Jog / Relative Control)

After sending `m` from instruction mode. Each keypress moves joint by fixed `moveStep` angle.

chart

| Key | Action |
| --- | --- |
| `f` | Base turn clockwise |
| `t` | Base turn anti‑clockwise |
| `g` | Arm1 move down |
| `y` | Arm1 move up |
| `h` | Arm2 move down |
| `u` | Arm2 move up |
| `j` | Arm3 move down |
| `i` | Arm3 move up |
| `k` | Wrist turn clockwise |
| `o` | Wrist turn anti‑clockwise |
| `l` | Claw open |
| `p` | Claw close |
| `m` | Switch back to Instruction Mode |

## Parameters

- `DSD = 15`: Servo step delay(ms), smoothness of joint movement
- `moveStep = 3`: Step increment for handle jog mode
- Angle limit for every joint: `0‑180°`
