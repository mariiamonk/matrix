#pragma once

#include <iostream>
#include <cstddef> // size_t
#include <cstring> // strlen, strcpy
#include <algorithm>
#include <vector>
#include <iterator>
#include <type_traits>
#include <stdexcept>
#include <cstddef>

namespace t_m{
    template <class Type>
    class Matrix{
    private:
        size_t n;
        size_t m;
        size_t capasity;
        Type *data = nullptr;

        void resize(size_t newCapacity) {
            Type* newData = static_cast<Type*>(operator new[](newCapacity * sizeof(Type)));
            for (size_t i = 0; i < n * m; ++i) {
                new (newData + i) Type(std::move(data[i]));
                data[i].~Type();
            }
            for (size_t i = n * m; i < newCapacity; ++i) {
                new (newData + i) Type(); 
            }
            operator delete[](data);
            data = newData;
            capasity = newCapacity;
        }

    public:
        Matrix(size_t n = 0, size_t m = 0): n(n), m(m), capasity(2 * n * n), data(static_cast<Type*>(operator new[](capasity * sizeof(Type)))) {
            for (size_t i = 0; i < capasity; ++i) {
                new (data + i) Type();
            }
        }

        Matrix(const Matrix& other): n(other.n), m(other.m), capasity(other.capasity), data(static_cast<Type*>(operator new[](capasity * sizeof(Type)))) {
            for (size_t i = 0; i < n * m; ++i) {
                new (data + i) Type(other.data[i]);
            }
            for (size_t i = n * m; i < capasity; ++i) {
                new (data + i) Type();
            }   
        }

        Matrix(std::initializer_list<std::initializer_list<Type>> l): n(l.size()), m(l.begin()->size()), capasity(2 * n * n), data(static_cast<Type*>(operator new[](capasity * sizeof(Type)))) {
            for (const auto& row : l) {
                if (row.size() != m) {
                    throw std::invalid_argument("All rows must have the same number of columns");
                }
            }

            size_t index = 0;
            for (const auto& row : l) {
                for (const auto& value : row) {
                    new (data + index++) Type(value);
                }
            }

            for (size_t i = index; i < capasity; ++i) {
                new (data + i) Type();
            }
        }

        ~Matrix()
        {
            n = 0;
            m = 0;
            for (size_t i = 0; i < capasity; ++i) {
                data[i].~Type();
            }
            operator delete[](data);
            capasity = 0;
        }

        class Row 
        {
        private:
            Type *row_data;
            size_t cols;
        public:
            Row(Type *row_data, size_t cols) : row_data(row_data), cols(cols) {}

            Type &operator[](size_t j) {
                if (j >= cols)
                    throw std::invalid_argument("Column index out of range");
                return row_data[j];
            }

            const Type &operator[](size_t j) const {
                if (j >= cols)
                    throw std::invalid_argument("Column index out of range");
                return row_data[j];
            }
        };

        //перегрузка скобочек
        Row operator[](size_t i) 
        {
            if (i >= n)
                throw std::invalid_argument("Row index out of range");
            return Row(data + i * m, m);
        }

        const Row operator[](size_t i) const 
        {
            if (i >= n)
                throw std::invalid_argument("Row index out of range");
            return Row(data + i * m, m);
        }

        //вспомогательные методы
        void shift_rows_down(size_t index) {
            if (m * n + m >= capasity) {
                resize(capasity * 2);
            }

            for (size_t i = size() + 1; i > index; --i) {
                data[i].~Type();
                new (data + i) Type(std::move(data[i - 1]));
            }

            data[index].~Type();
            new (data + index) Type();
            n++;
        }

        //геттеры ;)
        size_t get_rows()     const {return n;}
        size_t get_cols()     const {return m;}
        size_t get_capasity() const {return capasity;}
        
        template <typename T, bool is_const>
        class MatrixIterator{
        private:
            typedef std::conditional_t<is_const, const T, T>* elem_ptr_t;
            elem_ptr_t elem;

            typedef ptrdiff_t difference_type;
            typedef T value_type;
            typedef std::conditional_t<is_const, const T, T>* pointer;
            typedef std::conditional_t<is_const, const T, T>& reference;
            typedef std::bidirectional_iterator_tag iterator_category;

            friend MatrixIterator<T, !is_const>;
        public:
            MatrixIterator() noexcept :     elem(nullptr){}
            MatrixIterator(elem_ptr_t elem):elem(elem){}

            reference operator*() const{return *elem;}
            pointer operator->()  const{return &elem;}

            MatrixIterator& operator=(MatrixIterator const &a){
                if(this != &a){
                    elem = a.elem;
                }
                return *this;
            }

            MatrixIterator& operator++(){
                elem++;
                return *this;
            }
            MatrixIterator operator++(int){
                MatrixIterator tmp(elem);
                elem++;
                return tmp;
            }
            MatrixIterator& operator--(){
                elem--;
                return *this;
            }
            MatrixIterator operator--(int){
                MatrixIterator tmp(elem);
                elem--;
                return tmp;
            }

            MatrixIterator operator+(int i){
                return MatrixIterator(elem+i);
            }
            MatrixIterator& operator+=(int i){
                elem += i;
                return *this;
            }
            MatrixIterator operator-(int i){
                return MatrixIterator(elem-i);
            }
            MatrixIterator& operator-=(int i){
                elem -= i;
                return *this;
            }

            MatrixIterator operator+(ptrdiff_t n) const {
                return MatrixIterator(elem + n);
            }

            MatrixIterator& operator+=(ptrdiff_t n) {
                elem += n;
                return *this;
            }

            MatrixIterator operator-(ptrdiff_t n) const {
                return MatrixIterator(elem - n);
            }

            MatrixIterator& operator-=(ptrdiff_t n) {
                elem -= n;
                return *this;
            }

            ptrdiff_t operator-(const MatrixIterator& other) const {
                return elem - other.elem;
            }

            bool operator==(const MatrixIterator<T, is_const> &other)const{
                return elem == other.elem;
            }
            bool operator!=(const MatrixIterator<T, is_const> &other)const{
                return elem != other.elem;
            }

            bool operator>(const MatrixIterator<T, is_const> &other)const{
                return elem > other.elem;
            }

            bool operator<(const MatrixIterator<T, is_const> &other)const{
                return elem < other.elem;
            }

            bool operator>=(const MatrixIterator<T, is_const> &other)const{
                return elem >= other.elem;
            }

            bool operator<=(const MatrixIterator<T, is_const> &other)const{
                return elem <= other.elem;
            }
        };

        typedef MatrixIterator<Type, false> iterator;
        typedef MatrixIterator<Type, true> const_iterator;

        size_t size() const {return n*m;}
        bool empty() const {return (m == 0) && (n == 0);}

        iterator begin()            {return iterator(data);}
        const_iterator begin()const {return iterator(data);}
        iterator end()              {return iterator(data + m*n);}
        const_iterator end()const   {return iterator(data + m*n);}
        
        void clear(){
            n = 0;
            m = 0;
            delete[]data;
            data  = new Type[capasity];
        }

        template <typename... Args, typename = std::enable_if_t<std::is_constructible_v<Type, Args...>>>
        iterator emplace(iterator p, Args&&... args)
        {
            if (p < begin() || p > end()) {
                throw std::out_of_range("Iterator is out of bounds");
            }
            if (n * m >= capasity) {resize(capasity*2);}

            size_t index = p - this->begin();

            for (size_t i = n * m; i > index; --i) {
                data[i] = std::move(data[i - 1]);
            }

            new (data + index) Type(std::forward<Args>(args)...);

            ++m;

            return iterator(data + index);
        }

        template <typename = std::enable_if_t<std::is_copy_constructible_v<Type>>>
        iterator insert(iterator pos, const Type& value)
        {
            return emplace(pos, value);
        }

        template <typename = std::enable_if_t<std::is_move_constructible_v<Type>>>
        iterator insert(iterator pos, const Type&& value)
        {
            return emplace(pos, std::move(value));
        }

        template <typename = std::enable_if_t<std::is_copy_constructible_v<Type>>>
        iterator insert(iterator p, size_t count, const Type& value) {
            if (p < begin() || p > end()) {
                throw std::out_of_range("Iterator is out of bounds");
            }

            if (count == 0) {
                return p;
            }

            size_t index = p - begin(); 
            size_t newSize = n * m + count;

            if (newSize > capasity) {
                resize(std::max(newSize, capasity * 2));
            } else {
                for (size_t i = n * m; i > index; --i) {
                    data[i + count - 1] = std::move(data[i - 1]);
                }
            }

            for (size_t i = 0; i < count; ++i) {
                data[index + i] = value;
            }

            m += count / n;
            if (count % n != 0) {
                ++m; 
            }
            return iterator(data + index);
        }
        
        template <typename = std::enable_if_t<std::is_copy_constructible_v<Type>>>
        iterator insert(iterator p, std::initializer_list<Type> l) {
            size_t newColSize = l.size();
            size_t index = p - begin(); 
            size_t rowIndex = index / m; 

            if (newColSize > m) {
                size_t oldCols = m;
                m = newColSize;

                for (size_t i = n * oldCols; i > 0; i -= oldCols) {
                    for (size_t j = 0; j < m - oldCols; ++j) {
                        shift_rows_down(i);
                    }
                }
            }

            if (n * m + m >= capasity) {
                resize(capasity * 2);
            }

            for (size_t i = n; i > rowIndex; --i) {
                for (size_t j = 0; j < m; ++j) {
                    data[i * m + j].~Type();
                    new (data + i * m + j) Type(std::move(data[(i - 1) * m + j]));
                }
            }

            size_t col = 0;
            for (const auto& elem : l) {
                new (data + rowIndex * m + col++) Type(elem);
            }

            for (; col < m; ++col) {
                new (data + rowIndex * m + col) Type();
            }

            ++n;
            return begin() + rowIndex * m;
        }

        
        template <typename = std::enable_if_t<std::is_copy_constructible_v<Type>>>
        iterator insert(iterator p, std::initializer_list<std::initializer_list<Type>> l) {
            size_t rowIndex = (p- begin()) / m; 
            for (const auto& row : l) {
                p = insert(begin() + rowIndex * m, row); 
                ++rowIndex;                              
            }
            return p;
        }

        iterator erase(iterator q) noexcept {
            size_t index = q - begin();

            if (index >= n * m) {
                throw std::out_of_range("Iterator is out of range");
            }

            for (size_t i = index; i < n * m - 1; ++i) {
                data[i] = data[i + 1];
            }

            data[n * m - 1] = 0;

            size_t row = index / m;
            size_t col = index % m;

            if (col == m - 1 && n * m - 1 < capasity) {
                --m;
                if (m == 0) {
                    --n;
                }
            }

            return begin() + index;
        }
    };
}
