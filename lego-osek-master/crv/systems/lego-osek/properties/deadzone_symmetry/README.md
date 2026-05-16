# Deadzone Symmetry (CRV)

## Property
Within the steering deadzone, left and right wheel commands must be equal.

Formally:
- $G(|steering\_angle| \le 2 \rightarrow left\_cmd = right\_cmd)$

## Inputs used by the monitor
- `steering_angle` (int32)
- `motor_left_cmd` (int32)
- `motor_right_cmd` (int32)

## CRV targets in this harness
- `steering_angle`
- `motor_left_cmd`
- `motor_right_cmd`
- `bt_left` (speed command)

## How to run
```bash
make crv_summary
```
