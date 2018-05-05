#pragma once

class CHitTableList : public IHitTable
{
private:
	TArray< IHitTable* > m_list;

public:
	CHitTableList()
		: IHitTable( nullptr )
	{}

	CHitTableList( IHitTable** objects, unsigned int const num )
		: IHitTable( nullptr )
	{
		m_list.Resize( num );
		for ( unsigned int i = 0; i < num; ++i )
		{
			m_list[ i ] = objects[ i ];
		}
	}

	virtual ~CHitTableList() override;

	void AddHitTable( IHitTable* const hitTable );
	TArray< IHitTable* >& GetList() { return m_list; }
	// Inherited via IHitTable
	virtual bool Hit( CRay const& ray, float const tMin, float const tMax, SHitInfo& outInfo ) const override;
	virtual bool BoundingBox( float const t0, float const t1, CAABB& box ) const override;
};