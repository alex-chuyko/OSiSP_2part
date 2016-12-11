#include "Person.h"


Person::Person()
{
}

Person::Person(string personData)
{
	char* data = new char[personData.size() + 1];
	strcpy(data, personData.c_str());
	char* str = strtok(data, "\t");
	int i = 1;
	while (str != NULL)
	{
		switch (i)
		{
		case 1:
		{
			phone.assign(str);
			break;
		}
		case 2:
		{
			lastName.assign(str);
			break;
		}
		case 3:
		{
			firstName.assign(str);
			break;
		}
		case 4:
		{
			patronymic.assign(str);
			break;
		}
		case 5:
		{
			streetName.assign(str);
			break;
		}
		case 6:
		{
			house.assign(str);
			break;
		}
		case 7:
		{
			housing.assign(str);
			break;
		}
		case 8:
		{
			apartment.assign(str);
			break;
		}
		}
		str = strtok(NULL, "\t");
		i++;
	}
}

Person::~Person()
{
}

Person Person::operator= (Person per)
{
	phone = per.phone;
	lastName = per.lastName;
	firstName = per.firstName;
	patronymic = per.patronymic;
	streetName = per.streetName;
	house = per.house;
	housing = per.housing;
	apartment = per.apartment;
	hash = per.hash;
	return *this;
}

bool operator< (Person per1, Person per2)
{
	if (per1.lastName != " " && per2.lastName != " ")
		return per1.lastName < per2.lastName;
	else if (per1.firstName != " " && per2.firstName != " ")
		return per1.firstName < per2.firstName;
	else if (per1.streetName != " " && per2.streetName != " ")
		return per1.streetName < per2.streetName;
}

bool operator> (Person per1, Person per2)
{
	if (per1.lastName != " " && per2.lastName != " ")
		return per1.lastName > per2.lastName;
	else if (per1.firstName != " " && per2.firstName != " ")
		return per1.firstName > per2.firstName;
	else if (per1.streetName != " " && per2.streetName != " ")
		return per1.streetName > per2.streetName;
}

bool operator== (Person per1, Person per2)
{
	if (per2.lastName == " " && per2.firstName == " " && per2.streetName == " ")
	{
		return true;
	}
	else if (per2.firstName == " " && per2.streetName == " ")
	{
		if (per1.lastName == per2.lastName)
			return true;
		else
			return false;
	}
	else if (per2.lastName == " " && per2.firstName == " ")
	{
		if (per1.streetName == per2.streetName)
			return true;
		else
			return false;
	}
	else if (per2.lastName == " " && per2.streetName == " ")
	{
		if (per1.firstName == per2.firstName)
			return true;
		else
			return false;
	}
	else if (per2.lastName == " ")
	{
		if (per1.firstName == per2.firstName && per1.streetName == per2.streetName)
			return true;
		else
			return false;
	}
	else if (per2.firstName == " ")
	{
		if (per1.lastName == per2.lastName && per1.streetName == per2.streetName)
			return true;
		else
			return false;
	}
	else if (per2.streetName == " ")
	{
		if (per1.lastName == per2.lastName && per1.firstName == per2.firstName)
			return true;
		else
			return false;
	}
	else if (per1.lastName == per2.lastName || per1.firstName == per2.firstName || per1.streetName == per1.streetName)
		return true;
	else
		return false;
}
