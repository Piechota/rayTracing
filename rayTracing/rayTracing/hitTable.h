#pragma once

class IMaterial;

struct SHitInfo
{
	IMaterial* m_material;

	Vec3 m_position;
	Vec3 m_normal;
	float m_t;
};

class __declspec(novtable) IHitTable
{
protected:
	IMaterial* m_material;

public:
	IHitTable(IMaterial* material)
		: m_material( material )
	{}
	virtual ~IHitTable();

	virtual bool Hit( CRay const& ray, float const tMin, float const tMax, SHitInfo& outInfo ) const = 0;
};
