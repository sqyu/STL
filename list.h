#include <cassert>
#include <cstdlib>
#include <functional>
#include <memory>

template <typename value_type>
class list{
    struct Node{
        Node *prev, *next;
        value_type datum;
        Node (value_type datum):datum(datum){}
    };
    Node *head, *tail;
    
public:
    
    class basic_iterator;
    class iterator;
    class const_iterator;
    class reverse_iterator;
    class const_reverse_iterator;
    
    list():head(nullptr), tail(nullptr){}
    list(const list& l);
    ~list();
    
    template <class InputIterator> void assign(InputIterator first, InputIterator last);
    void assign(size_t n, const value_type& val);
    value_type& back();
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    const_iterator cend() const;
    void clear();
    void copyAll(const list<value_type> &l);
    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;
    //template <class... Args> iterator emplace(const_iterator position, Args&&... args);
    iterator emplace(const_iterator position, const value_type& val);
    void emplace_back(const value_type& val);
    void emplace_front(const value_type& val);
    bool empty();
    iterator end();
    const_iterator end() const;
    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);
    value_type& front();
    std::allocator<value_type> get_allocator() const;
    iterator insert(iterator position, const value_type& val);
    void insert(iterator position, size_t n, const value_type& val);
    template <class InputIterator> void insert(iterator position, InputIterator first, InputIterator last);
    size_t max_size();
    void merge(list& x);
    template <class Compare> void merge(list& x, Compare comp);
    list& operator =(const list& l);
    void pop_back();
    void pop_front();
    void print();
    void push_back(const value_type& val);
    void push_front(const value_type& val);
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    void remove(const value_type& val);
    template <class Predicate> void remove_if(Predicate pred);
    reverse_iterator rend();
    const_reverse_iterator rend() const;
    void resize(size_t n, value_type val = value_type());
    void reverse();
    void reverse_print();
    size_t size() const;
    void sort();
    template <class Compare> void sort(Compare comp); ////
    void splice(iterator position, list& x);
    void splice(iterator position, list& x, iterator i);
    void splice(iterator position, list& x, iterator first, iterator last);
    list* split();
    void swap(list& x);
    void unique();
    template <class BinaryPredicate> void unique(BinaryPredicate binary_pred);
    
    iterator last_iter(){return iterator(tail);}
};

template <typename value_type>
class list<value_type>::basic_iterator{
    friend class list<value_type>;
protected:
    Node *ptr;
public:
    basic_iterator():ptr(nullptr){}
    basic_iterator(Node *ptr):ptr(ptr){}
    bool operator ==(basic_iterator& rhs){
        return this -> ptr == rhs.ptr;
    }
    bool operator !=(basic_iterator& rhs){
        return this -> ptr != rhs.ptr;
    }
};

template <typename value_type>
class list<value_type>::iterator: public basic_iterator{
public:
    iterator():basic_iterator(){}
    iterator(Node *ptr):basic_iterator(ptr){}
    iterator(const iterator& rhs):basic_iterator(rhs.ptr){}
    value_type& operator *() const{
        return this -> ptr -> datum;
    }
    list<value_type>::iterator& operator++(){
        this -> ptr = this -> ptr -> next;
        return *this;
    }
    list<value_type>::iterator operator++(int){
        iterator prev(this -> ptr);
        this -> ptr = this -> ptr -> next;
        return prev;
    }
    list<value_type>::iterator& operator--(){
        this -> ptr = this -> ptr -> prev;
        return *this;
    }
    list<value_type>::iterator operator--(int){
        iterator next(this -> ptr);
        this -> ptr = this -> ptr -> prev;
        return next;
    }
    bool operator ==(const list<value_type>::iterator& rhs) const{
        return this -> ptr == rhs.ptr;
    }
    bool operator !=(const list<value_type>::iterator& rhs) const{
        return this -> ptr != rhs.ptr;
    }
    list<value_type>::iterator& operator =(const list<value_type>::iterator& rhs){
        if (this == &rhs) return *this;
        this -> ptr = rhs.ptr; return *this;
    }
};

template <typename value_type>
class list<value_type>::const_iterator: public basic_iterator{
public:
    const_iterator():basic_iterator(){}
    const_iterator(Node *ptr):basic_iterator(ptr){}
    const_iterator(const iterator& iter){this -> ptr = iter.ptr;}
    const value_type& operator *() const{
        return this -> ptr -> datum;
    }
    list<value_type>::const_iterator& operator++(){
        this -> ptr = this -> ptr -> next;
        return *this;
    }
    list<value_type>::const_iterator operator++(int){
        const_iterator prev(this -> ptr);
        this -> ptr = this -> ptr -> next;
        return prev;
    }
    list<value_type>::const_iterator& operator--(){
        this -> ptr = this -> ptr -> prev;
        return *this;
    }
    list<value_type>::const_iterator operator--(int){
        const_iterator next(this -> ptr);
        this -> ptr = this -> ptr -> prev;
        return next;
    }
    bool operator ==(const list<value_type>::const_iterator& rhs) const{
        return this -> ptr == rhs.ptr;
    }
    bool operator !=(const list<value_type>::const_iterator& rhs) const{
        return this -> ptr != rhs.ptr;
    }
};

template <typename value_type>
class list<value_type>::reverse_iterator: public basic_iterator{
public:
    reverse_iterator():list<value_type>::basic_iterator(){}
    reverse_iterator(Node *ptr):basic_iterator(ptr){}
    value_type& operator *() const{
        return this -> ptr.datum();
    }
    list<value_type>::reverse_iterator& operator++(){
        this -> ptr = this -> ptr -> prev;
        return *this;
    }
    list<value_type>::reverse_iterator operator++(int){
        reverse_iterator prev(this -> ptr);
        this -> ptr = this -> ptr -> prev;
        return prev;
    }
    list<value_type>::reverse_iterator& operator--(){
        this -> ptr = this -> ptr -> next;
        return *this;
    }
    list<value_type>::reverse_iterator operator--(int){
        iterator next(this -> ptr);
        this -> ptr = this -> ptr -> next;
        return next;
    }
    bool operator ==(const list<value_type>::reverse_iterator& rhs) const{
        return this -> ptr == rhs.ptr;
    }
    bool operator !=(const list<value_type>::reverse_iterator& rhs) const{
        return this -> ptr != rhs.ptr;
    }
};

template <typename value_type>
class list<value_type>::const_reverse_iterator: public basic_iterator{
public:
    const_reverse_iterator():basic_iterator(){}
    const_reverse_iterator(Node *ptr):basic_iterator(ptr){}
    const_reverse_iterator(reverse_iterator iter){this -> ptr = iter.ptr;} // For testing purpose only
    const value_type& operator *() const{
        return this -> ptr.datum();
    }
    list<value_type>::const_reverse_iterator& operator++(){
        this -> ptr = this -> ptr -> prev;
        return *this;
    }
    list<value_type>::const_reverse_iterator operator++(int){
        const_reverse_iterator prev(this -> ptr);
        this -> ptr = this -> ptr -> prev;
        return prev;
    }
    list<value_type>::const_reverse_iterator& operator--(){
        this -> ptr = this -> ptr -> next;
        return *this;
    }
    list<value_type>::const_reverse_iterator operator--(int){
        const_reverse_iterator next(this -> ptr);
        this -> ptr = this -> ptr -> next;
        return next;
    }
    bool operator ==(const list<value_type>::const_reverse_iterator& rhs) const{
        return this -> ptr == rhs.ptr;
    }
    bool operator !=(const list<value_type>::const_reverse_iterator& rhs) const{
        return this -> ptr != rhs.ptr;
    }
};

template <typename value_type>
list<value_type>::~list(){
    clear();
}

template <class value_type>
template <class InputIterator>
void list<value_type>::assign(InputIterator first, InputIterator last){
    clear();
    while (first != last)
        push_back(*(first++));
}

template <class value_type>
void list<value_type>::assign(size_t n, const value_type& val){
    clear();
    while (n--)
        push_back(val);
}

template <typename value_type>
value_type& list<value_type>::back(){
    return tail -> datum;
}

template <typename value_type>
typename list<value_type>::iterator list<value_type>::begin(){
    return iterator(head);
}

template <typename value_type>
typename list<value_type>::const_iterator list<value_type>::begin() const{
    return const_iterator(head);
}

template <typename value_type>
typename list<value_type>::const_iterator list<value_type>::cbegin() const{
    return const_iterator(head);
}

template <typename value_type>
typename list<value_type>::const_iterator list<value_type>::cend() const{
    return const_iterator();
}

template <typename value_type>
void list<value_type>::clear(){
    while (!empty())
        pop_back();
}

template <typename value_type>
void list<value_type>::copyAll(const list<value_type> &l){
    for (list<value_type>::const_iterator iter = l.begin(); iter != l.cend(); ++iter){
        push_back(*iter);
    }
}

template <typename value_type>
typename list<value_type>::const_reverse_iterator list<value_type>::crbegin() const{
    return const_reverse_iterator(tail);
}

template <typename value_type>
typename list<value_type>::const_reverse_iterator list<value_type>::crend() const{
    return const_reverse_iterator();
}

/*template <typename TYPE, class... Args>
typename list<value_type>::iterator emplace(typename list<value_type>::const_iterator position, Args&&... args){
    value_type *t = nullptr;
    std::allocator_traits<value_type> alloc;
    alloc.construct(alloc, t, std::forward<Args>(args)...);
    std::cout << sizeof(t);
    return list<value_type>::iterator();
}*/

template <typename value_type>
typename list<value_type>::iterator list<value_type>::emplace(typename list<value_type>::const_iterator position, const value_type& val){
    if (position == cend()){
        push_back(val);
        return list<value_type>::iterator(tail);
    }
    else if (position == cbegin()){
        push_front(val);
        return begin();
    }
    else{
        Node* newNode = new Node(val);
        newNode -> prev = position.ptr -> prev;
        newNode -> next = position.ptr;
        newNode -> prev -> next = newNode;
        newNode -> next -> prev = newNode;
        return list<value_type>::iterator(newNode);
    }
}

template <typename value_type>
void list<value_type>::emplace_back(const value_type& val){
    push_back(val);
}

template <typename value_type>
void list<value_type>::emplace_front(const value_type& val){
    push_front(val);
}

template <typename value_type>
bool list<value_type>::empty(){
    return (head == nullptr);
}

template <typename value_type>
typename list<value_type>::iterator list<value_type>::end(){
    return iterator();
}

template <typename value_type>
typename list<value_type>::const_iterator list<value_type>::end() const{
    return const_iterator();
}

template <typename value_type>
typename list<value_type>::iterator list<value_type>::erase(iterator position){
    if (position == end())
        return end();
    else if (position.ptr == tail){
        pop_back();
        return end();
    }
    else if (position.ptr == head){
        pop_front();
        return begin();
    }
    else{
        Node *tmp = position.ptr;
        iterator ret(tmp -> next);
        tmp -> prev -> next = tmp -> next;
        tmp -> next -> prev = tmp -> prev;
        delete tmp;
        return ret;
    }
}

template <typename value_type>
typename list<value_type>::iterator list<value_type>::erase(list<value_type>::iterator first, list<value_type>::iterator last){
    Node *prev = first.ptr -> prev, *next = last.ptr;
    while (first != last){
        Node *cur = first.ptr;
        ++first;
        delete cur;
    }
    if (prev)
        prev -> next = next;
    else    // Head deleted
        head = next;
    if (next)
        next -> prev = prev;
    else
        tail = prev;
    return list<value_type>::iterator(next);
}

template <typename value_type>
value_type& list<value_type>::front(){
    return head -> datum;
}

template <typename value_type>
std::allocator<value_type> list<value_type>::get_allocator() const{
    return std::allocator<value_type>();
}

template <typename value_type>
typename list<value_type>::iterator list<value_type>::insert(list<value_type>::iterator position, const value_type& val){
    return emplace(position, val);
}

template <typename value_type>
void list<value_type>::insert(list<value_type>::iterator position, size_t n, const value_type& val){
    while (n--)
        emplace(position, val);
}

template <typename value_type>
template <class InputIterator>
void list<value_type>::insert(list<value_type>::iterator position, InputIterator first, InputIterator last){
    while (first != last){
        position = insert(position, *first);
        ++position;
        ++first;
    }
}

template <typename value_type>
list<value_type>::list(const list<value_type> &l){
    head = tail = nullptr;
    copyAll(l);
}

template <typename value_type>
size_t list<value_type>::max_size(){
    return size_t(-1);
}

template <typename value_type>
void list<value_type>::merge(list<value_type>& x){
    merge(x, std::less<value_type>());
}

template <typename value_type>
template <class Compare>
void list<value_type>::merge(list<value_type>& x, Compare comp){
    list<value_type>::iterator iter = begin(), xiter = x.begin();
    while (iter != end() && xiter != x.end()){
        if (comp(*xiter, *iter)){
            list<value_type>::iterator xnextiter = list<value_type>::iterator(xiter.ptr -> next);
            splice(iter, x, xiter);
            xiter = xnextiter;
        }
        else
            ++iter;
    }
    if (xiter != x.end())
        splice(end(), x);
}

template <typename value_type>
list<value_type>& list<value_type>::operator =(const list<value_type> &l){
    if (&l == this)
        return *this;
    clear();
    copyAll(l);
    return *this;
}

template <typename value_type>
void list<value_type>::pop_back(){
    assert(!empty());
    Node *tmp = tail;
    tail = tail -> prev;
    delete tmp;
    if (!tail)
        head = nullptr;
    else
        tail -> next = nullptr;
}

template <typename value_type>
void list<value_type>::pop_front(){
    assert(!empty());
    Node *tmp = head;
    head = head -> next;
    delete tmp;
    if (!head)
        tail = nullptr;
    else
        head -> prev = nullptr;
}

template <typename value_type>
void list<value_type>::print(){
    if (empty()){
        std::cout << "List empty.\n";
        return;
    }
    Node *cur = head;
    while (cur != tail){
        std::cout << cur -> datum << " -> ";
        cur = cur -> next;
    }
    std::cout << cur -> datum << "\n";
}

template <typename value_type>
void list<value_type>::push_back(const value_type& val){
    Node *newNode = new Node(val);
    newNode -> next = nullptr;
    newNode -> prev = tail;
    if (empty())
        head = tail = newNode;
    else{
        tail -> next = newNode;
        tail = newNode;
    }
}

template <typename value_type>
void list<value_type>::push_front(const value_type& val){
    Node *newNode = new Node(val);
    newNode -> prev = nullptr;
    newNode -> next = head;
    if (empty())
        head = tail = newNode;
    else{
        head -> prev = newNode;
        head = newNode;
    }
}

template <typename value_type>
typename list<value_type>::reverse_iterator list<value_type>::rbegin(){
    return reverse_iterator(tail);
}

template <typename value_type>
typename list<value_type>::const_reverse_iterator list<value_type>::rbegin() const{
    return const_reverse_iterator(tail);
}

template <typename value_type>
void list<value_type>::remove(const value_type& val){
    for (list<value_type>::iterator iter = begin(); iter != end();){
        if (*iter == val)
            iter = erase(iter);
        else
            ++iter;
    }
}

template <typename value_type>
template <class Predicate>
void list<value_type>::remove_if(Predicate pred){
    for (list<value_type>::iterator iter = begin(); iter != end();){
        if (pred(*iter))
            iter = erase(iter);
        else
            ++iter;
    }
}

template <typename value_type>
typename list<value_type>::reverse_iterator list<value_type>::rend(){
    return reverse_iterator();
}

template <typename value_type>
typename list<value_type>::const_reverse_iterator list<value_type>::rend() const{
    return const_reverse_iterator();
}

template <typename value_type>
void list<value_type>::resize(size_t n, value_type val){
    size_t s = size();
    while (n-- > s)
        push_back(val);
    ++n;
    while (n++ < s)
        pop_back();
}

template <typename value_type>
void list<value_type>::reverse(){
    if (empty() || head == tail)
        return;
    Node *cur = head, *next = head -> next;
    while (next){
        Node *tmp = next -> next;
        cur -> prev = next;
        next -> next = cur;
        cur = next;
        next = tmp;
    }
    std::swap(head, tail);
    head -> prev = tail -> next = nullptr;
}

template <typename value_type>
void list<value_type>::reverse_print(){
    if (empty()){
        std::cout << "List empty.\n";
        return;
    }
    Node *cur = tail;
    while (cur != head){
        std::cout << cur -> datum << " <- ";
        cur = cur -> prev;
    }
    std::cout << cur -> datum << "\n";
}

template <typename value_type>
size_t list<value_type>::size() const{
    size_t count = 0;
    for (list<value_type>::const_iterator iter = begin(); iter != end(); ++iter)
        ++count;
    return count;
}

template <typename value_type>
void list<value_type>::sort(){
    sort(std::less<value_type>());
}


template <typename value_type>
template <class Compare>
void list<value_type>::sort(Compare comp){
    if (empty() || head == tail)
        return;
    list<value_type>::list *l2 = split(); //
    sort(comp); l2 -> sort(comp);
    merge(*l2, comp);
    delete l2;
}

template <typename value_type>
void list<value_type>::splice(list<value_type>::iterator position, list<value_type>& x){
    splice(position, x, x.begin(), x.end());
}

template <typename value_type>
void list<value_type>::splice(list<value_type>::iterator position, list<value_type>&x, list<value_type>::iterator i){
    Node *xprev = i.ptr -> prev, *xnext = i.ptr -> next;
    if (empty()){
        i.ptr -> prev = nullptr;
        i.ptr -> next = nullptr;
        head = tail = i.ptr;
        head -> prev = head -> next = nullptr;
    }
    else if (position == begin()){
        i.ptr -> prev = nullptr;
        i.ptr -> next = head;
        head -> prev = i.ptr;
        head = i.ptr;
    }
    else if (position == end()){
        i.ptr -> prev = tail;
        i.ptr -> next = nullptr;
        tail -> next = i.ptr;
        tail = i.ptr;
    }
    else{
        position.ptr -> prev -> next = i.ptr;
        i.ptr -> prev = position.ptr -> prev;
        position.ptr -> prev = i.ptr;
        i.ptr -> next = position.ptr;
    }
    if (xprev)
        xprev -> next = xnext;
    else
        x.head = xnext;
    if (xnext)
        xnext -> prev = xprev;
    else
        x.tail = xprev;
}

template <typename value_type>
void list<value_type>::splice(list<value_type>::iterator position, list<value_type>& x, list<value_type>::iterator first, list<value_type>::iterator last){
    if (x.empty() || first == last)
        return;
    Node *xprev = first.ptr -> prev, *xnext = last.ptr;
    Node *xfirst = first.ptr, *xlast = xnext ? xnext -> prev : x.tail;
    if (empty()){
        xfirst -> prev = nullptr;
        xlast -> next = nullptr;
        head = xfirst;
        tail = xlast;
    }
    else if (position == begin()){
        xfirst -> prev = nullptr;
        xlast -> next = head;
        head -> prev = xlast;
        head = xfirst;
    }
    else if (position == end()){
        xfirst -> prev = tail;
        xlast -> next = nullptr;
        tail -> next = xfirst;
        tail = xlast;
    }
    else{
        xfirst -> prev = position.ptr -> prev;
        xlast -> next = position.ptr;
        position.ptr -> prev -> next = xfirst;
        position.ptr -> prev = xlast;
    }
    if (xprev)
        xprev -> next = xnext;
    else
        x.head = xnext;
    if (xnext)
        xnext -> prev = xprev;
    else
        x.tail = xprev;
}

template <typename value_type>
list<value_type>* list<value_type>::split(){
    list<value_type>::Node *slow = head, *fast = head -> next;
    while (fast){
        slow = slow -> next;
        fast = fast -> next;
        if (fast)
            fast = fast -> next;
    }
    list<value_type>* newList = new list<value_type>;
    newList -> head = slow;
    newList -> tail = tail;
    tail = slow -> prev;
    newList -> head -> prev = tail -> next = nullptr;
    return newList;
}

template <typename value_type>
void list<value_type>::swap(list<value_type>& x){
    std::swap(head, x.head);
    std::swap(tail, x.tail);
}

template <typename value_type>
void list<value_type>::unique(){
    struct equal{
        bool operator ()(value_type t1, value_type t2){
            return t1 == t2;
        }
    };
    unique(equal());
}

template <typename value_type>
template <class BinaryPredicate>
void list<value_type>::unique(BinaryPredicate binary_pred){
    list<value_type>::iterator left = begin(), right = begin();
    ++right;
    while (right != end()){
        if (binary_pred(*left, *right)){
            right = erase(right);
        }
        else{
            ++right;
            ++left;
        }
    }
}

template <typename value_type>
bool operator ==(const list<value_type>& lhs, const list<value_type>& rhs){
    if (lhs.size() != rhs.size())
        return false;
    for (typename list<value_type>::const_iterator liter = lhs.begin(), riter = rhs.begin(); liter != lhs.end(); ++liter, ++riter){
        if (!(*liter == *riter))
            return false;
    }
    return true;
}

template <typename value_type>
bool operator !=(const list<value_type>& lhs, const list<value_type>& rhs){
    return !(lhs == rhs);
}

template <typename value_type>
bool operator <(const list<value_type>& lhs, const list<value_type>& rhs){
    typename list<value_type>::const_iterator liter = lhs.begin(), riter = rhs.begin();
    for (; liter != lhs.end() && riter != rhs.end(); ++liter, ++riter){
        if (*liter < *riter)
            return true;
        else if (*riter < *liter)
            return false;
    }
    if (liter == lhs.end() && riter != rhs.end())
        return true;
    else
        return false;
}

template <typename value_type>
bool operator <=(const list<value_type>& lhs, const list<value_type>& rhs){
    return !(rhs < lhs);
}

template <typename value_type>
bool operator >(const list<value_type>& lhs, const list<value_type>& rhs){
    return (rhs < lhs);
}

template <typename value_type>
bool operator >=(const list<value_type>& lhs, const list<value_type>& rhs){
    return !(lhs < rhs);
}

template <typename value_type>
void swap(list<value_type>& x, list<value_type>& y){
    x.swap(y);
}
