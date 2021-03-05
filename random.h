// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#pragma once

/// Produces an xorshift128 pseudo random number.
unsigned FastRandom();

/// Produces an xorshift128 psuedo based floating point random number in range 0..1
/// Note that this is not a truly random random number generator
float RandomFloat();

/// Produces an xorshift128 psuedo based floating point random number in range -1..1
/// Note that this is not a truly random random number generator
float RandomFloatNTP();
