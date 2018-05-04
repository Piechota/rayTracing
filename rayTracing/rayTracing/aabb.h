#pragma once

class CAABB
{
private:
	Vec3 m_min;
	Vec3 m_max;

public:
	CAABB()	{}
	CAABB( Vec3 const min, Vec3 const max )
		: m_min( min )
		, m_max( max )
	{}

	Vec3 GetMin() const { return m_min; }
	Vec3 GetMax() const { return m_max; }

	void Set( Vec3 const min, Vec3 const max )
	{
		m_min = min;
		m_max = max;
	}

	bool Hit( CRay const& ray, float tmin, float tmax ) const
	{
		for ( unsigned int i = 0; i < 3; ++i )
		{
			float const a = ( m_min.data[ i ] - ray.GetOrigin().data[ i ] ) / ray.GetDirection().data[ i ];
			float const b = ( m_max.data[ i ] - ray.GetOrigin().data[ i ] ) / ray.GetDirection().data[ i ];

			float const t0 = a < b ? a : b;
			float const t1 = a < b ? b : a;

			tmin = t0 < tmin ? tmin : t0;
			tmax = t1 < tmax ? t1 : tmax;

			if ( tmax <= tmin )
			{
				return false;
			}
		}

		return true;
	}

	void Combine( CAABB const& a, CAABB const& b )
	{
		m_min.Set( fminf( a.m_min.x, b.m_min.x ), fminf( a.m_min.y, b.m_min.y ), fminf( a.m_min.z, b.m_min.z ) );
		m_max.Set( fmaxf( a.m_max.x, b.m_max.x ), fmaxf( a.m_max.y, b.m_max.y ), fmaxf( a.m_max.z, b.m_max.z ) );
	}
};
