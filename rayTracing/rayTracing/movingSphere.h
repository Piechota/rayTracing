#pragma once

class CMovingSphere : public IHitTable
{
private:
	Vec3 m_center0;
	Vec3 m_center1;
	float m_time0;
	float m_time1;

	float m_radius;

	Vec3 GetCenter( float const time ) const
	{
		return m_center0 + ( m_center1 - m_center0 ) * (( time - m_time0 ) / ( m_time1 - m_time0 ));
	}

public:
	CMovingSphere()
		: IHitTable( nullptr )
	{}
	CMovingSphere( Vec3 const center0, Vec3 const center1, float const time0, float const time1, float const radius, IMaterial* const material )
		: IHitTable( material )
		, m_center0( center0 )
		, m_center1( center1 )
		, m_time0( time0 )
		, m_time1( time1 )
		, m_radius( radius )
	{}

	virtual bool Hit( CRay const& ray, float const tMin, float const tMax, SHitInfo& outInfo ) const override;
	virtual bool BoundingBox( float const t0, float const t1, CAABB& box ) const override;
};
