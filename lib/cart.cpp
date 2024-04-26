/*
    cart.cpp - Gameboy Cartridge Loader
    This file contains the implementation of the cart_load function
    which is used to load a Gameboy ROM file into memory.
*/

#include "cart.h"
#include <cstdint>
#include <map>
#include <string>
#include <stdexcept>
#include <iostream>

struct Cart {
    char filename[1024];
    uint32_t romSize;
    uint8_t *romData;
    RomHeader *header;
};

static Cart ctx;

// map used to look up the license name based on the id
// this is a static map that is only used in this file
// so it is defined here and not in the header file
// this information is taken from the GBATEK documentation
static const std::map<int, std::string> licenseCode =
{
    {0x00, "None"},
    {0x01, "Nintendo R&D1"},
    {0x08, "Capcom"},
    {0x13, "Electronic Arts"},
    {0x18, "Hudson Soft"},
    {0x19, "b-ai"},
    {0x20, "kss"},
    {0x22, "pow"},
    {0x24, "PCM Complete"},
    {0x25, "san-x"},
    {0x28, "Kemco Japan"},
    {0x29, "seta"},
    {0x30, "Viacom"},
    {0x31, "Nintendo"},
    {0x32, "Bandai"},
    {0x33, "Ocean/Acclaim"},
    {0x34, "Konami"},
    {0x35, "Hector"},
    {0x37, "Taito"},
    {0x38, "Hudson"},
    {0x39, "Banpresto"},
    {0x41, "Ubi Soft"},
    {0x42, "Atlus"},
    {0x44, "Malibu"},
    {0x46, "angel"},
    {0x47, "Bullet-Proof"},
    {0x49, "irem"},
    {0x50, "Absolute"},
    {0x51, "Acclaim"},
    {0x52, "Activision"},
    {0x53, "American sammy"},
    {0x54, "Konami"},
    {0x55, "Hi tech entertainment"},
    {0x56, "LJN"},
    {0x57, "Matchbox"},
    {0x58, "Mattel"},
    {0x59, "Milton Bradley"},
    {0x60, "Titus"},
    {0x61, "Virgin"},
    {0x64, "LucasArts"},
    {0x67, "Ocean"},
    {0x69, "Electronic Arts"},
    {0x70, "Infogrames"},
    {0x71, "Interplay"},
    {0x72, "Broderbund"},
    {0x73, "sculptured"},
    {0x75, "sci"},
    {0x78, "THQ"},
    {0x79, "Accolade"},
    {0x80, "misawa"},
    {0x83, "lozc"},
    {0x86, "Tokuma Shoten Intermedia"},
    {0x87, "Tsukuda Original"},
    {0x91, "Chunsoft"},
    {0x92, "Video system"},
    {0x93, "Ocean/Acclaim"},
    {0x95, "Varie"},
    {0x96, "Yonezawa/s’pal"},
    {0x97, "Kaneko"},
    {0x99, "Pack in soft"},
    {0xA4, "Konami (Yu-Gi-Oh!)"}
};

// array used to look up the rom type based on the id
// this is a static array that is only used in this file
// so it is defined here and not in the header file
// this information is taken from the gbdev.io documentation
static const char *romTypes[] = {
    "ROM ONLY",
    "MBC1",
    "MBC1+RAM",
    "MBC1+RAM+BATTERY",
    "0x04 ???",
    "MBC2",
    "MBC2+BATTERY",
    "0x07 ???",
    "ROM+RAM 1",
    "ROM+RAM+BATTERY 1",
    "0x0A ???",
    "MMM01",
    "MMM01+RAM",
    "MMM01+RAM+BATTERY",
    "0x0E ???",
    "MBC3+TIMER+BATTERY",
    "MBC3+TIMER+RAM+BATTERY 2",
    "MBC3",
    "MBC3+RAM 2",
    "MBC3+RAM+BATTERY 2",
    "0x14 ???",
    "0x15 ???",
    "0x16 ???",
    "0x17 ???",
    "0x18 ???",
    "MBC5",
    "MBC5+RAM",
    "MBC5+RAM+BATTERY",
    "MBC5+RUMBLE",
    "MBC5+RUMBLE+RAM",
    "MBC5+RUMBLE+RAM+BATTERY",
    "0x1F ???",
    "MBC6",
    "0x21 ???",
    "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
};

// function used to look up the license name based on the id
const char *cart_lic_name() {
    if (ctx.header->newLicenseeCode <= 0xA4 ) {
        try {
            return licenseCode.at(ctx.header->LicenseeCode).c_str();
        } catch (const std::out_of_range&) {
            return "Unknown";
        }
    }

    return "Unknown";
}

// function used to look up the rom type based on the id
const char *cart_type_name() {
    if (ctx.header->cartType <= 0x22) {
        return romTypes[ctx.header->cartType];
    }

    return "Unknown";
}

bool cart_load(char *cart) {
    snprintf(ctx.filename, sizeof(ctx.filename), "%s", cart);
    FILE *fp = fopen(cart, "r");
    if(!fp) {
        std::cout << "Error: Could not open file " << cart << std::endl;
        return false;
    }

    // go to the end to see how big the file
    fseek(fp, 0, SEEK_END);
    ctx.romSize = ftell(fp);

    // go back to the beginning
    rewind(fp);

    // allocate memory for the rom data
    ctx.romData = new uint8_t[ctx.romSize];
    fread(ctx.romData, ctx.romSize, 1,fp);
    fclose(fp);

    //the header is located at 0x100 in the rom, we also null terminate the title
    ctx.header = (RomHeader *)(ctx.romData + 0x100);
    ctx.header->title[15] = 0;

    std::cout << "Title: " << ctx.header->title << std::endl;
    std::cout << "Type: " << cart_type_name() << std::endl;
    std::cout << "ROM Size: " << (32 << ctx.header->romSize) << "KB" << std::endl;
    std::cout << "RAM Size: " << (ctx.header->ramSize == 0 ? 0 : 1 << (ctx.header->ramSize + 3)) << "KB" << std::endl;

    uint16_t x = 0;
    for(uint16_t i = 0x0134; i<=0x014C; i++) {
        x = x - ctx.romData[i] - 1;
    }

    std::cout << "Header Checksum: " << (x == ctx.header->headerChecksum ? "OK" : "BAD") << std::endl;

    return true;
}