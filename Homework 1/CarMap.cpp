#include "Map.h"
#include "CarMap.h"
#include <iostream>

CarMap::CarMap()
{
	Map m_carMap;
}

bool CarMap::addCar(std::string license)
{
	return m_carMap.insert(license, 0);
}

double CarMap::gas(std::string license) const
{
	if (m_carMap.contains(license))
	{
		double gas;
		m_carMap.get(license, gas);

		return gas;
	}

	return -1;
}

bool CarMap::addGas(std::string license, double gallons)
{
	if (!(m_carMap.contains(license)) || gallons < 0)
		return false;

	double gas;
	m_carMap.get(license, gas);

	return m_carMap.update(license, gas + gallons);
}

bool CarMap::useGas(std::string license, double gallons)
{
	if (!(m_carMap.contains(license)) || gallons < 0)
		return false;

	double gas;
	m_carMap.get(license, gas);

	if (gallons > gas)
		return false;

	return m_carMap.update(license, gas - gallons);
}

int CarMap::fleetSize() const
{
	return m_carMap.size();
}

void CarMap::print() const
{
	for (int i = 0; i < m_carMap.size(); i++)
	{
		std::string k;
		double v;
		m_carMap.get(i, k, v);

		std::cout << k << " " << v << std::endl;
	}
}