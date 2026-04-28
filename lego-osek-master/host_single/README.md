# Host simulation for nxtgt (no NXT hardware)

This folder builds a small Linux-hosted simulation of the `../nxtgt.c` application using minimal OSEK/nxtOSEK API stubs.

It does **not** generate a real `.rxe` binary and does **not** require the nxtOSEK toolchain.

## Run

```bash
cd lego-osek-master/host_single
make clean all
./host_single_demo
```

The simulator approximates the schedule from `../nxtgt.oil`:
- `TaskControl`: every 10ms
- `TaskSonar`: every 50ms
- `TaskLCD`: every 500ms

