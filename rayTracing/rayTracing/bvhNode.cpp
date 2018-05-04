#include <cstdlib>

#include "math.h"
#include "ray.h"
#include "hitTable.h"
#include "bvhNode.h"

int BoxXCompare( void const* a, void const* b )
{
	CAABB leftBox, rightBox;
	IHitTable const* aHitTable = *( IHitTable* const*)a;
	IHitTable const* bHitTable = *( IHitTable* const*)b;

	aHitTable->BoundingBox( 0, 0, leftBox );
	bHitTable->BoundingBox( 0.f, 0.f, rightBox );

	if ( leftBox.GetMin().x - rightBox.GetMin().x < 0.f )
	{
		return -1;
	}

	return 1;
}

int BoxYCompare( void const* a, void const* b )
{
	CAABB leftBox, rightBox;
	IHitTable const* aHitTable = *( IHitTable* const*)a;
	IHitTable const* bHitTable = *( IHitTable* const*)b;

	aHitTable->BoundingBox( 0, 0, leftBox );
	bHitTable->BoundingBox( 0.f, 0.f, rightBox );

	if ( leftBox.GetMin().y - rightBox.GetMin().y < 0.f )
	{
		return -1;
	}

	return 1;
}

int BoxZCompare( void const* a, void const* b )
{
	CAABB leftBox, rightBox;
	IHitTable const* aHitTable = *( IHitTable* const*)a;
	IHitTable const* bHitTable = *( IHitTable* const*)b;

	aHitTable->BoundingBox( 0, 0, leftBox );
	bHitTable->BoundingBox( 0.f, 0.f, rightBox );

	if ( leftBox.GetMin().z - rightBox.GetMin().z < 0.f )
	{
		return -1;
	}

	return 1;
}

CBVHNode::CBVHNode( IHitTable ** children, int const num, float t0, float t1 )
	: IHitTable( nullptr )
{
	unsigned int const axis = unsigned int( 3.f * Math::Rand() );
	switch ( axis )
	{
		case 0:
			std::qsort( children, num, sizeof( IHitTable* ), BoxXCompare );
			break;
		case 1:
			std::qsort( children, num, sizeof( IHitTable* ), BoxYCompare );
			break;
		case 2:
			std::qsort( children, num, sizeof( IHitTable* ), BoxZCompare );
			break;
	}

	if ( num == 1 )
	{
		m_left = m_right = children[ 0 ];
	}
	else if ( num == 2 )
	{
		m_left = children[ 0 ];
		m_right = children[ 1 ];
	}
	else
	{
		m_left = new CBVHNode( children, num / 2, t0, t1 );
		m_right = new CBVHNode( children + num / 2, num - num / 2, t0, t1 );
	}

	CAABB leftBox, rightBox;
	m_left->BoundingBox( t0, t1, leftBox );
	m_right->BoundingBox( t0, t1, rightBox );

	m_box.Combine( leftBox, rightBox );
}

CBVHNode::~CBVHNode()
{
	if ( m_left == m_right )
	{
		m_right = nullptr;
	}
	delete m_left;
	delete m_right;
}

bool CBVHNode::Hit( CRay const & ray, float const tMin, float const tMax, SHitInfo & outInfo ) const
{
	if ( m_box.Hit( ray, tMin, tMax ) )
	{
		SHitInfo leftHitInfo;
		SHitInfo rightHitInfo;

		bool const leftHit = m_left->Hit( ray, tMin, tMax, leftHitInfo );
		bool const rightHit = m_right->Hit( ray, tMin, tMax, rightHitInfo );

		if ( leftHit && rightHit )
		{
			if ( leftHitInfo.m_t < rightHitInfo.m_t )
			{
				outInfo = leftHitInfo;
			}
			else
			{
				outInfo = rightHitInfo;
			}
			return true;
		}
		else if ( leftHit )
		{
			outInfo = leftHitInfo;
			return true;
		}
		else if ( rightHit )
		{
			outInfo = rightHitInfo;
			return true;
		}
	}

	return false;
}

bool CBVHNode::BoundingBox( float const t0, float const t1, CAABB & box ) const
{
	box = m_box;
	return true;
}
