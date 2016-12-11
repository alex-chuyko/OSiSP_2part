#include "PhoneBook.h"

PhoneBook phB;

extern "C" __declspec(dllexport) void loadBD(LPCWSTR fileName)
{
	phB.load(fileName);//myPhone, newphonebook, myNewPhone
	phB.createIndex();
}

extern "C" __declspec(dllexport) vector<string> searchInBD(string lastName, string firstName, string streetName)
{
	Person pr(" \t" + lastName + "\t" + firstName + "\t \t" + streetName + "\t \t \t ");
	return phB.find(pr);
}