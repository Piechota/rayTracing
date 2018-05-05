#pragma once

class CFlipNormals : public IHitTable
{
private:
	IHitTable* m_main;

public:
	CFlipNormals()
		: IHitTable( nullptr )
		, m_main( nullptr )
	{}
	CFlipNormals( IHitTable* main )
		: IHitTable( nullptr )
		, m_main( main )
	{}

	virtual bool Hit( CRay const& ray, float const tMin, float const tMax, SHitInfo& outInfo ) const override
	{
		bool const hit = m_main->Hit( ray, tMin, tMax, outInfo );
		outInfo.m_normal = -outInfo.m_normal;
		return hit;
	}
	virtual bool BoundingBox( float const t0, float const t1, CAABB& box ) const override
	{
		return m_main->BoundingBox( t0, t1, box );
	}
};

class CTranslate : public IHitTable
{
private:
	Vec3 m_offset;
	IHitTable* m_main;

public:
	CTranslate()
		: IHitTable( nullptr )
		, m_main( nullptr )
	{}
	CTranslate( Vec3 const offset, IHitTable* main )
		: IHitTable( nullptr )
		, m_offset( offset )
		, m_main( main )
	{}

	virtual bool Hit( CRay const& ray, float const tMin, float const tMax, SHitInfo& outInfo ) const override
	{
		CRay const rayMoved( ray.GetOrigin() - m_offset, ray.GetDirection(), ray.GetTime() );
		bool const hit = m_main->Hit( rayMoved, tMin, tMax, outInfo );
		outInfo.m_position += m_offset;
		return hit;
	}
	virtual bool BoundingBox( float const t0, float const t1, CAABB& box ) const override
	{
		bool const boundingBox = m_main->BoundingBox( t0, t1, box );
		box.Set( box.GetMin() + m_offset, box.GetMax() + m_offset );

		return boundingBox;
	}
};

class CRotateY : public IHitTable
{
private:
	CAABB m_box;
	bool m_hasBox;

	IHitTable* m_main;
	float m_sinTheta;
	float m_cosTheta;

public:
	CRotateY()
		: IHitTable( nullptr )
		, m_main( nullptr )
	{}
	CRotateY( IHitTable* main, float const angle )
		: IHitTable( nullptr )
		, m_main( main )
	{
		float const radius = angle * MathConsts::DegToRad;
		m_sinTheta = sinf( radius );
		m_cosTheta = cosf( radius );

		m_hasBox = main->BoundingBox( 0.f, 1.f, m_box );
		Vec3 minValue( FLT_MAX, FLT_MAX, FLT_MAX );
		Vec3 maxValue( -FLT_MAX, -FLT_MAX, -FLT_MAX );
		for ( float i = 0; i < 2.f; ++i )
		{
			for ( float j = 0; j < 2.f; ++j )
			{
				for ( float k = 0; k < 2.f; ++k )
				{
					float const x = i * m_box.GetMax().x + ( 1.f - i ) * m_box.GetMin().x;
					float const y = i * m_box.GetMax().y + ( 1.f - i ) * m_box.GetMin().y;
					float const z = i * m_box.GetMax().z + ( 1.f - i ) * m_box.GetMin().z;
					float const newX = m_cosTheta * x + m_sinTheta * z;
					float const newZ = -m_sinTheta * x + m_cosTheta * z;
					Vec3 const tester( newX, y, newZ );
					for ( unsigned int c = 0; c < 3; ++c )
					{
						if ( maxValue.data[ c ] < tester.data[ c ] )
						{
							maxValue.data[ c ] = tester.data[ c ];
						}

						if ( tester.data[ c ] < minValue.data[ c ] )
						{
							minValue.data[ c ] = tester.data[ c ];
						}
					}
				}
			}
		}

		m_box.Set( minValue, maxValue );
	}

	virtual bool Hit( CRay const& ray, float const tMin, float const tMax, SHitInfo& outInfo ) const override
	{
		Vec3 origin = ray.GetOrigin();
		Vec3 direction = ray.GetDirection();
		origin.x = m_cosTheta * ray.GetOrigin().x - m_sinTheta * ray.GetOrigin().z;
		origin.z = m_sinTheta * ray.GetOrigin().x + m_cosTheta * ray.GetOrigin().z;

		direction.x = m_cosTheta * ray.GetDirection().x - m_sinTheta * ray.GetDirection().z;
		direction.z = m_sinTheta * ray.GetDirection().x + m_cosTheta * ray.GetDirection().z;

		CRay const rayRotated( origin, direction, ray.GetTime() );
		if ( m_main->Hit( rayRotated, tMin, tMax, outInfo ) )
		{
			Vec3 position = outInfo.m_position;
			Vec3 normal = outInfo.m_normal;

			position.x = m_cosTheta * outInfo.m_position.x + m_sinTheta * outInfo.m_position.z;
			position.z = -m_sinTheta * outInfo.m_position.x + m_cosTheta * outInfo.m_position.z;

			normal.x = m_cosTheta * outInfo.m_normal.x + m_sinTheta * outInfo.m_normal.z;
			normal.z = -m_sinTheta * outInfo.m_normal.x + m_cosTheta * outInfo.m_normal.z;

			outInfo.m_position = position;
			outInfo.m_normal = normal;
			return true;
		}
		return false;
	}
	virtual bool BoundingBox( float const t0, float const t1, CAABB& box ) const override
	{
		box = m_box;
		return m_hasBox;
	}
};
