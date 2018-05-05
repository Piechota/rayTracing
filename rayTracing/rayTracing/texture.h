#pragma once

class __declspec( novtable ) ITexture
{
public:
	virtual ~ITexture() {}
	virtual Vec3 GetValue( Vec3 const p, Vec2 const uv ) const = 0;
};

class CConstantTexture : public ITexture
{
private:
	Vec3 m_color;

public:
	CConstantTexture() {}
	CConstantTexture( Vec3 const color )
		: m_color( color )
	{}
	virtual Vec3 GetValue( Vec3 const p, Vec2 const uv ) const override
	{
		return m_color;
	}
};

class CCheckerTexture : public ITexture
{
private:
	ITexture* m_odd;
	ITexture* m_even;

public:
	CCheckerTexture()
		: m_odd( nullptr )
		, m_even( nullptr )
	{}
	CCheckerTexture( ITexture* const odd, ITexture* const even )
		: m_odd( odd )
		, m_even( even )
	{}

	virtual Vec3 GetValue( Vec3 const p, Vec2 const uv ) const override
	{
		float const sines = sinf( 10.f * p.x ) * sinf( 10.f * p.y ) * sinf( 10.f * p.z );
		if ( sines < 0.f )
		{
			return m_odd->GetValue( p, uv );
		}
		else
		{
			return m_even->GetValue( p, uv );
		}
	}
};

class CNoiseTexture : public ITexture
{
private:
	CPerlin m_perlin;
	float m_scale;

public:
	CNoiseTexture() 
		: m_scale( 1.f )
	{}
	CNoiseTexture( float const scale ) 
		: m_scale( scale )
	{}

	virtual Vec3 GetValue( Vec3 const p, Vec2 const uv ) const override
	{
		//return Vec3( 1.f, 1.f, 1.f ) * m_perlin.TurboVec( p * m_scale );
		return Vec3( 1.f, 1.f, 1.f ) * 0.5f * ( 1.f + sinf( m_scale * p.z + 10.f * m_perlin.TurboVec( p ) ) );
	}
};

class CImageTexture : public ITexture	
{
private:
	Vec3* m_imageData;
	int m_width;
	int m_height;

public:
	CImageTexture() 
		: m_imageData( nullptr )
	{}
	CImageTexture( Vec3* imageData, int width, int height ) 
		: m_imageData( imageData )
		, m_width( width )
		, m_height( height )
	{}

	virtual Vec3 GetValue( Vec3 const p, Vec2 const uv ) const override
	{
		int x = int( uv.x * float( m_width ) );
		int y = int( ( 1.f - uv.y ) * float( m_height ) );
		if ( x < 0 ) x = 0;
		if ( y < 0 ) y = 0;
		if ( m_width - 1 < x ) x = m_width - 1;
		if ( m_height - 1 < y ) y = m_height - 1;

		return m_imageData[ y * m_width + x ];
	}
};