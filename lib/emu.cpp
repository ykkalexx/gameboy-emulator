/*
The implementation of the emulator core header file
The emulator has the following core components: 
Cart -> load the cartridge to read data from it/ write data
CPU -> emulating all the different instructions and registration of a gameboy CPU
Address Bus -> Reading and writing to addresses in the memory
PPU -> pixel processing unit 
Timer -> keeps the timer process of the gameboy
*/

#include <iostream>
#include <string>
#include <emu.h>
#include <cart.h>
#include <cpu.h>
#include <SDL2/SDL.h>

static EmuContext ctx;

EmuContext *emu_get_context() {
    return &ctx;
}

void delay(uint32_t ms) {
    SDL_Delay(ms);
}

int emu_init(int argc, char **argv) {
    // check if a rom file is provided
    if (argc < 2) {
        std::cout << "Usage: emu <rom_file>\n";
        return -1;
    }
    std::string rom_file = argv[1];
    //implemeting the cart_load function
    if (!cart_load(rom_file.c_str())) {
        std::cout << "Failed to load rom file\n";
        return -1;
    }

    // if cartridge is present, initialize the SDL
    SDL_Init(SDL_INIT_VIDEO);
    
    // TODO: install SDL Fonts

    // initialize the CPU
    cpu_init();

    ctx.running = true;
    ctx.running = false;
    ctx.ticks = 0;

    while(ctx.running) {
        if (ctx.paused) {
            delay(10);
            continue;
        }

        if (!cpu_step()) {
            std::cout << "CPU step failed\n";
            return -3;
        }
        ctx.ticks++;
    }

    return 0;
}