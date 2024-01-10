#include "IntArray.h"

IntArray::IntArray(int size): _size(size)
{
	(_size <= 0)? throw bad_lenght() : _data = new int[size];
}

IntArray::~IntArray()
{
	delete[] _data;
}

int& IntArray::operator[](int pos) 
{
	if (pos < 0 || pos > _size - 1 ) throw bad_range();
	else	return _data[pos];
	
}

int IntArray::getSize() const
{
	return _size;
}

int IntArray::search(int value)
{
	for (int i = 0; i < _size; ++i) {
		if (_data[i] == value) 
			return i;
		
	}
	return -1;
	
}

void IntArray::erase()
{
	delete[] _data;
	_data = nullptr;
	_size = 0;
}

void IntArray::reAllocate(int newSize)
{
	erase();
	if (newSize <= 0)
		return;
	_data = new int[newSize];
	_size = newSize;
}

void IntArray::resize(int newSize)
{
	if (newSize == _size)
		return;
	if (newSize <= 0) {
		erase();
		return;
	}
	int* data{ new int[newSize] };
	if (_size > 0) {
		int amount{ (newSize > _size) ? _size : newSize };
		copy_n(_data, amount, data);
	}
	delete[] _data;
	_data = data;
	_size = newSize;
}

void IntArray::remove(int pos)
{
	if (pos <= 0 || pos > _size) throw bad_range();
	if (_size == 1) {
		erase();
		return;
	}
	int* data{ new int[_size - 1] };
	copy_n(_data, pos, data);
	copy_n(_data + pos + 1, _size - pos - 1, data + pos);
	delete[] _data;
	_data = data;
	--_size;

}

void IntArray::insert(int value, int pos)
{
	if (pos < 0 || pos > _size) throw bad_range();
	int* data{ new int[_size + 1] };
	copy_n(_data, pos, data);
	data[pos] = value;
	copy_n(_data + pos, _size - pos, data + pos + 1);
	delete[] _data;
	_data = data;
	++_size;
}

IntArray& IntArray::operator=(const IntArray& a)
{
	if (&a == this)
		return *this;
	reAllocate(a.getSize());
	copy_n(a._data, _size, _data); 
	return *this;
}

void print(IntArray& a)
{
	for (int i = 0; i < a.getSize(); ++i)
		cout << a[i] << " ";
	cout << endl;
}

const char* bad_range::what() const noexcept
{
	return "Неверный индекс в массиве";
}

const char* bad_lenght::what() const noexcept
{
	return "Неверная длина массива";
}
