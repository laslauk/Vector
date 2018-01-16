//Simple Vector with dynamic memory management.
//Purpose was to learn C++ memory management.
//Lasse Laukkanen 2017

#pragma once
#include <initializer_list>
#include <string>
#include <iostream>
#include <stdexcept>

using size_type = unsigned int;
template <class T>
class Vector
{
private:
	size_type capacity_;
	size_type size_;
	void resize();

public:
	T *dataArray_;
	typedef T* Iterator;

	//Constructors
	Vector(std::initializer_list<T>);
	Vector(size_type init_size = 1);
	~Vector();

	//Operators
	T& operator[](const size_type index);
	Vector<T>& operator=(const Vector<T>& other);

	//Iterators
	Iterator begin();
	Iterator end();

	//Access
	T& at(const size_type index);
	T& front(); //returns first element
	T& back();	//returns last element
	void print();
	bool is_empty();
	size_type capacity() const { return capacity_; }
	size_type size() const { return size_; }

	//modifiers
	void push_back(const T& obj); //adds one element to the vector to the end
	void pop_back();
	void clear();	//clears the content
	Iterator insert(size_type pos, const T& value);	//inserts item to pos
	Iterator erase(size_type pos);	//erases thing from pos returns iterator f ollowing the last removed element

};

//Default constructor
template<class T>
Vector<T>::Vector(size_type init_size) :capacity_(init_size), size_(0), dataArray_(new T[init_size]) {
}

//Initializer list constructor
template<class T>
Vector<T>::Vector(std::initializer_list<T> ilist) : capacity_(ilist.size()), size_(ilist.size()), dataArray_(new T[ilist.size()])
{
	size_type index = 0;
	for (auto &element : ilist) {
		dataArray_[index] = element;
		++index;
	}
}

//Destructor
template<class T>
Vector<T>::~Vector() {
	size_ = 0;
	capacity_ = 0;
	delete[] dataArray_;
}

//Returns pointer pointing to the first element of the vector
template<class T>
T* Vector<T>::begin()
{
	if (size_ == 0) //empty container
	{
		return end();
	}
	else {
		return dataArray_;
	}
}

//Returns pointer to the next memory slot of the last element in the vector
template<class T>
T* Vector<T>::end() {
	T* ptr = &dataArray_[size_];
	return ptr;
}

//Removes the last element in the vector by setting size one smaller
template <class T>
void Vector<T>::pop_back()
{
	if (!this->is_empty())
	{
		//	(reinterpret_cast<T*>(dataArray_)[size_ - 1]).~T();
		--size_;
	}
}

//Returns true if vector is empty
template <class T>
bool Vector<T>::is_empty() {
	return size_ == 0;
}

//Inserts the element to the vector to the position
//defined with the parameter pos. Will double capacity if needed.
//Returns a pointer to the inserted element.
//Returns nullptr if fail
template<class T>
T* Vector<T>::insert(size_type pos, const T& value) {
	if (pos >= 0 && pos < size_)
	{
		if (size_ == capacity_)
		{
			capacity_ = capacity_ * 2;
		}

		T* new_array = new T[capacity_];
		for (size_type i = 0; i <= size_; ++i)
		{
			if (i < pos)
			{
				new_array[i] = dataArray_[i];
			}

			else if (i == pos)
			{
				new_array[i] = value;
			}
			else {
				new_array[i] = dataArray_[i - 1];
			}
		}

		delete[] dataArray_;
		dataArray_ = new_array;
		++size_;
		return  &dataArray_[pos];
	}
	else { return nullptr; }
}

//Removes the element from the vector from the position
//defined with the parameter pos.
//Returns a pointer to the next element of the removed item in the vector,
// which will be same as end() , if the next item is the last one.
//Returns nullptr if fail
template <class T>
T* Vector<T>::erase(size_type pos) {
	if (pos >= 0 && pos < size_)
	{
		T* new_array = new T[capacity_];
		for (size_type i = 0; i < size_; ++i)
		{
			if (i < pos)
			{
				new_array[i] = dataArray_[i];
			}

			else if (i >= pos && size_ > 1)
			{
				if (&dataArray_[i + 1] != this->end())
				{
					new_array[i] = dataArray_[i + 1];
				}
			}
		}
		delete[] dataArray_;
		dataArray_ = new_array;
		--size_;

		if (pos - 1 == size_) { return end(); }
		else 
		{ 
			return  &dataArray_[pos];
		}
	}
	else 
	{
		return nullptr;
	}
}
//Returns a reference to the
//first element in the vector
template<class T>
T& Vector<T>::front() {
	return *begin();
}

//Returns a reference to the
//last element in the vector
template<class T>
T& Vector<T>::back() {
	T* temp = this->end();
	--temp;
	return *temp;
}

//Method doubles the capacity the vector can have
//Sets the dataArray_ member to point to new memory locations
//twice the size of the old.
//No return values or parameters.
template<class T>
void Vector<T>::resize()
{
	capacity_ = capacity_ * 2;
	T* new_array = new T[capacity_];

	for (size_type i = 0; i < size_; ++i)
	{
		new_array[i] = dataArray_[i];
	}

	delete[] dataArray_;
	dataArray_ = new_array;
}

//Inserts item to the back of the vector
//If capacity is maximum, it will be doubled with resize()
//No return values
template<class T>
void Vector<T>::push_back(const T& obj)
{
	if (size_ == capacity_)
	{
		resize();
	}
	dataArray_[size_] = obj;
	++size_;
}

//Prints the contents of the vector, if the elements
//contained has defined << operator. No return values or params.
template <class T>
void Vector<T>::print() {
	for (T* ptr = begin(); ptr != end(); ++ptr)
	{
		std::cout << *ptr << std::endl;
	}
}

//Copies the contents,capacity and size of the parameter other vector 
//returns a reference to the new vector created based on the other.
template <class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {

	if (this == &other)
		return *this;

	T* new_array = new T[other.size_];
	size_ = other.size_;
	capacity_ = other.capacity_;

	for (size_type i = 0; i < size_; ++i)
	{
		new_array[i] = other.dataArray_[i];
	}

	std::swap(this->dataArray_, new_array);
	delete[] new_array;
	return *this;
}


//Clears vector, leaves capacity to last value but
//sets the size to 0
template <class T>
void Vector<T>::clear() {
	//for (size_type i = 0; i < size_; ++i)
	//{
		//(reinterpret_cast<T*>(dataArray_)[i].~T());
	//}
	size_ = 0;
}

//overloaded [] operator
template <class T>
T& Vector<T>::operator[](const size_type index) {
	return dataArray_[index];
}

//Returns reference to the the specific element of the parameter index
template <class T>
T& Vector<T>::at(const size_type index) {
	if (index >= size_ || index < 0)
	{
		throw std::out_of_range("index out of range!");
	}
	else
	{
		return dataArray_[index];
	}
}


