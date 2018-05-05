#pragma once

class CSphere : public IHitTable
{
private:
	Vec3 m_center;
	float m_radius;

public:
	CSphere()
		: IHitTable( nullptr )
	{}
	CSphere( Vec3 const center, float const radius, IMaterial* const material )
		: IHitTable( material )
		, m_center( center )
		, m_radius( radius )
	{}

	virtual bool Hit( CRay const& ray, float const tMin, float const tMax, SHitInfo& outInfo ) const override;
	virtual bool BoundingBox( float const t0, float const t1, CAABB& box ) const override;
};
