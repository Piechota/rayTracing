#pragma once

class CConstantMedium : public IHitTable
{
private:
	IHitTable* m_boundary;
	float m_density;

public:
	CConstantMedium()
		: IHitTable( nullptr )
	{}
	CConstantMedium( IHitTable* boundary, float const density, IMaterial* const material )
		: IHitTable( material )
		, m_boundary( boundary )
		, m_density( density )
	{}

	virtual bool Hit( CRay const& ray, float const tMin, float const tMax, SHitInfo& outInfo ) const override;
	virtual bool BoundingBox( float const t0, float const t1, CAABB& box ) const override
	{
		return m_boundary->BoundingBox( t0, t1, box );
	}
};
