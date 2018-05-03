#pragma once

typedef unsigned char Byte;
struct SPixel
{
	uint32_t argb;

	Vec4 ToVec4() const
	{
		uint32_t const a = argb >> 24;
		uint32_t const r = (argb >> 16) & 0x000000FF;
		uint32_t const g = (argb >> 8) & 0x000000FF;
		uint32_t const b = argb & 0x000000FF;

		return Vec4( float( r ) * ( 1.f / 255.f ), float( g ) * ( 1.f / 255.f ), float( b ) * ( 1.f / 255.f ), float( a ) * ( 1.f / 255.f ) );
	}

	static SPixel ToPixel( Vec4 const color )
	{
		uint32_t const r = uint32_t( color.x * 255.f ) << 16;
		uint32_t const g = uint32_t( color.y * 255.f ) << 8;
		uint32_t const b = uint32_t( color.z * 255.f );
		uint32_t const a = uint32_t( color.w * 255.f ) << 24;

		return { r | g | b | a };
	}

	static SPixel ToPixel( Vec3 const color )
	{
		uint32_t const r = uint32_t( color.x * 255.f ) << 16;
		uint32_t const g = uint32_t( color.y * 255.f ) << 8;
		uint32_t const b = uint32_t( color.z * 255.f );

		return { r | g | b | 0xFF000000 };
	}
};

template<class Key, class Value>
struct TPair
{
	Key m_key;
	Value m_value;

	TPair() {}
	TPair(Key const& key, Value const& value)
		: m_key(key)
		, m_value(value)
	{}
};