#pragma once

class CPerlin
{
private:
	TArray< Vec3 > m_randomVectors;
	TArray< float > m_randomFloats;
	TArray< int > m_permutationX;
	TArray< int > m_permutationY;
	TArray< int > m_permutationZ;

	void PerlinGenerate( unsigned int const num )
	{
		m_randomFloats.Resize( num );
		m_randomVectors.Resize( num );
		for ( unsigned int i = 0; i < num; ++i )
		{
			m_randomFloats[ i ] = Math::Rand();
			m_randomVectors[ i ] = Math::RandomInUnitSphere();
		}
	}

	void Permute( TArray< int >& p )
	{
		for ( int i = p.Size() - 1; 0 < i; --i )
		{
			int const target = int( Math::Rand() * ( i + 1 ) );
			int const tmp = p[ i ];
			p[ i ] = p[ target ];
			p[ target ] = tmp;
		}
	}

	void PerlinGeneratePermute( TArray< int >& p, unsigned int const num )
	{
		p.Resize( num );
		for ( unsigned int i = 0; i < num; ++i )
		{
			p[ i ] = i;
		}

		Permute( p );
	}

	float TrilinearInterpolation( float const c[ 2 ][ 2 ][ 2 ], float const u, float const v, float const w ) const
	{
		float accum = 0.f;
		for ( float i = 0.f; i < 2.f; ++i )
		{
			for ( float j = 0.f; j < 2.f; ++j )
			{
				for ( float k = 0.f; k < 2.f; ++k )
				{
					accum +=	( i * u + (1.f - i) * (1.f - u ) ) *
								( j * v + (1.f - j) * (1.f - v ) ) *
								( k * w + (1.f - k) * (1.f - w ) ) *
								c[int(i)][int(j)][int(k)];
				}
			}
		}

		return accum;
	}

	float TrilinearInterpolation( Vec3 const c[ 2 ][ 2 ][ 2 ], float const u, float const v, float const w ) const
	{
		float uu = u * u * ( 3.f - 2.f * u );
		float vv = v * v * ( 3.f - 2.f * v );
		float ww = w * w * ( 3.f - 2.f * w );

		float accum = 0.f;
		for ( float i = 0.f; i < 2.f; ++i )
		{
			for ( float j = 0.f; j < 2.f; ++j )
			{
				for ( float k = 0.f; k < 2.f; ++k )
				{
					Vec3 const weight( u - i, v - j, w - k );
					accum +=	( i * uu + (1.f - i) * (1.f - uu ) ) *
								( j * vv + (1.f - j) * (1.f - vv ) ) *
								( k * ww + (1.f - k) * (1.f - ww ) ) *
								(Vec3::Dot( c[int(i)][int(j)][int(k)], weight ));
				}
			}
		}

		return accum;
	}

public:
	CPerlin()
	{
		PerlinGenerate( 256 );
		PerlinGeneratePermute( m_permutationX, 256 );
		PerlinGeneratePermute( m_permutationY, 256 );
		PerlinGeneratePermute( m_permutationZ, 256 );
	}

	float Noise( Vec3 const p ) const
	{
		int const i = int( 4.f * p.x ) & 255;
		int const j = int( 4.f * p.y ) & 255;
		int const k = int( 4.f * p.z ) & 255;
		return m_randomFloats[ m_permutationX[ i ] ^ m_permutationY[ j ] ^ m_permutationZ[ k ] ];
	}

	float NoiseLinear( Vec3 const p ) const
	{
		float u = p.x - floorf( p.x );
		float v = p.y - floorf( p.y );
		float w = p.z - floorf( p.z );
		u = u * u * ( 3.f - 2.f * u );
		v = v * v * ( 3.f - 2.f * v );
		w = w * w * ( 3.f - 2.f * w );

		int i = int( floorf( p.x ) );
		int j = int( floorf( p.y ) );
		int k = int( floorf( p.z ) );

		float c[ 2 ][ 2 ][ 2 ];
		for ( unsigned int di = 0; di < 2; ++di )
		{
			for ( unsigned int dj = 0; dj < 2; ++dj )
			{
				for ( unsigned int dk = 0; dk < 2; ++dk )
				{
					c[ di ][ dj ][ dk ] = m_randomFloats[ m_permutationX[ ( i + di ) & 255 ] ^ m_permutationY[ ( j + dj ) & 255 ] ^ m_permutationZ[ ( k + dk ) & 255 ] ];
				}
			}
		}
		return TrilinearInterpolation( c, u, v, w );
	}

	float NoiseLinearVec( Vec3 const p ) const
	{
		float u = p.x - floorf( p.x );
		float v = p.y - floorf( p.y );
		float w = p.z - floorf( p.z );

		int i = int( floorf( p.x ) );
		int j = int( floorf( p.y ) );
		int k = int( floorf( p.z ) );

		Vec3 c[ 2 ][ 2 ][ 2 ];
		for ( unsigned int di = 0; di < 2; ++di )
		{
			for ( unsigned int dj = 0; dj < 2; ++dj )
			{
				for ( unsigned int dk = 0; dk < 2; ++dk )
				{
					c[ di ][ dj ][ dk ] = m_randomVectors[ m_permutationX[ ( i + di ) & 255 ] ^ m_permutationY[ ( j + dj ) & 255 ] ^ m_permutationZ[ ( k + dk ) & 255 ] ];
				}
			}
		}
		return TrilinearInterpolation( c, u, v, w );
	}

	float TurboVec( Vec3 const p, unsigned int const depth = 7 ) const
	{
		float accum = 0.f;
		Vec3 tmp = p;
		float weight = 1.f;
		for ( unsigned int i = 0; i < depth; ++i )
		{
			accum += weight * NoiseLinearVec( tmp );
			weight *= 0.5f;
			tmp *= 2.f;
		}

		return fabsf( accum );
	}
};
