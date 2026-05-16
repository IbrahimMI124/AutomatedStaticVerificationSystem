# EDC-on Differential (CRV)

## Property
When EDC is ON and the steering angle is outside the deadzone, the wheel commands must differ unless the speed command is zero.

Formally:
- $G(EDC\_ON \wedge |steering\_angle| > 2 \rightarrow (cmd\_speed = 0 \lor left\_cmd \ne right\_cmd))$

## Inputs used by the monitor
- `EDC_flag` (int8)
- `steering_angle` (int32)
- `cmd_speed` (int32)
- `motor_left_cmd` (int32)
- `motor_right_cmd` (int32)

## CRV targets in this harness
- `EDC_flag`
- `steering_angle`
- `bt_left` (speed command)
- `motor_left_cmd`

## How to run
```bash
make crv_summary
```
