#pragma once
#include <vector>
#include <utility>
#include <algorithm>
#include <tuple>

#include "utils.h"

using std::vector;
struct haildata
{
	ul_int N = 1;
	ul_int largest = 0;
	vector<ul_int> data;

	haildata(ul_int n) : N(n), largest(0)
	{
		data = vector<ul_int>();
		//data = new vector<ul_int>();
	}

	~haildata()
	{
		//delete data;
	}

	void checkLargest(ul_int nLargest)
	{
		largest = (nLargest > largest ? nLargest : largest);
	}
};