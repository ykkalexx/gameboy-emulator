#pragma once

#include <cstdint>
#include <cstdlib>

// macros for getting and settings bits
constexpr bool BIT(uint32_t a, uint32_t n) { return (a & (1 << n)) ? true : false; }
#define BIT_SET(a, n, on) (on ? a |= (1 << n) : a &= ~(1 << n))
// macros for checking number between 2 value
constexpr bool BETWEEN(uint32_t a, uint32_t b, uint32_t c) { return (a >= b) && (a <= c); }

void delay(uint32_t ms);