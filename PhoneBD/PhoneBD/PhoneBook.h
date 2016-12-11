#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <sstream>
#include "Person.h"
#include <Windows.h>
#include <conio.h>
#include <fstream>
#include <vector>

#define PHONE_DB_MAP_OBJECT_NAME TEXT("Osisp_lab4, PhoneDB")

using namespace std;

class PhoneBook
{
private:
	struct tree_node
	{
		tree_node* left;
		tree_node* right;
		int index;
		Person data;
	};
	tree_node* rootLastName;
	tree_node* rootFirstName;
	tree_node* rootStreetName;
	FILE* hFile;
	vector<string> records;
public:
	PhoneBook();
	~PhoneBook();
	bool isEmpty(tree_node *node) const { return node == NULL; }
	void load(LPCWSTR fileName);
	void insert(tree_node *&, Person, int, int);
	void createIndex();
	vector<string> find(Person);
};

