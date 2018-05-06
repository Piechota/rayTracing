#include <stdlib.h>    
#include "math.h"


CRandomNumber::CRandomNumber()
	: m_engine( m_device() )
	, m_ditribution( 0., 1. )
{}

float CRandomNumber::Random()
{
	return float( m_ditribution( m_engine ) );
}

CRandomNumber GRandomNumber;

Vec3 Math::RandomInUnitSphere()
{
	Vec3 p;

	do
	{
		p.x = GRandomNumber.Random() * 2.f - 1.f;
		p.y = GRandomNumber.Random() * 2.f - 1.f;
		p.z = GRandomNumber.Random() * 2.f - 1.f;
	} while ( 1.f <= p.MagnitudeSq() );

	return p;
}

Vec3 Math::RandomInUnitDisk()
{
	Vec3 p;
	p.z = 0.f;
	do
	{
		p.x = GRandomNumber.Random() * 2.f - 1.f;
		p.y = GRandomNumber.Random() * 2.f - 1.f;
	} while ( 1.f <= p.MagnitudeSq() );

	return p;
}

Vec3 Math::Reflect( Vec3 const v, Vec3 const n )
{
	return v - n * 2.f * Vec3::Dot( v, n );
}

bool Math::Refract( Vec3 const v, Vec3 const n, float niOverNt, Vec3& outRefracted )
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
	return GRandomNumber.Random();
}

Vec2 Math::GetSphereUV( Vec3 const p )
{

	float const phi = atan2f( p.y, p.x );
	float const theta = asinf( p.z );

	return Vec2( 1.f - ( phi + MathConsts::PI ) / ( 2.f * MathConsts::PI ), ( theta + MathConsts::PI * 0.5f ) / MathConsts::PI );
}
