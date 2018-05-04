#include "headers.h"

void CHitTableList::AddHitTable( IHitTable* const hitTable )
{
	m_list.Add( hitTable );
}

void CHitTableList::Clear()
{
	unsigned int const listSize = m_list.Size();
	for ( unsigned int i = 0; i < listSize; ++i )
	{
		delete m_list[ i ];
	}
}

bool CHitTableList::Hit( CRay const & ray, float const tMin, float const tMax, SHitInfo& outInfo ) const
{
	SHitInfo tmpHitInfo;
	bool hitAnything = false;
	float closest = tMax;

	unsigned int const listSize = m_list.Size();
	for ( unsigned int i = 0; i < listSize; ++i )
	{
		if ( m_list[ i ]->Hit( ray, tMin, closest, tmpHitInfo ) )
		{
			hitAnything = true;
			closest = tmpHitInfo.m_t;
		}
	}

	outInfo = tmpHitInfo;
	return hitAnything;
}

bool CHitTableList::BoundingBox( float const t0, float const t1, CAABB & box ) const
{
	unsigned int const listSize = m_list.Size();
	if ( listSize == 0 )
	{
		return false;
	}

	CAABB tmpBox;
	if ( !m_list[ 0 ]->BoundingBox( t0, t1, tmpBox ) )
	{
		return false;
	}
	else
	{
		box = tmpBox;
	}

	for ( unsigned int i = 1; i < listSize; ++i )
	{
		if ( m_list[ i ]->BoundingBox( t0, t1, tmpBox ) )
		{
			box.Combine( box, tmpBox );
		}
		else
		{
			return false;
		}
	}

	return true;
}
