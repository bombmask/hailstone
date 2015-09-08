#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <thread>
#include <Windows.h>
#include <omp.h> 



std::vector<unsigned int> hailstone(unsigned int n)
{
	// Create values vector
	std::vector<unsigned int> v;

	// Super awesome forloop to calc the sequence, TODO: time against while loop
	for (int i = n; i > 1; i = (i % 2 == 0 ? i / 2 : 3 * i + 1)) { v.push_back(i); }

	// Add final value to vector since for exits on `1`
	v.push_back(1);
	
	// Annnnnnnd return
	return v;
}

unsigned int hailstoneLength(unsigned int n)
{// Same function as hailstone except counts just length instead of the memory useage of vector
	
	// Allocate for forloop exit on `1`
	unsigned int tLength = 1;
	// Super awesome forloop
	for (int i = n; i > 1; i = (i % 2 == 0 ? i / 2 : 3 * i + 1)) { tLength++; }
	// Annnnnnnd return length
	return tLength;
}

std::pair<unsigned int, unsigned int> maxHailstoneLength(unsigned int start = 1, unsigned int end = 2, uint32_t step = 1)
{
	// Create tracking variables, cVal being signed long long was required by openMP
	long long cVal = 0;
	uint32_t largest = 0;
	uint32_t largestNVal = start;

	// Print general information
	std::cout << "doing numbers between : " << start << " and " << end << " for a total of: " << end - start << std::endl;

	// Set OpenMP total threads used, works best with amount of processors in the computer
	//omp_set_num_threads(omp_get_num_procs());

	
	// Get execution start time
	auto execStart = std::chrono::high_resolution_clock::now();

	// OpenMP pragma to create parallel for loop
	#pragma omp parallel
	{
	#pragma omp master
	{		std::cout << "Using '" << omp_get_num_threads() << "' thread(s)" << std::endl;	}

	#pragma omp for 
		for (cVal = start; cVal <= end; cVal += step)
		{
			// Calc current 'n' value for hailstone
			uint32_t cLength = hailstoneLength(cVal);
			// Test largest and set values
			if (cLength > largest)
			{
				largest = cLength;
				largestNVal = cVal;
			}
		}
	}

	// End Chrono timeit of execution and print
	auto total = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - execStart).count();
	std::cout << "total time: " << total << "ms" << std::endl;

	// return data in <length, Nvalue>
	return std::pair<uint32_t, uint32_t>(largest, largestNVal);
}

int WinMain()
{
	 
	// Return largest size and print
	auto largestSize = maxHailstoneLength(1, 100'000'000);
	std::cout << "Length: "<< largestSize.first << "\t NValue: " << largestSize.second << std::endl;
	
	// Wait for user input to close program
	std::cin.get();
	return 0;
}
