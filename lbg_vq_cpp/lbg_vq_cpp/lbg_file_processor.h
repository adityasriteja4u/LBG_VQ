#ifndef LBG_FILE_PROCESSOR_H
#define LBG_FILE_PROCESSOR_H

class LbgCodeBook
{
	/* current class takes main input - codebook length! from it we can get:
	1) Length in bits of each code word (by current task it spans from 16 to 1024 bits (i.e. uint16 - uint32[32])
	2) Acquire each codes' binary representation

	Class simply wrapps a vector< MyVector<T, vec_len> >, where each MyVector - is in its' turn too a vector of vec_len values
	Each element of codebook has it's binary representation. 

	LbgCodeBook can also provide a roitine FindCode(MyVector<T, vec_len>& X), where X is a value, that should be encoded
	LbgCodeBook can also be saved to a file in some predefined format ...
	*/
};

class LbgTrainer
{
	/* 
	This one wrapss a training routine, that take intputs:
	1) training DB (file or mb an array ...)
	2) length of a vector in a training set
	3) number of desired codes
	4) epsilon (convergense ratio)
	Result of train() is an instance of LbgCodeBook
	*/
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

#endif