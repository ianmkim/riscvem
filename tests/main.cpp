#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include <string>
#include <plog/Log.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Initializers/RollingFileInitializer.h>

#include "cpu.hpp"

const std::string basepath = "../riscv-tests/isa/";

TEST_CASE("rv32ui-p-add"){
    CHECK(run(basepath + std::string("rv32ui-p-add")));
}

TEST_CASE("rv32ui-p-addi"){
    CHECK(run(basepath + std::string("rv32ui-p-addi")));
}

TEST_CASE("rv32ui-p-and"){
    CHECK(run(basepath + std::string("rv32ui-p-and")));
}

TEST_CASE("rv32ui-p-auipc"){
    CHECK(run(basepath + std::string("rv32ui-p-auipc")));
}

TEST_CASE("rv32ui-p-beq"){
    CHECK(run(basepath + std::string("rv32ui-p-beq")));
}

TEST_CASE("rv32ui-p-bge"){
    CHECK(run(basepath + std::string("rv32ui-p-bge")));
}

TEST_CASE("rv32ui-p-bgeu"){
    CHECK(run(basepath + std::string("rv32ui-p-bgeu")));
}

TEST_CASE("rv32ui-p-blt"){
    CHECK(run(basepath + std::string("rv32ui-p-blt")));
}

TEST_CASE("rv32ui-p-bltu"){
    CHECK(run(basepath + std::string("rv32ui-p-bltu")));
}

TEST_CASE("rv32ui-p-bne"){
    CHECK(run(basepath + std::string("rv32ui-p-bne")));
}

TEST_CASE("rv32ui-p-fence_i"){
    CHECK(run(basepath + std::string("rv32ui-p-fence_i")));
}

TEST_CASE("rv32ui-p-jal"){
    CHECK(run(basepath + std::string("rv32ui-p-jal")));
}

TEST_CASE("rv32ui-p-jalr"){
    CHECK(run(basepath + std::string("rv32ui-p-jalr")));
}

TEST_CASE("rv32ui-p-lb"){
    CHECK(run(basepath + std::string("rv32ui-p-lb")));
}

TEST_CASE("rv32ui-p-lbu"){
    CHECK(run(basepath + std::string("rv32ui-p-lbu")));
}

TEST_CASE("rv32ui-p-lh"){
    CHECK(run(basepath + std::string("rv32ui-p-lh")));
}

TEST_CASE("rv32ui-p-lhu"){
    CHECK(run(basepath + std::string("rv32ui-p-lhu")));
}

TEST_CASE("rv32ui-p-lui"){
    CHECK(run(basepath + std::string("rv32ui-p-lui")));
}

TEST_CASE("rv32ui-p-lw"){
    CHECK(run(basepath + std::string("rv32ui-p-lw")));
}

TEST_CASE("rv32ui-p-or"){
    CHECK(run(basepath + std::string("rv32ui-p-or")));
}

TEST_CASE("rv32ui-p-ori"){
    CHECK(run(basepath + std::string("rv32ui-p-ori")));
}

TEST_CASE("rv32ui-p-sb"){
    CHECK(run(basepath + std::string("rv32ui-p-sb")));
}

TEST_CASE("rv32ui-p-sh"){
    CHECK(run(basepath + std::string("rv32ui-p-sh")));
}

TEST_CASE("rv32ui-p-simple"){
    CHECK(run(basepath + std::string("rv32ui-p-simple")));
}

TEST_CASE("rv32ui-p-sll"){
    CHECK(run(basepath + std::string("rv32ui-p-sll")));
}

TEST_CASE("rv32ui-p-slli"){
    CHECK(run(basepath + std::string("rv32ui-p-slli")));
}

TEST_CASE("rv32ui-p-slt"){
    CHECK(run(basepath + std::string("rv32ui-p-slt")));
}

TEST_CASE("rv32ui-p-slti"){
    CHECK(run(basepath + std::string("rv32ui-p-slti")));
}

TEST_CASE("rv32ui-p-sltiu"){
    CHECK(run(basepath + std::string("rv32ui-p-sltiu")));
}

TEST_CASE("rv32ui-p-sltu"){
    CHECK(run(basepath + std::string("rv32ui-p-sltu")));
}

TEST_CASE("rv32ui-p-sra"){
    CHECK(run(basepath + std::string("rv32ui-p-sra")));
}

TEST_CASE("rv32ui-p-srai"){
    CHECK(run(basepath + std::string("rv32ui-p-srai")));
}

TEST_CASE("rv32ui-p-srl"){
    CHECK(run(basepath + std::string("rv32ui-p-srl")));
}

TEST_CASE("rv32ui-p-sub"){
    CHECK(run(basepath + std::string("rv32ui-p-sub")));
}

TEST_CASE("rv32ui-p-sw"){
    CHECK(run(basepath + std::string("rv32ui-p-sw")));
}

TEST_CASE("rv32ui-p-xor"){
    CHECK(run(basepath + std::string("rv32ui-p-xor")));
}

TEST_CASE("rv32ui-p-xori"){
    CHECK(run(basepath + std::string("rv32ui-p-xori")));
}


int main(int argc, char** argv) {
    doctest::Context context;

    // !!! THIS IS JUST AN EXAMPLE SHOWING HOW DEFAULTS/OVERRIDES ARE SET !!!
    plog::init(plog::debug, "testlog.txt");

    // defaults
    context.addFilter("test-case-exclude", "*math*"); // exclude test cases with "math" in their name
    context.setOption("abort-after", 5);              // stop test execution after 5 failed assertions
    context.setOption("order-by", "name");            // sort the test cases by their name

    context.applyCommandLine(argc, argv);

    // overrides
    context.setOption("no-breaks", true);             // don't break in the debugger when assertions fail

    int res = context.run(); // run

    if(context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
        return res;          // propagate the result of the tests
    
    int client_stuff_return_code = 0;
    // your program - if the testing framework is integrated in your production code
    
    return res + client_stuff_return_code; // the result from doctest is propagated here as well
}
