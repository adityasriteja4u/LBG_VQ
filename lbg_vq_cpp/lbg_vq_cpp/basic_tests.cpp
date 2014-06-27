

#include "stdafx.h"

#include "basic_tests.h"
#include <stdio.h>
#define DBG_SIMPLE_VECTORS
#include "simple_vectors.h"

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
	printf(" ******* END OF basic_my_vector_tst() ... ****** \n\n\n");
}
