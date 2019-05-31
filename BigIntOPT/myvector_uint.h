#include "consts.h"
#include <memory>
#include <vector>

typedef uint32_t TYP;

typedef std::vector<TYP> vector_TYP;

class MyVectorTYP {
public:
    MyVectorTYP();
    MyVectorTYP(size_t sz);
    MyVectorTYP(MyVectorTYP const& x);
    ~MyVectorTYP();

    void copy(size_t sz);
    void clear();
    void resize(size_t newsz);
    void push_back(TYP x);
    void pop_back();

    MyVectorTYP& operator=(MyVectorTYP const &x);
    TYP& operator[](const size_t i);
    TYP const& operator[](const size_t i) const;
    size_t size() const;

private:
    std::shared_ptr<vector_TYP> sh_ptr;
    TYP stat;
    bool small;
    size_t len;
};
