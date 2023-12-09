//Array.h
#ifndef _ARRAY_H
#define _ARRAY_H

typedef signed long int Long;

template <typename T>
class Array {

	public:
		Array(Long capacity = 256);
		Array(const Array& source);
		~Array();
	
		Long Store(Long index, T object);
		Long Insert(Long index, T object);
		Long AppendFromFront(T object);
		Long AppendFromRear(T object);
		Long Delete(Long index);
		Long DeleteFromFront();
		Long DeleteFromRear();
		void Clear();
		Long Modify(Long index, T object);
		Long LinearSearchUnique(void* key, int (*compare)(void*,void*));
		void LinearSearchDuplicate(void* key,Long*(*indexes),Long *count, int (*compare)(void*, void*));
		Long BinarySearchUnique(void* key, int (*compare)(void*, void*));
		void BinarySearchDuplicate(void* key ,Long* (*indexes), Long* count, int(*compare)(void*, void*));
		void SelectionSort(int (*compare)(void*, void*));
		void BubbleSort(int (*compare)(void*, void*));
		void InsertionSort(int(*compare)(void*, void*));
		void Merge(const Array& one, const Array& other, int(*compare)(void*, void*));
		T& GetAt(Long index);
		
		Array<T>& operator =(const Array& source);
		T& operator [](Long index);
		T* operator +(Long index);

		Long GetCapacity() const;
		Long GetLength() const;
		
	private:
		T(*front);
		Long capacity;
		Long length;
};

template <typename T>
Array<T>::Array(Long capacity) {
	this->front = new T[capacity];
	this->capacity = capacity;
	this->length = 0;
}

template <typename T>
Array<T>::Array(const Array& source) {
	Long i;

	this->front = new T[source.capacity];
	i = 0;
	while (i < source.length) {
		this->front[i] = source.front[i];
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
}

template <typename T>
Array<T>::~Array() {
	if (this->front != 0) {
		delete[]this->front;
	}
}


template <typename T>
Long Array<T>::Store(Long index, T object) {
	this->front[index] = object;
	this->length++;

	return index;
}


template <typename T>
Long Array<T>::Insert(Long index, T object) {
	T(*temp);
	Long i = 0;

	temp = new T[this->capacity + 1];
	while (i < index) {
		temp[i] = this->front[i];
		i++;
	}

	while (i < this->length) {
		temp[i + 1] = this->front[i];
		i++;
	}

	if (this->front != 0) {
		delete[]this->front;
	}

	this->front = temp;
	this->capacity++;
	this->front[index] = object;
	this->length++;

	return index;
}

template <typename T>
Long Array<T>::AppendFromFront(T object){
	Long index = 0;
	Long i = 0;
	T(*temp);

	temp = new T[this->capacity + 1];

	while (i < this->length) {
		temp[i + 1] = this->front[i];
		i++;
	}

	if (this->front != 0) {
		delete[]this->front;
	}

	this->front = temp;
	this->capacity++;
	this->front[index] = object;
	this->length++;

	return index;
}

template <typename T>
Long Array<T>::AppendFromRear(T object) {
	Long index;
	Long i = 0;
	T(*temp);
	temp = new T[this->capacity + 1];

	while (i < this->length) {
		temp[i] = this->front[i];
		i++;
	}

	if (this->front != 0) {
		delete[]this->front;
		this->front = 0;
	}

	this->front = temp;
	this->capacity++;
	index = this->capacity - 1;
	this->front[index] = object;
	this->length++;

	return index;

}

template <typename T>
Long Array<T>::Delete(Long index) {
	Long i = 0;
	T(*temp) = 0;

	if (this->capacity > 1) {
		temp = new T[this->capacity - 1];
	}

	while (i < index) {
		temp[i] = this->front[i];
		i++;
	}
	i++;
	while (i < this->length) {
		temp[i - 1] = this->front[i];
		i++;
	}

	if (this->front != 0) {
		delete[] this->front;
		this->front = 0;
	}

	if (this->capacity > 1) {
		this->front = temp;
	}
	this->length--;
	this->capacity--;
	
	index = -1;

	return index;
}

template <typename T>
Long Array<T>::DeleteFromFront(){
	Long i = 1;
	Long index;
	T(*temp) =0;

	temp = new T[this->capacity - 1];

	while (i < this->length) {
		temp[i - 1] = this->front[i];
		i++;
	}

	if (this->front != 0) {
		delete[]this->front;
		this->front = 0;
	}

	if (this->capacity > 1) {
		this->front = temp;
	}

	this->length--;
	this->capacity--;

	index = -1;

	return index;
}

template <typename T>
Long Array<T>::DeleteFromRear(){
	Long i = 0;
	Long index;
	T(*temp) = 0;

	if (this->capacity > 1) {
		temp = new T[this->capacity - 1];
	}

	while (i < this->capacity - 1) {
		temp[i] = this->front[i];
		i++;
	}

	if (this->front != 0) {
		delete[]this->front;
		this->front = 0;
	}

	if (this->capacity > 1) {
		this->front = temp;
	}

	this->length--;
	this->capacity--;

	index = -1;

	return index;
}

template <typename T>
void Array<T>::Clear() {
	if (this->front != 0) {
		delete[]this->front;
		this->front = 0;
	}
	this->length = 0;
	this->capacity = 0;
}

template <typename T>
Long Array<T>::Modify(Long index, T object) {
	this->front[index] = object;
	return index;
}

template <typename T>
Long Array<T>::LinearSearchUnique(void* key, int(*compare)(void*,void*)) {
	Long i = 0;
	Long index = -1;

	while (i < this->length && compare(this->front + i, key) != 0) {
		i++;
	}
	if (i < this->length) {
		index = i;
	}
	return index;
}

template <typename T>
void Array<T>::LinearSearchDuplicate(void* key, Long*(*indexes),Long *count, int(*compare)(void*, void*)) {
	Long i = 0;
	Long j = 0;

	*count = 0;
	*indexes = new Long[this->length];

	while (i < this->length) {
		if (compare(this->front + i, key) == 0) {
			(*indexes)[j] = i;
			j++;
			(*count)++;
		}
		i++;
	}
}

template <typename T>
Long Array<T>::BinarySearchUnique(void* key, int (*compare)(void*, void*)) {
	Long index = -1;
	Long low = 0;
	Long high;
	Long middle;

	high = this->length - 1;
	middle = (low + high) / 2;
	while (low <= high && compare(this -> front + middle, key) != 0) {
		if (compare(this->front + middle, key) < 0) {
			low = middle + 1;
		}
		else {
			high = middle - 1;
		}
		middle = (low + high) / 2;
	}
	if (low <= high) {
		index = middle;
	}
	return index;
}

template <typename T>
void Array<T>::BinarySearchDuplicate(void* key, Long* (*indexes), Long* count, int(*compare)(void*, void*)) {
	Long low = 0;
	Long high;
	Long middle;
	Long i = 0;
	Long index = -1;

	*count = 0;
	*indexes = new Long[this->length];
	high = this->length - 1;
	middle = (low + high) / 2;
	while (low <= high && compare(this->front + middle, key) != 0) {
		if (compare(this->front + middle, key) < 0) {
			low = middle + 1;
		}
		else {
			high = middle - 1;
		}
		middle = (low + high) / 2;
	}
	index = middle - 1;
	while (index >= 0 && compare(this->front + index, key) == 0) {
		index--;
	}
	index++;
	while (index < this->length && compare(this->front + index, key) == 0) {
		(*indexes)[i] = index;
		i++;
		(*count)++;
		index++;
	}
}

template <typename T>
void Array<T>::SelectionSort(int (*compare)(void*, void*)) {
	Long i = 0;
	Long j;
	Long indexLow;
	T temp;

	while (i < this->length - 1) {
		indexLow = i;
		j = i + 1;
		while (j < this->length) {
			if (compare(this->front + indexLow, this->front + j) > 0) {
				indexLow = j;
			}
			j++;
		}
		temp = this->front[i];
		this->front[i] = this->front[indexLow];
		this->front[indexLow] = temp;
		i++;
	}
}

template <typename T>
void Array<T>::BubbleSort(int(*compare)(void*, void*)) {
	Long i;
	Long j;
	T temp;
	Long count = 1;

	j = this->length - 1;
	while (j >= 0 && count > 0) {
		count = 0;
		i = 0;
		while (i < j) {
			if (compare(this->front + i, this->front + j) > 0) {
				temp = this->front[i];
				this->front[i] = this->front[j];
				this->front[j] = temp;
				count++;
			}
			i++;
		}
		j--;
	}
}


template <typename T>
void Array<T>::InsertionSort(int (*compare)(void*, void*)) {
	T temp;
	Long i = 1;
	Long j;

	while (i < this->length) {
		temp = this->front[i];
		j = i - 1;
		while (j >= 0 && compare(this->front + j, &temp) > 0) {
			this->front[j + 1] = this->front[j];
			j--;
		}
		this->front[j + 1] = temp;
		i++;
	}
}

template <typename T>
void Array<T>::Merge(const Array& one, const Array& other, int(*compare)(void*,void*)) {
	Long i = 0;
	Long j = 0;
	Long k = 0;

	if (this->front != 0) {
		delete[]this->front;
	}

	this->front = new T[one.length + other.length];
	this->capacity = one.length + other.length;
	this->length = 0;

	while (i < one.length && j < other.length) {
		if (compare(one.front + i, other.front + j) < 0) {
			this->front[k] = one.front[i];
			k++;
			this->length++;
			i++;
		}
		else {
			this->front[k] = other.front[j];
			k++;
			this->length++;
			j++;
		}
	}

	while (i < one.length) {
		this->front[k] = one.front[i];
		k++;
		this->length++;
		i++;
	}

	while (j < other.length) {
		this->front[k] = other.front[j];
		k++;
		this->length++;
		j++;
	}
}

template <typename T>
T& Array<T>::GetAt(Long index) {
	return this->front[index];
}

template<typename T>
inline Long Array<T>::GetCapacity() const {
	return this->capacity;
}

template <typename T>
inline Long Array<T>::GetLength() const {
	return this->length;
}

template <typename T>
Array<T>& Array<T>::operator = (const Array& source) {
	Long i;

	if (this->front != 0) {
		delete[]this->front;
	}

	this->front = new T[source.capacity];
	i = 0;
	while (i < source.length) {
		this->front[i] = source.front[i];
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}

template <typename T>
T& Array<T>::operator[](Long index) {
	return this->front[index];
}

template <typename T>
T* Array<T>::operator+(Long index) {
	return this->front + index;
}


#endif //_ARRAY_H