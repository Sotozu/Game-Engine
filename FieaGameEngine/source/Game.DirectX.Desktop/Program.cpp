#include "pch.h"
#include "SList.h"
#include "Vector.h"
#include <iostream>

using namespace FieaGameEngine;

int main() 
{
	SList<int> mylist{ 1, 2, 3, 4, 5 , 4, 3, 2, 1 };

	mylist.Back();
	mylist.Front();

}