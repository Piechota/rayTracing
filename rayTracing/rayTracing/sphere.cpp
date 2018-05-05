#include "math.h"
#include "ray.h"
#include "hitTable.h"
#include "sphere.h"

bool CSphere::Hit( CRay const& ray, float const tMin, float const tMax, SHitInfo& outInfo ) const
{
	Vec3 const sphereOrigin = ray.GetOrigin() - m_center;
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
			outInfo.m_normal = ( outInfo.m_position - m_center ) / m_radius;
			outInfo.m_uv = Math::GetSphereUV( outInfo.m_normal );
			outInfo.m_t = root;
			return true;
		}

		root = ( -b + sqrtf( b * b - a * c ) ) / a;
		if ( tMin < root && root < tMax )
		{
			outInfo.m_material = m_material;
			outInfo.m_position = ray.GetPoint( root );
			outInfo.m_normal = ( outInfo.m_position - m_center ) / m_radius;
			outInfo.m_uv = Math::GetSphereUV( outInfo.m_normal );
			outInfo.m_t = root;
			return true;
		}
	}

	return false;
}

bool CSphere::BoundingBox( float const t0, float const t1, CAABB& box ) const
{
	box.Set( m_center - Vec3( m_radius, m_radius, m_radius ), m_center + Vec3( m_radius, m_radius, m_radius ) );
	return true;
}
