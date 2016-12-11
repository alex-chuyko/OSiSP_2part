#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
using namespace std;

class Person
{
public:
	Person();
	Person(string str);
	~Person();
	friend bool operator< (Person, Person);
	friend bool operator> (Person, Person);
	friend bool operator== (Person, Person);
	Person operator= (Person);

	string phone;
	string lastName;
	string firstName;
	string patronymic;
	string streetName;
	string house;
	string housing;
	string apartment;
	int hash;
};

