#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <tuple>


#include <math.h>
#include <omp.h>

#include "../include/sequence.hpp"
#include "../include/utils.h"

#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef _WIN32
	#include <Windows.h>
	#include <direct.h>
	#define GetCurrentDir _getcwd
#else
	#include <unistd.h>
	#define GetCurrentDir getcwd
#endif

typedef uint64_t ul_int;

haildata * hailstone(ul_int n)
{
	// Create values vector
	/// This is a problem, this data never gets deleted
	auto v = new haildata(n);
	
	// Super awesome forloop to calc the sequence, TODO: time against while loop
	for (ul_int i = n; i > 1; i = (i % 2 == 0 ? i / 2 : 3 * i + 1)) 
	{
		v->data.push_back(i);
		v->checkLargest(i);
	}
	

	// Add final value to vector since for exits on `1`
	v->data.push_back(1);
	
	// Annnnnnnd return
	return v;
}


ul_int hailstoneLength(ul_int n)
{// Same function as hailstone except counts just length instead of the memory usage of vector
	
	// Allocate for forloop exit on `1`
	ul_int tLength = 1;
	// Super awesome forloop
	for (ul_int i = n; i > 1; i = (i % 2 == 0 ? i / 2 : 3 * i + 1)) { tLength++; }
	// Annnnnnnd return length
	return tLength;
}

std::pair<ul_int, ul_int> maxHailstoneLength(ul_int start = 1, ul_int end = 2, ul_int step = 1)
{
	// Create tracking variables, cVal being signed long long was required by openMP
	long long cVal = 0;
	ul_int largest = 0;
	ul_int largestNVal = start;

	// Print general information
	std::cout << "doing numbers between : " << start << " and " << end << " for a total of: " << end - start << std::endl;
	
	// Get execution start time
	auto execStart = std::chrono::high_resolution_clock::now();

	// Begin OpenMP Section
	#pragma omp parallel
	{
	// Run on master thread only (otherwise prints this*core_count times)
	#pragma omp master
	{		std::cout << "Using '" << omp_get_num_threads() << "' thread(s)" << std::endl;	}

	// Begin optimize for loop OMP Don't wait on each cycle. 
	#pragma omp for schedule(guided)
		for (cVal = start; cVal <= end; cVal += step)
		{
			// Calc current 'n' value for hailstone
			ul_int cLength = hailstoneLength(cVal);
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

std::vector<haildata *> * hailstones_mutliprocess(ul_int start = 1, ul_int end = 1000, ul_int step = 1)
{
	// Store values
	auto *totals = new vector<haildata*>();

	// Print general information
	std::cout 
		<< "doing numbers between : " << start << " and " << end << " for a total of: " << (end - start)/step 
		<< std::endl;

	// Get execution start time
	auto execStart = std::chrono::high_resolution_clock::now();

	// OpenMP pragma to create parallel for loop
	#pragma omp parallel
	{
	
	#pragma omp master
	{	std::cout << "Using '" << omp_get_num_threads() << "' thread(s)" << std::endl;	}

	std::vector<haildata*> vec_private;
	long long int index = start;
	#pragma omp for nowait, schedule(dynamic, 1)
		for (index=start; index <= end; index += step)
		{
			vec_private.push_back(hailstone(index));
		}
	#pragma omp critical
		{ totals->insert(totals->end(), vec_private.begin(), vec_private.end()); }
	}
	

	// End Chrono timeit of execution and print
	auto total = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - execStart).count();
	std::cout 
		<< "total time: " << total << "ms" 
		<< std::endl 
		<< std::endl;

	return totals;
}

void write_hailstones(std::string filename, std::vector<haildata*> &data)
{
	std::cout 
		<< "sorting..." 
		<< std::endl;

	auto sortlambda = [](haildata *a, haildata *b) {return (a->N < b->N); };
	std::sort(data.begin(), data.end(), sortlambda);

	std::cout 
		<< "Done." 
		<< std::endl 
		<< std::endl;


	// Print current directory
	char cCurrentPath[FILENAME_MAX];

	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		throw errno;
	}

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

	std::cout 
		<< "The current working directory is " << cCurrentPath << std::endl
		<< "writing file..." << std::endl 
		<< std::endl;

	std::ofstream fout(filename);
	if (!fout.good())
	{
		throw fout.exceptions();
	}

	fout << "Numbers" << ", " << "Length" << ", " << "largest" << ", " << "Sequence" << std::endl;
	for (auto line : data)
	{
		fout << line->N << ", " << line->data.size() << ", " << line->largest << ", , ";
		for (auto value : (line->data))
		{
			fout << ", " << value;
		}
		fout << std::endl;
	}
	
	// Todo: Write file size to console
	fout.close();

	std::cout 
		<< "Finished." 
		<< std::endl;
}

int main()
{

	// Make numbers human readable
	std::locale loc("");
	std::cout.imbue(loc);

	//enum class EXECUTION { CSV, LENGTH };
	
	//std::cout << "Execution type: " << std::endl;
	//EXECUTION tSwitch;
	//std::cin >> tSwitch;

	uint64_t expo = 0;
	
	// Get User Value and Print to confirm
	std::cout << "Enter top value... ";
	std::cin >> expo; std::cin.ignore();
	std::cout 
		<< expo 
		<< std::endl 
		<< std::endl;

	const ul_int Value = expo;

	auto data = hailstones_mutliprocess(1, Value);
	write_hailstones("output.csv", *data);
	for (auto index : *data) { delete index; }; data->clear();
	delete data;
	//auto pair_size = maxHailstoneLength(1,Value);	std::cout << "N:" << pair_size.first << "\tV:" << pair_size.second << std::endl;

	// Wait for user input to close program
	std::cin.get();

	return 0;
}
