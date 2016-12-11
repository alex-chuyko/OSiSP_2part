#include "PhoneBook.h"

PhoneBook::PhoneBook()
{
}


PhoneBook::~PhoneBook()
{
}

void PhoneBook::insert(tree_node *&node, Person newPerson, int ind, int treeNumber)
{
	tree_node* newNode = new tree_node;
	tree_node* parentNode;
	newNode->data = newPerson;
	newNode->index = ind;
	newNode->left = NULL;
	newNode->right = NULL;
	parentNode = NULL;
	byte isLarger = false;

	if (isEmpty(node))
		node = newNode;
	else
	{
		tree_node* current;
		current = node;
		while (true)
		{
			parentNode = current;
			switch (treeNumber)
			{
			case 1:
				isLarger = newNode->data.lastName > current->data.lastName;
				break;
			case 2:
				isLarger = newNode->data.firstName > current->data.firstName;
				break;
			case 3:
				isLarger = newNode->data.streetName > current->data.streetName;
				break;
			}
			if (isLarger)
			{
				if (current->right == NULL)
				{
					current->right = newNode;
					break;
				}
				else
					current = current->right;
			}
			else
			{
				if (current->left == NULL)
				{
					current->left = newNode;
					break;
				}
				else
					current = current->left;
			}
		}
	}
}

void PhoneBook::load(LPCWSTR fileName)
{
	std::ifstream file(fileName);
	int i = 0;
	while (file) {
		std::string personData;
		std::getline(file, personData);
		records.push_back(personData);
		i++;
	}
	file.close();
	file.clear();
}

void PhoneBook::createIndex()
{
	int i = 0;
	while (i < records.size())
	{
		Person pers(records[i]);
		insert(rootLastName, pers, i, 1);
		insert(rootFirstName, pers, i, 2);
		insert(rootStreetName, pers, i, 3);
		i++;
	}
}

vector<string> PhoneBook::find(Person findPerson)
{
	vector<string> result;
	if (isEmpty(rootLastName) && isEmpty(rootFirstName) && isEmpty(rootStreetName))
		return result;

	tree_node* currentNode;
	tree_node* parentNode;

	if (findPerson.lastName != " ")
		currentNode = rootLastName;
	else if (findPerson.firstName != " ")
		currentNode = rootFirstName;
	else if (findPerson.streetName != " ")
		currentNode = rootStreetName;
	else
		return records;

	while (currentNode != NULL)
	{
		if (currentNode->data == findPerson)
		{
			result.push_back(records[currentNode->index]);
		}
		parentNode = currentNode;
		if (findPerson > currentNode->data)
			currentNode = currentNode->right;
		else
			currentNode = currentNode->left;
	}
	return result;
}