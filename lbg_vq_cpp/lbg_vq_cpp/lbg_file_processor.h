#ifndef LBG_FILE_PROCESSOR_H
#define LBG_FILE_PROCESSOR_H

#include <vector>
#include <stdexcept>
#include <stdlib.h>


template <typename T, size_t vec_len> class LbgCode
{
        /* a very simple container-class to hold a code-vector and it's binary value at one place ... */
        /* this container does not allow modification of contained entities ...*/

        MyVector<T, vec_len> values;
        size_t code_len_bits; // in bits!!!
        unsigned short code; /* in current task uint16 is more then enough to store all codes from 16 to 1024 */
public:
        LbgCode(const std::vector<T>& vals, size_t bitsize, unsigned short code_value) : values(vals), code_len_bits(bitsize),
        code(code_value) {}
        LbgCode() {}

        const MyVector<T, vec_len>& Values() const { return values; }
        const size_t BitLength() const { return code_len_bits; }
        const unsigned short Code() const { return code; }

};

template <size_t vec_len> class LbgCodeBookFloat
{
        std::vector < LbgCode<float, vec_len> > book;
        size_t book_len;
        size_t max_book_len;
public:
        LbgCodeBookFloat(size_t in_book_size) : book_len (0), max_book_len(in_book_size)
        {
                book.resize(max_book_len);
        }

        const size_t CurrentBookLen() const { return book_len; }
        const size_t TotalBookLen() const { return max_book_len; }
        bool IsBookReady() const { return book_len == max_book_len; }

        void AddCode (const LbgCode<float, vec_len>& code)
        {
                if (book_len >= max_book_len)
                        throw std::runtime_error("Current book is allready filled!");
                book[book_len++] = code;
        }

        const LbgCode<float, vec_len>& SelectCodeForInput(const MyVector<float, vec_len>& input)
        {
                if (book_len < max_book_len)
                        throw std::runtime_error("Book is not filled yet, can not select optimal code!");

                unsigned int min_dist_idx = 0;
                float dist = (input - book[0]).length();
                float cur_len = 0;
                for (unsigned int n = 1; n < max_book_len; ++n) {
                        cur_len = (input - book[n]).length();
                        if (cur_len < dist) {
                                dist = cur_len;
                                min_dist_idx = n;
                        }
                }

                return book[min_dist_idx];
        }

        const LbgCode<float, vec_len>& operator[](size_t idx)
        {
                if(idx >= max_book_len)
                        return NULL;
                else
                        return book[idx];
        }
        const size_t GetCodeBitLength()
        {
                if (book_len > 0)
                        return book[0].BigLength();
                else
                        return 0;
        }
	/* current class takes main input - codebook length! from it we can get:
	1) Length in bits of each code word ( by current task it spans from 2 to 10 bits )
	2) Acquire each codes' binary representation

	Class simply wrapps a vector< MyVector<T, vec_len> >, where each MyVector - is in its' turn too a vector of vec_len values
	Each element of codebook has it's binary representation. 

	LbgCodeBook can also provide a routine FindCode(MyVector<T, vec_len>& X), where X is a value, that should be encoded
	LbgCodeBook can also be saved to a file in some predefined format ...
	*/
};


template<size_t vec_len> LbgCodeBookFloat<vec_len>  CreateCodeBookFromFloats(const std::vector < MyVector<float,
                        vec_len> >& training_set, size_t num_codes, float epsilon);

class LbgTrainer
{
	/* 
	This one wrapss a training routine, that take intputs:
	1) training set as an array of data-vectors
	2) length of each vector in a training set
	3) number of desired codes
	4) epsilon (convergense ratio)
	Result of train() is an instance of LbgCodeBook
	*/
public:
        template<size_t vec_len> static LbgCodeBookFloat<vec_len> TrainBookFloat(const std::vector < MyVector<float,
                        vec_len> >& training_set, size_t num_codes, float epsilon)
        {
                return CreateCodeBookFromFloats<vec_len>(training_set, num_codes, epsilon);
        }

};

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
                fseek(binlog, 0, SEEK_END);
                unsigned long log_size = ftell(binlog);
                rewind(binlog);

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

class LbgEncoder
{
	/* 
		this one takes some LbgCodeBook& and uses it to encode some file, results:
		1) byte*, containing encoded bit stream
		2) this byte* can be saved to some specified file
	
	*/
};

class LbgDecoder
{
	/*
		This one takes some LbgCodeBook& and uses it to decode some file or byte*, results:
		1) some DATA_TYPE*
		2) this DATA_TYPE* can also be saved to some specified file ...
	*/
};


template<typename T, size_t vec_len> MyVector<T, vec_len>
        CalcMeanVector(const std::vector< MyVector<T, vec_len> >& data);

template<typename T, size_t vec_len> T
        CalcMeanDistance(const std::vector< MyVector<T, vec_len> >& data, MyVector<T, vec_len> X);

template<size_t vec_len> float
        CalcMeanDistanceByCodeBookFloat(const std::vector< MyVector<float, vec_len> >& data,
                                        const LbgCodeBookFloat<vec_len>& book);


template<size_t vec_len> LbgCodeBookFloat<vec_len>  CreateCodeBookFromFloats(
                const std::vector < MyVector<float,vec_len> >& training_set, 
                size_t num_codes,
                float epsilon)
{
        LbgCodeBookFloat<vec_len> result(num_codes);

        /* initialization */
        MyVector<float, vec_len> init_center = CalcMeanVector(training_set);
        float init_mean_dist = CalcMeanDistance<float, vec_len> (training_set, init_center);

        /* DBG */
        printf(" init_center: (%f ; %f ),  init_mean_dist: %f ...\n",
                        init_center.X(), init_center.Y(), init_mean_dist);

        return result;
}


template<typename T, size_t vec_len> MyVector<T, vec_len>
        CalcMeanVector(const std::vector< MyVector<T, vec_len> >& data)
{
        MyVector<T, vec_len> result;

        for (unsigned int n = 0; n < data.size(); ++n)
                result += data[n];
        result /= static_cast<T>(data.size());

        return result;
}


template<typename T, size_t vec_len> T
        CalcMeanDistance(const std::vector< MyVector<T, vec_len> >& data, MyVector<T, vec_len> X)
{
        T result = T(0);

        for (unsigned int n = 0; n < data.size(); ++n)
                result += (X - data[n]).length();

        result /= ((T)data.size()* (T)vec_len);
        return result;
}


template<size_t vec_len> float
        CalcMeanDistanceByCodeBookFloat(const std::vector< MyVector<float, vec_len> >& data,
                                        const LbgCodeBookFloat<vec_len>& book)
{
        float result = 0.0f;

        for (unsigned int n = 0; n < data.size(); ++n) {
                result += (book.SelectCodeForInput(data[n]) - data[n]).length();
        }

        result /= ((float)data.size()* (float)vec_len);

        return result;
}

#endif
