# EDC-off Symmetry (CRV)

## Property
When the EDC flag is OFF, left and right wheel commands must be equal.

Formally:
- $G(EDC\_OFF \rightarrow left\_cmd = right\_cmd)$

## Inputs used by the monitor
- `EDC_flag` (int8)
- `motor_left_cmd` (int32)
- `motor_right_cmd` (int32)

## CRV targets in this harness
- `motor_left_cmd`
- `motor_right_cmd`
- `EDC_flag`
- `bt_left` (speed command)

## How to run
```bash
make crv_summary
```
