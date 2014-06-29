#ifndef LBG_CORE_H
#define LBG_CORE_H

#include <vector>
#include <stdexcept>
#include <cmath>
#include <iostream>

#include "some_utils.h"
#include "simple_vectors.h"

template <typename T, size_t vec_len> class LbgCode
{
        /* a very simple container-class to hold a code-vector and it's binary value at one place ... */
        /* this container does not allow modification of contained entities ...*/

        MyVector<T, vec_len> values;
        size_t code_len_bits; // in bits!!!
		size_t code_len_bytes; // in bytes!!!
        unsigned short code; /* in current task uint16 is more then enough to store all codes from 16 to 1024 */
public:
		typedef T _data_type;
		size_t dim() { return vec_len; }

        LbgCode(const std::vector<T>& vals, size_t bitsize, unsigned short code_value) : 
			values(vals), code_len_bits(bitsize), code(code_value) 
		{
			code_len_bytes = code_len_bits / 8;
			if (code_len_bits % 8) code_len_bytes++;

		}

        LbgCode() {}

		LbgCode operator*(const T factor) const
		{
			return LbgCode<T, vec_len>((values * factor).data(), code_len_bits, code);
		}

        const MyVector<T, vec_len>& Values() const { return values; }
        const size_t BitLength() const { return code_len_bits; }
        const unsigned short Code() const { return code; }
		const size_t BytLength() const { return code_len_bytes; }

};

template <typename T, size_t vec_len> class LbgCodeBook
{
        std::vector < LbgCode<T, vec_len> > book;
        size_t book_len;
        size_t max_book_len;
public:
		typedef T _data_type;
		size_t dim() { return book[0].dim(); }

        LbgCodeBook(size_t in_book_size) : book_len (0), max_book_len(in_book_size)
        {
                book.resize(max_book_len);
        }

        const size_t CurrentBookLen() const { return book_len; }
        const size_t TotalBookLen() const { return max_book_len; }
        bool IsBookReady() const { return book_len == max_book_len; }

        void AddCode (const LbgCode<T, vec_len>& code)
        {
                if (book_len >= max_book_len)
                        throw std::runtime_error("Current book is allready filled!");
                book[book_len++] = code;
        }

		void Clear() { book_len = 0; }

        const LbgCode<T, vec_len>& SelectCodeForInput(const MyVector<T, vec_len>& input, unsigned int *out_min_idx) const 
        {
                if (book_len < max_book_len)
                        throw std::runtime_error("Book is not filled yet, can not select optimal code!");

                unsigned int min_dist_idx = 0;
                float dist = ( input - (book[0]).Values() ).length();
                float cur_len = 0;
                for (unsigned int n = 1; n < max_book_len; ++n) {
                        cur_len = ( input - (book[n]).Values() ).length();
                        if (cur_len < dist) {
                                dist = cur_len;
                                min_dist_idx = n;
                        }
                }

				if(out_min_idx) *out_min_idx = min_dist_idx;
                return book[min_dist_idx];
        }

        const LbgCode<T, vec_len>& operator[](size_t idx) const
        {
                if(idx >= max_book_len)
						throw std::runtime_error("Indexation error in LbgCodeBookFloat!");
                else
                        return book[idx];
        }
        const size_t GetCodeBitLength() const
        {
                if (book_len > 0)
                        return book[0].BitLength();
                else
                        return 0;
        }
		const size_t GetCodeByteLength() const
        {
                if (book_len > 0)
                        return book[0].ByteLength();
                else
                        return 0;
        }
};

class LbgTrainer
{
public:
        template<typename T, size_t vec_len> static LbgCodeBook<T, vec_len> TrainBook(
			const std::vector < MyVector<T, vec_len> >& training_set, 
			size_t codes_total, 
			float epsilon)
        {
                //return CreateCodeBook<T, vec_len>(training_set, num_codes, epsilon);
					/* FOR NOW WE ONLY SUPPORT MODE, WHERE codes_total == 2^n !!!!!!!!!! */
			if (!is_pow_2(codes_total))
				throw std::runtime_error("Number of codes must be strictly integer power of 2 !!!");

			unsigned int bitlen = (unsigned int) ceil(log2((float)codes_total));

			/*************** initialization */
			MyVector<T, vec_len> init_center = CalcMeanVector(training_set);
			float init_mean_dist = CalcMeanDistance<T, vec_len> (training_set, init_center);

			unsigned int N = 1;
			unsigned int cur_bitlen = 1;
			unsigned int cur_code = 0;
			LbgCodeBook<T, vec_len>* center_cur = new LbgCodeBook<T, vec_len> (N);
			center_cur->AddCode(LbgCode<T, vec_len>(init_center.data(), cur_bitlen, cur_code));
			float mean_dist_cur = init_mean_dist;

			LbgCodeBook<T, vec_len> *center_i, *center_i1;
			float mean_dist_tmp;
			unsigned int iter_num;
			bool DO;

			std::vector< MyVector<T, vec_len> > vec_sums (codes_total);
			std::vector<size_t> vec_nums(codes_total);

			unsigned int STEPS_NUM = bitlen;
			for (unsigned int step = 0; step < STEPS_NUM; ++step) {
				/* DBG */
				center_i = new LbgCodeBook<T, vec_len>(N*2);
				DO = true;

				for (size_t n = 0; n < N; ++n){
					center_i->AddCode((*center_cur)[n] * (1 + epsilon));
					center_i->AddCode((*center_cur)[n] * (1 - epsilon));
				}

				iter_num = 0;
				N *= 2;
				mean_dist_tmp = mean_dist_cur;
				cur_code = 0;
				cur_bitlen = (unsigned int) ceil(log2((float)N));
				
				center_i1 = new LbgCodeBook<T, vec_len>(N);

				while (DO){
					printf("Working on step %d,  iter_num = %d,  D_tmp = %f ... (N = %u) \n", step, iter_num, mean_dist_tmp, N);
					
					center_i1->Clear();
					unsigned int tmp_idx = 0;
					for (size_t n = 0; n < N; ++n) {
							/* fucked-up in here ... !??!*/
						vec_sums[n].clear();
						vec_nums[n] = 0;
					}
					for (size_t m = 0; m < training_set.size(); ++m) {
						center_i->SelectCodeForInput(training_set[m], &tmp_idx);
						vec_sums[tmp_idx] += training_set[m];
						vec_nums[tmp_idx] += 1;
					}
					for (size_t n = 0; n < N; ++n) {
						vec_sums[n] /= vec_nums[n] == 0 ? 1 : vec_nums[n];
						center_i1->AddCode( LbgCode<T, vec_len>((vec_sums[n]).data(), cur_bitlen, cur_code++) );
					}
					
					mean_dist_tmp = CalcMeanDistanceByCodeBook<T, vec_len> (training_set, *center_i1);
					if ((mean_dist_cur - mean_dist_tmp)/mean_dist_tmp < epsilon)
						DO = false;

					mean_dist_cur = mean_dist_tmp;
					center_i->Clear();
					for (size_t n = 0; n < N; ++n)
						center_i->AddCode((*center_i1)[n]);
				}

				mean_dist_cur = mean_dist_tmp;
				/* here's some ugly pointer manipulations, but works faster this way */
				delete center_cur;
				center_cur = center_i;
				delete center_i1;
			}
			LbgCodeBook<T, vec_len> result(codes_total);

			for (size_t n = 0; n < codes_total; ++n)
				result.AddCode( (*center_cur)[n]);
			
			delete center_cur;
			return result;
        }

};


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

template<typename T, size_t vec_len> T
        CalcMeanDistanceByCodeBook(const std::vector< MyVector<T, vec_len> >& data,
                                        const LbgCodeBook<T, vec_len>& book)
{
        T result(0);

		
        for (unsigned int n = 0; n < data.size(); ++n) {
			const LbgCode<T, vec_len> &tmp = book.SelectCodeForInput(data[n], 0);
            result += (tmp.Values() - data[n]).length();
        }

        result /= ((float)data.size()* (float)vec_len);

        return result;
}

template <typename T, size_t vec_len> void trace_code_book(const LbgCodeBook<T, vec_len>& book)
{
	for (size_t n = 0; n < book.TotalBookLen(); ++n) {
		printf("Book[%d]: ", n);
		for (size_t m = 0; m < vec_len; ++m) {
			std::cout << (book[n].Values())[m] << " ";
		}
		printf("\n CODE:%x \n", book[n].Code());
	}
}

#endif
