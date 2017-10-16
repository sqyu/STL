//
//  map.h
//  CC
//
//  Created by 清水 正巳 on H26/12/30.
//  Copyright (c) 平成26年 清水 正巳. All rights reserved.
//

#include <queue>
#include <stack>

template <class Key, class T, class Compare = std::less<Key>, class Alloc = std::allocator<std::pair<const Key, T> > >
class map{
    struct Node{
        Node *_left, *_right, *_parent;
        std::pair<const Key, T> _datum;
        size_t _lh, _rh;
        Node(std::pair<const Key, T> _datum):_left(nullptr),_right(nullptr),_parent(nullptr),_datum(_datum),_lh(0),_rh(0){}
        Node(Node *_parent, std::pair<const Key, T> _datum):_left(nullptr),_right(nullptr),_parent(_parent),_datum(_datum),_lh(0),_rh(0){}
        Node* max(Node *node) const{
            if (!node) return nullptr;
            Node *cur = node;
            while (cur -> _right)
                cur = cur -> _right;
            return cur;
        }
        Node* min(Node *node) const{
            if (!node) return nullptr;
            Node *cur = node;
            while (cur -> _left)
                cur = cur -> _left;
            return cur;
        }
        Node* nextnode(Node *node) const{
            Node* next = min(node -> _right);
            if (next) return next;
            Node *cur = node;
            next = cur -> _parent;
            while (next && next -> _left != cur){
                cur = next;
                next = next -> _parent;
            }
            assert(next != node);
            return next;
        }
        Node* prevnode(Node *node) const{
            Node* prev = max(node -> _left);
            if (prev) return prev;
            Node *cur = node;
            prev = cur -> _parent;
            while (prev && prev -> _right != cur){
                cur = prev;
                prev = prev -> _parent;
            }
            return prev;
        }
        inline size_t height() const{
            return std::max(_lh, _rh) + 1;
        }
        int balance(){
            if (_left) _lh = _left -> height();
            if (_right) _rh = _right -> height();
            //std::cout << (this -> _datum).first << ": _lh-_rh: " << _lh << " " << _rh << " bal: " << int(_lh) - int(_rh) << '\n';
            return int(_lh) - int(_rh);
        }
    };
    Compare _comp;
    Alloc _alloc;
    
public:
    Node* _root;
    size_t _size;
    typedef Key key_type;
    typedef T mapped_type;
    typedef std::pair<const key_type, mapped_type> value_type;
    typedef Compare key_compare;
    class value_comp{
        friend class map;
    protected:
        Compare comp;
        value_comp(Compare c): comp(c){}
    public:
        typedef bool result_type;
        typedef value_type first_argument_type;
        typedef value_type second_argument_type;
        bool operator()(const value_type& x, const value_type& y)const {return comp(x.first, y.first);}
    };
    typedef value_comp value_compare;
    typedef std::allocator<value_type> allocator_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    
    explicit map(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()):_comp(comp), _alloc(alloc), _root(nullptr), _size(0){}
    template <class InputIterator> map(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()):_comp(comp), _alloc(alloc), _root(nullptr), _size(0){
        insert(first, last);
    }
    map(const map& x){
        copyAll(x);
    }
    ~map(){clear();}
    
    class iterator;
    class const_iterator;
    class reverse_iterator;
    class const_reverse_iterator;
    
    mapped_type& at(const key_type& k);
    const mapped_type& at(const key_type& k) const;
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    void clear();
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;
    ////template <class... Args> std::pair<iterator,bool> emplace(Args&&... args); ////
    ////template <class... Args> iterator emplace_hint(const_iterator position, Args&&... args); ////
    bool empty() const;
    iterator end();
    const_iterator end() const;
    std::pair<const_iterator,const_iterator> equal_range(const key_type& k) const;
    std::pair<iterator,iterator> equal_range(const key_type& k);
    void erase(iterator position);
    size_t erase(const key_type& k);
    void erase(iterator first, iterator last);
    iterator find(const key_type& k);
    const_iterator find(const key_type& k) const;
    allocator_type get_allocator() const;
    std::pair<iterator,bool> insert(const value_type& val);
    ////iterator insert(iterator position, const value_type& val);
    template <class InputIterator> void insert(InputIterator first, InputIterator last);
    key_compare key_comp() const;
    iterator lower_bound(const key_type& k);
    const_iterator lower_bound(const key_type& k)const;
    size_t max_size() const;
    map& operator =(const map& x);
    mapped_type& operator[](const key_type& k);
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;
    size_t size() const;
    void swap(map& x);
    iterator upper_bound(const key_type& k);
    const_iterator upper_bound(const key_type& k) const;
    value_compare value_comp() const;
    
    void inorder() const;//Node *node) const;
    void preorder(Node *node) const;
    void postorder(Node *node) const;
    void print() const;
    void levelorder() const;
    
private:
    void copyAll(const map& x);
    Node* find_Node(Node *node, const key_type& k) const;
    void rotate_left(Node *node);
    void rotate_right(Node *node);
    void rotation(Node *node);
    void transplant(Node *old, Node *neu);
    
    bool _equal(key_type k1, key_type k2) const{
        Compare _comp = Compare();
        return !_comp(k1, k2) && !_comp(k2, k1);
    }
};

template <class Key, class T, class Compare, class Alloc>
class map<Key, T, Compare, Alloc>::iterator{
    friend class map;
    Node* cur;
public:
    iterator():cur(nullptr){}
    iterator(Node* cur):cur(cur){}
    value_type& operator *(){return cur -> _datum;}
    bool operator ==(const iterator& _rhs){return cur == _rhs.cur;}
    bool operator !=(const iterator& _rhs){return cur != _rhs.cur;}
    iterator& operator ++(){cur = cur -> nextnode(cur); return *this;}
    iterator operator ++(int){iterator old = *this; cur = cur -> nextnode(cur); return old;}
    iterator& operator --(){cur = cur -> prevnode(cur); return *this;}
    iterator operator --(int){iterator old = *this; cur = cur -> prevnode(cur); return old;}
};

template <class Key, class T, class Compare, class Alloc>
class map<Key, T, Compare, Alloc>::const_iterator{
    Node* cur;
public:
    const_iterator():cur(nullptr){}
    const_iterator(Node* cur):cur(cur){}
    const value_type& operator *(){return cur -> _datum;}
    bool operator ==(const const_iterator& _rhs){return cur == _rhs.cur;}
    bool operator !=(const const_iterator& _rhs){return cur != _rhs.cur;}
    const_iterator& operator ++(){cur = cur -> nextnode(cur); return *this;}
    const_iterator operator ++(int){const_iterator old = *this; cur = cur -> nextnode(cur); return old;}
    const_iterator& operator --(){cur = cur -> prevnode(cur); return *this;}
    const_iterator operator --(int){const_iterator old = *this; cur = cur -> prevnode(cur); return old;}
};

template <class Key, class T, class Compare, class Alloc>
class map<Key, T, Compare, Alloc>::reverse_iterator{
    Node* cur;
public:
    reverse_iterator():cur(nullptr){}
    reverse_iterator(Node* cur):cur(cur){}
    value_type& operator *(){return cur -> _datum;}
    bool operator ==(const reverse_iterator& _rhs){return cur == _rhs.cur;}
    bool operator !=(const reverse_iterator& _rhs){return cur != _rhs.cur;}
    reverse_iterator& operator --(){cur = cur -> nextnode(cur); return *this;}
    reverse_iterator operator --(int){reverse_iterator old = *this; cur = cur -> nextnode(cur); return old;}
    reverse_iterator& operator ++(){cur = cur -> prevnode(cur); return *this;}
    reverse_iterator operator ++(int){reverse_iterator old = *this; cur = cur -> prevnode(cur); return old;}
};

template <class Key, class T, class Compare, class Alloc>
class map<Key, T, Compare, Alloc>::const_reverse_iterator{
    Node* cur;
public:
    const_reverse_iterator():cur(nullptr){}
    const_reverse_iterator(Node* cur):cur(cur){}
    const value_type& operator *(){return cur -> _datum;}
    bool operator ==(const const_reverse_iterator& _rhs){return cur == _rhs.cur;}
    bool operator !=(const const_reverse_iterator& _rhs){return cur != _rhs.cur;}
    const_reverse_iterator& operator --(){cur = cur -> nextnode(cur); return *this;}
    const_reverse_iterator operator --(int){const_reverse_iterator old = *this; cur = cur -> nextnode(cur); return old;}
    const_reverse_iterator& operator ++(){cur = cur -> prevnode(cur); return *this;}
    const_reverse_iterator operator ++(int){const_reverse_iterator old = *this; cur = cur -> prevnode(cur); return old;}
};

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::mapped_type& map<Key, T, Compare, Alloc>::at(const key_type& k){
    iterator iter = find(k);
    if (iter != end())
        return (*iter).second;
    else
        throw std::out_of_range("map::at:  key not found");
}

template <class Key, class T, class Compare, class Alloc>
const typename map<Key, T, Compare, Alloc>::mapped_type& map<Key, T, Compare, Alloc>::at(const key_type& k) const{
    const_iterator iter = find(k);
    if (iter != cend())
        return (*iter).second;
    else
        throw std::out_of_range("map::at:  key not found");
}

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::iterator map<Key, T, Compare, Alloc>::begin(){
    return iterator(_root -> min(_root));
}

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::const_iterator map<Key, T, Compare, Alloc>::begin() const{
    return const_iterator(_root -> min(_root));
}

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::const_iterator map<Key, T, Compare, Alloc>::cbegin() const noexcept{
    return const_iterator(_root -> min(_root));
}

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::const_iterator map<Key, T, Compare, Alloc>::cend() const noexcept{
    return const_iterator();
}

template <class Key, class T, class Compare, class Alloc>
void map<Key, T, Compare, Alloc>::clear(){
    std::stack<Node*> st;
    if (_root) st.push(_root);
    while (!st.empty()){
        Node *cur = st.top();
        st.pop();
        if (cur -> _left)
            st.push(cur -> _left);
        if (cur -> _right)
            st.push(cur -> _right);
        delete cur;
    }
    _root = nullptr;
    _size = 0;
}

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::const_reverse_iterator map<Key, T, Compare, Alloc>::crbegin() const noexcept{
    return reverse_const_iterator(_root -> max(_root));
}

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::const_reverse_iterator map<Key, T, Compare, Alloc>::crend() const noexcept{
    return const_reverse_iterator();
}////

template <class Key, class T, class Compare, class Alloc>
void map<Key, T, Compare, Alloc>::copyAll(const map<Key, T, Compare, Alloc>& x){
    _size = x._size;
    _comp = x._comp;
    _alloc = x._alloc;
    if (!x._root){
        _root = nullptr;
        return;
    }
    _root = new Node((x._root) -> _datum);
    Node *help = x._root, *newhelp = _root;
    while (help){
        if (help -> _left && !(newhelp -> _left)){
            newhelp -> _left = new Node(newhelp, help -> _left -> _datum);
            help = help -> _left; newhelp = newhelp -> _left;
        }
        else if (help -> _right && !(newhelp -> _right)){
            newhelp -> _right = new Node(newhelp, help -> _right -> _datum);
            help = help -> _right; newhelp = newhelp -> _right;
        }
        else{
            help = help -> _parent; newhelp = newhelp -> _parent;
        }
    }
}

/*template <class Key, class T, class Compare, class Alloc>
template <class... Args> std::pair<typename map<Key, T, Compare, Alloc>::iterator,bool> map<Key, T, Compare, Alloc>::emplace(Args&&... args){
}////*/

/*template <class Key, class T, class Compare, class Alloc>
template <class... Args>
typename map<Key, T, Compare, Alloc>::iterator map<Key, T, Compare, Alloc>::emplace_hint(typename map<Key, T, Compare, Alloc>::const_iterator position, Args&&... args){
}////*/

template <class Key, class T, class Compare, class Alloc>
bool map<Key, T, Compare, Alloc>::empty() const{
    return !_size;
}

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::iterator map<Key, T, Compare, Alloc>::end(){
    return iterator();
}

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::const_iterator map<Key, T, Compare, Alloc>::end() const{
    return const_iterator();
}

template <class Key, class T, class Compare, class Alloc>
std::pair<typename map<Key, T, Compare, Alloc>::const_iterator, typename map<Key, T, Compare, Alloc>::const_iterator> map<Key, T, Compare, Alloc>::equal_range(const key_type& k) const{
    if (!_root) return std::pair<const_iterator,const_iterator>(const_iterator(), const_iterator());
    Node *next = _root, *upper = nullptr, *lower = nullptr;
    while (next){
        if (_comp(k, (next -> _datum).first)){
            upper = next;
            next = next -> _left;
        }
        else{
            if (!_comp((next -> _datum).first, k))
                lower = next;
            next = next -> _right;
        }
    }
    if (lower == nullptr)
        lower = upper;
    return std::pair<const_iterator,const_iterator>(const_iterator(lower),const_iterator(upper));
    //return std::pair<const_iterator,const_iterator>(lower_bound(k), upper_bound(k));
}////

template <class Key, class T, class Compare, class Alloc>
std::pair<typename map<Key, T, Compare, Alloc>::iterator, typename map<Key, T, Compare, Alloc>::iterator> map<Key, T, Compare, Alloc>::equal_range(const key_type& k){
    if (!_root) return std::pair<iterator,iterator>(iterator(), iterator());
    Node *next = _root, *upper = nullptr, *lower = nullptr;
    //std::cout << "ROOT" << (_root -> _datum).first << '\n';
    while (next){
        //std::cout << "WHEY" << k << " " << (next -> _datum).first << '\n';
        if (_comp(k, (next -> _datum).first)){
            upper = next;
            next = next -> _left;
        }
        else{
            if (!_comp((next -> _datum).first, k))
                lower = next;
            //std::cout << "NEXT " << (next -> _datum).first << " ";
            next = next -> _right;
            //if (next) std::cout << "NEXTNEXT " << (next -> _datum).first << " ";
        }
    }
    if (lower == nullptr)
        lower = upper;
    return std::pair<iterator,iterator>(iterator(lower),iterator(upper));
    //return std::pair<iterator,iterator>(lower_bound(k), upper_bound(k));
}////

template <class Key, class T, class Compare, class Alloc>
void map<Key, T, Compare, Alloc>::erase(typename map<Key, T, Compare, Alloc>::iterator position){
    assert(position != end());
    Node *removed = position.cur, *tobe = nullptr, *rot = nullptr;
    --_size;
    if (!(removed -> _left) && !(removed -> _right)){
        if (removed == _root) _root = nullptr;
        else{
            if (removed == removed -> _parent -> _left)
                removed -> _parent -> _left = nullptr;
            else
                removed -> _parent -> _right = nullptr;
        }
        delete removed;
        return;
    }
    else if (!(removed -> _left)){
        transplant(removed, removed -> _right);
        rot = tobe = removed -> _right;
    }
    else if (!(removed -> _right)){
        transplant(removed, removed -> _left);
        rot = tobe = removed -> _left;
    }
    else{
        tobe = removed -> _right -> min(removed -> _right);
        rot = tobe;
        if (tobe != removed -> _right){ // Takes care of the _right subtree of the inordersuccessor
            rot = tobe -> _parent;
            transplant(tobe, tobe -> _right);
            Node *fix = rot;
            --(rot -> _lh);
            while (fix != removed){
                fix -> balance();
                fix = fix -> _parent;
            }
            tobe -> _right = removed -> _right;
            tobe -> _right -> _parent = tobe;
        }
        transplant(removed, tobe);
        tobe -> _left = removed -> _left;
        tobe -> _left -> _parent = tobe;
    }
    rotation(rot);
    delete removed;
}

template <class Key, class T, class Compare, class Alloc>
size_t map<Key, T, Compare, Alloc>::erase(const map<Key, T, Compare, Alloc>::key_type& k){
    iterator iter = find(k);
    if (iter == end())
        return 0;
    erase(iter);
    return 1;
}

template <class Key, class T, class Compare, class Alloc>
void map<Key, T, Compare, Alloc>::erase(typename map<Key, T, Compare, Alloc>::iterator first, typename map<Key, T, Compare, Alloc>::iterator last){
    while (first != last){
        iterator old = first++;
        erase(old);
    }
}

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::iterator map<Key, T, Compare, Alloc>::find(const typename map<Key, T, Compare, Alloc>::key_type& k){
    return iterator(find_Node(_root, k));
}

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::const_iterator map<Key, T, Compare, Alloc>::find(const typename map<Key, T, Compare, Alloc>::key_type& k) const{
    return const_iterator(find_Node(_root, k));
}

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::Node* map<Key, T, Compare, Alloc>::find_Node(typename map<Key, T, Compare, Alloc>::Node* node, const typename map<Key, T, Compare, Alloc>::key_type& k) const{
    /*if (node == nullptr || (node -> _datum).first == k)
        return node;
    return ((node -> _datum).first > k) ? find_Node(node -> _left, k) : find_Node(node -> _right, k);*/
    while (node != nullptr){
        if (_comp(k, (node -> _datum).first)) node = node -> _left;
        else if (_comp((node -> _datum).first, k)) node = node -> _right;
        else return node;
    }
    return node;
}

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::allocator_type map<Key, T, Compare, Alloc>::get_allocator() const{
    return _alloc;
}

template <class Key, class T, class Compare, class Alloc>
std::pair<typename map<Key, T, Compare, Alloc>::iterator,bool> map<Key, T, Compare, Alloc>::insert(const typename map<Key, T, Compare, Alloc>::value_type& val){
    Node *cur = nullptr, *next = _root;
    while (next != nullptr){
        if (_comp(val.first, (next -> _datum).first)){cur = next; next = next -> _left;}
        else if (_comp((next -> _datum).first, val.first)){cur = next; next = next -> _right;}
        else break;
    }
    if (next != nullptr)
        return std::make_pair<iterator,bool>(iterator(next),false);
    ++_size;
    if (cur == nullptr){
        _root = new Node(val);
        return std::make_pair<iterator,bool>(iterator(_root),true);
    }
    Node *newnode = new Node(cur, val);
    if (_comp(val.first, (cur -> _datum).first))
        cur -> _left = newnode;
    else
        cur -> _right = newnode;
    //std::cout << "HERE ";
    cur -> balance();
    rotation(cur -> _parent);
    return std::make_pair<iterator,bool>(newnode,true);
}

/*template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::iterator insert(typename map<Key, T, Compare, Alloc>::iterator position, const typename map<Key, T, Compare, Alloc>::value_type& val){
}////*/

template <class Key, class T, class Compare, class Alloc>
template <class InputIterator>
void map<Key, T, Compare, Alloc>::insert(InputIterator first, InputIterator last){
    while (first != last)
        insert(*first++);
}

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::key_compare map<Key, T, Compare, Alloc>::key_comp() const{
    return _comp;
}

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::iterator map<Key, T, Compare, Alloc>::lower_bound(const typename map<Key, T, Compare, Alloc>::key_type& k){
    if (!_root) return iterator();
    Node *next = _root, *watershed = nullptr;
    while (next){
        if (_comp((next -> _datum).first, k))
            next = next -> _right;
        else if (_comp(k, (next -> _datum).first)){
            watershed = next;
            next = next -> _left;
        }
        else
            return iterator(next);
    }
    return iterator(watershed);
}

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::const_iterator map<Key, T, Compare, Alloc>::lower_bound(const typename map<Key, T, Compare, Alloc>::key_type& k) const{
    if (!_root) return const_iterator();
    Node *next = _root, *watershed = nullptr;
    while (next){
        if (_comp((next -> _datum).first, k))
            next = next -> _right;
        else if (_comp(k, (next -> _datum).first)){
            watershed = next;
            next = next -> _left;
        }
        else
            return const_iterator(next);
    }
    return const_iterator(watershed);
}

template <class Key, class T, class Compare, class Alloc>
size_t map<Key, T, Compare, Alloc>::max_size() const{
    return size_t(-1);
}

template <class Key, class T, class Compare, class Alloc>
map<Key, T, Compare, Alloc>& map<Key, T, Compare, Alloc>::operator =(const map<Key, T, Compare, Alloc>& x){
    if (this == &x) return *this;
    clear();
    copyAll(x);
    return *this;
}

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::mapped_type& map<Key, T, Compare, Alloc>::operator[](const key_type& k){
    iterator iter = find(k);
    if (iter != end())
        return (*iter).second;
    else
        return (*(insert(std::pair<Key, T>(k, T())).first)).second;
}

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::reverse_iterator map<Key, T, Compare, Alloc>::rbegin(){
    return reverse_iterator(_root -> max(_root));
}

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::const_reverse_iterator map<Key, T, Compare, Alloc>::rbegin() const{
    return const_reverse_iterator(_root -> max(_root));
}

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::reverse_iterator map<Key, T, Compare, Alloc>::rend(){
    return reverse_iterator();
}

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::const_reverse_iterator map<Key, T, Compare, Alloc>::rend() const{
    return const_reverse_iterator(_root -> max(_root));
}

template <class Key, class T, class Compare, class Alloc>
void map<Key, T, Compare, Alloc>::rotate_left(Node* node){
    Node*& P = node;
    Node*& RC = node -> _right;
    P -> _rh = RC -> _lh;
    RC -> _lh = P -> height();
    if (P -> _parent){
        if (P -> _parent -> _left == P)
            P -> _parent -> _left = RC;
        else
            P -> _parent -> _right = RC;
    }
    else
        _root = RC;
    //////std::cerr << " &" << (RC -> _datum).first << "& ";
    RC -> _parent = P -> _parent;
    P -> _parent = RC;
    if (RC -> _left)
        RC -> _left -> _parent = P;
    Node *tmp = RC -> _left;
    RC -> _left = P;
    P -> _right = tmp;
}

template <class Key, class T, class Compare, class Alloc>
void map<Key, T, Compare, Alloc>::rotate_right(Node* node){
    Node*& P = node;
    Node*& LC = node -> _left;
    P -> _lh = LC -> _rh;
    LC -> _rh = P -> height();
    if (P -> _parent){
        if (P -> _parent -> _left == P)
            P -> _parent -> _left = LC;
        else
            P -> _parent -> _right = LC;
    }
    else
        _root = LC;
    LC -> _parent = P -> _parent;
    P -> _parent = LC;
    if (LC -> _right)
        LC -> _right -> _parent = P;
    Node *tmp = LC -> _right;
    LC -> _right = P;
    P -> _left = tmp;
}

template <class Key, class T, class Compare, class Alloc>
void map<Key, T, Compare, Alloc>::rotation(Node *node){
        while (node){
        int bal = node -> balance();
        //////std::cout << "HAHA " << (node -> _datum).first << "has bal " << bal << '\n';
        if (bal > 1){
            if (node -> _left -> balance() < 0)
                rotate_left(node -> _left);
            rotate_right(node);
            //////std::cout << "HAHA\n";
            break;
        }
        else if (bal < -1){
            if (node -> _right -> balance() > 0)
                rotate_right(node -> _right);
            rotate_left(node);
            //////std::cout << "HAHA\n";
            break;
        }
        node = node -> _parent;
    }
}

template <class Key, class T, class Compare, class Alloc>
size_t map<Key, T, Compare, Alloc>::size() const{
    return _size;
}

template <class Key, class T, class Compare, class Alloc>
void map<Key, T, Compare, Alloc>::transplant(Node *old, Node *neu){
    if (old -> _parent){
        if (old -> _parent -> _left == old)
            old -> _parent -> _left = neu;
        else
            old -> _parent -> _right = neu;
    }
    else
        _root = neu;
    if (neu) // Possible that neu == nullptr for the _right subtree of the inorder successor
        neu -> _parent = old -> _parent;
}

template <class Key, class T, class Compare, class Alloc>
void map<Key, T, Compare, Alloc>::swap(map<Key, T, Compare, Alloc>& x){
    std::swap(_root,x._root);
    std::swap(_size,x._size);
}

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::iterator map<Key, T, Compare, Alloc>::upper_bound(const map<Key, T, Compare, Alloc>::key_type& k){
    if (!_root) return iterator();
    Node *next = _root, *watershed = nullptr;
    while (next){
        if (!_comp(k, (next -> _datum).first)){
            next = next -> _right;
        }else{
            watershed = next;
            next = next -> _left;
        }
    }
    return iterator(watershed);
}////

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::const_iterator map<Key, T, Compare, Alloc>::upper_bound(const map<Key, T, Compare, Alloc>::key_type& k) const{
    if (!_root) return const_iterator();
    Node *next = _root, *watershed = nullptr;
    while (next){
        if (!_comp(k, (next -> _datum).first))
            next = next -> _right;
        else{
            watershed = next;
            next = next -> _left;
        }
    }
    return const_iterator(watershed);
}////

template <class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::value_compare map<Key, T, Compare, Alloc>::value_comp() const{
    return value_comp();
}


template <class Key, class T, class Compare, class Alloc>
void map<Key, T, Compare, Alloc>::inorder() const{//Node *node) const{
    /*if (!node) return;
    inorder(node -> _left);
    std::cout << (node -> _datum).first << ": " << (node -> _datum).second << " ";
    inorder(node -> _right);*/
    for (const_iterator iter = begin(); iter != end(); ++iter)
        std::cout << (*iter).first << ": " << (*iter).second << " ";
}

template <class Key, class T, class Compare, class Alloc>
void map<Key, T, Compare, Alloc>::levelorder() const{
    if (!_root) return;
    std::queue<Node *> q;
    q.push(_root);
    while (!q.empty()){
        Node *node = q.front();
        q.pop();
        std::cout << (node -> _datum).first << ": " << (node -> _datum).second << " ";
        if (node -> _left) q.push(node -> _left);
        if (node -> _right) q.push(node -> _right);
    }
}

template <class Key, class T, class Compare, class Alloc>
void map<Key, T, Compare, Alloc>::preorder(Node *node) const{
    if (!node) return;
    std::cout << (node -> _datum).first << ": " << (node -> _datum).second << " ";
    preorder(node -> _left);
    preorder(node -> _right);
}

template <class Key, class T, class Compare, class Alloc>
void map<Key, T, Compare, Alloc>::postorder(Node *node) const{
    if (!node) return;
    postorder(node -> _left);
    postorder(node -> _right);
    std::cout << (node -> _datum).first << ": " << (node -> _datum).second << " ";
}

template <class Key, class T, class Compare, class Alloc>
void map<Key, T, Compare, Alloc>::print() const{
    std::cout << "\n***************\n";
    std::cout << "Size: " << _size << '\n';
    std::cout << "Inorder: "; inorder();
    std::cout << "\nPreorder: "; preorder(_root);
    std::cout << "\nPostorder: "; postorder(_root);
    std::cout << "\nLevelorder: "; levelorder();
    std::cout << "\n***************\n";
}

template <class Key, class T, class Compare, class Alloc>
bool operator== (const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs){
    if (lhs.size() != rhs.size()) return false;
    for (typename map<Key,T,Compare,Alloc>::const_iterator iter = lhs.cbegin(); iter != lhs.cend(); ++iter){
        typename map<Key,T,Compare,Alloc>::const_iterator iter2 = rhs.find((*iter).first);
        if (iter2 == rhs.cend() || (*iter).second != (*iter2).second)
            return false;
    }
    return true;
}

template <class Key, class T, class Compare, class Alloc>
bool operator!= (const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs){
    return !(lhs == rhs);
}

template <class Key, class T, class Compare, class Alloc>
bool operator< (const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs){
    typename map<Key,T,Compare,Alloc>::const_iterator lit = lhs.cbegin(), rit = rhs.cbegin();
    for (; lit != lhs.cend() && rit != rhs.cend(); ++lit, ++rit){
        if ((*lit).second < (*rit).second) return true;
        else if ((*rit).second < (*lit).second) return false;
    }
    if (lit == lhs.cend() && rit != rhs.cend()) return true;
    return false;
}

template <class Key, class T, class Compare, class Alloc>
bool operator<= (const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs){
    return !(rhs < lhs);
}

template <class Key, class T, class Compare, class Alloc>
bool operator> (const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs){
    return (rhs < lhs);
}

template <class Key, class T, class Compare, class Alloc>
bool operator>= (const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs){
    return !(lhs < rhs);
}

template <class Key, class T, class Compare, class Alloc>
void swap (map<Key,T,Compare,Alloc>& x, map<Key,T,Compare,Alloc>& y){
    x.swap(y);
}
