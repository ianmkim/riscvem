#include <iostream>

#include <elfio/elfio.hpp>

#include "utils.hpp"
#include "memory.hpp"
#include "registers.hpp"
#include "cpu.hpp"

int main(int argc, char** argv){
    run("../riscv-tests/isa/rv32ui-p-add");
    run("../riscv-tests/isa/rv32ui-p-addi");
    return 0;
}
