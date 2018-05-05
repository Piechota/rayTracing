#include "math.h"
#include "ray.h"
#include "hitTable.h"
#include "rect.h"

bool CXYRect::Hit( CRay const& ray, float const tMin, float const tMax, SHitInfo& outInfo ) const
{
	float const t = ( m_z - ray.GetOrigin().z ) / ray.GetDirection().z;
	if ( t < tMin || tMax < t )
	{
		return false;
	}

	float const x = ray.GetOrigin().x + t * ray.GetDirection().x;
	float const y = ray.GetOrigin().y + t * ray.GetDirection().y;

	if ( x < m_x0 || m_x1 < x ||
		 y < m_y0 || m_y1 < y )
	{
		return false;
	}

	outInfo.m_uv.Set( ( x - m_x0 ) / ( m_x1 - m_x0 ), ( y - m_y0 ) / ( m_y1 - m_y0 ) );
	outInfo.m_t = t;
	outInfo.m_material = m_material;
	outInfo.m_position = ray.GetPoint( t );
	outInfo.m_normal = Vec3( 0.f, 0.f, 1.f );
	return true;
}

bool CXZRect::Hit( CRay const& ray, float const tMin, float const tMax, SHitInfo& outInfo ) const
{
	float const t = ( m_y - ray.GetOrigin().y ) / ray.GetDirection().y;
	if ( t < tMin || tMax < t )
	{
		return false;
	}

	float const x = ray.GetOrigin().x + t * ray.GetDirection().x;
	float const z = ray.GetOrigin().z + t * ray.GetDirection().z;

	if ( x < m_x0 || m_x1 < x ||
		 z < m_z0 || m_z1 < z )
	{
		return false;
	}

	outInfo.m_uv.Set( ( x - m_x0 ) / ( m_x1 - m_x0 ), ( z - m_z0 ) / ( m_z1 - m_z0 ) );
	outInfo.m_t = t;
	outInfo.m_material = m_material;
	outInfo.m_position = ray.GetPoint( t );
	outInfo.m_normal = Vec3( 0.f, 1.f, 0.f );
	return true;
}

bool CYZRect::Hit( CRay const& ray, float const tMin, float const tMax, SHitInfo& outInfo ) const
{
	float const t = ( m_x - ray.GetOrigin().x ) / ray.GetDirection().x;
	if ( t < tMin || tMax < t )
	{
		return false;
	}

	float const y = ray.GetOrigin().y + t * ray.GetDirection().y;
	float const z = ray.GetOrigin().z + t * ray.GetDirection().z;

	if (  y < m_y0 || m_y1 < y ||
		 z < m_z0 || m_z1 < z )
	{
		return false;
	}

	outInfo.m_uv.Set( ( y - m_y0 ) / ( m_y1 - m_y0 ), ( z - m_z0 ) / ( m_z1 - m_z0 ) );
	outInfo.m_t = t;
	outInfo.m_material = m_material;
	outInfo.m_position = ray.GetPoint( t );
	outInfo.m_normal = Vec3( 1.f, 0.f, 0.f );
	return true;
}
