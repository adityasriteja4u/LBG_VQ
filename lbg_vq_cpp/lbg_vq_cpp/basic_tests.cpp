

#include "stdafx.h"

#include <stdio.h>
#include <typeinfo>
#include <iostream>
#include <fstream>
#include <bitset>

#include "basic_tests.h"
#define DBG_SIMPLE_VECTORS
#include "simple_vectors.h"
#include "lbg_file_processor.h"

void basic_my_vector_tst()
{
	printf(" ******* RUNNING basic_my_vector_tst() ... ****** \n");

	/* same stuff for MyVector<T,2> class ... */
	MyVector<int, 2>::data_type a1;
	std::cout << "type of Vec2<int>::data_type : " << typeid(a1).name() << std::endl;
	MyVector<float, 2> b1(0.0f, 2.0f);
	printf("Vector B:"); b1.TRACE();
	MyVector<float, 2> c1(3.0f, 0.0f);
	printf("Vector C:"); c1.TRACE();
	MyVector<float, 2> d1(c1.data());
	printf("Vector D:"); d1.TRACE();
	//std::cout << c1[3] << std::endl; // will throws runtime_error cause breaking boundaries of Vec2
	(-c1).TRACE(); /* no memory leak but an actual object is created and returned ... */
	-c1; /* no memory leak but an actual object is created and returned ... */
	(b1 + c1).TRACE(); /* and again a new object is created and returned ... */
	(d1 += c1).TRACE(); /* no new object is created, but keep in mind, that d1 is object, created in local scope */
	(d1 -= c1).TRACE();
	(d1 * 10).TRACE();
	(d1 *= 5).TRACE();
	(d1 / 3).TRACE();
	(d1 /= 3).TRACE();
	printf("B dot C : %f ...\n", b1.dot(c1));

        float arr[] = { 10.0f, 12.0f, 13.0f, 14.0f };
        MyVector<float, 2> a2(&arr[1]);
        a2.TRACE();

	printf(" ******* END OF basic_my_vector_tst() ... ****** \n\n\n");
}

void basic_bit_set_tst()
{
        printf(" ******* RUNNING basic_bit_set_tst() ... ****** \n");
        std::bitset<4> set(10);
        // set << 1;
        // set << 0;
        // set << 1;
        // set << 0;

        std::ofstream myfile;
        myfile.open("bitset.log");
        myfile << set;
        myfile.close();
	printf(" ******* END OF basic_bit_set_tst() ... ****** \n\n\n");
}

void basic_file_io_tst()
{
        printf(" ******* RUNNING basic_file_io_tst() ... ****** \n");

        std::ofstream write;
        std::ifstream read;

        write.open("write.log");
        write << 10.f;
        write << 12.f;
        write << 'a';
        write.close();

        read.open("write.log");
        float a,b,c;
        read >> a;
        read >> b;
        read >> c;
        read.close();

        printf(" Read results: %f, %f, %f ...\n", a, b, c);

        printf(" ******* END OF basic_file_io_tst() ... ****** \n\n\n");
}

void basic_data_set_constructor_tst()
{
        printf(" ******* RUNNING basic_data_set_constructor_tst() ... ****** \n");

        const size_t vec_num = 13;

        typedef MyVector<float, vec_num> data_vector;

        try {
                std::vector< data_vector > dataset =
                        DataSetConstructor::DataSetFromFloatFile<vec_num> ("../../base.dat");
                dataset[0].TRACE();
                dataset[1].TRACE();

                LbgTrainer::TrainBookFloat<vec_num>(dataset, 8, 0.1);

        } catch (std::runtime_error e) {
                printf(" Error occured during execution: %s ...\n", e.what());
        }

        printf(" ******* END OF basic_data_set_constructor_tst() ... ****** \n\n\n");
}


