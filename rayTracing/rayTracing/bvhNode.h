#pragma once

class CBVHNode : public IHitTable
{
private:
	CAABB m_box;

	IHitTable* m_left;
	IHitTable* m_right;

public:
	CBVHNode()
		: IHitTable( nullptr )
	{}
	CBVHNode( IHitTable** children, int const num, float t0, float t1 );

	virtual ~CBVHNode();
	virtual bool Hit( CRay const& ray, float const tMin, float const tMax, SHitInfo& outInfo ) const override;
	virtual bool BoundingBox( float const t0, float const t1, CAABB& box ) const override;
};
