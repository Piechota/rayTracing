#pragma once
#include "perlin.h"
#include "texture.h"

class __declspec( novtable ) IMaterial
{
public:
	virtual bool Scatter( CRay const& ray, SHitInfo const& hitInfo, Vec3& outAttenuation, CRay& outRayScattered ) const = 0;
	virtual Vec3 Emitted( Vec3 const p, Vec2 const uv ) const { return Vec3(0.f, 0.f, 0.f); }
};

class CLambertianMat : public IMaterial
{
private:
	ITexture* m_albedo;

public:
	CLambertianMat( ITexture* const albedo )
		: m_albedo( albedo )
	{}

	virtual bool Scatter( CRay const& ray, SHitInfo const& hitInfo, Vec3& outAttenuation, CRay& outRayScattered ) const override
	{
		Vec3 const target = hitInfo.m_position + hitInfo.m_normal + Math::RandomInUnitSphere();
		outRayScattered = CRay( hitInfo.m_position, target - hitInfo.m_position, ray.GetTime() );
		outAttenuation = m_albedo->GetValue( hitInfo.m_position, hitInfo.m_uv );
		return true;
	}
};

class CMetalMat : public IMaterial
{
private:
	Vec3 m_albedo;
	float m_fuzz;

public:
	CMetalMat( Vec3 const albedo, float const fuzz )
		: m_albedo( albedo )
	{
		m_fuzz = ( 1.f < fuzz ) ? 1.f : fuzz;
	}

	virtual bool Scatter( CRay const& ray, SHitInfo const& hitInfo, Vec3& outAttenuation, CRay& outRayScattered ) const override
	{
		Vec3 const reflected = Math::Reflect( ray.GetDirection().GetNormalized(), hitInfo.m_normal );
		outRayScattered = CRay( hitInfo.m_position, reflected + Math::RandomInUnitSphere() * m_fuzz, ray.GetTime() );
		outAttenuation = m_albedo;

		return 0.f < Vec3::Dot( outRayScattered.GetDirection(), hitInfo.m_normal );
	}
};

class CDielectricMat : public IMaterial
{
private:
	float m_refIdx;

public:
	CDielectricMat( float const refIdx )
		: m_refIdx( refIdx )
	{}

	// Inherited via IMaterial
	virtual bool Scatter( CRay const& ray, SHitInfo const& hitInfo, Vec3& outAttenuation, CRay& outRayScattered ) const override
	{
		Vec3 outwardNormal;
		Vec3 const reflected = Math::Reflect( ray.GetDirection(), hitInfo.m_normal );
		float niOverNt;
		outAttenuation.Set( 1.f, 1.f, 1.f );
		Vec3 refracted;
		float reflectProb;
		float cosine;
		if ( 0.f < Vec3::Dot( ray.GetDirection(), hitInfo.m_normal ) )
		{
			outwardNormal = -hitInfo.m_normal;
			niOverNt = m_refIdx;
			cosine = m_refIdx * Vec3::Dot( ray.GetDirection(), hitInfo.m_normal ) / ray.GetDirection().Magnitude();
		}
		else
		{
			outwardNormal = hitInfo.m_normal;
			niOverNt = 1.f / m_refIdx;
			cosine = -Vec3::Dot( ray.GetDirection(), hitInfo.m_normal ) / ray.GetDirection().Magnitude();
		}

		if ( Math::Refract( ray.GetDirection(), outwardNormal, niOverNt, refracted ) )
		{
			reflectProb = Math::Schlick( cosine, m_refIdx );
		}
		else
		{
			reflectProb = 1.f;
		}

		if ( Math::Rand() < reflectProb )
		{
			outRayScattered = CRay( hitInfo.m_position, reflected, ray.GetTime() );
		}
		else
		{
			outRayScattered = CRay( hitInfo.m_position, refracted, ray.GetTime() );
		}

		return true;
	}
};

class CDiffuseLightMat : public IMaterial
{
private:
	ITexture* m_emit;

public:
	CDiffuseLightMat( ITexture* emit )
		: m_emit( emit )
	{}
	virtual bool Scatter( CRay const& ray, SHitInfo const& hitInfo, Vec3& outAttenuation, CRay& outRayScattered ) const override
	{
		return false;
	}
	virtual Vec3 Emitted( Vec3 const p, Vec2 const uv ) const override
	{
		return m_emit->GetValue( p, uv );
	}
};

class CIsotropicMat : public IMaterial
{
private:
	ITexture* m_albedo;

public:
	CIsotropicMat( ITexture* const albedo )
		: m_albedo( albedo )
	{}
	virtual bool Scatter( CRay const& ray, SHitInfo const& hitInfo, Vec3& outAttenuation, CRay& outRayScattered ) const override
	{
		outRayScattered = CRay( hitInfo.m_position, Math::RandomInUnitSphere() );
		outAttenuation = m_albedo->GetValue( hitInfo.m_position, hitInfo.m_uv );
		return true;
	}
};