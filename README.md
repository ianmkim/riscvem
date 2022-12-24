# RISCVem
Tiny RISCV emulator written in C. Started off as a python implementation
following geohot's [twitchcore](https://github.com/geohot/twitchcore) and
ultimately evolved into a more in-depth C/C++ implementation.

## Installations
You first need to build the tests for RISC-V emulators. This particular repo only implements `rv32ui` - RV32, user level, integer only. The test suite only runs the `p` environment which disables the use of virtual memory and only allows for one core.
