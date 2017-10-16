//
//  vector.h
//  CC
//
//  Created by 清水 正巳 on H26/12/27.
//  Copyright (c) 平成26年 清水 正巳. All rights reserved.
//

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <memory>

template <typename value_type>
class vector{
    value_type* data;
    size_t Size, Capacity;
public:
    vector(size_t len = 0): Size(0), Capacity(len){data = len ? new value_type[len] : nullptr;}
    vector(const vector& v){clear(); copyAll(v);}
    ~vector(){clear();}
    
    class iterator;
    class const_iterator;
    class reverse_iterator;
    class const_reverse_iterator;
    
    template <class InputIterator> void assign(InputIterator first, InputIterator last);
    void assign(size_t n, const value_type& val);
    value_type& at(size_t n);
    const value_type& at(size_t n) const;
    value_type& back();
    const value_type& back() const;
    iterator begin();
    const_iterator begin() const;
    size_t capacity() const;
    const_iterator cbegin() const;
    const_iterator cend() const;
    void clear();
    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;
    iterator emplace(const_iterator position, const value_type& val);
    void emplace_back(const value_type& val);
    bool empty() const;
    iterator end();
    const_iterator end() const;
    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);
    iterator erase(const_iterator position);
    iterator erase(const_iterator first, const_iterator last);
    value_type& front();
    const value_type& front() const;
    std::allocator<value_type> get_allocator() const;
    iterator insert(iterator position, const value_type& val);
    void insert(iterator position, size_t n, const value_type& val);
    template <class InputIterator> void insert(iterator position, InputIterator first, InputIterator last);
    size_t max_size() const;
    vector& operator =(const vector& x);
    value_type& operator [](size_t n);
    const value_type& operator [](size_t n) const;
    void pop_back();
    void print();
    void push_back(const value_type& val);
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    void removeAll();
    reverse_iterator rend();
    const_reverse_iterator rend() const;
    void resize(size_t n, value_type val = value_type());
    void reserve(size_t n);
    void shrink_to_fit();
    size_t size() const;
    void swap(vector<value_type>& x);
    
    iterator last_iter(){return iterator(Size - 1, this);}
    
private:
    void changeCapacity(size_t n);
    value_type* copy(value_type* data2, size_t data2size, size_t Capacity);
    void copyAll(const vector& v);
    void insertHelper(iterator position, size_t n, const value_type& val, bool double_capacity);
};

template <typename value_type>
class vector<value_type>::iterator{
    friend class vector<value_type>;
    int index;
    const vector<value_type>* vec;
public:
    iterator():index(-1), vec(nullptr){}
    iterator(const vector<value_type>* vec):index(-1),vec(vec){}
    iterator(int i, const vector<value_type>* vec):index(i), vec(vec){}
    iterator(const iterator& rhs):index(rhs.index), vec(rhs.vec){}
    iterator& operator ++(){
        ++index;
        if (index == vec -> Size)
            index = -1;
        return *this;
    }
    iterator operator ++(int){
        iterator iter(index, vec);
        ++index;
        if (index == vec -> Size)
            index = -1;
        return iter;
    }
    iterator& operator --(){--index; return *this;}
    iterator& operator --(int){--index; return iterator(index + 1);}
    value_type& operator *(){return vec -> data[index];}
    bool operator ==(const iterator& i){return index == i.index;}
    bool operator !=(const iterator& i){return index != i.index;}
    iterator& operator =(const iterator& rhs){if (this == &rhs) return *this;
        index = rhs.index; vec = rhs.vec; return *this;}
};

template <typename value_type>
class vector<value_type>::const_iterator{
    friend class vector<value_type>;
    int index;
    const vector<value_type>* vec;
public:
    const_iterator(const vector<value_type>* vec):index(-1),vec(vec){}
    const_iterator(int i, const vector<value_type>* vec):index(i), vec(vec){}
    const_iterator(const iterator& iter){this -> index = iter.index; this -> vec = iter.vec;}
    const_iterator& operator ++(){
        ++index;
        if (index == vec -> Size)
            index = -1;
        return *this;
    }
    const_iterator operator ++(int){
        const_iterator iter(index, vec);
        ++index;
        if (index == vec -> Size)
            index = -1;
        return iter;
    }
    const_iterator& operator --(){--index; return *this;}
    const_iterator& operator --(int){--index; return const_iterator(index + 1);}
    value_type& operator *(){return vec -> data[index];}
    bool operator ==(const const_iterator& i){return index == i.index;}
    bool operator !=(const const_iterator& i){return index != i.index;}
};

template <typename value_type>
class vector<value_type>::reverse_iterator{
    friend class vector<value_type>;
    int index;
    const vector<value_type>* vec;
public:
    reverse_iterator(const vector<value_type>* vec):index(-1),vec(vec){}
    reverse_iterator(int i, const vector<value_type>* vec):index(i), vec(vec){}
    reverse_iterator& operator --(){
        ++index;
        if (index == vec -> Size)
            index = -1;
        return *this;
    }
    reverse_iterator operator --(int){
        reverse_iterator iter(index, vec);
        ++index;
        if (index == vec -> Size)
            index = -1;
        return iter;
    }
    reverse_iterator& operator ++(){--index; return *this;}
    reverse_iterator& operator ++(int){--index; return reverse_iterator(index + 1);}
    value_type& operator *(){return vec -> data[index];}
    bool operator ==(const reverse_iterator& i){return index == i.index;}
    bool operator !=(const reverse_iterator& i){return index != i.index;}
};

template <typename value_type>
class vector<value_type>::const_reverse_iterator{
    friend class vector<value_type>;
    int index;
    const vector<value_type>* vec;
public:
    const_reverse_iterator(const vector<value_type>* vec):index(-1),vec(vec){}
    const_reverse_iterator(int i, const vector<value_type>* vec):index(i), vec(vec){}
    const_reverse_iterator& operator --(){
        ++index;
        if (index == vec -> Size)
            index = -1;
        return *this;
    }
    const_reverse_iterator operator --(int){
        const_reverse_iterator iter(index, vec);
        ++index;
        if (index == vec -> Size)
            index = -1;
        return iter;
    }
    const_reverse_iterator& operator ++(){--index; return *this;}
    const_reverse_iterator& operator ++(int){--index; return const_reverse_iterator(index + 1);}
    const value_type& operator *() const{return vec -> data[index];}
    bool operator ==(const const_reverse_iterator& i){return index == i.index;}
    bool operator !=(const const_reverse_iterator& i){return index != i.index;}
};

template <typename value_type>
template <class InputIterator>
void vector<value_type>::assign(InputIterator first, InputIterator last){
    clear();
    while (first != last)
        push_back(*(first++));
}

template <typename value_type>
void vector<value_type>::assign(size_t n, const value_type& val){
    clear();
    while (n--)
        push_back(val);
}

template <typename value_type>
value_type& vector<value_type>::at(size_t n){
    if (n >= 0 && n < Size) //// THROW
        return data[n];
    throw std::out_of_range("vector");
}

template <typename value_type>
const value_type& vector<value_type>::at(size_t n) const{
    if (n >= 0 && n < Size) //// THROW
        return data[n];
    throw std::out_of_range("vector");
}

template <typename value_type>
value_type& vector<value_type>::back(){
    assert(Size);
    return data[Size - 1];
}

template <typename value_type>
const value_type& vector<value_type>::back() const{
    assert(Size);
    return data[Size - 1];
}

template <typename value_type>
typename vector<value_type>::iterator vector<value_type>::begin(){
    if (Size)
        return iterator(0, this);
    else
        return iterator(this);
}

template <typename value_type>
typename vector<value_type>::const_iterator vector<value_type>::begin() const{
    if (Size)
        return const_iterator(0, this);
    else
        return const_iterator(this);
}

template <typename value_type>
size_t vector<value_type>::capacity() const{
    return Capacity;
}

template <typename value_type>
typename vector<value_type>::const_iterator vector<value_type>::cbegin() const{
    if (Size)
        return const_iterator(0, this);
    else
        return const_iterator(this);
}

template <typename value_type>
typename vector<value_type>::const_iterator vector<value_type>::cend() const{
    return const_iterator(this);
}

template <typename value_type>
void vector<value_type>::changeCapacity(size_t n){
    Capacity = n;
    value_type *newdata = copy(data, Size, n);
    delete[] data;
    data = newdata;
}

template <typename value_type>
void vector<value_type>::clear(){
    // Destruct all elts
    Size = 0;
}

template <typename value_type>
value_type* vector<value_type>::copy(value_type* data2, size_t data2size, size_t capa){
    value_type *data1 = capa ? new value_type[capa] : nullptr;
    for (size_t i = 0; i < data2size; ++i)
        data1[i] = data2[i];
    return data1;
}

template <typename value_type>
void vector<value_type>::copyAll(const vector& v){
    Size = v.Size;
    Capacity = Size;
    data = copy(v.data, Size, Size);
}

template <typename value_type>
typename vector<value_type>::const_reverse_iterator vector<value_type>::crbegin() const{
    return const_reverse_iterator(Size - 1, this);
}

template <typename value_type>
typename vector<value_type>::const_reverse_iterator vector<value_type>::crend() const{
    return const_reverse_iterator(this);
}

template <typename value_type>
typename vector<value_type>::iterator vector<value_type>::emplace(const_iterator position, const value_type& val){
    return insert(position, val);
} ////

template <typename value_type>
void vector<value_type>::emplace_back(const value_type& val){
    push_back(val);
}

template <typename value_type>
bool vector<value_type>::empty() const{
    return Size;
}

template <typename value_type>
typename vector<value_type>::iterator vector<value_type>::end(){
    return iterator(this);
}

template <typename value_type>
typename vector<value_type>::const_iterator vector<value_type>::end() const{
    return const_iterator(this);
}

template <typename value_type>
typename vector<value_type>::iterator vector<value_type>::erase(iterator position){
    return erase(static_cast<const_iterator>(position));
}

template <typename value_type>
typename vector<value_type>::iterator vector<value_type>::erase(iterator first, iterator last){
    return erase(static_cast<const_iterator>(first), static_cast<const_iterator>(last));
}

template <typename value_type>
typename vector<value_type>::iterator vector<value_type>::erase(const_iterator position){
    if (position == cend()) // Can be removed, causing undefined behavior if position == end() as in std::vector
        return end();
    const_iterator old = position++;
    return erase(old, position);
}

template <typename value_type>
typename vector<value_type>::iterator vector<value_type>::erase(const_iterator first, const_iterator last){
    if (first == last)
        return iterator(first.index, this);
    if (last.index == -1){
        Size = first.index;
        return iterator(this);
    }
    int left = first.index, right = last.index;
    for (size_t pos = left; pos < Size - (right - left); ++pos)
        data[pos] = data[pos + right - left];
    Size -= (right - left);
    return iterator(left, this);
}

template <typename value_type>
value_type& vector<value_type>::front(){
    assert(Size);
    return data[0];
}

template <typename value_type>
const value_type& vector<value_type>::front() const{
    assert(Size);
    return data[0];
}

template <typename value_type>
std::allocator<value_type> vector<value_type>::get_allocator() const{
    return std::allocator<value_type>();
}

template <typename value_type>
void vector<value_type>::insertHelper(iterator position, size_t n, const value_type& val, bool double_capacity){
    if (double_capacity)
        assert(n == 1);
    size_t newCapacity = double_capacity ? 2 * Capacity : Size + n;
    size_t pivot = (position.index == -1) ? Size : position.index;
    value_type* newdata = (Size + n > Capacity) ? new value_type[newCapacity] : data;
    for (int s = int(Size) - 1; s >= int(pivot) - 1; --s)
        newdata[s + n] = data[s];
    for (size_t s = 0; s < n; ++s)
        newdata[pivot + s] = val;
    if (data != newdata){
        Capacity = newCapacity;
        for (size_t s = 0; s < pivot; ++s)
            newdata[s] = data[s];
        delete[] data;
        data = newdata;
        newdata = nullptr;
    }
    Size = Size + n;
}

template <typename value_type>
typename vector<value_type>::iterator vector<value_type>::insert(iterator position, const value_type& val){
    size_t n = 1;
    insertHelper(position, n, val, false);
    if (position.index == -1)
        position.index = int(Size - 1);
    return position;
}

template <typename value_type>
void vector<value_type>::insert(iterator position, size_t n, const value_type& val){
    insertHelper(position, n, val, false);
}

template <typename value_type>
template <class InputIterator>
void vector<value_type>::insert(iterator position, InputIterator first, InputIterator last){
    bool end = (position.index == -1);
    size_t n = 1;
    while (first != last){
        insertHelper(position, n, *first++, true);
    
        if (!end)
            ++position;
    }
}

template <typename value_type>
size_t vector<value_type>::max_size() const{
    return size_t(-1);
}

template <typename value_type>
vector<value_type>& vector<value_type>::operator =(const vector& x){
    if (this == &x)
        return *this;
    if (Capacity < x.Size){
        removeAll();
        copyAll(x);
    }
    else{
        Size = x.Size;
        for (size_t i = 0; i < Size; ++i)
            data[i] = x.data[i];
    }
    return *this;
}

template <typename value_type>
value_type& vector<value_type>::operator [](size_t n){
    if (n < Size && n >= 0)
        return data[n];
    assert(true);
    return data[n];
}

template <typename value_type>
const value_type& vector<value_type>::operator [](size_t n) const{
    if (n < Size && n >= 0)
        return data[n];
    assert(true);
    return data[n];
}

template <typename value_type>
void vector<value_type>::pop_back(){
    assert(Size);
    --Size;
}

template <typename value_type>
void vector<value_type>::print(){
    std::cout << "Data: ";
    /*for (size_t i = 0; i < Size; ++i)
        std::cout << data[i] << " ";*/
    for (const_iterator iter = cbegin(); iter != cend(); ++iter)
        std::cout << *iter << " ";
    std::cout << "Size: " << Size << " Capacity: " << Capacity << std::endl;
}

template <typename value_type>
void vector<value_type>::push_back(const value_type& val){
    if (Size + 1 < Capacity)
        data[Size++] = val;
    else{
        Capacity = Size ? 2 * Size : 1;
        value_type *newdata = new value_type[Capacity];
        for (size_t i = 0; i < Size; ++i)
            newdata[i] = data[i];
        newdata[Size++] = val;
        delete[] data;
        data = newdata;
        newdata = nullptr;
    }
}

template <typename value_type>
typename vector<value_type>::reverse_iterator vector<value_type>::rbegin(){
    return reverse_iterator(Size - 1, this);
}

template <typename value_type>
typename vector<value_type>::const_reverse_iterator vector<value_type>::rbegin() const{
    return const_reverse_iterator(Size - 1, this);
}

template <typename value_type>
void vector<value_type>::removeAll(){
    if (data)
        delete[] data;
    data = nullptr;
    Size = Capacity = 0;
}

template <typename value_type>
typename vector<value_type>::reverse_iterator vector<value_type>::rend(){
    return reverse_iterator(this);
}

template <typename value_type>
typename vector<value_type>::const_reverse_iterator vector<value_type>::rend() const{
    return const_reverse_iterator(this);
}

template <typename value_type>
void vector<value_type>::resize(size_t n, value_type val){
    if (n > Size){
        if (n > Capacity){
            Capacity = n;
            value_type *newdata = new value_type[Capacity];
            size_t i = 0;
            for (; i < Size; ++i)
                newdata[i] = data[i];
            for (; i < Capacity; ++i)
                newdata[i] = val;
            delete[] data;
            data = newdata;
            newdata = nullptr;
        }
        else
            for (size_t i = Size; i < n; ++i)
                data[i] = val;
    }
    Size = n;
}

template <typename value_type>
void vector<value_type>::reserve(size_t n){
    if (n <= Capacity)
        return;
    changeCapacity(n);
}

template <typename value_type>
void vector<value_type>::shrink_to_fit(){
    if (Size == Capacity)
        return;
    changeCapacity(Size);
}

template <typename value_type>
size_t vector<value_type>::size() const{
    return Size;
}

template <typename value_type>
void vector<value_type>::swap(vector<value_type>& x){
    std::swap(data, x.data);
    std::swap(Size, x.Size);
    std::swap(Capacity, x.Capacity);
} //// ITERATOR NOT VALID AFTER SWAP!!!!!!

template <class T>
bool operator== (const vector<T>& lhs, const vector<T>& rhs){
    if (lhs.size() != rhs.size())
        return false;
    for (typename vector<T>::const_iterator liter = lhs.begin(), riter = rhs.begin(); liter != lhs.end(); ++liter, ++riter)
        if (!(*liter == *riter))
            return false;
    return true;
}

template <class T>
bool operator!= (const vector<T>& lhs, const vector<T>& rhs){
    return !(lhs == rhs);
}

template <class T>
bool operator< (const vector<T>& lhs, const vector<T>& rhs){
    typename vector<T>::const_iterator liter = lhs.begin(), riter = rhs.begin();
    while (liter != lhs.end() && riter != rhs.end()){
        if (*liter < *riter)
            return true;
        else if (*liter > *riter)
            return false;
        ++liter; ++riter;
    }
    if (liter == lhs.end() && riter != rhs.end())
        return true;
    return false;
}

template <class T>
bool operator<= (const vector<T>& lhs, const vector<T>& rhs){
    return !(rhs < lhs);
}

template <class T>
bool operator> (const vector<T>& lhs, const vector<T>& rhs){
    return rhs < lhs;
}

template <class T>
bool operator>= (const vector<T>& lhs, const vector<T>& rhs){
    return !(lhs < rhs);
}

template <class T>
void swap(vector<T>& x, vector<T>& y){
    x.swap(y);
}
