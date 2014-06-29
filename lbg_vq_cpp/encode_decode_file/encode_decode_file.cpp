// encode_decode_file.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "lbg_file_utils.h"

#define TST_CB_FILE_NAME "../Release/lbg_codebook_sz_32_eps_0.10.lbg"

#define EPSILON 0.01f
#define MAX_BOOK_SIZE 1024

void testing_routine()
{
	const size_t vec_len = 13;

	LbgCodeBook<float, vec_len> book = LbgBookToFile<float, vec_len>::ReadBookFromFile(TST_CB_FILE_NAME);
	trace_code_book<float, vec_len>(book);

	typedef MyVector<float, vec_len> data_vector;
	std::vector< data_vector > dataset = DataSetConstructor::DataSetFromFloatFile<vec_len> ("../../test.dat");
	LbgEncoder<float, vec_len>::EncodeDataToFile(dataset, book, "test.cod");

	//std::vector< data_vector> decoded_data = LbgDecoder<float, vec_len>::DecodeFileToData("test.cod", book);

	LbgDecoder<float, vec_len>::DecodeFileToFile("test.cod", "test.dec", book);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//testing_routine();

	unsigned int cur_book_size = 2;
	const size_t vec_len = 13;
	typedef MyVector<float, vec_len> data_vector;

	char book_file_name[256];
	char encoded_file_name[256];
	char decoded_file_name[256];

	std::vector< data_vector > dataset = DataSetConstructor::DataSetFromFloatFile<vec_len> ("../../test.dat");

	while (cur_book_size < MAX_BOOK_SIZE) {
		cur_book_size <<=1;

		sprintf_s(book_file_name, 256, "lbg_codebook_sz_%d_eps_%1.2f.lbg", cur_book_size, EPSILON);
		sprintf_s(encoded_file_name, 256, "encoded_codebook_sz_%d_eps_%1.2f.cod", cur_book_size, EPSILON);
		sprintf_s(decoded_file_name, 256, "decoded_codebook_sz_%d_eps_%1.2f.dec", cur_book_size, EPSILON);

		printf(" Working with book: %s ...\n", book_file_name);
		printf(" Encoding data to: %s ...\n", encoded_file_name);
		printf(" Decoding data to: %s ...\n", decoded_file_name);

		LbgCodeBook<float, vec_len> book = LbgBookToFile<float, vec_len>::ReadBookFromFile(book_file_name);
		
		LbgEncoder<float, vec_len>::EncodeDataToFile(dataset, book, encoded_file_name);
		LbgDecoder<float, vec_len>::DecodeFileToFile(encoded_file_name, decoded_file_name, book);
	}


	printf("All done, press enter to exit ...");
	getchar();
	return 0;
}

