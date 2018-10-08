#include "newMap.h"

Map::Map()
{
	mapCount = 0;
	memorySpace = DEFAULT_MAX_ITEMS;
	m_map = new Card[memorySpace];
}

Map::Map(int x)
{
	if (x < 1)
	{
		x = 1;
	}

	mapCount = 0;
	memorySpace = x;
	m_map = new Card[memorySpace];
}

Map::Map(const Map& other)
{
	mapCount = other.mapCount;
	memorySpace = other.memorySpace;

	m_map = new Card[memorySpace];

	for (int i = 0; i < mapCount; i++)
	{
		m_map[i].k = other.m_map[i].k;
		m_map[i].v = other.m_map[i].v;
	}
}

Map& Map::operator=(const Map& rhs)
{
	if (this != &rhs)
	{
		mapCount = rhs.mapCount;
		memorySpace = rhs.memorySpace;

		m_map = new Card[memorySpace];

		for (int i = 0; i < mapCount; i++)
		{
			m_map[i].k = rhs.m_map[i].k;
			m_map[i].v = rhs.m_map[i].v;
		}
	}

	return *this;
}

Map::~Map()
{
	delete [] m_map;
}

bool Map::empty() const
{
	return (mapCount == 0);
}

int Map::size() const
{
	return mapCount;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
	if (mapCount >= memorySpace)
		return false;

	for (int i = 0; i < mapCount; i++)
	{
		if (m_map[i].k == key)
			return false;
	}

	m_map[mapCount].k = key;
	m_map[mapCount].v = value;

	mapCount++;

	return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
	for (int i = 0; i < mapCount; i++)
	{
		if (m_map[i].k == key)
		{
			m_map[i].v = value;
			return true;
		}
	}

	return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	for (int i = 0; i < mapCount; i++)
	{
		if (m_map[i].k == key)
		{
			m_map[i].v = value;
			return true;
		}
	}

	if (mapCount >= memorySpace)
		return false;

	m_map[mapCount].k = key;
	m_map[mapCount].v = value;

	mapCount++;

	return true;
}

bool Map::erase(const KeyType& key)
{
	for (int i = 0; i < mapCount; i++)
	{
		if (m_map[i].k == key)
		{
			for (int j = i; j < mapCount - 1; j++)
			{
				m_map[j] = m_map[j + 1];
			}
			return true;
		}
	}

	return false;
}

bool Map::contains(const KeyType& key) const
{
	for (int i = 0; i < mapCount; i++)
	{
		if (m_map[i].k == key)
		{
			return true;
		}
	}

	return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	for (int i = 0; i < mapCount; i++)
	{
		if (m_map[i].k == key)
		{
			value = m_map[i].v;
			return true;
		}
	}

	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	if (i >= 0 || i < mapCount)
	{
		key = m_map[i].k;
		value = m_map[i].v;

		return true;
	}

	return false;
}

void Map::swap(Map& other)
{
	Map temp = *this;

	*this = other;
	other = temp;
}
