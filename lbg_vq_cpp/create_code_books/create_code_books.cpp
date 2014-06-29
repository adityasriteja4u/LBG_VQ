// create_code_books.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "lbg_file_utils.h"

#define EPSILON 0.01f
#define MAX_BOOK_SIZE 1024

int _tmain(int argc, _TCHAR* argv[])
{
	
	const size_t vec_len = 13;
	typedef MyVector<float, vec_len> data_vector;

	char book_file_name[256];
	unsigned int cur_book_size = 2;
	try {
		std::vector< data_vector > dataset =
                        DataSetConstructor::DataSetFromFloatFile<vec_len> ("../../base.dat");
		
		while (cur_book_size < MAX_BOOK_SIZE) {
			cur_book_size <<=1;
            LbgCodeBook<float, vec_len> book = LbgTrainer::TrainBook<float, vec_len>(dataset, cur_book_size, EPSILON);
			trace_code_book<float, vec_len>(book);

			sprintf_s(book_file_name, 256, "lbg_codebook_sz_%d_eps_%1.2f.lbg", cur_book_size, EPSILON);
			LbgBookToFile<float, vec_len>::WriteCodeBookToFile(book_file_name, book);

			printf("%s \n", book_file_name);
		}

    } catch (std::runtime_error e) {
                printf(" Error occured during creation of LGB codebook: %s ...\n", e.what());
    }

	printf("All done, press enter to exit ...");
	getchar();
	return 0;
}

