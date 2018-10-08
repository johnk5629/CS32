// MyHash.h

// Skeleton for the MyHash class template.  You must implement the first seven
// member functions; we have implemented the eighth.

template<typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5);
    ~MyHash();
    void reset();
    void associate(const KeyType& key, const ValueType& value);
    int getNumItems() const;
    double getLoadFactor() const;
	

	unsigned int getBucketNumber(const KeyType& key) const;

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }

      // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;

private:
	double m_maxLoadFactor;
	int m_numBuckets;
	int m_numItems;

	struct Node
	{
		KeyType key;
		ValueType value;
		Node* next;
	};

	void insertNode(int bNumber, Node**& hashTable, const KeyType& key, const ValueType& value);

	Node** m_hashTable;
};

template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor)
	: m_maxLoadFactor(maxLoadFactor), m_numBuckets(100), m_numItems(0)
{
	m_hashTable = new Node*[m_numBuckets];

	for (int i = 0; i < m_numBuckets; i++)
		m_hashTable[i] = nullptr;
}

template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::~MyHash()
{
	for (int i = 0; i < m_numBuckets; i++)
	{
		Node* p = m_hashTable[i];

		if (p != nullptr)
		{
			while (p->next != nullptr)
			{
				Node* q = p->next;
				p->next = q->next;
				delete q;
			}
			delete p;
		}
	}

	delete [] m_hashTable;
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::reset()
{
	//deleting
	for (int i = 0; i < m_numBuckets; i++)
	{
		Node* p = m_hashTable[i];

		if (p != nullptr)
		{
			while (p->next != nullptr)
			{
				Node* q = p->next;
				p->next = q->next;
				delete q;
			}
			delete p;
		}
	}

	delete[] m_hashTable;

	//set number of items to 0 and set the number of buckets to default which is 100
	m_numItems = 0;
	m_numBuckets = 100;

	//maxLoadFactor should not have changed so no resetting is necessary

	//creating new one
	m_hashTable = new Node*[m_numBuckets];

	for (int i = 0; i < m_numBuckets; i++)
		m_hashTable[i] = nullptr;
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::insertNode(int bNumber, Node**& hashTable, const KeyType& key, const ValueType& value)
{
	//adding new Node* to the beginning of the linked list and pushing the old ones back
	//works even if m_hashTable[bucketNumber] == nullptr (aka empty bucket)

	Node* p = hashTable[bNumber];
	Node* q = new Node;
	q->key = key;
	q->value = value;
	q->next = p;

	hashTable[bNumber] = q;
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	int bucketNumber = getBucketNumber(key);

	//if there is a same hash and the same key, update the value
	for (Node* p = m_hashTable[bucketNumber]; p != nullptr; p = p->next)
	{
		if (p->key == key)
		{
			p->value = value;
			return;
		}
	}

	m_numItems++;
	if (getLoadFactor() > m_maxLoadFactor)
	{
		//creating a new hash table with twice the number of buckets
		int newNumBuckets = m_numBuckets * 2;
		Node** newHashTable = new Node*[newNumBuckets];

		for (int i = 0; i < newNumBuckets; i++)
			newHashTable[i] = nullptr;

		//inserting all the old data into the new hash table
		for (int j = 0; j < m_numBuckets; j++)
		{
			for (Node* p = m_hashTable[j]; p != nullptr; p = p->next)
			{
				int newBucketNumber = getBucketNumber(p->key);
				insertNode(newBucketNumber, newHashTable, p->key, p->value);
			}
		}

		//inserting the new data
		int newBucketNumber = getBucketNumber(key);
		insertNode(newBucketNumber, newHashTable, key, value);

		//deleting old stuff
		for (int i = 0; i < m_numBuckets; i++)
		{
			Node* p = m_hashTable[i];

			if (p != nullptr)
			{
				while (p->next != nullptr)
				{
					Node* q = p->next;
					p->next = q->next;
					delete q;
				}
				delete p;
			}
		}

		delete[] m_hashTable;

		//setting the pointer to the original hash table to point to this temporary hash table pointer
		m_hashTable = newHashTable;
		//finalizing the doubling of the number of buckets
		m_numBuckets = newNumBuckets;

	}
	else
	{
		insertNode(bucketNumber, m_hashTable, key, value);
	}
}

template<typename KeyType, typename ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const
{
	int i = getBucketNumber(key);

	for (Node* p = m_hashTable[i]; p != nullptr; p = p->next)
	{
		if (p->key == key)
			return &(p->value);
	}

	return nullptr;
}

template<typename KeyType, typename ValueType>
int MyHash<KeyType, ValueType>::getNumItems() const
{
	return m_numItems;
}

template<typename KeyType, typename ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor() const
{
	return static_cast<double>(m_numItems) / m_numBuckets;
}

template<typename KeyType, typename ValueType>
unsigned int MyHash<KeyType, ValueType>::getBucketNumber(const KeyType& key) const
{
	unsigned int hash(const KeyType& k);		//	prototype
	unsigned int h = hash(key);	

	return h % m_numBuckets;
}