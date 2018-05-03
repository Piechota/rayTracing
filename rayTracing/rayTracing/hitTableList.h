#pragma once

class CHitTableList : public IHitTable
{
private:
	TArray< IHitTable* > m_list;

public:
	CHitTableList()
		: IHitTable( nullptr )
	{}

	void AddHitTable( IHitTable* const hitTable );
	void Clear();
	// Inherited via IHitTable
	virtual bool Hit( CRay const & ray, float const tMin, float const tMax, SHitInfo & outInfo ) const override;
};