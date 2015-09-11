#pragma once
#include <utility>
#include <math.h>
#include <stdint.h>
#include <cstdint>

typedef uint64_t ul_int;

std::pair<bool, unsigned int> chkPow2(uint64_t N)
{
	if (N % 2 != 0) { return std::make_pair<bool, unsigned int>(false, 0); }

	double t = log2(N);

	//std::cout << t << "\t" << fmod(t, 1.0) << "\t" << (fmod(t, 1.0) == 0) << std::endl;

	return std::make_pair<bool, unsigned int>(fmod(t, 1.0) == 0, fmod(t, 1.0));
}


uint64_t myPow(uint8_t A, uint8_t B)
{
	uint64_t Total = 1;
	for (int index = 1; index <= B; index++) { Total *= A; }

	return Total;
}
