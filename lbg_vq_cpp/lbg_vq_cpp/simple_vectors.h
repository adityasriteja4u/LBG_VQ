#ifndef SIMPLE_VECTORS_H
#define SIMPLE_VECTORS_H


#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

template<typename T, size_t sz> T vector_length(std::vector<T> inp)
{
	T result(0);
	for (size_t k = 0; k < sz; ++k)
		result += inp[k] * inp[k];
	result = sqrt(result);
	return result;
}

template<typename T, size_t sz> std::vector<T> vector_add(const std::vector<T>& inp1,
		const std::vector<T>& inp2)
{
	std::vector<T> result(sz);
	for (size_t k = 0; k < sz; ++k)
		result[k] = inp1[k] + inp2[k];
	return result;
}

template<typename T, size_t sz> void vector_inplace_add(std::vector<T>& inp1, const std::vector<T>& inp2)
{
	for (size_t k = 0; k < sz; ++k)
		inp1[k] += inp2[k];
}

template<typename T, size_t sz> std::vector<T> vector_sub(const std::vector<T>& inp1,
		const std::vector<T>& inp2)
{
	std::vector<T> result(sz);
	for (size_t k = 0; k < sz; ++k)
		result[k] = inp1[k] - inp2[k];
	return result;
}

template<typename T, size_t sz> void vector_inplace_sub(std::vector<T>& inp1, const std::vector<T>& inp2)
{
	for (size_t k = 0; k < sz; ++k)
		inp1[k] -= inp2[k];
}

template<typename T, size_t sz> std::vector<T> vector_inverse(const std::vector<T> inp)
{
	std::vector<T> result(sz);
	for (size_t k = 0; k < sz; ++k)
		result[k] = -inp[k];
	return result;
}

template<typename T, size_t sz> std::vector<T> vector_mult_by_scalar(const std::vector<T>& inp1, const T inp2)
{
	std::vector<T> result(sz);
	for (size_t k = 0; k < sz; ++k)
		result[k] = inp1[k] * inp2;
	return result;
}

template<typename T, size_t sz> void vector_mult_by_scalar_inplace(std::vector<T>& inp1, const T inp2)
{
	for (size_t k = 0; k < sz; ++k)
		inp1[k] *= inp2;
}

template<typename T, size_t sz> T vector_dot_product(const std::vector<T>& inp1,
		const std::vector<T>& inp2)
{
	T result(0);
	for (size_t k = 0; k < sz; ++k)
		result += inp1[k] * inp2[k];
	return result;
}

//template<typename T, size_t sz1, size_t sz2>std::array<T,3> vector_cross_product(const std::array<T, sz1>& inp1,
//		const std::array<T, sz2>& inp2)
//{
//	std::array<T, 3> result;
//	if ( (sz1 < 2 || sz1 > 3) || (sz2 < 2 || sz2 > 3) )
//		throw new std::runtime_error("ERROR: in MyVector.cross() : supported sizes are only 2 and 3");
//	std::array<T,3>* v1, v2;
//	bool del1, del2;
//	if (sz1 < 3) {
//		v1 = new std::array<T,3>{{inp1[0], inp1[1], 0}};
//		del1 = true;
//	} else {
//		v1 = &inp1;
//		del1 = false;
//	}
//	if (sz2 < 3) {
//		v2 = new std::array<T,3>{{inp2[0], inp2[1], 0}};
//		del2 = true;
//	} else {
//		v2 = &inp2;
//		del2 = false;
//	}
//
//	/* do the actual cross-product ... */
//
//	if (del1) delete v1;
//	if (del2) delete v2;
//	return result;
//}

template<typename T, size_t sz> class MyVector;

//template<typename T> using Vec2 = MyVector<T, 2>;
//template<typename T> using Vec3 = MyVector<T, 3>;
//template<typename T> using Vec4 = MyVector<T, 4>;

template<typename T, size_t vec_size> class MyVector
{
public:
	static const size_t size = vec_size;
private:
	std::vector<T> coords;
	T len;
public:
	typedef T data_type;
	T X() const { return coords[0]; }
	T Y() const { if (size < 2) return T(0); else return coords[1]; }
	T Z() const { if (size < 3) return T(0); else return coords[2]; }
	T U() const { if (size < 4) return T(0); else return coords[3]; }
	T length() const { return len; };
	const std::vector<T>& data() const { return coords; }
#ifdef DBG_SIMPLE_VECTORS
	void TRACE() const
	{
		std::cout << "TRACING VECTOR " << this << ", size=" << size << ": X=" << this->X() << ", Y="
			<< this->Y() << ", Z=" << this->Z() << ", U=" << this->U() << ", LEN=" << len << std::endl;
	}
#endif
	
	/* constructors ... */
	MyVector(): coords(size)
	{
		for (size_t k = 0; k < size; ++k)
			coords[k] = T(0);
		len = T(0);
	}
	MyVector(T x, T y): coords(size)
	{
		if (size != 2)
			throw new std::runtime_error("ERROR in MyVector : MyVector(x,y) constructor is only available \
for MyVector<size=2>");
		coords[0] = x;
		coords[1] = y;
		len = vector_length<T, size>(coords);
	}
	MyVector(T x, T y, T z):coords(size)
	{
		if (size != 3)
			throw new std::runtime_error("ERROR in MyVector : MyVector(x,y,z) constructor is only \
available for MyVector<size=3>");
		coords[0] = x;
		coords[1] = y;
		coords[2] = z;
		len = vector_length<T, size>(coords);
	}
	MyVector(T x, T y, T z, T u):coords(size)
	{
		if (size != 4)
			throw new std::runtime_error("ERROR in MyVector : MyVector(x,y,z,u) constructor is only \
available for MyVector<size=4>");
		coords[0] = x;
		coords[1] = y;
		coords[2] = z;
		coords[3] = u;
		len = vector_length<T, size>(coords);
	}
	MyVector(const T* inp_data) : coords(&inp_data[0], &inp_data[vec_size])
	{
		len = vector_length<T, size>(coords);
	}
	MyVector(const std::vector<T>& inp_data) : coords(inp_data)
	{
		len = vector_length<T, size>(coords);
	}

	const T operator[](size_t idx) const
	{
		if (idx < size)
			return coords[idx];
		else
			throw new std::runtime_error("ERROR: In MyVector[] - index out of bound!!!");
	}
	MyVector<T, size> operator-() const
	{
		return MyVector<T, size> (vector_inverse<T, size> (coords));
	}
	/* WARNING! Reference is returned! Use with extra-care! */
	MyVector<T, size>& normalize()
	{
		for (size_t k = 0; k < size; ++k)
			coords[k] /= len;
		len = vector_length<T, size> (coords);
		return *this;
	}
	MyVector<T, size> operator+(const MyVector<T, size>& inp) const
	{
		return MyVector<T, size> ( vector_add<T, size> (coords, inp.data()) );
	}
	MyVector<T, size> operator-(const MyVector<T, size>& inp) const
	{
		return MyVector<T, size> ( vector_sub<T, size> (coords, inp.data()) );
	}
	/* WARNING! Reference is returned! Use with extra-care! */
	MyVector<T, size>& operator+=(const MyVector<T, size>& inp)
	{
		vector_inplace_add<T, size>(coords, inp.data());
		len = vector_length<T, size>(coords);
		return *this;
	}
	/* WARNING! Reference is returned! Use with extra-care! */
	MyVector<T, size>& operator-=(const MyVector<T, size>& inp)
	{
		vector_inplace_sub<T, size>(coords, inp.data());
		len = vector_length<T, size>(coords);
		return *this;
	}

	MyVector<T, size> operator*(const T inp) const
	{
		return MyVector<T, size> ( vector_mult_by_scalar<T, size>(coords, inp) );
	}

	/* WARNING! Reference is returned! Use with extra-care! */
	MyVector<T, size>& operator*=(const T inp)
	{
		vector_mult_by_scalar_inplace<T, size>(coords, inp);
		len = vector_length<T, size>(coords);
		return *this;
	}

	MyVector<T, size> operator/(const T inp) const
	{
		return (*this)*(1 / inp);
	}

	/* WARNING! Reference is returned! Use with extra-care! */
	MyVector<T, size>& operator/=(const T inp)
	{
		vector_mult_by_scalar_inplace<T, size>(coords, 1/inp);
		len = vector_length<T, size>(coords);
		return *this;
	}

	T dot(const MyVector<T, size>& inp)
	{
		return vector_dot_product<T, size>(coords, inp.data());
	}

	// template<size_t inp_size> MyVector<T, 3> cross(const MyVector<T, inp_size>& inp)
	// {
		// return Vec3<T>( vector_cross_product<T, size, inp_size> (coords, inp.data()) );
	// }
};

/*
 * NOTE: current class is for debug purpuses only, it's code is unrecommended in many ways and is only present here for
 * fast implementation and debugging of future normal implementation ...
 */
template<typename T> class Vec2_dbg
{
	T x;
	T y;
	T len;
	

public:
	typedef T data_type;
	
	/* basic interfaces ... */
	T X() const { return x; };
	T Y() const { return y; };
	T length() const { return len; }
#ifdef DBG_SIMPLE_VECTORS
	void TRACE() const
	{
		std::cout << "TRACING DBG VECTOR " << this <<": X=" << x << ", Y=" << y << ", LEN="
			<< len << std::endl;
	}
#endif

	/* constructors ... */
	Vec2_dbg(): x(0), y(0), len(0) {}
	Vec2_dbg(T ix, T iy): x(ix), y(iy)
	{
		len = static_cast<T>( sqrt( x*x + y*y) );
	}

	/* operators and operations ... */
	Vec2_dbg<T>& operator+(const Vec2_dbg<T> &inp) const
	{
		return *new Vec2_dbg<T>(this->x + inp.X(), this->y + inp.Y());
	}
	Vec2_dbg<T>& operator-() const
	{
		return *new Vec2_dbg<T>(-(this->x), -(this->y));
	}
	Vec2_dbg<T>& operator-(const Vec2_dbg<T> &inp) const
	{
		return *this + (-inp);
	}
	Vec2_dbg<T>& normalize()
	{
		x /= len;
		y /= len;
		len = 1.0f;
		return *this;
	}
	Vec2_dbg<T>& operator+=(const Vec2_dbg<T> &inp)
	{
		this->x += inp.X();
		this->y += inp.Y();
		this->len = sqrt(x*x + y*y);
		return *this;
	}
	Vec2_dbg<T>& operator-=(const Vec2_dbg<T> &inp)
	{
		return *this+=(-inp);
	}
	Vec2_dbg<T>& operator*(const T inp)
	{
		return *(new Vec2_dbg<T>(this->x * inp, this->y * inp));
	}
	Vec2_dbg<T>& operator/(const T inp)
	{
		return (*this) * (1/inp);
	}
	Vec2_dbg<T>& operator*=(const T inp)
	{
		this->x *= inp;
		this->y *= inp;
		this->len = sqrt(x*x + y*y);
		return *this;
	}
	Vec2_dbg<T>& operator/=(const T inp)
	{
		return *this*=(1/inp);
	}
	T dot(const Vec2_dbg<T> &inp)
	{
		return this->x * inp.X() + this->y * inp.Y();
	}
	T cross(const Vec2_dbg<T> &inp)
	{
		throw new std::runtime_error("ERROR: vector cross-product is undefined in two-dimensional space ...");
		return T(0);
	}
};

#endif


