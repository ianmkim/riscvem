#include <iostream>

#include <elfio/elfio.hpp>
#include <plog/Log.h>
#include <plog/Initializers/ConsoleInitializer.h>

#include "utils.hpp"
#include "memory.hpp"
#include "registers.hpp"
#include "cpu.hpp"

#include "CLI11.hpp"

int main(int argc, char** argv){
    CLI::App app("RISCVEm - A simple RISC-V emulator written in C++");

    std::string filename = "";
    app.add_option("-f,--file", filename, "A compiled ELF binary file for RISC-V RV32UI architecture")->required();

    int verbosity = 0;
    app.add_option("-v,--verbose", verbosity, "stdout log verbosity. 0 - no logs, 1 - debug logs, 2 - super verbose logs");
    if(verbosity < 0 || verbosity >= 3)
        throw std::runtime_error("Verbosity level must be between 0 and 2");

    CLI11_PARSE(app, argc, argv)

    plog::Severity verbosity_levels[3]{
        plog::none,
        plog::debug,
        plog::verbose,
    };
    plog::init(verbosity_levels[verbosity]);

    bool success = run(filename);

    return (int)success;
}
