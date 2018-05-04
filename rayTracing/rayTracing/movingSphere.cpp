#include "math.h"
#include "ray.h"
#include "hitTable.h"
#include "movingSphere.h"

bool CMovingSphere::Hit( CRay const & ray, float const tMin, float const tMax, SHitInfo & outInfo ) const
{
	Vec3 const center = GetCenter( ray.GetTime() );
	Vec3 const sphereOrigin = ray.GetOrigin() - center;
	float const a = Vec3::Dot( ray.GetDirection(), ray.GetDirection() );
	float const b = Vec3::Dot( sphereOrigin, ray.GetDirection() );
	float const c = Vec3::Dot( sphereOrigin, sphereOrigin ) - m_radius * m_radius;

	float const discriminant = b * b - a * c;
	if ( 0.f < discriminant )
	{
		float root = ( -b - sqrtf( b * b - a * c ) ) / a;
		if ( tMin < root && root < tMax )
		{
			outInfo.m_material = m_material;
			outInfo.m_position = ray.GetPoint( root );
			outInfo.m_normal = ( outInfo.m_position - center ) / m_radius;
			outInfo.m_t = root;
			return true;
		}

		root = ( -b + sqrtf( b * b - a * c ) ) / a;
		if ( tMin < root && root < tMax )
		{
			outInfo.m_material = m_material;
			outInfo.m_position = ray.GetPoint( root );
			outInfo.m_normal = ( outInfo.m_position - center ) / m_radius;
			outInfo.m_t = root;
			return true;
		}
	}

	return false;
}

bool CMovingSphere::BoundingBox( float const t0, float const t1, CAABB & box ) const
{
	Vec3 const center0 = GetCenter( t0 );
	Vec3 const center1 = GetCenter( t1 );
	CAABB const a( center0 - Vec3( m_radius, m_radius, m_radius ), center0 + Vec3( m_radius, m_radius, m_radius ) );
	CAABB const b( center1 - Vec3( m_radius, m_radius, m_radius ), center1 + Vec3( m_radius, m_radius, m_radius ) );
	box.Combine( a, b );

	return true;
}
