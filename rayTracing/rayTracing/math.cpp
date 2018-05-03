#include <stdlib.h>     
#include "math.h"

Vec3 Math::RandomInUnitSphere()
{
	Vec3 p;

	do
	{
		p.x = float( rand() ) / float( RAND_MAX );
		p.y = float( rand() ) / float( RAND_MAX );
		p.z = float( rand() ) / float( RAND_MAX );
	} while ( 1.f <= p.MagnitudeSq() );

	p = p * 2.f - 1.f;

	return p;
}

Vec3 Math::RandomInUnitDisk()
{
	Vec3 p;
	p.z = 0.f;
	do
	{
		p.x = float( rand() ) / float( RAND_MAX );
		p.y = float( rand() ) / float( RAND_MAX );
	} while ( 1.f <= p.MagnitudeSq() );

	p = p * 2.f - 1.f;

	return p;
}

Vec3 Math::Reflect( Vec3 const v, Vec3 const n )
{
	return v - n * 2.f * Vec3::Dot( v, n );
}

bool Math::Refract( Vec3 const v, Vec3 const n, float niOverNt, Vec3 & outRefracted )
{
	Vec3 const uv = v.GetNormalized();
	float const dt = Vec3::Dot( uv, n );
	float const discriminant = 1.f - niOverNt * niOverNt * ( 1.f - dt * dt );
	if ( 0.f < discriminant )
	{
		outRefracted = ( uv - n *dt ) * niOverNt - n * sqrtf( discriminant );
		return true;
	}

	return false;
}

float Math::Schlick( float const cosine, float const refIdx )
{
	float r0 = ( 1.f - refIdx ) / ( 1.f + refIdx );
	r0 = r0 * r0;
	return r0 + ( 1.f - r0 ) * powf( 1.f - cosine, 5.f );
}

float Math::Rand()
{
	return float( rand() ) / float( RAND_MAX );
}
