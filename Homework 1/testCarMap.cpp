#include "CarMap.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	CarMap m;  // maps strings to doubles
	assert(m.fleetSize() == 0);
	ValueType v = -1234.5;
	m.addCar("xyz");
	assert(m.fleetSize() == 1);
	assert(m.addGas("xyz", 14.5) == true);
	KeyType k = "hello";
	assert(m.gas("xyz") == 14.5);
	m.addCar("asdf");
	m.addCar("fwaerw");
	m.addGas("asdf", 12.3);
	m.addGas("fwaerw", 11.0);
	m.useGas("asdf", 10.3);

	m.print();

	cout << "Passed all tests" << endl;
}