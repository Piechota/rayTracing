#pragma once

class CXYRect : public IHitTable
{
private:
	float m_x0;
	float m_x1;
	float m_y0;
	float m_y1;
	float m_z;

public:
	CXYRect()
		: IHitTable( nullptr )
	{}
	CXYRect( float const x0, float const x1, float const y0, float const y1, float const z, IMaterial* material )
		: IHitTable( material )
		, m_x0( x0 )
		, m_x1( x1 )
		, m_y0( y0 )
		, m_y1( y1 )
		, m_z( z )
	{}

	virtual bool Hit( CRay const& ray, float const tMin, float const tMax, SHitInfo& outInfo ) const override;
	virtual bool BoundingBox( float const t0, float const t1, CAABB& box ) const override
	{
		box.Set( Vec3( m_x0, m_y0, m_z - 0.0001f ), Vec3( m_x1, m_y1, m_z + 0.0001f ) );
		return true;
	}
};

class CXZRect : public IHitTable
{
private:
	float m_x0;
	float m_x1;
	float m_z0;
	float m_z1;
	float m_y;

public:
	CXZRect()
		: IHitTable( nullptr )
	{}
	CXZRect( float const x0, float const x1, float const z0, float const z1, float const y, IMaterial* material )
		: IHitTable( material )
		, m_x0( x0 )
		, m_x1( x1 )
		, m_z0( z0 )
		, m_z1( z1 )
		, m_y( y )
	{}

	virtual bool Hit( CRay const& ray, float const tMin, float const tMax, SHitInfo& outInfo ) const override;
	virtual bool BoundingBox( float const t0, float const t1, CAABB& box ) const override
	{
		box.Set( Vec3( m_x0, m_y - 0.0001f, m_z0 ), Vec3( m_x1, m_y + 0.0001f, m_z1 ) );
		return true;
	}
};

class CYZRect : public IHitTable
{
private:
	float m_y0;
	float m_y1;
	float m_z0;
	float m_z1;
	float m_x;

public:
	CYZRect()
		: IHitTable( nullptr )
	{}
	CYZRect( float const y0, float const y1, float const z0, float const z1, float const x, IMaterial* material )
		: IHitTable( material )
		, m_y0( y0 )
		, m_y1( y1 )
		, m_z0( z0 )
		, m_z1( z1 )
		, m_x( x )
	{}

	virtual bool Hit( CRay const& ray, float const tMin, float const tMax, SHitInfo& outInfo ) const override;
	virtual bool BoundingBox( float const t0, float const t1, CAABB& box ) const override
	{
		box.Set( Vec3( m_x - 0.0001f, m_y0, m_z0 ), Vec3( m_x + 0.0001f, m_y1, m_z1 ) );
		return true;
	}
};