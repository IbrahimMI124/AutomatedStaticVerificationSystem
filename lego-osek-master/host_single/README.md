# Host simulation for nxtgt (no NXT hardware)

This folder builds small Linux-hosted simulations of the `../nxtgt.c` and `../rms.c` applications using minimal OSEK/nxtOSEK API stubs.

It does **not** generate a real `.rxe` binary and does **not** require the nxtOSEK toolchain.

## Run

```bash
cd lego-osek-master/host_single
make clean all
./host_single_demo
```

For the RMS example:

```bash
cd lego-osek-master/host_single
make clean all
./host_single_rms_demo
```

The simulator approximates the schedule from `../nxtgt.oil`:
- `TaskControl`: every 10ms
- `TaskSonar`: every 50ms
- `TaskLCD`: every 500ms

For `../rms.c`, it follows `../rms.oil`:
- `Task1`: every 1ms
- `Task2`: every 500ms
- `TaskLCD`: every 500ms

