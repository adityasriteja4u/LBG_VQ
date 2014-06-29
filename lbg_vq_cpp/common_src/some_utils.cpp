
#include "stdafx.h"

#include <cmath>

#include "some_utils.h"

float log2(float inp)
{
	static float log_2 = log(2.0f);

	return log(inp) / log_2;
}

bool is_pow_2(unsigned int inp)
{
	return (inp != 0) && ((inp & (inp - 1)) == 0);
}

unsigned long get_file_size_in_bytes(FILE* file)
{
	fseek(file, 0, SEEK_END);
	unsigned long file_size = ftell(file);
	rewind(file);
	return file_size;

}