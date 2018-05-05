#pragma once

class CBox : public IHitTable
{
private:
	IHitTable* m_forDelete[ 3 ];
	IHitTable* m_box;
	Vec3 m_pMin;
	Vec3 m_pMax;

public:
	CBox()
		: IHitTable( nullptr )
	{}
	CBox( Vec3 const p0, Vec3 const p1, IMaterial* const material )
		: IHitTable( material )
		, m_pMin( p0 )
		, m_pMax( p1 )
	{
		IHitTable* planes[ 6 ];

		planes[ 0 ] = new CXYRect( p0.x, p1.x, p0.y, p1.y, p1.z, material );

		m_forDelete[ 0 ] = new CXYRect( p0.x, p1.x, p0.y, p1.y, p0.z, material );
		planes[ 1 ] = new CFlipNormals( m_forDelete[ 0 ] );

		planes[ 2 ] = new CXZRect( p0.x, p1.x, p0.z, p1.z, p1.y, material );

		m_forDelete[ 1 ] = new CXZRect( p0.x, p1.x, p0.z, p1.z, p0.y, material );
		planes[ 3 ] = new CFlipNormals( m_forDelete[ 1 ] );

		planes[ 4 ] = new CYZRect( p0.y, p1.y, p0.z, p1.z, p1.x, material );

		m_forDelete[ 2 ] = new CYZRect( p0.y, p1.y, p0.z, p1.z, p0.x, material );
		planes[ 5 ] = new CFlipNormals( m_forDelete[ 2 ] );

		m_box = new CHitTableList( planes, 6 );
	}

	virtual ~CBox() override
	{
		delete m_box;
		for ( unsigned int i = 0; i < ARRAYSIZE(m_forDelete); ++i )
		{
			delete m_forDelete[ i ];
		}
	}

	virtual bool Hit( CRay const& ray, float const tMin, float const tMax, SHitInfo& outInfo ) const override
	{
		return m_box->Hit( ray, tMin, tMax, outInfo );
	}
	virtual bool BoundingBox( float const t0, float const t1, CAABB& box ) const override
	{
		box.Set( m_pMin, m_pMax );
		return true;
	}
};
