//
//  unordered_map.h
//  CC
//
//  Created by 清水 正巳 on H26/12/29.
//  Copyright (c) 平成26年 清水 正巳. All rights reserved.
//

#include <cstdlib>
#include <iostream>
#include "list.h"
#include <string>
#include "vector.h"

template <class T1, class T2>
struct pair{
    T1 first;
    T2 second;
    pair():first(T1()), second(T2()){}
    pair(T1 t1, T2 t2):first(t1), second(t2){}
};

template <class T1, class T2>
pair<T1, T2> make_pair(T1 t1, T2 t2){
    return pair<T1, T2>(t1, t2);
}

struct hash{
    size_t operator()(const bool i) const{return i;}
    size_t operator()(const char i) const{return size_t(i);}
    size_t operator()(const signed char i) const{return size_t(i);}
    size_t operator()(const unsigned char i) const{return size_t(i);}
    size_t operator()(const char16_t i) const{return size_t(i);}
    size_t operator()(const char32_t i) const{return size_t(i);}
    size_t operator()(const wchar_t i) const{return size_t(i);}
    size_t operator()(const short i) const{return size_t(i);}
    size_t operator()(const unsigned short i) const{return size_t(i);}
    size_t operator()(const int i) const{return size_t(i);}
    size_t operator()(const unsigned int i) const{return size_t(i);}
    size_t operator()(const long i) const{return size_t(i);}
    size_t operator()(const unsigned long i) const{return i;}
    size_t operator()(const long long i) const{return i;}
    size_t operator()(const unsigned long long i) const{return i;}
    size_t operator()(const float i) const{return (size_t&)i;}
    size_t operator()(const double i) const{return (size_t&)i;}
    size_t operator()(const long double i) const{return (size_t&)i;}
    template <typename T> size_t operator()(const T* i) const{return (size_t&)i;}
    
    size_t operator()(std::string str) const{
        unsigned int a = 63689, b = 378551, hash = 0;
        for (char c : str){hash = hash * a + c; a *= b;}
        return (hash & 0x7FFFFFFF);
    }
    size_t operator()(std::wstring str) const{
        unsigned int a = 63689, b = 378551, hash = 0;
        for (char c : str){hash = hash * a + c; a *= b;}
        return (hash & 0x7FFFFFFF);
    }
    size_t operator()(std::u16string str) const{
        unsigned int a = 63689, b = 378551, hash = 0;
        for (char c : str){hash = hash * a + c; a *= b;}
        return (hash & 0x7FFFFFFF);
    }
    size_t operator()(std::u32string str) const{
        unsigned int a = 63689, b = 378551, hash = 0;
        for (char c : str){hash = hash * a + c; a *= b;}
        return (hash & 0x7FFFFFFF);
    }
};

template <class Key, class T, class Hash = hash, class Pred = std::equal_to<Key>,
        class Alloc = std::allocator<pair<Key, T> > > // const Key
class unordered_map{
    vector<list<pair<Key, T> >* >* buckets;
    size_t Size, Buckets;
    float MAX_load_factor;
    Hash _hasher;
    Pred _equal_to;
    
public:
    unordered_map(): Size(0), Buckets(0), MAX_load_factor(1), _hasher(Hash()), _equal_to(Pred()){buckets = new vector<list<pair<Key, T> >* >;}
    ~unordered_map(){if (buckets) for (auto l : *buckets) delete l; delete buckets;}
    typedef Key key_type;
    typedef T mapped_type;
    typedef pair<key_type, mapped_type> value_type; // const key_type
    typedef Hash hasher;
    typedef Pred key_equal;
    typedef Alloc allocator_type;
    typedef typename Alloc::reference reference;
    typedef typename Alloc::const_reference const_reference;
    typedef typename Alloc::pointer pointer;
    typedef typename Alloc::const_pointer const_pointer;
    
    class iterator{
        typename list<pair<Key, T> >::iterator lit;
        typename vector<list<pair<Key, T> >* >::iterator vit;
        vector<list<pair<Key, T> >* >* buckets;
    public:
        iterator(vector<list<pair<Key, T> >* >* buck, bool begin){
            buckets = buck;
            if (begin){
                vit = buckets -> begin();
                while (vit != buckets -> end() && (*vit) -> size() == 0)
                    ++vit;
                if (vit != buckets -> end())
                    lit = (*vit) -> begin();
            }
            else
                vit = buckets -> end();
        }
        iterator(typename list<pair<Key, T> >::iterator lit, typename vector<list<pair<Key, T> >* >::iterator vit, vector<list<pair<Key, T> >* >* buckets):lit(lit),vit(vit),buckets(buckets){}
        pair<Key, T>& operator *(){return *lit;}
        iterator& operator ++(){
            ++lit;
            while (lit == (*vit) -> end()){
                ++vit;
                if (vit != buckets -> end())
                    lit = (*vit) -> begin();
                else
                    break;
            }
            return *this;
        }
        iterator operator ++(int){
            iterator old = *this;
            ++lit;
            while (lit == (*vit) -> end()){
                ++vit;
                if (vit != buckets -> end())
                    lit = (*vit) -> begin();
                else
                    break;
            }
            return old;
        }
        iterator& operator --(){
            if (lit == vit -> begin()){--vit; lit = vit -> last_iter();}
            else --lit;
        }
        iterator operator --(int){
            iterator old = *this;
            if (lit == vit -> begin()){--vit; lit = vit -> last_iter();}
            else --lit;
        }
        bool operator ==(const iterator& rhs){return lit == rhs.lit && vit == rhs.vit;}
        bool operator !=(const iterator& rhs){return lit != rhs.lit || vit != rhs.vit;}
    };
    
    class local_iterator{
        typename list<pair<Key, T> >::iterator lit;
    public:
        local_iterator(typename list<pair<Key, T> >::iterator lit):lit(lit){}
        pair<Key, T>& operator *(){return *lit;}
        local_iterator& operator ++(){++lit; return *this;}
        local_iterator operator ++(int){return local_iterator(lit++);}
        local_iterator& operator --(){--lit; return *this;}
        local_iterator operator --(int){return local_iterator(lit--);}
        bool operator ==(const local_iterator& rhs){return lit == rhs.lit;}
        bool operator !=(const local_iterator& rhs){return lit != rhs.lit;}
    };
    
    class const_iterator{
        typename list<pair<Key, T> >::iterator lit;
        typename vector<list<pair<Key, T> >* >::iterator vit;
        vector<list<pair<Key, T> >* >* buckets;
    public:
        const_iterator(vector<list<pair<Key, T> >* >* buck, bool begin){
            buckets = buck;
            if (begin){
                vit = buckets -> begin();
                while (vit != buckets -> end() && (*vit) -> size() == 0)
                    ++vit;
                if (vit != buckets -> end())
                    lit = (*vit) -> begin();
            }
            else
                vit = buckets -> end();
        }
        const_iterator(typename list<pair<Key, T> >::iterator lit, typename vector<list<pair<Key, T> >* >::iterator vit, vector<list<pair<Key, T> >* >* buckets):lit(lit),vit(vit),buckets(buckets){}
        const_iterator(const iterator& iter):lit(iter.lit), vit(iter.vit), buckets(iter.buckets){}
        const pair<Key, T>& operator *() const{return *lit;}
        const_iterator& operator ++(){
            ++lit;
            while (lit == (*vit) -> end()){
                ++vit;
                if (vit != buckets -> end())
                    lit = (*vit) -> begin();
                else
                    break;
            }
            return *this;
        }
        const_iterator operator ++(int){
            const_iterator old = *this;
            ++lit;
            while (lit == (*vit) -> end()){
                ++vit;
                if (vit != buckets -> end())
                    lit = (*vit) -> begin();
                else
                    break;
            }
            return old;
        }
        const_iterator& operator --(){
            if (lit == vit -> begin()){--vit; lit = vit -> last_iter();}
            else --lit;
        }
        const_iterator operator --(int){
            const_iterator old = *this;
            if (lit == vit -> begin()){--vit; lit = vit -> last_iter();}
            else --lit;
        }
        bool operator ==(const const_iterator& rhs){return lit == rhs.lit && vit == rhs.vit;}
        bool operator !=(const const_iterator& rhs){return lit != rhs.lit || vit != rhs.vit;}
    };

    class const_local_iterator{
        typename list<pair<Key, T> >::iterator lit;
    public:
        const_local_iterator(typename list<pair<Key, T> >::iterator lit):lit(lit){}
        pair<Key, T>& operator *(){return *lit;}
        const_local_iterator& operator ++(){++lit; return *this;}
        const_local_iterator operator ++(int){return const_local_iterator(lit++);}
        const_local_iterator& operator --(){--lit; return *this;}
        const_local_iterator operator --(int){return const_local_iterator(lit--);}
        bool operator ==(const const_local_iterator& rhs){return lit == rhs.lit;}
        bool operator !=(const const_local_iterator& rhs){return lit != rhs.lit;}
    };
    
    /*class reverse_iterator{
        typename list<pair<Key, T> >::iterator lit;
        typename vector<list<pair<Key, T> > >::iterator vit;
    public:
        reverse_iterator(bool begin){
            if (begin){
                vit = this -> buckets -> last_iter();
                if (vit != this -> buckets -> end()) lit = vit -> last_iter();
            }
            else vit = this -> buckets -> end();
        }
        pair<Key, T>& operator *(){return *lit;}
        reverse_iterator& operator --(){
            ++lit;
            if (lit == vit -> end()){++vit; lit = vit -> begin();}
            return *this;
        }
        reverse_iterator operator --(int){
            reverse_iterator old = *this;
            ++lit;
            if (lit == vit -> end()){++vit; lit = vit -> begin();}
            return old;
        }
        reverse_iterator& operator ++(){
            if (lit == vit -> begin()){--vit; if (vit != this -> buckets -> end()) lit = vit -> last_iter();}
            else --lit;
        }
        reverse_iterator operator ++(int){
            reverse_iterator old = *this;
            if (lit == vit -> begin()){--vit; if (vit != this -> buckets -> end()) lit = vit -> last_iter();}
            else --lit;
        }
        bool operator ==(const reverse_iterator& rhs){return lit == rhs.lit && vit == rhs.vit;}
        bool operator !=(const reverse_iterator& rhs){return lit != rhs.lit || vit != rhs.vit;}
    };*/
    
    mapped_type& at(const key_type& k){
        iterator iter = find(k);
        if (iter != end())
            return (*iter).second;
        throw std::out_of_range("unordered_map::at: key not found");
    }
    const mapped_type& at(const key_type& k) const{
        iterator iter = find(k);
        if (iter != end())
            return (*iter).second;
        throw std::out_of_range("unordered_map::at: key not found");
    }
    iterator begin() noexcept{return iterator(buckets, true);}
    const_iterator begin() const noexcept{return const_iterator(buckets, true);}
    local_iterator begin(size_t n){assert(n >= 0 && n < Buckets); return local_iterator((*buckets)[n] -> begin());}
    const_local_iterator begin(size_t n) const{assert(n >= 0 && n < Buckets); return const_local_iterator((*buckets)[n] -> begin());}
    size_t bucket(const key_type& k) const{return _hasher(k) % Buckets;}
    size_t bucket_count() const noexcept{return Buckets;}
    size_t bucket_size(size_t n) const{assert(n >= 0 && n < Size); return *(this -> buckets)[n].size();}
    const_iterator cbegin() const noexcept{return const_iterator(buckets, true);}
    const_local_iterator cbegin(size_t n){assert(n >= 0 && n < Buckets); return const_local_iterator((*buckets)[n] -> begin());}
    const_iterator cend() const noexcept{return const_iterator(buckets, false);}
    const_local_iterator cend(size_t n) const{assert(n >= 0 && n < Buckets); return const_local_iterator((*buckets)[n] -> end());}
    void clear() noexcept{for (auto l : *buckets) l -> clear(); Size = 0;}
    void copyAll(const unordered_map& ump){
        rehash(ump.Buckets); Size = ump.Size;
        for (iterator iter = ump.begin(); iter != ump.end(); ++iter)
            buckets[Hash((*iter).first) % Buckets].push_back(*iter);
    }
    size_t count(const key_type& k) const{return (find(k) != cend());}
    template <class... Args> pair<iterator, bool> emplace ( Args&&... args ); // ???
    template <class... Args> iterator emplace_hint ( const_iterator position, Args&&... args ); // ???
    bool empty() const noexcept{return !Size;};
    iterator end() noexcept{return iterator(buckets, false);}
    const_iterator end() const noexcept{return const_iterator(buckets, false);}
    local_iterator end(size_t n){assert(n >= 0 && n < Buckets); return local_iterator((*buckets)[n] -> end());}
    const_local_iterator end(size_t n) const{assert(n >= 0 && n < Buckets); return const_local_iterator((*buckets)[n] -> end());}
    pair<iterator,iterator> equal_range(const key_type& k){iterator iter = find(k), old = iter++; return make_pair<iterator,iterator>(old, iter);}
    pair<const_iterator,const_iterator> equal_range(const key_type& k) const{const_iterator iter = find(k), old = iter++; return make_pair<const_iterator,const_iterator>(old, iter);}
    iterator erase(const_iterator position){
        position.lit = position.vit -> erase(position.lit);
        --Size;
        if (position.lit == position.vit -> end()){
            ++position.vit;
            if (position.vit != buckets -> end())
                position.lit = position.vit -> begin();
        }
        return position;
    }
    size_t erase(const key_type& k){
        iterator iter = find(k);
        if (iter != end()){
            erase(iter);
            --Size;
            return 1;
        }
        else
            return 0;
    }
    iterator erase(const_iterator first, const_iterator last){
        iterator iter;
        while (first != last)
            iter = erase(first++);
        return iter;
    }
    iterator find(const key_type& k){
        iterator iter = begin();
        while (iter != end()){
            if (_equal_to((*iter).first, k))
                return iter;
            ++iter;
        }
        return iter;
    }
    const_iterator find(const key_type& k) const{
        const_iterator iter = cbegin();
        while (iter != cend()){
            if (_equal_to((*iter).first, k))
                return iter;
            ++iter;
        }
        return iter;
    }
    pair<iterator,bool> insert(const value_type& val){ // const value_type
        iterator iter = find(val.first);
        if (iter != end()) return make_pair<iterator, bool>(iter, false);
        if (Buckets == 0)
            rehash(2);
        else if (float(Size + 1) / Buckets > MAX_load_factor)
            rehash(2 * Buckets + 1);
        size_t bu = _hasher(val.first) % Buckets;
        (*buckets)[bu] -> push_back(make_pair<key_type, mapped_type>(val.first, val.second));
        ++Size;
        typename vector<list<pair<Key, T> >* >::iterator vit = buckets -> begin();
        for (size_t i = 0; i <= bu; ++i)
            ++vit;
        return make_pair<iterator, bool>(iterator((*buckets)[bu] -> last_iter(), vit, buckets), true);
    }
    template <class InputIterator> void insert(InputIterator first, InputIterator last){
        while (first != last)
            insert(*(first++));
    }
    float load_factor() const noexcept{return float(Size) / Buckets;}
    allocator_type get_allocator() const noexcept{return allocator_type();}
    hasher hash_function() const{return hasher();}
    key_equal key_eq() const{return _equal_to;}
    size_t max_bucket_count() const noexcept{return size_t(-1);} // ???
    float max_load_factor() const noexcept{return MAX_load_factor;}
    void max_load_factor(float z){
        MAX_load_factor = z;
        //if (double(Size) / Buckets > MAX_load_factor)   // ??? std NOT rehashing
        //    rehash(size_t(Size / MAX_load_factor) + 1);
    }
    size_t max_size() const noexcept{return size_t(-1);}
    unordered_map& operator =(const unordered_map& ump){
        if (this == &ump) return *this;
        clear(); copyAll(ump);
        return *this;
    }
    ////unordered_map& operator =(unordered_map&& ump); // ???
    ////unordered_map& operator =(std::initializer_list<value_type> il); // ???
    mapped_type& operator[](const key_type& k){ // // const key_type
        iterator iter = find(k);
        if (iter != end())
            return (*iter).second;
        else{
            return (*(insert(make_pair<key_type, mapped_type>(k, mapped_type())).first)).second;
        }
    }
    ////mapped_type& operator[](key_type&& k); // ???
    void print(){
        std::cout << "****** own ******\n";
        std::cout << Buckets << " buckets, " << Size << " elements.\n";
        for (size_t i = 0; i < Buckets; ++i){
            std::cout << "Bucket " << i << " contains ";
            for (const_local_iterator local = cbegin(i); local != cend(i); ++local)
                std::cout << (*local).first << ": " << (*local).second << " ";
            std::cout << '\n';
        }
        std::cout << "All contains ";
        for (const_iterator iter = cbegin(); iter != cend(); ++iter){
            std::cout << (*iter).first << ": " << (*iter).second << " ";
        }
        std::cout << "\n****** own ******\n\n";
    }
    void rehash(size_t n){
        if (n < Buckets) return;
        size_t newBucket = n;
        vector<list<pair<Key, T> >* >* newbucket = new vector<list<pair<Key, T> >* >;
        newbucket -> resize(newBucket);
        for (auto& l : *newbucket)
            l = new list<pair<Key, T> >;
        for (iterator iter = begin(); iter != end(); ++iter)
            (*newbucket)[_hasher((*iter).first) % newBucket] -> push_back(*iter);
        for (auto l : *(buckets))
            delete l;
        delete buckets;
        buckets = newbucket;
        Buckets = n;
        newbucket = nullptr;
    }
    size_t size() const noexcept{return Size;}
    void swap(unordered_map& ump){std::swap(buckets, ump.buckets); std::swap(Size, ump.Size); std::swap(Buckets, ump.Buckets); std::swap(MAX_load_factor, ump.MAX_load_factor);}
    void reserve(size_t n){
        if (float(n) / Buckets > MAX_load_factor)
            rehash(int(n / MAX_load_factor) + 1); // ???
    }
};

template <class Key, class T, class Hash, class Pred, class Alloc>
bool operator ==(const unordered_map<Key, T, Hash, Pred, Alloc>& lhs, const unordered_map<Key, T, Hash, Pred, Alloc>& rhs){
    if (lhs.size() != rhs.size())
        return false;
    for (typename unordered_map<Key, T, Hash, Pred, Alloc>::const_iterator iter = lhs.cbegin(); iter != lhs.cend(); ++iter){
        typename unordered_map<Key, T, Hash, Pred, Alloc>::const_iterator iter2 = rhs.find((*(iter)).first);
        if (iter2 == rhs.cend())
            return false;
        if ((*iter).second != (*iter2).second)
            return false;
    }
    return true;
}

template <class Key, class T, class Hash, class Pred, class Alloc>
bool operator !=(const unordered_map<Key, T, Hash, Pred, Alloc>& lhs, const unordered_map<Key, T, Hash, Pred, Alloc>& rhs){
    return !(lhs == rhs);
}

template <class Key, class T, class Hash, class Pred, class Alloc>
void swap(unordered_map<Key, T, Hash, Pred, Alloc>& lhs, unordered_map<Key, T, Hash, Pred, Alloc>& rhs){
    lhs.swap(rhs);
}
