//Array.cpp

#include "Array.h"
#include <iostream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if 0
int CompareNumbers(void*, void*);

int main(int argc, char* argv[]) {
	Array<int> array(4);
	Long index;
	Long ret;
	Long i;
	Long(*indexes);
	Long count;
	int key;

	index = array.Store(0, 3);
	cout << array[index] << endl;
	ret = array.GetAt(index);
	cout << ret << endl;

	index = array.Store(1, 2);
	cout << array[index] << endl;
	ret = array.GetAt(index);
	cout << ret << endl;

	index = array.Store(2, 1);
	cout << array[index] << endl;
	ret = array.GetAt(index);
	cout << ret << endl;

	index = array.Store(3, 5);
	cout << array[index] << endl;
	ret = array.GetAt(index);
	cout << ret << endl;

	key = 5;
	index = array.LinearSearchUnique(&key, CompareNumbers);
	cout << array[index] << endl;
	ret = array.GetAt(index);
	cout << ret << endl;

	array.SelectionSort(CompareNumbers);
	i = 0;
	while (i < array.GetLength()) {
		cout << array[i] << endl;
		ret = array.GetAt(i);
		cout << ret << endl;
		i++;
	}

	key = 1;
	index = array.BinarySearchUnique(&key, CompareNumbers);
	cout << array[index] << endl;
	ret = array.GetAt(index);
	cout << ret << endl;

	index = array.Insert(1, 2);
	cout << array[index] << endl;
	ret = array.GetAt(index);
	cout << ret << endl;

	index = array.AppendFromFront(2);
	cout << array[index] << endl;
	ret = array.GetAt(index);
	cout << ret << endl;

	index = array.AppendFromRear(2);
	cout << array[index] << endl;
	ret = array.GetAt(index);
	cout << ret << endl;

	key = 2;
	array.LinearSearchDuplicate(&key, &indexes, &count, CompareNumbers);
	i = 0;
	while (i < count) {
		cout << array[indexes[i]] << endl;
		ret = array.GetAt(indexes[i]);
		cout << ret << endl;
		i++;
	}

	if (indexes != 0) {
		delete[]indexes;
	}

	array.BubbleSort(CompareNumbers);
	i = 0;
	while (i < array.GetLength()) {
		cout << array[i] << endl;
		ret = array.GetAt(i);
		cout << ret << endl;
		i++;
	}
	
	key = 3;
	array.BinarySearchDuplicate(&key, &indexes, &count, CompareNumbers);
	i = 0;
	while (i < count) {
		cout << array[indexes[i]] << endl;
		ret = array.GetAt(indexes[i]);
		cout << ret << endl;
		i++;
	}

	if (indexes != 0) {
		delete[]indexes;
	}

	index = array.Delete(3);
	if (index == -1) {
		cout << "지워졌습니다." << endl;
	}

	index = array.DeleteFromFront();
	if (index == -1) {
		cout << "지워졌습니다." << endl;
	}

	index = array.DeleteFromRear();
	if (index == -1) {
		cout << "지워졌습니다." << endl;
	}

	index = array.Modify(0,10);
	cout << array[index] << endl;
	ret = array.GetAt(index);
	cout << ret << endl;

	array.Clear();

	Array<int> one(3);

	index = one.Store(0, 3);
	cout << one[index] << endl;
	ret = one.GetAt(index);
	cout << ret << endl;

	index = one.Store(1, 10);
	cout << one[index] << endl;
	ret = one.GetAt(index);
	cout << ret << endl;

	index = one.Store(2, 1);
	cout << one[index] << endl;
	ret = one.GetAt(index);
	cout << ret << endl;

	one.InsertionSort(CompareNumbers);
	i = 0;
	while (i < one.GetLength()) {
		cout << one[i] << endl;
		ret = one.GetAt(i);
		cout << ret << endl;
		i++;
	}

	Array<int> other(3);
	
	index = other.Store(0, 2);
	cout << other[index] << endl;
	ret = other.GetAt(index);
	cout << ret << endl;

	index = other.Store(1, 3);
	cout << other[index] << endl;
	ret = other.GetAt(index);
	cout << ret << endl;

	index = other.Store(2, 5);
	cout << other[index] << endl;
	ret = other.GetAt(index);
	cout << ret << endl;

	array.Merge(one, other, CompareNumbers);
	i = 0;
	while (i < array.GetLength()) {
		cout << array[i] << endl;
		i++;
	}

	Array<int> copyArray(array);
	i = 0;
	while (i < copyArray.GetLength()) {
		cout << copyArray[i] << endl;
		i++;
	}


	return 0;
}

int CompareNumbers(void* one, void* other) {
	int* one_ = (int*)one;
	int* other_ = (int*)other;
	int ret;

	if (*one_ > *other_) {
		ret = 1;
	}

	else if (*one_ == *other_) {
		ret = 0;
	}

	else if (*one_ < *other_) {
		ret = -1;
	}

	return ret;
}

#endif
