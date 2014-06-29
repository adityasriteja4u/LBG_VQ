// lbg_processor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

/* standard library includes ... */
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>

/* our common utilities includes ... */
#include <lbg_core.h>
#include <lbg_file_utils.h>
#include <some_utils.h>


const char* HELP_STR = "Mandatory input parameters (in order of their usage):\n\
arg1 - training dataset file (string with path to file)\n\
arg2 - input data file to encode (string with path to file)\n\
arg3 - resulting encoded/decoded filenames (just a string with filename, extension will be added!)\n\
arg4 - size of codebook (unsigned integer number, POWER OF 2 !!!)\n\
arg5 - epsilon (positive floating-point number, less then 1.0)";

//#define DBG

int _tmain(int argc, char* argv[])
{
	/* DBG */
#ifdef DBG
	for (int i = 0; i < argc; ++i)
		printf("INPUT PARAM %d : %s ...\n", i+1, argv[i]);
#endif

	/* Current demonstrational program can only work with data-space of fixed dimension. In case of our case it's 13 by task conditions */
	const size_t vec_len = 13;

	if ( (argc == 2 && !strcmp(argv[1], "h")) || (argc != 6) )
	{
		printf("\nErroneous input parameters, correct program usage: \n\n%s\n", HELP_STR);
		getchar();
		return 0;
	}

	/* reading input parametes, performing only MINIMAL input check! */
	
	std::string training_file(argv[1]);
	FILE* tmp = fopen(training_file.c_str(), "r");
	if (! tmp) {
		printf("Specified training file does not exist or can not be accessed at the moment! Aborting ...");
		return 1;
	}
	fclose(tmp);

	std::string input_file(argv[2]);
	tmp = fopen(input_file.c_str(), "r");
	if (! tmp) {
		printf("Specified input file does not exist or can not be accessed at the moment! Aborting ...");
		return 2;
	}
	fclose(tmp);

	std::string out_file_name(argv[3]);
	out_file_name.append(".cod");
	tmp = fopen(out_file_name.c_str(), "w");
	if (! tmp) {
		printf("Specified output filename is erroneous! Aborting ...");
		return 3;
	}
	fclose(tmp);
	out_file_name.resize(out_file_name.size() - 4);

	int tmp1 = atoi(argv[4]);
	if ( !is_pow_2(tmp1) ) {
		printf("Invalid codebook size specified - must be power of 2! Aborting ...");
		return 5;
	}
	size_t cb_size = tmp1;

	float epsilon = atof(argv[5]);
	if( epsilon <= 0.0f || epsilon >= 1.0f) {
		printf("Invalid epsilon specified - must be real number in (0:1) interval! Aborting ...");
		return 6;
	}

	/* seems like we can work with inputs, print them and continue ...*/
	std:: cout << " Training file: " << training_file <<  std::endl
		<< " Input file to process: " << input_file << std::endl
		<< " Output files: " << out_file_name << std::endl 
		<< " Space dimensionality: " << vec_len << std::endl
		<< " Codebook size: " << cb_size << std::endl
		<< " Epsilon: " << epsilon << std::endl;

	typedef MyVector<float, vec_len> data_vector;

	try {
		/* start actual data processing ... */ 

		/* step 1 : load training dataset and input dataset to memory, arranging them into a handy object ... */
		std::cout << "Reading training data ..." << std::endl;
		std::vector< data_vector > train_dataset =
			DataSetConstructor::DataSetFromFloatFile<vec_len> (training_file.c_str());
		std::cout << "Reading input data ..." << std::endl;
		std::vector< data_vector > inp_dataset =
			DataSetConstructor::DataSetFromFloatFile<vec_len> (input_file.c_str());

		/* step 2 : train a codebook of specified size from dataset*/
		std::cout << "Training a codebook of size " << cb_size << " ..." << std::endl;
		LbgCodeBook<float, vec_len> book = LbgTrainer::TrainBook<float, vec_len>(train_dataset, cb_size, epsilon);

		/* step 3 : encode an input datafile and save results */
		std::string encoded_file_name(out_file_name);
		encoded_file_name.append(".cod");
		std::cout << "Coding input data, writing results to : " << encoded_file_name << " ..." << std::endl;
		LbgEncoder<float, vec_len>::EncodeDataToFile(inp_dataset, book, encoded_file_name.c_str());
		
		/* step 4 : decode resulting encoded datafile */
		std::string decoded_file_name(out_file_name);
		decoded_file_name.append(".dec");
		std::cout << "Decoding data, writing results to : " << decoded_file_name << " ..." << std::endl;
		LbgDecoder<float, vec_len>::DecodeFileToFile(encoded_file_name.c_str(), decoded_file_name.c_str(), book);

		/* that's about it for the CPP part of the task ... */

	} catch (std::runtime_error e) {
		std::cout << "Exception occured during execution: %s ..." << e.what() << std::endl;
		return 7;
	}

	printf("All done, press enter to exit ...");
	getchar();
	return 0;
}

