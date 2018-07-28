#include "MyOctantExample.h"

using namespace Simplex;

Simplex::MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{

}

Simplex::MyOctant::MyOctant(vector3 a_v3center, float a_fsize)
{
	m_v3Center = a_v3center; // root v3 = 0.0f
	m_fSize = a_fsize; // root cube size = 36.0f
	m_v3Min = m_v3Center - m_fSize;
	m_v3Max = m_v3Center + m_fSize;
}

vector3 Simplex::MyOctant::GetMin(void)
{
	return m_v3Min;
}

void Simplex::MyOctant::SetMin(vector3 min)
{
	m_v3Min = min;
}
vector3 Simplex::MyOctant::GetMax(void)
{
	return m_v3Max;
}

void Simplex::MyOctant::SetMax(vector3 max)
{
	m_v3Max = max;
}
vector3 Simplex::MyOctant::GetCenterGlobal(void)
{
	return m_v3Center;
}

void Simplex::MyOctant::SetCenterGlobal(vector3 center)
{
	m_v3Center = center;
}

float Simplex::MyOctant::GetSize(void)
{
	return m_fSize;
}

void Simplex::MyOctant::SetSize(float size)
{
	m_fSize = size;
}

bool Simplex::MyOctant::Contains(MyRigidBody* rigidbody)
{
	// get position for rigidbody
	//vector3 v3Position = rigidbody->GetCenterGlobal();
	return false;
}

bool Simplex::MyOctant::HasPoint(vector3 v3Position)
{
	/*std::cout << v3Position.x << v3Position.y << v3Position.z << " - ";
	std::cout << m_v3Min.x << m_v3Min.y << m_v3Min.z << " - ";
	std::cout << m_v3Max.x << m_v3Max.y << m_v3Max.z << " - ";
	*/
	if (m_lChild.empty()) {
		if (v3Position.x > m_v3Min.x && v3Position.y > m_v3Min.y && v3Position.z > m_v3Min.z) { // Check greater than minimum
			if (v3Position.x < m_v3Max.x && v3Position.y < m_v3Max.y && v3Position.z < m_v3Max.z) // check less than maximum
				return true;
		}
	}
	else {
		for (MyOctant* nOctant : m_lChild) 
		{
			if (nOctant->HasPoint(v3Position)) 
			{
				return true;
			}
		}
	}
	std::cout << "ERR: v3Position not found";
	return false;
}

void Simplex::MyOctant::AddEntityID(uint uID)
{

}

bool Simplex::MyOctant::HasEntity(uint uID)
{
	if (m_uChildren != 0){
		for (int i = 0; i < 8; i++)
			HasEntity(uID);
	}
	else {
		for (uint x : m_EntityList) {
			if (uID == x) 
				return true;
		}
	}
}

void Simplex::MyOctant::Populate(uint uID)
{
	m_EntityList.push_back(uID);
}

std::vector<uint> Simplex::MyOctant::GetEntityList()
{
	return m_EntityList;
}

void Simplex::MyOctant::Subdivide(void)
{
	vector3 newMax = ZERO_V3; // store new max positions 
	vector3 newMin = ZERO_V3; // store new min positions 
	vector3 newCenter = ZERO_V3; // store new center positions
	float newSize = m_fSize / 2.0f; // calculate new size
	for (int i = 1; i < 9; i++)
	{
			MyOctant* octant = new MyOctant(); // create new octant
			newMax = vector3(0.0f);
			newMin = vector3(0.0f);
			newCenter = vector3(0.0f);
			if (i == 1)
			{
				newMax = m_v3Max;
				newMin = m_v3Center;
			}
			if (i == 2)
			{
				newMax = vector3(m_v3Center.x, m_v3Max.y, m_v3Max.z);
				newMin = vector3(m_v3Min.x, m_v3Center.y, m_v3Center.z);
			}
			if (i == 3)
			{
				newMax = vector3(m_v3Max.x, m_v3Center.y, m_v3Max.z);
				newMin = vector3(m_v3Center.x, m_v3Min.y, m_v3Center.z);
			}
			if (i == 4)
			{
				newMax = vector3(m_v3Center.x, m_v3Center.y, m_v3Max.z);
				newMin = vector3(m_v3Min.x, m_v3Min.y, m_v3Center.z);
			}
			if (i == 5)
			{
				newMax = vector3(m_v3Max.x, m_v3Max.y, m_v3Center.z);
				newMin = vector3(m_v3Center.x, m_v3Center.y, m_v3Min.z);
			}
			if (i == 6)
			{
				newMax = vector3(m_v3Center.x, m_v3Max.y, m_v3Center.z);
				newMin = vector3(m_v3Min.x, m_v3Center.y, m_v3Min.z);
			}
			if (i == 7)
			{
				newMax = vector3(m_v3Max.x, m_v3Center.y, m_v3Center.z);
				newMin = vector3(m_v3Center.x, m_v3Min.y, m_v3Min.z);
			}
			if (i == 8)
			{
				newMax = m_v3Center;
				newMin = m_v3Min;
			}

			newCenter = (newMax - m_fSize) + (m_fSize / 2.0f); // calculate new center for child octant
			octant->SetSize(newSize); // set new size for octant
			octant->SetCenterGlobal(newCenter); // set new center for octant
			octant->SetMin(newMin); // set the new min position for the octant
			octant->SetMax(newMax); // set the new max position for the octant

			m_lChild.push_back(octant);

			for (uint id : m_EntityList) {

			}
	}
	/*for(MyOctant* octant : m_lChild)
		std::cout << "CENTER:" << octant->GetCenterGlobal().x << "  " << octant->GetCenterGlobal().y << "  " << octant->GetCenterGlobal().z;
	*/
}
