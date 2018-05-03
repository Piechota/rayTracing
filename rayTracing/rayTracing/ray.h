#pragma once

class CRay
{
private:
	Vec3 m_origin;
	Vec3 m_direction;

public:
	CRay() {}
	CRay( Vec3 const origin, Vec3 const direction )
		: m_origin( origin )
		, m_direction( direction )
	{}

	Vec3 GetOrigin() const { return m_origin; }
	Vec3 GetDirection() const { return m_direction; }
	Vec3 GetPoint( float const t ) const { return m_origin + m_direction * t; }
};
