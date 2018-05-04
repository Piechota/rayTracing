#pragma once

class CCamera
{
private:
	Vec3 m_origin;
	Vec3 m_bottomLeftCorner;
	Vec3 m_horizontal;
	Vec3 m_vertical;

	Vec3 m_w;
	Vec3 m_u;
	Vec3 m_v;

	float m_time0;
	float m_time1;
	float m_lensRadius;

public:
	CCamera( Vec3 const lookFrom, Vec3 const lookAt, Vec3 const up, unsigned int const width, unsigned int const height, float const vfov, float const aperture, float const focusDistance, float const time0, float const time1 )
	{
		float const aspect = float( width ) / float( height );

		m_time0 = time0;
		m_time1 = time1;

		m_lensRadius = aperture * 0.5f;

		float const theta = vfov * MathConsts::DegToRad;
		float const halfHeight = focusDistance * tanf( theta * 0.5f );
		float const halfWidth = aspect * halfHeight;

		m_origin = lookFrom;
		m_w = ( lookFrom - lookAt ).GetNormalized();
		m_u = Vec3::Cross( up, m_w ).GetNormalized();
		m_v = Vec3::Cross( m_w, m_u );

		m_bottomLeftCorner = m_origin - m_u * halfWidth - m_v * halfHeight - m_w * focusDistance;
		m_horizontal = m_u * 2.f * halfWidth;
		m_vertical = m_v * 2.f * halfHeight;
	}

	CRay GetRay(float const s, float const t ) const
	{
		Vec3 const randomInDisk = Math::RandomInUnitDisk() * m_lensRadius;
		Vec3 const offset = m_u * randomInDisk.x + m_v * randomInDisk.y;
		float const time = m_time0 + Math::Rand() * ( m_time1 - m_time0 );

		return CRay( m_origin + offset, m_bottomLeftCorner + m_horizontal * s + m_vertical * t - m_origin - offset, time );
	}
};
