#ifndef LBG_FILE_UTILS_H
#define LBG_FILE_UTILS_H

#include <vector>
#include <stdexcept>
#include <stdlib.h>
#include <cmath>
#include <bitset>

#include "some_utils.h"
#include "lbg_core.h"

class DataSetConstructor
{

public:
        /* this one takes some file as const char* and creates vector<MyVector<...>> from it */
        template<size_t vec_len> static std::vector< MyVector<float, vec_len> >
                DataSetFromFloatFile(const char* file_name)
        {
            FILE* binlog = fopen(file_name, "rb");
            if (! binlog)
                    throw std::runtime_error("Specified file could not be opened!");
            //fseek(binlog, 0, SEEK_END);
            //unsigned long log_size = ftell(binlog);
            //rewind(binlog);
			unsigned long log_size = get_file_size_in_bytes(binlog);

            float* data = (float*) malloc ( sizeof(char) * log_size );
            if (!data)
                    throw std::runtime_error("Could not allocate enough size to read whole data from specified file!");

            size_t read_num = fread((char*)data, sizeof(float), log_size / sizeof(float), binlog);
            if (read_num != log_size / sizeof(float))
                    throw std::runtime_error("Number of read floats is not equal to expected one!");


            unsigned long vec_num = read_num / vec_len;
            std::vector< MyVector<float, vec_len> > result(vec_num);

            for (unsigned long n = 0; n < vec_num; ++n) {
                    /* DBG */
                    /* printf("Processing vector: %lu of %lu (pos in log : %lu of %lu ) ...\n",
                                    n, vec_num, n*vec_len, read_num); */
                    result[n] = MyVector<float, vec_len> (&data[n*vec_len]);
            }

            fclose(binlog);
            free(data);
            return result;
        }

};

template <typename T, size_t vec_len> class LbgBookToFile
{
public:

	static void WriteCodeBookToFile(const char* filename, const LbgCodeBook<T, vec_len>& book)
	{
		FILE* out_file = fopen(filename, "wb");
		if (!out_file)
			throw std::runtime_error("Could not create file with specified name!");
		
		if (!book.IsBookReady())
			throw std::runtime_error("Error! Specified LBG codebook in not full!");

		unsigned int book_len = book.TotalBookLen();
		unsigned int code_bit_len = book.GetCodeBitLength();
		unsigned int dim = vec_len;

		fwrite(&book_len, sizeof(unsigned int), 1, out_file);
		fwrite(&code_bit_len, sizeof(unsigned int), 1, out_file);
		fwrite(&dim, sizeof(unsigned int), 1, out_file);

		for (size_t n = 0; n < book_len; ++n)
			WriteCodeToFile(out_file, book[n]);

		fclose(out_file);
	}

	static void WriteCodeToFile(FILE* file, const LbgCode<T, vec_len>& code)
	{
		const std::vector<T>& tmp = code.Values().data();
		unsigned short code_value = code.Code();
		fwrite(&tmp[0], sizeof(T), vec_len, file);
		fwrite(&code_value, sizeof(unsigned short), 1, file);
	}

	static LbgCode<T, vec_len> ReadCodeFromMemory(unsigned char* location, size_t bitlen)
	{
		const T* data_ptr = (T*)location;
		unsigned int data_size = sizeof(T)*vec_len;
		unsigned short code = * ((unsigned short*)(location + data_size));
		return LbgCode<T, vec_len> (std::vector<T>(data_ptr, &data_ptr[vec_len]), bitlen, code);
	}

	static LbgCodeBook<T, vec_len> ReadBookFromFile(const char* filename)
	{
		FILE* in_file = fopen(filename, "rb");
		if (!in_file)
			throw std::runtime_error("Could not open specified file to read LBG codebook from!");

		unsigned int book_len;
		unsigned int code_bit_len;
		unsigned int dim;

		fread(&book_len, sizeof(unsigned int), 1, in_file);
		fread(&code_bit_len, sizeof(unsigned int), 1, in_file);
		fread(&dim, sizeof(unsigned int), 1, in_file);

		if (dim != vec_len) {
			fclose(in_file);
			throw std::runtime_error("Error! Specified LBG codebook file contains codebook with different dimensionality!");
		}

		const size_t codebook_size_in_bytes = sizeof(T)*dim*book_len + sizeof(unsigned short)*book_len;
		const size_t code_size_in_bytes = sizeof(T)*dim + sizeof(unsigned short);

		unsigned char* raw_codes_data = (unsigned char*) malloc (codebook_size_in_bytes);
		if (!raw_codes_data)
		{
			fclose(in_file);
			throw std::runtime_error("Error! Could not allocate enough space to read data from specified LBG codebook file!");
		}

		size_t bytes_read = fread(raw_codes_data, sizeof(unsigned char), codebook_size_in_bytes, in_file);
		if (bytes_read != codebook_size_in_bytes) {
			fclose(in_file);
			free(raw_codes_data);
			throw std::runtime_error("Error! Could not read correct expected number of bytes from specified LBG codebook file!");
		}

		fclose(in_file);

		LbgCodeBook<T, vec_len> book(book_len);
		for (unsigned int n = 0; n < book_len; ++n)
			book.AddCode(ReadCodeFromMemory(raw_codes_data + n*code_size_in_bytes, code_bit_len));
				
		return book;
	}

};

template<typename T, size_t vec_len> class LbgEncoder
{
	/* 
		this one takes some LbgCodeBook& and uses it to encode some file, results:
		1) byte*, containing encoded bit stream
		2) this byte* can be saved to some specified file
	
	*/

public:

	static void EncodeDataToFile(
		const std::vector< MyVector<T, vec_len> >& data, 
		const LbgCodeBook<T, vec_len>& book, 
		const char* filename)
	{
		FILE* out_file = fopen(filename, "wb");
		if (!out_file)
			throw std::runtime_error("Error! Could not open file to put encoded data to!");

		std::vector<unsigned char> encoded_data = EncodeDataToByteArray(data, book);
		fwrite(&encoded_data[0], sizeof(unsigned char), encoded_data.size(), out_file);

		fclose (out_file);
	}

	// a debugging routine
	//static void EncodeFloatFileToFile(
	//	const char* input_file, 
	//	const LbgCodeBook<T, vec_len>& book, 
	//	const char* output_file)
	//{
	//	typedef MyVector<float, vec_len> data_vector;
	//	std::vector< data_vector > dataset =
 //                       DataSetConstructor::DataSetFromFloatFile<vec_len> (input_file);

	//	FILE* out_file = fopen(output_file, "wb");
	//	if (!out_file)
	//		throw std::runtime_error("Error! Could not open file to put encoded data to!");

	//	std::vector<unsigned char> encoded_data = EncodeDataToByteArray<T, vec_len> (dataset, book);
	//	fwrite(&encoded_data[0], sizeof(unsigned char), encoded_data.size(), out_file);

	//	fclose (out_file);
	//}

	static std::vector<unsigned char> EncodeDataToByteArray(
		const std::vector< MyVector<T, vec_len> >& data, 
		const LbgCodeBook<T, vec_len>& book)
	{
		/* 
			Current implementation does not involve tight-bit packing, cause it only serves for demonstrative purpuse!!!
			In here we encode our data-vectors as sequences of 0's and 1's, where each 0 or 1 is a full-length character!!!
			This implementation is in no way optimal or fast, its' just a demo!
		*/

		const size_t bitlen = book.GetCodeBitLength();

		std::vector<unsigned char> result (data.size() * bitlen);

		size_t pos = 0;
		for (size_t n = 0; n < data.size(); ++n) {

			const LbgCode<T, vec_len>& c = book.SelectCodeForInput(data[n], 0);
			std::bitset<sizeof(unsigned short)*8> bs(c.Code()); /* we here use maximum possible number of bits in codebooks for this task */
			
			/* DBG */
			printf("Codding value %u : code = %x ...\n", n, c.Code());

			for (size_t m = 0; m < bitlen; ++m)
				result[pos + m] = bs[bitlen - 1 - m] + 48; /* +48 is used to translate 0/1 to '0'/'1'*/

			pos += bitlen;
		}

		return result;
	}

};

template<typename T, size_t vec_len> class LbgDecoder
{
public:
	typedef MyVector<T, vec_len> data_vector;
	/*
		This one takes some LbgCodeBook& and uses it to decode some file or byte*, results:
		1) some DATA_TYPE*
		2) this DATA_TYPE* can also be saved to some specified file ...
	*/

	static std::vector< data_vector> DecodeByteArrayToData(
		const std::vector<unsigned char>& byte_arr, 
		const LbgCodeBook<T, vec_len>& book)
	{
		/* 
			Current implementation does not presume tight-bit packing of source data, cause it only serves for demonstrative purpuse!!!
			In here suppose that our data-vectors are sequences of 0's and 1's, where each 0 or 1 is a full-length character!!!
			This implementation is in no way optimal or fast, its' just a demo!
		*/
		const size_t bitlen = book.GetCodeBitLength();
		const size_t result_size = byte_arr.size() / bitlen;

		std::vector < data_vector> result(result_size);

		unsigned int code_idx;
		unsigned int pos = 0;
		for (size_t n = 0; n < result_size; ++n) {
			/* getting current code from input data array */
			code_idx = 0;
			for ( size_t m = 0; m < bitlen - 1; ++m ) {
				code_idx += byte_arr[pos + m] - 48; /* -48 is used to translate '0'/'1' to 0/1*/
				code_idx <<=1;
			}
			code_idx += byte_arr[pos + bitlen - 1] - 48;
			pos += bitlen;

			/* DBG */
			printf("Decoding value %u (of %u) : code_idx = %x ...\n", n, result_size, code_idx);

			result [n] = book[code_idx].Values();
		}
		return result;
	}

	static std::vector< data_vector> DecodeFileToData(
		const char* encoded_file, 
		const LbgCodeBook<T, vec_len>& book)
	{
		FILE* in_file = fopen(encoded_file, "rb");
		if (!in_file)
			throw std::runtime_error("Error! Could not open spectified file for LBG decoding!");

		unsigned long file_size = get_file_size_in_bytes(in_file);
		if (file_size == 0) {
			fclose(in_file);
			throw std::runtime_error("Error! Specified file contains no data!");
		}

		std::vector<unsigned char> bindata(file_size);
		size_t bytes_read = fread(&bindata[0], 1, file_size, in_file);
		if( bytes_read != file_size) {
			fclose(in_file);
			throw std::runtime_error("Error! Could not read expected number of bytes from specified LBG-encoded file!");
		}
		
		fclose(in_file);

		return DecodeByteArrayToData( bindata, book);
	}

	static void DecodeFileToFile(
		const char* encoded_file,
		const char* result_file,
		const LbgCodeBook<T, vec_len>& book)
	{
		std::vector< data_vector> data = DecodeFileToData(encoded_file, book);

		FILE* out_file = fopen(result_file, "wb");
		if (!out_file)
			throw std::runtime_error(" Error! Could not open file to which decoded data is supposed to be written!");

		for (size_t n = 0; n < data.size(); ++n)
			fwrite( &(data[n].data())[0], sizeof(T), vec_len, out_file);

		fclose(out_file);
	}
};


#endif
