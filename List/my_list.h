#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <new>
#include <type_traits>
#include <utility>

template <typename T>
struct list {
private:
    template <typename V>
    struct it;
public:
    typedef it<T> iterator;
    typedef it<const T> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    list() noexcept { }
    ~list() noexcept;
    list(list<T> const &);
    list& operator=(list<T>);
    void swap(list<T> &);

    bool empty() const noexcept;
    void clear() noexcept;

    T& front();
    T const & front() const;
    T& back();
    T const & back() const;

    void push_front(T const &);
    void push_back(T const &);
    void pop_front() noexcept;
    void pop_back() noexcept;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    reverse_iterator rbegin();
    reverse_iterator rend();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    iterator insert(const_iterator, T const &);
    iterator erase(const_iterator) noexcept;
    iterator erase(const_iterator, const_iterator) noexcept;
    void splice(const_iterator pos, list& other, const_iterator first, const_iterator last);

private:
    struct node_base {
        node_base *l, *r;
        node_base() : l(this), r(this) { }
        node_base(node_base* ll, node_base* rr) : l(ll), r(rr) { }
        virtual ~node_base() = default;
        T& get_val() {
            return static_cast<node&>(*this).val;
        }
        void init()
        {
            l->r = this;
            r->l = this;
        }
        void del()
        {
            l = this;
            r = this;
        }
    };

    struct node : node_base {
        T val;
        node(node_base* ll, node_base* rr, T const& value) : node_base(ll, rr), val(value) {}
    };

    node_base fake;

    template <typename V>
    struct it {
         typedef V value_type;
         typedef V &reference;
         typedef std::ptrdiff_t difference_type;
         typedef V *pointer;
         typedef std::bidirectional_iterator_tag iterator_category;

        it() {
            p_ = nullptr;
        }

        it& operator++() {
            p_ = p_->r;
            return *this;
        }
        it& operator--(	) {
            p_ = p_->l;
            return *this;
        }
        it operator++(int) {
            it t(*this);
            ++(*this);
            return t;
        }
        it operator--(int) {
            it t(*this);
            --(*this);
            return t;
        }

        friend bool operator==(it const& a, it const& b)
        {
            return a.p_ == b.p_;
        }

        friend bool operator!=(it const& a, it const& b)
        {
            return a.p_ != b.p_;
        }

        V& operator*() const {
            return p_->get_val();
        }

        V* operator->() const {
            return &p_->get_val();
        }
        template <typename U>
        it(it<U> const&, typename std::enable_if<std::is_same<U const, V>::value>::type* = nullptr);
    private:
        it(node_base* p) {
            p_ = p;
        }
        node_base* p_;
        friend struct list;
    };

    void write() const;

};

template <typename T>
list<T>::~list() noexcept {
    clear();
}

template <typename T>
list<T>::list(list<T> const & v) : list() {
    const_iterator first = v.begin();
    const_iterator last = v.end();
    for (; first != last; ++first) {
        T const& t = *first;
        push_back(t);
    }
}

template <typename T>
list<T> & list<T>::operator=(list<T> v) {
    //list tmp = v;
    swap(v);
    return *this;
}

template <typename T>
void list<T>::swap(list& other)
{
    if(empty()) {
        if(other.empty())
            return;

        fake = other.fake;
        fake.init();
        other.fake.del();
    } else {
        if (!other.empty())
        {
            std::swap(fake, other.fake);
            fake.init();
            other.fake.init();
        } else {
            other.fake = fake;
            other.fake.init();
            fake.del();
        }
    }
}

template <typename T>
bool list<T>::empty() const noexcept {
    return (fake.l == &fake && fake.r == &fake);
}

template <typename T>
void list<T>::clear() noexcept {
    if (fake.r == &fake)
        return;
    node_base* cur = fake.r;
    while (cur != &fake) {
        node_base* next = cur->r;
        delete static_cast<node*>(cur);
        cur = next;
    }
    fake.l = fake.r = &fake;
}

template <typename T>
T& list<T>::front() {
    return *begin();
}

template <typename T>
T const & list<T>::front() const {
    return *begin();
}

template <typename T>
T& list<T>::back() {
    return *(std::prev(end()));
}

template <typename T>
T const & list<T>::back() const {
    return *(std::prev(end()));
}

template <typename T>
void list<T>::push_front(T const & val) {
    insert(begin(), val);
}

template <typename T>
void list<T>::push_back(T const & val) {
    insert(end(), val);
}

template <typename T>
void list<T>::pop_front() noexcept {
    erase(begin());
}

template <typename T>
void list<T>::pop_back() noexcept {
     erase(std::prev(end()));
}

template <typename T>
typename list<T>::iterator list<T>::begin() {
    return iterator(fake.r);
}
template <typename T>
typename list<T>::const_iterator list<T>::begin() const {
    return const_iterator(fake.r);
}

template <typename T>
typename list<T>::iterator list<T>::end() {
    return iterator(&fake);
}

template <typename T>
typename list<T>::const_iterator list<T>::end() const {
    return const_iterator(const_cast<node_base*>(&fake));
}

template<typename T>
typename list<T>::reverse_iterator list<T>::rbegin() {
    return std::reverse_iterator<iterator>(end());
}

template<typename T>
typename list<T>::reverse_iterator list<T>::rend() {
    return std::reverse_iterator<iterator>(begin());
}

template<typename T>
typename list<T>::const_reverse_iterator list<T>::rbegin() const {
    return std::reverse_iterator<const_iterator>(end());
}

template<typename T>
typename list<T>::const_reverse_iterator list<T>::rend() const {
    return std::reverse_iterator<const_iterator>(begin());
}

template <typename T>
typename list<T>::iterator list<T>::insert(const_iterator pos, T const & val) {
    node* q = new node(pos.p_->l, pos.p_, val);
    q->l->r = q;
    q->r->l = q;
    return it<T>(q);
}

template <typename T>
typename list<T>::iterator list<T>::erase(list<T>::const_iterator pos) noexcept {
    node_base* ans = pos.p_->r;
    pos.p_->l->r = pos.p_->r;
    ans->l = pos.p_->l;
    delete static_cast<node*>(pos.p_);
    return iterator(ans);
}

template <typename T>
typename list<T>::iterator list<T>::erase(list<T>::const_iterator left, list<T>::const_iterator right) noexcept {
    node_base* prev = left.p_->l;
    while (left != right) {
        node_base* tmp = left.p_->r;
        erase(left);
        left = list<T>::const_iterator(tmp);
    }
    return list<T>::iterator(prev->r);
}

template <typename T>
void list<T>::splice(list<T>::const_iterator pos, list<T> & other, list<T>::const_iterator first, list<T>::const_iterator last) {
    if (first == last) {
        return;
    }

    node_base* pl = first.p_->l;
    node_base* pr = last.p_;

    node_base* ld = pos.p_;
    node_base* fd = ld->l;

    node_base* fs = first.p_;
    node_base* ls = last.p_->l;

    fd->r = fs;
    fs->l = fd;

    ld->l = ls;
    ls->r = ld;

    pl->r = pr;
    pr->l = pl;
}

/// creating it<V = U const> from it<U>
template <typename T> template <typename V> template <typename U>
list<T>::it<V>::it(it<U> const& other, typename std::enable_if<std::is_same<U const, V>::value>::type*)
    : p_(other.p_)
{}



template <typename T>
void list<T>::write() const {
    const_iterator it = begin();
    const_iterator end_ = end();
    while (it != end_) {
        std::cout << *it << " ";
        ++it;
    }
    std::cout << std::endl;
}

template <typename T>
void swap(list<T> & a, list<T> & b) {
    a.swap(b);
}

#endif // LIST_H
