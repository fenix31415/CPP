#include "myvector_uint.h"
#include <stdexcept>
#include <iostream>

MyVectorTYP::MyVectorTYP(){
    small = true;
    len = 0;
    stat = 0;
}

MyVectorTYP::MyVectorTYP(size_t sz) {
    len = sz;
    small = sz <= 1;
    if (sz > 1) {
        sh_ptr = std::make_shared<vector_TYP>(sz);
    } else {
        stat = 0;
    }
}

MyVectorTYP::MyVectorTYP(MyVectorTYP const& x) {
    operator=(x);
}

MyVectorTYP::~MyVectorTYP() {
    if (!small)
        sh_ptr.reset();
}

void MyVectorTYP::copy(size_t sz) {
    if (small)
        return;
    vector_TYP new_vec(sh_ptr->begin(), sh_ptr->begin() + sz);
    sh_ptr = std::make_shared<vector_TYP>(new_vec);
}

void MyVectorTYP::clear() {
    sh_ptr.reset();
    small = true;
    len = 0;
}

void MyVectorTYP::resize(size_t sz) {
    if (len == sz) {
        return;
    }

    if (sz <= 1) {
        if (!small) {
            if (!sh_ptr.unique()) {
                copy(1);
            }
            small = true;
            stat = sh_ptr->operator [](0);
            sh_ptr.reset();
        }
    } else {
        if (small) {
            sh_ptr = std::make_shared<vector_TYP>(sz);
            sh_ptr->operator [](0) = stat;
            small = false;
        } else {
            if (!sh_ptr.unique()) {
                copy(len);
            }
            sh_ptr->resize(sz, 0);
        }
    }
    len = sz;
}

void MyVectorTYP::push_back(TYP x) {
    if (len > 1) {
        if(!sh_ptr.unique()) {
            copy(len);
        }
        sh_ptr->push_back(x);
    }
    if (len == 0) {
        small = true;
        stat = x;
    }
    if (len == 1) {
        small = false;
        sh_ptr.reset();
        sh_ptr = std::make_shared<vector_TYP>(2);
        sh_ptr->operator[](0) = stat;
        sh_ptr->operator[](1) = x;
    }
    ++len;
}

void MyVectorTYP::pop_back() {
    if (len == 0) {
        throw std::runtime_error("Empty vector");
    }
    if (len == 2) {
        small = true;
        stat = sh_ptr->operator [](0);
        sh_ptr.reset();
    } else {
        if (sh_ptr.unique()) {
            sh_ptr->pop_back();
        } else {
            copy(len - 1);
        }
    }
    --len;
}

MyVectorTYP &MyVectorTYP::operator=(MyVectorTYP const &vec) {
    if (!small) {
        sh_ptr.reset();
    }
    len = vec.len;
    small = vec.small;
    if (vec.small) {
        stat = vec.stat;
    } else {
        sh_ptr = vec.sh_ptr;
    }
    return *this;
}

TYP &MyVectorTYP::operator[](const size_t i) {
    if (len < i) {
        throw new std::out_of_range("index > length");
    }
    if (small) {
        return stat;
    } else {
        if (!sh_ptr.unique()) {
            copy(len);
        }
        return sh_ptr->operator [](i);
    }
}

TYP const& MyVectorTYP::operator[](const size_t i) const {
    if (i > len) {
        throw new std::out_of_range("index > length");
    }
    if (small) {
        return stat;
    } else {
        return sh_ptr->operator [](i);
    }
}

size_t MyVectorTYP::size() const {
    return len;
}
