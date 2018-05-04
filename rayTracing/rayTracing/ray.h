#pragma once

class CRay
{
private:
	Vec3 m_origin;
	Vec3 m_direction;
	float m_time;

public:
	CRay() {}
	CRay( Vec3 const origin, Vec3 const direction, float const time = 0.f )
		: m_origin( origin )
		, m_direction( direction )
		, m_time( time )
	{}

	Vec3 GetOrigin() const { return m_origin; }
	Vec3 GetDirection() const { return m_direction; }
	float GetTime() const { return m_time; }
	Vec3 GetPoint( float const t ) const { return m_origin + m_direction * t; }
};
