#include "Map.h"

Map::Map()
	: m_size(0)
{
	dummy = new Node();
	dummy->next = dummy;
	dummy->prev = dummy;
}

Map::Map(const Map& other)
{
	//similar as default constructor except the size

	m_size = other.m_size;

	dummy = new Node();
	dummy->next = dummy;
	dummy->prev = dummy;

	//loop through the other Map to copy the nodes

	Node* o;
	for (o = other.dummy->next; o != other.dummy; o = o->next)
	{
		Node* n = new Node();

		n->data.m_key = o->data.m_key;
		n->data.m_value = o->data.m_value;
		n->next = dummy;
		n->prev = dummy->prev;

		n->prev->next = n;
		dummy->prev = n;
	}
}

Map& Map::operator=(const Map& rhs)
{
	if (this != &rhs)
	{
		Map temp(rhs);		//”copy-and-swap”
		swap(temp);
	}

	return *this;
}

Map::~Map()
{
	Node* p = dummy->prev;
	int i = 0;
	while (i < m_size)		//reversing from end to delete
	{
		p = p->prev;
		delete p->next;
		i++;
	}
	delete dummy;			//delete the head at the end (dummy node)
}

bool Map::erase(const KeyType& key)
{
	Node* e;
	if (!(find(key, e)))  // not found
		return false;

	// Move last array item to replace the one to be erased

	m_size--;
	e->prev->next = e->next;
	e->next->prev = e->prev;
	delete e;
	return true;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	Node* g;
	if (!(find(key, g)))  // not found
		return false;

	value = g->data.m_value;
	return true;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	if (i < 0 || i >= m_size)
		return false;

	Node* g = dummy->next;
	for (int j = 0; j < i; j++)
	{
		g = g->next;			//loop enough times to get the the (i)-th node in the list (disregarding dummy node)
	}

	key = g->data.m_key;
	value = g->data.m_value;
	return true;
}

bool Map::contains(const KeyType& key) const
{
	Node* c;
	return find(key, c);
}

void Map::swap(Map& other)
{
	//swap the head pointer of the two Maps
	Node* temp = dummy;
	dummy = other.dummy;
	other.dummy = temp;

	// Swap sizes

	int t = m_size;
	m_size = other.m_size;
	other.m_size = t;
}

bool Map::find(const KeyType& key, Node*& p) const
{
	// Search through the linked lists.

	for (p = dummy->next; p != dummy; p = p->next)
	{
		if (p->data.m_key == key)
			return true;
	}

	return false;
}

bool Map::doInsertOrUpdate(const KeyType& key, const ValueType& value, bool mayInsert, bool mayUpdate)
{
	Node* d;
	if (find(key, d))  // found
	{
		if (mayUpdate)
		{
			d->data.m_value = value;
		}
		return mayUpdate;
	}
	if (!mayInsert)  // not found, and not allowed to insert
		return false;

	//unlimited room so no check for size

	//create new node
	Node* n = new Node();

	//initialize the new node (adding it at the end of the list)
	n->data.m_key = key;
	n->data.m_value = value;
	n->next = dummy;
	n->prev = dummy->prev;

	n->prev->next = n;		// change the previous end of list to point at new node
	dummy->prev = n;		//change the dummy node's prev to point at the new end of the list

	m_size++;

	return true;
}



bool combine(const Map& m1, const Map& m2, Map& result)
{
	Map temp(m1);				//copy constructing a temporary Map with m1
	bool condition = true;

	for (int i = 0; i < m2.size(); i++)
	{
		KeyType key;
		ValueType value;
		m2.get(i, key, value);	//get the value of the Pair of m2

		if (temp.contains(key))	//check if temp contains the same key as one in m2
		{
			ValueType value2;
			temp.get(key, value2);		//get the value of the Pair
			if (value != value2)		
			{
				condition = false;		//since value are not equal set condition to false
				temp.erase(key);		//removal that key from the combined list (temp)
			}
		}
		else
		{
			temp.insert(key, value);	//if the key doesn't match just add to list
		}
	}

	result = temp;						// assign temp to result

	return condition;
}

void subtract(const Map& m1, const Map& m2, Map& result)
{
	Map temp(m1);					//copy constructing a temporary Map with m1

	for (int i = 0; i < m2.size(); i++)
	{
		KeyType key;
		ValueType value;
		m2.get(i, key, value);		//get the values of the Pair in m2

		if (temp.contains(key))		//compare to the values of temp which is m1
			temp.erase(key);		//erase it if the keys match
	}

	result = temp;					//assign temp to result
}
