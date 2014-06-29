// lbg_vq_cpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "basic_tests.h"

int main(int argc, char* argv[])
{
	basic_my_vector_tst();

        basic_bit_set_tst();

        basic_file_io_tst();

        basic_data_set_constructor_tst();

		basic_codebook_from_file_tst();

	printf("All done, press enter to exit ...");
	getchar();
	return 0;
}

