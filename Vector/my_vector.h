#ifndef MY_VECTOR
#define MY_VECTOR

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <variant>
#include <iterator>

const size_t SMALLSIZ = 1;

template <typename T>
struct my_vector {
public:
    typedef T* iterator;
    typedef T const* const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    ~my_vector();
    my_vector();
    my_vector(my_vector const& other);
    my_vector& operator =(my_vector const& other);
    T &operator[](size_t i);
    T const& operator[](size_t i) const;
    const T & front() const;
    const T & back() const;
    T & front();
    T & back();
    T* data() noexcept;
    const T* data() const noexcept;
    void push_back(T const& a);
    void pop_back();
    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t capacity() const noexcept;
    void reserve(size_t sz);
    void shrink_to_fit();
    void resize(size_t sz, T a);
    void clear() noexcept;
    iterator insert(const_iterator pos, T const& val);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    reverse_iterator rbegin() noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator rend() const noexcept;
    void swap(my_vector & other) ;

    bool operator==(const my_vector & rhs) const;
    bool operator!=(const my_vector & rhs) const;
    bool operator>(const my_vector & rhs) const;
    bool operator<(const my_vector & rhs) const;
    bool operator>=(const my_vector & rhs) const;
    bool operator<=(const my_vector & rhs) const;

private:
    bool isSmall;
    size_t curSiz;
    union DataType {
        DataType() { }
        ~DataType() { }
        size_t* big_data;
        T small_data[SMALLSIZ];
    } data_;

    void setUniq();
    void swap(T& a, T& b);
    bool equals(my_vector const&a) const;
    bool less(my_vector const& a) const;
    T* getPt(size_t* res);
    void reverse(iterator first, iterator last);
    std::string to_string() const;
    size_t* doCopy(size_t newCap);
    size_t* doAlloc(size_t newCap);
    void freeData();
    void subscribe(size_t* const& other);
    void diz_subscribe();
};

template<typename T>
bool my_vector<T>::operator==(const my_vector<T> & rhs) const {
    return equals(rhs);
}

template<typename T>
bool my_vector<T>::operator!=(const my_vector<T> & rhs) const {
    return !equals(rhs);
}

template<typename T>
bool my_vector<T>::operator<(const my_vector<T> & rhs) const {
    return less(rhs);
}

template<typename T>
bool my_vector<T>::operator>(const my_vector<T> & rhs) const {
    return rhs.less(*this);
}

template<typename T>
bool my_vector<T>::operator>=(const my_vector<T> & rhs) const {
    return !less(rhs);
}

template<typename T>
bool my_vector<T>::operator<=(const my_vector<T> & rhs) const {
    return !(rhs.less(*this));
}

template<typename T>
my_vector<T>::~my_vector() {
    freeData();
}

template<typename T>
my_vector<T>::my_vector() {
    isSmall = true;
    curSiz = 0;
}

template<typename T>
void my_vector<T>::subscribe(size_t* const& other) {
    ++other[0];
    data_.big_data = other;
}

template<typename T>
void my_vector<T>::diz_subscribe() {
    --data_.big_data[0];
    if (data_.big_data[0] == 0) {
        for(size_t i = 0; i < curSiz; ++i) {
            data()[i].~T();
        }
        free(data_.big_data);
    }
}

template<typename T>
void my_vector<T>::setUniq() {
    if(isSmall || data_.big_data[0] == 1)
        return;
    --data_.big_data[0];
    size_t* tmp = 0;
    try {
        tmp = doCopy(data_.big_data[1]);
    } catch (...) {
        ++data_.big_data[0];
    }
    if(!tmp)
        throw std::bad_alloc();
    data_.big_data = tmp;
}

template<typename T>
T* my_vector<T>::getPt(size_t* res) {
    return static_cast<T*>(static_cast<void*>(res + 2));
}

template<typename T>
void my_vector<T>::reverse(iterator first, iterator last)
{
    while ((first != last) && (first != --last)) {
        swap(*first++, *last);
    }
}

template<typename T>
std::string my_vector<T>::to_string() const {
    if(curSiz == 0)
        return "()";
    std::string ans = "(";
    for(size_t i = 0; i < curSiz - 1; ++i) {
        ans += std::to_string(data()[i]);
        ans += ", ";
    }
    ans+=std::to_string(data()[curSiz - 1]);
    ans+=")";
    return ans;
}

template<typename T>
size_t* my_vector<T>::doCopy(size_t newCap) {
    size_t* res = doAlloc(newCap);
    T* newBig = getPt(res);
    size_t cou = 0;
    try {
        for(; cou < curSiz; ++cou) {
            new (newBig + cou) T(data()[cou]);
        }
    } catch (...) {
        for(size_t i = 0; i < cou; ++i)
            newBig[i].~T();
        free(static_cast<void*>(res));
        throw;
    }
    return res;
}

template<typename T>
size_t* my_vector<T>::doAlloc(size_t newCap) {
    size_t byte_cap = newCap * sizeof(T) + 2 * sizeof(size_t);
    size_t tmp = byte_cap & 7;
    if(tmp > 0)
        byte_cap += 8 - tmp;

    void* newPt = malloc(byte_cap);
    if(newPt == nullptr)
        throw std::bad_alloc();

    size_t* ans = static_cast<size_t*>(newPt);
    ans[0] = 1;
    ans[1] = newCap;
    return ans;
}

template<typename T>
void my_vector<T>::freeData() {
    if(isSmall) {
        for (size_t i = 0; i < curSiz; ++i) {
            data_.small_data[i].~T();
        }
    } else {
        diz_subscribe();
    }
}

template<typename T>
my_vector<T>::my_vector(my_vector const& other) {
    isSmall = other.isSmall;
    if(other.isSmall) {
        size_t cou = 0;
        try {
            for(; cou < other.curSiz; ++cou)
                new (data_.small_data + cou) T(other.data_.small_data[cou]);
        } catch (...) {
            for(size_t i = 0; i < cou; ++i)
                data_.small_data[i].~T();
            throw;
        }
    } else {
        subscribe(other.data_.big_data);
    }
    curSiz = other.curSiz;
}

template<typename T>
my_vector<T>& my_vector<T>::operator =(my_vector<T> const& other) {
    if(other.isSmall) {
        setUniq();
        for (size_t i = other.curSiz; i < curSiz; ++i) {
            data()[i].~T();
        }
        size_t cou = 0;
        try {
            for(; cou < curSiz && cou < other.curSiz; ++cou) {
                data()[cou] = other.data_.small_data[cou];
            }
        } catch (...) {
            for (size_t i = cou; i < std::min(curSiz, other.curSiz); ++i)
                data()[i].~T();
            curSiz = cou;
            throw;
        }
        try {
            for (cou = curSiz; cou < other.curSiz && cou < SMALLSIZ; ++cou) {
                new (data()+cou) T(other.data_.small_data[cou]);
            }
        } catch (...) {
            curSiz = cou;
            throw;
        }
        curSiz = other.curSiz;
    } else {
        ++other.data_.big_data[0];
        freeData();
        data_.big_data = other.data_.big_data;
        curSiz = other.curSiz;
        isSmall = false;
    }
    return *this;
}

template<typename T>
T & my_vector<T>::operator[](size_t i) {
    if (curSiz < i) {
        throw std::out_of_range("index > length");
    }
    if (isSmall) {
        return data()[i];
    } else {
        setUniq();
        return data()[i];
    }
}

template<typename T>
T const& my_vector<T>::operator[](size_t i) const {
    if (curSiz < i) {
        throw std::out_of_range("index > length");
    }
    if (isSmall) {
        return data()[i];
    } else {
        return data()[i];
    }
}

template<typename T>
const T & my_vector<T>::front() const {
    return operator [](0);
}

template<typename T>
const T & my_vector<T>::back() const {
    return operator [](curSiz - 1);
}

template<typename T>
T & my_vector<T>::front() {
    return operator [](0);
}

template<typename T>
T & my_vector<T>::back() {
    return operator [](curSiz - 1);
}

template<typename T>
T* my_vector<T>::data() noexcept {
    if(!isSmall) {
        return static_cast<T*>(static_cast<void*>(data_.big_data + 2));
    } else {
        return static_cast<T*>(static_cast<void*>(data_.small_data));
    }
}

template<typename T>
const T* my_vector<T>::data() const noexcept {
    if(!isSmall) {
        return static_cast<const T*>(static_cast<const void*>(data_.big_data + 2));
    } else {
        return static_cast<const T*>(static_cast<const void*>(data_.small_data));
    }
}

template<typename T>
void my_vector<T>::push_back(T const& a) {
    setUniq();
    if((isSmall && curSiz + 1 <= SMALLSIZ) || (!isSmall && curSiz + 1 <= data_.big_data[1])) {
        try{
            new (data() + curSiz) T(a);
            ++curSiz;
        } catch (...) {
            throw;
        }
    } else {
        size_t* tmp = nullptr;
        T* tmpData = nullptr;
        try {
            tmp = doCopy(2 * curSiz);
        } catch (...) {
            throw;
        }
        try {
            tmpData = static_cast<T*>(static_cast<void*>(getPt(tmp)));
            new (tmpData + curSiz) T(a);
        } catch (...) {
            for(size_t i = 0; i < curSiz; ++i)
                tmpData[i].~T();
            free(tmp);
            throw;
        }
        freeData();
        ++curSiz;
        data_.big_data = tmp;
        isSmall = false;
    }
}

template<typename T>
void my_vector<T>::pop_back() {
    setUniq();
    data()[curSiz-1].~T();
    --curSiz;
}

template<typename T>
bool my_vector<T>::empty() const noexcept {
    return curSiz == 0;
}

template<typename T>
size_t my_vector<T>::size() const noexcept {
    return curSiz;
}

template<typename T>
size_t my_vector<T>::capacity() const noexcept {
    return isSmall ? SMALLSIZ : data_.big_data[1];
}

template<typename T>
void my_vector<T>::reserve(size_t sz) {
    sz = std::max(2 * SMALLSIZ, sz);
    if(sz <= capacity())
        return;

    size_t* tmp = doCopy(sz);
    freeData();
    data_.big_data = tmp;
    isSmall = false;
}

template<typename T>
void my_vector<T>::shrink_to_fit() {

}

template<typename T>
void my_vector<T>::resize(size_t sz, T a) {
    reserve(sz);
    for (size_t i = curSiz; i < sz; ++i) {
        data()[i] = T(a);
    }
    curSiz = sz;
}

template<typename T>
void my_vector<T>::clear() noexcept {
    if(!isSmall) {
        diz_subscribe();
    }
    curSiz = 0;
}

template<typename T>
bool my_vector<T>::equals(my_vector const&a) const {
    if(curSiz != a.curSiz)
        return false;
    bool ans = true;
    for(size_t i = 0; i < curSiz; ++i)
        ans &= (data()[i] == a.data()[i]);
    return ans;
}

template<typename T>
bool my_vector<T>::less(my_vector const& a) const {
    size_t i = 0;
    while(i < curSiz && i < a.curSiz && (data()[i] == a.data()[i])) {
        ++i;
    }
    if(i == curSiz)
        return a.curSiz > i;
    if(i == a.curSiz)
        return false;
    return data()[i] < a.data()[i];
}

template<typename T>
void my_vector<T>::swap(T& a, T& b) {
    T c = T(a);
    a = b;
    b = c;
}

template<typename T>
typename my_vector<T>::iterator my_vector<T>::insert(const_iterator pos, T const& val) {
    size_t posInd = pos - begin();
    push_back(val);
    iterator p = end() - 1;
    while ((size_t)(p-begin()) != posInd) {
        swap(*(p-1), *p);
        --p;
    }
    return p;
}

template<typename T>
typename my_vector<T>::iterator my_vector<T>::erase(const_iterator pos) {
    iterator cur = begin() + (pos-begin());
    while(cur != end() - 1) {
        swap(*cur, *(cur+1));
        ++cur;
    }
    size_t posInd = pos - begin();
    pop_back();
    return begin() + posInd;
}

template<typename T>
typename my_vector<T>::iterator my_vector<T>::erase(const_iterator first, const_iterator last) {
    size_t pos = first - begin();
    iterator f = begin() + (first-begin());
    iterator l = begin() + (last-begin());
    reverse(f, l);
    reverse(l, end());
    reverse(f, end());
    for(size_t i = 0; i < (size_t)(last - first); ++i)
        pop_back();
    return begin() + pos;
}

template<typename T>
typename my_vector<T>::iterator my_vector<T>::begin() noexcept {
    return data();
}

template<typename T>
typename my_vector<T>::iterator my_vector<T>::end() noexcept {
    return data() + curSiz;
}

template<typename T>
typename my_vector<T>::const_iterator my_vector<T>::begin() const noexcept {
    return data();
}

template<typename T>
typename my_vector<T>::const_iterator my_vector<T>::end() const noexcept {
    return data() + curSiz;
}

template<typename T>
typename my_vector<T>::reverse_iterator my_vector<T>::rbegin() noexcept {
    return std::reverse_iterator<iterator>(end());
}

template<typename T>
typename my_vector<T>::reverse_iterator my_vector<T>::rend() noexcept {
    return std::reverse_iterator<iterator>(begin());
}

template<typename T>
typename my_vector<T>::const_reverse_iterator my_vector<T>::rbegin() const noexcept {
    return std::reverse_iterator<const_iterator>(end());
}

template<typename T>
typename my_vector<T>::const_reverse_iterator my_vector<T>::rend() const noexcept {
    return std::reverse_iterator<const_iterator>(begin());
}

template<typename T>
void my_vector<T>::swap(my_vector & other) {
    my_vector a(*this);
    *this = other;
    other = a;
}

template<typename T>
void swap(my_vector<T> &a, my_vector<T> &b) {
    a.swap(b);
}

#endif
