#include "math.h"
#include "ray.h"
#include "hitTable.h"
#include "material.h"

IHitTable::~IHitTable()
{
	delete m_material;
}
