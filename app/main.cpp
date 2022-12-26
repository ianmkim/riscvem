#include <iostream>

#include <elfio/elfio.hpp>
#include <plog/Log.h>
#include <plog/Initializers/ConsoleInitializer.h>

#include "utils.hpp"
#include "memory.hpp"
#include "registers.hpp"
#include "cpu.hpp"

int main(int argc, char** argv){
    plog::init(plog::debug);
    return 0;
}
