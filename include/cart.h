#pragma once
#include <cstdint>

class RomHeader {
public:
    uint8_t entryPoint[4];
    uint8_t licenseLogo[48];
    char title[16];
    uint16_t newLicenseeCode;
    uint8_t sgbFlag;
    uint8_t cartType;
    uint8_t romSize;
    uint8_t ramSize;
    uint8_t destinationCode;
    uint8_t LicenseeCode;
    uint8_t maskRomVersion;
    uint8_t headerChecksum;
    uint16_t globalChecksum;
};

bool cart_load(char *cart);