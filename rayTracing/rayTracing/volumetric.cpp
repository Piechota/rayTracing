#include "math.h"
#include "ray.h"
#include "hitTable.h"
#include "volumetric.h"

bool CConstantMedium::Hit( CRay const & ray, float const tMin, float const tMax, SHitInfo & outInfo ) const
{
	SHitInfo info0, info1;
	if ( m_boundary->Hit( ray, -FLT_MAX, FLT_MAX, info0 ) )
	{
		if ( m_boundary->Hit( ray, info0.m_t + 0.0001f, FLT_MAX, info1 ) )
		{
			if ( info0.m_t < tMin )
			{
				info0.m_t = tMin;
			}

			if ( tMax < info1.m_t )
			{
				info1.m_t = tMax;
			}

			if ( info1.m_t <= info0.m_t )
			{
				return false;
			}

			if ( info0.m_t < 0.f )
			{
				info0.m_t = 0.f;
			}

			float const distanceInsideBoundary = ( info1.m_t - info0.m_t ) * ray.GetDirection().Magnitude();
			float const hitDistance = -( 1.f / m_density ) * logf( Math::Rand() );
			if ( hitDistance < distanceInsideBoundary )
			{
				outInfo.m_t = info0.m_t + hitDistance / ray.GetDirection().Magnitude();
				outInfo.m_position = ray.GetPoint( outInfo.m_t );
				outInfo.m_normal.Set( 1.f, 0.f, 0.f );
				outInfo.m_material = m_material;
				return true;
			}
		}
	}

	return false;
}
