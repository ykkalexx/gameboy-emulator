/*
Core emulator header file
*/

#pragma once
#include "common.h"
#include <cstdint>

class EmuContext {
public:
    bool paused; // checks if the emulator is paused
    bool running; // checks if the emulator is running
    uint64_t ticks; //processor ticks
};

int emu_init(int argc, char **argv);

EmuContext* emu_get_context();