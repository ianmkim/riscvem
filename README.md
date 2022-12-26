# RISCVem
Tiny RISCV emulator written in C. Started off as a python implementation
following geohot's [twitchcore](https://github.com/geohot/twitchcore) and
ultimately evolved into a more in-depth C/C++ implementation.

Status: passes all tests!

![alt text](https://github.com/ianmkim/riscvem/blob/master/docs/tests.png?raw=true)

## Installations
### Requirements
You first need to build the tests for RISC-V emulators. This particular repo only implements `rv32ui` - RV32, user level, integer only. The test suite only runs the `p` environment which disables the use of virtual memory and only allows for one core.

Clone and build the submodules of this repository:
```bash
git submodule update --init --recursive
cd riscv-tests
./configure
make -j
```

Now you should be able to see the RISCV tests listed in the directory
`riscv-tests/isa/`

If you got an error during the make stage, make sure you have the RISCV toolkit
installed on your operating system.

### RISCVem
Installation for RISCVem is simple:
```bash
mkdir build
cd build
cmake ..
make
```

## Usage
If you wish to run an automated test of the emulator, run the included test
binary which performs all tests for RV32UI single mode, raw memory, privileged mode of
RISCV:
```bash
./rv32ui_tests
```

If you have your own ELF binary, then run:
```bash
./riscvem -f <path to your ELF> -v 1
```

Note that by default, verbosity level is set to 0. So you will only know if
your code ran through the return value of the program. There are three levels
of verbosity you can use. The highest level: 2 will print a dump of the 8 words
of memory near the program counter address + values of all registers at each
step of execution.


