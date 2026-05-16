# Steering Sign Consistency (CRV)

## Property
The steering motor command sign must match the steering error sign.

Formally:
- $G(steer\_err > 0 \rightarrow cmd\_steer\_motor > 0)$
- $G(steer\_err < 0 \rightarrow cmd\_steer\_motor < 0)$
- $G(steer\_err = 0 \rightarrow cmd\_steer\_motor = 0)$

Where:
- $steer\_err = (STEERING\_LIMIT * cmd\_steer) / 100 - steering\_angle$

## Inputs used by the monitor
- `steering_angle` (int32)
- `cmd_steer` (int32)
- `motor_steer_cmd` (int32)

## CRV targets in this harness
- `steering_angle`
- `bt_right` (steering command)
- `motor_steer_cmd`

## How to run
```bash
make crv_summary
```
