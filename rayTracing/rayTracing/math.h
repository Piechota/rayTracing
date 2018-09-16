#pragma once
#define USE_BLUE_NOISE 1

#include <math.h>
#include <float.h>
#include <random>

namespace MathConsts
{
	float const PI = 3.14159265359f;
	float const DegToRad = PI / 180.f;
}

struct Vec2i
{
	union
	{
		struct
		{
			int x, y;
		};
		int data[2];
	};

	Vec2i()
		: x(0)
		, y(0)
	{}
	Vec2i(Vec2i const& v)
		: x(v.x)
		, y(v.y)
	{}
	Vec2i(int const x, int const y)
		: x(x)
		, y(y)
	{}
	void operator -=(Vec2i const& v)
	{
		x -= v.x;
		y -= v.y;
	}
};

struct Vec2
{
	union
	{
		struct
		{
			float x;
			float y;
		};
		float data[2];
	};

	Vec2(){}
	Vec2(float const x, float const y)
		: x(x)
		, y(y)
	{}
	Vec2 operator* (float const f) const
	{
		return Vec2(x * f, y * f);
	}
	Vec2 operator/ (float const f) const
	{
		return Vec2(x / f, y / f);
	}

	Vec2 operator+ (Vec2 const vec) const
	{
		return Vec2(x + vec.x, y + vec.y);
	}

	Vec2 operator- (Vec2 const vec) const
	{
		return Vec2(x - vec.x, y - vec.y);
	}
	void Set( float const _x, float const _y )
	{
		x = _x;
		y = _y;
	}
};

struct Vec3
{
	union 
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		float data[3];
	};

	Vec3()
		: x(0.f)
		, y(0.f)
		, z(0.f)
	{}
	Vec3(float const _x, float const _y, float const _z)
		: x(_x)
		, y(_y)
		, z(_z)
	{}

	Vec3 operator* (float const f) const
	{
		return Vec3(x * f, y * f, z * f);
	}
	Vec3 operator/ (float const f) const
	{
		return Vec3(x / f, y / f, z / f);
	}

	Vec3 operator* (Vec3 const v) const
	{
		return Vec3(x * v.x, y * v.y, z * v.z);
	}

	Vec3 operator-(Vec3 const& v1) const
	{
		return Vec3(x - v1.x, y - v1.y, z - v1.z);
	}
	Vec3 operator+(Vec3 const& v1) const
	{
		return Vec3(x + v1.x, y + v1.y, z + v1.z);
	}

	Vec3 operator+( float const& f ) const
	{
		return Vec3(x + f, y + f, z + f);
	}

	Vec3 operator-( float const& f ) const
	{
		return Vec3(x - f, y - f, z - f);
	}

	Vec3 operator-() const
	{
		return Vec3( -x, -y, -z );
	}

	void operator+=(Vec3 const v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}

	void operator/=(float const f)
	{
		x /= f;
		y /= f;
		z /= f;
	}

	void operator*=(float const f)
	{
		x *= f;
		y *= f;
		z *= f;
	}

	void Set( float const _x, float const _y, float const _z )
	{
		x = _x;
		y = _y;
		z = _z;
	}

	float Magnitude() const
	{
		return sqrtf( x * x + y * y + z * z );
	}

	float MagnitudeSq() const
	{
		return x * x + y * y + z * z;
	}

	Vec3 GetNormalized() const
	{
		float const magnitude = Magnitude();
		if ( 0.f < magnitude )
		{
			return *this / magnitude;
		}
		else
		{
			return Vec3( 0.f, 0.f, 0.f );
		}
	}

	static __forceinline float Dot(Vec3 const& v0, Vec3 const& v1)
	{
		return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
	}

	static __forceinline Vec3 Cross(Vec3 const& v0, Vec3 const& v1)
	{
		return Vec3
		(
			v0.y * v1.z - v0.z * v1.y,
			v0.z * v1.x - v0.x * v1.z,
			v0.x * v1.y - v0.y * v1.x
		);
	}
};

struct Vec4
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		float data[4];
	};
	Vec4() 
		: x(0.f)
		, y(0.f)
		, z(0.f)
		, w(0.f) 
	{}
	Vec4(float const _x, float const _y, float const _z, float const _w)
		: x(_x)
		, y(_y)
		, z(_z)
		, w(_w)
	{}
	Vec4(Vec3 v, float const _w)
		: x(v.x)
		, y(v.y)
		, z(v.z)
		, w(_w)
	{}

	void Set(float const _x, float const _y, float const _z, float const _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	void operator/=(float const f)
	{
		x /= f;
		y /= f;
		z /= f;
		w /= f;
	}

	Vec3 const& AsVec3() const
	{
		return *reinterpret_cast< Vec3 const* >( this );
	}

	Vec4 operator-( Vec4 const& v1 ) const
	{
		return Vec4( x - v1.x, y - v1.y, z - v1.z, w - v1.w );
	}

	Vec4 operator+( Vec4 const& v1 ) const
	{
		return Vec4( x + v1.x, y + v1.y, z + v1.z, w + v1.w );
	}

	Vec4 operator*( float const a ) const
	{
		return Vec4( x * a, y  * a, z  * a, w  * a );
	}

	static __forceinline float Dot(Vec4 const& v0, Vec4 const& v1)
	{
		return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
	}
	static __forceinline float Dot(Vec4 const& v0, float const* const v1)
	{
		return v0.x * v1[0] + v0.y * v1[1] + v0.z * v1[2] + v0.w * v1[3];
	}

	static __forceinline float Distance( Vec4 const& v0, Vec4 const& v1 )
	{
		Vec4 const v = v0 - v1;
		float const mag2 = Dot( v, v );
		return sqrtf( mag2 );
	}
};

struct Vec4i
{
	union
	{
		struct
		{
			int x;
			int y;
			int z;
			int w;
		};
		int data[4];
	};
	Vec4i()
		: x(0)
		, y(0)
		, z(0)
		, w(0)
	{}
	Vec4i(int const _x, int const _y, int const _z, int const _w)
		: x(_x)
		, y(_y)
		, z(_z)
		, w(_w)
	{}

	void Set(int const _x, int const _y, int const _z, int const _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
};

struct Matrix4x4
{
	union
	{
		struct
		{
			Vec4 x;
			Vec4 y;
			Vec4 z;
			Vec4 w;
		};
		struct
		{
			float a00, a01, a02, a03;
			float a10, a11, a12, a13;
			float a20, a21, a22, a23;
			float a30, a31, a32, a33;
		};
		float data[16];
	};

	Matrix4x4()
	{
		x.Set(1.f, 0.f, 0.f, 0.f);
		y.Set(0.f, 1.f, 0.f, 0.f);
		z.Set(0.f, 0.f, 1.f, 0.f);
		w.Set(0.f, 0.f, 0.f, 1.f);
	}

	Matrix4x4(
		float const _a00, float const _a01, float const _a02, float const _a03,
		float const _a10, float const _a11, float const _a12, float const _a13,
		float const _a20, float const _a21, float const _a22, float const _a23,
		float const _a30, float const _a31, float const _a32, float const _a33)
		: a00(_a00), a01(_a01), a02(_a02), a03(_a03)
		, a10(_a10), a11(_a11), a12(_a12), a13(_a13)
		, a20(_a20), a21(_a21), a22(_a22), a23(_a23)
		, a30(_a30), a31(_a31), a32(_a32), a33(_a33)
	{}

	void Projection(float const hfov, float const aspect, float const zNear, float const zFar)
	{
		float const scaleXY = 1.f / tanf(hfov * MathConsts::DegToRad);
		float const scaleZ = zFar / (zFar - zNear);

		a00 = scaleXY;
		a11 = aspect * scaleXY;
		a22 = scaleZ;
		a33 = 0;
		a32 = -zNear * scaleZ;
		a23 = 1.f;
	}

	static Vec4 Mul(Vec4 const& vector, Matrix4x4 const& matrix)
	{
		return Vec4
		(
			  vector.x * matrix.a00 + vector.y * matrix.a10 + vector.z * matrix.a20 + vector.w * matrix.a30
			, vector.x * matrix.a01 + vector.y * matrix.a11 + vector.z * matrix.a21 + vector.w * matrix.a31
			, vector.x * matrix.a02 + vector.y * matrix.a12 + vector.z * matrix.a22 + vector.w * matrix.a32
			, vector.x * matrix.a03 + vector.y * matrix.a13 + vector.z * matrix.a23 + vector.w * matrix.a33
		);
	}
	static Matrix4x4 Mul(Matrix4x4 const& matrixA, Matrix4x4 const& matrixB)
	{
		return Matrix4x4
		(
			matrixA.a00 * matrixB.a00 + matrixA.a01 * matrixB.a10 + matrixA.a02 * matrixB.a20 + matrixA.a03 * matrixB.a30,
			matrixA.a00 * matrixB.a01 + matrixA.a01 * matrixB.a11 + matrixA.a02 * matrixB.a21 + matrixA.a03 * matrixB.a31,
			matrixA.a00 * matrixB.a02 + matrixA.a01 * matrixB.a12 + matrixA.a02 * matrixB.a22 + matrixA.a03 * matrixB.a32,
			matrixA.a00 * matrixB.a03 + matrixA.a01 * matrixB.a13 + matrixA.a02 * matrixB.a23 + matrixA.a03 * matrixB.a33,

			matrixA.a10 * matrixB.a00 + matrixA.a11 * matrixB.a10 + matrixA.a12 * matrixB.a20 + matrixA.a13 * matrixB.a30,
			matrixA.a10 * matrixB.a01 + matrixA.a11 * matrixB.a11 + matrixA.a12 * matrixB.a21 + matrixA.a13 * matrixB.a31,
			matrixA.a10 * matrixB.a02 + matrixA.a11 * matrixB.a12 + matrixA.a12 * matrixB.a22 + matrixA.a13 * matrixB.a32,
			matrixA.a10 * matrixB.a03 + matrixA.a11 * matrixB.a13 + matrixA.a12 * matrixB.a23 + matrixA.a13 * matrixB.a33,

			matrixA.a20 * matrixB.a00 + matrixA.a21 * matrixB.a10 + matrixA.a22 * matrixB.a20 + matrixA.a23 * matrixB.a30,
			matrixA.a20 * matrixB.a01 + matrixA.a21 * matrixB.a11 + matrixA.a22 * matrixB.a21 + matrixA.a23 * matrixB.a31,
			matrixA.a20 * matrixB.a02 + matrixA.a21 * matrixB.a12 + matrixA.a22 * matrixB.a22 + matrixA.a23 * matrixB.a32,
			matrixA.a20 * matrixB.a03 + matrixA.a21 * matrixB.a13 + matrixA.a22 * matrixB.a23 + matrixA.a23 * matrixB.a33,

			matrixA.a30 * matrixB.a00 + matrixA.a31 * matrixB.a10 + matrixA.a32 * matrixB.a20 + matrixA.a33 * matrixB.a30,
			matrixA.a30 * matrixB.a01 + matrixA.a31 * matrixB.a11 + matrixA.a32 * matrixB.a21 + matrixA.a33 * matrixB.a31,
			matrixA.a30 * matrixB.a02 + matrixA.a31 * matrixB.a12 + matrixA.a32 * matrixB.a22 + matrixA.a33 * matrixB.a32,
			matrixA.a30 * matrixB.a03 + matrixA.a31 * matrixB.a13 + matrixA.a32 * matrixB.a23 + matrixA.a33 * matrixB.a33
		);
	}
};

struct Quaternion
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		float data[4];
	};
	Quaternion()
		: x(0.f)
		, y(0.f)
		, z(0.f)
		, w(0.f)
	{}
	Quaternion(float const _x, float const _y, float const _z, float const _w)
		: x(_x)
		, y(_y)
		, z(_z)
		, w(_w)
	{}
	void Set(float const _x, float const _y, float const _z, float const _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	Vec3 operator*(Vec3 const& vector) const
	{
		float const x2 = x * x;
		float const y2 = y * y;
		float const z2 = z * z;
		float const w2 = w * w;

		float const xz = x * z;
		float const yw = y * w;
		float const xy = x * y;
		float const zw = z * w;
		float const yz = y * z;
		float const xw = x * w;

		return Vec3
		(
			  vector.x * (+x2 - y2 - z2 + w2) + 2.f * (vector.y * (xy - zw) + vector.z * (xz + yw))
			, vector.y * (-x2 + y2 - z2 + w2) + 2.f * (vector.x * (xy + zw) + vector.z * (yz - xw))
			, vector.z * (-x2 - y2 + z2 + w2) + 2.f * (vector.x * (xz - yw) + vector.y * (yz + xw))
		);
	}

	Matrix4x4 ToMatrix4x4() const
	{
		float const x2 = x * x;
		float const y2 = y * y;
		float const z2 = z * z;
		float const w2 = w * w;

		float const xz = x * z;
		float const yw = y * w;
		float const xy = x * y;
		float const zw = z * w;
		float const yz = y * z;
		float const xw = x * w;

		return Matrix4x4
		(
			(+x2 - y2 - z2 + w2),	2.f * (xy + zw),		2.f * (xz - yw),		0.f,
			2.f * (xy - zw),		(-x2 + y2 - z2 + w2),	2.f * (yz + xw),		0.f,
			2.f * (xz + yw),		2.f * (yz - xw),		(-x2 - y2 + z2 + w2),	0.f,
			0.f,					0.f,					0.f,					1.f
		);
	}

	static Quaternion FromAngleAxis(float const angle, float const* axis)
	{
		float const halfAngle = 0.5f * angle;
		float const sinA = sinf(halfAngle);
		return Quaternion(axis[0] * sinA, axis[1] * sinA, axis[2] * sinA, cosf(halfAngle));
	}
};

class CRandomNumber
{
private:
	std::random_device m_device;
	std::mt19937 m_engine;
	std::uniform_real_distribution<> m_ditribution;

public:
	CRandomNumber();
	float Random();
};

namespace Math
{
	int WangHash(int index);
	Vec2 BlueNoise2D(int const index);
	Vec3 RandomInUnitSphere();
	Vec3 RandomInUnitDisk();
	Vec3 Reflect( Vec3 const v, Vec3 const n );
	bool Refract( Vec3 const v, Vec3 const n, float niOverNt, Vec3& outRefracted );
	float Schlick( float const cosine, float const refIdx );
	float Rand();
	Vec2 GetSphereUV( Vec3 const p );
}