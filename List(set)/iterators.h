#include <type_traits>

template<typename T1, typename T2>
struct enable_if_cast {
    typedef typename std::enable_if<std::is_same<const T1, const T2>::value, bool>::type type;
};

template <typename T>
class list;

template <typename V, typename CAST_TO = V>
class it {
    friend class list<V>;
    friend class list<typename std::remove_const<V>::type>;
    friend class it<typename std::remove_const<V>::type, CAST_TO>;
    friend class it<const V, CAST_TO>;
public:

     typedef V value_type;
     typedef V &reference;
     typedef std::ptrdiff_t difference_type;
     typedef V *pointer;
     typedef std::bidirectional_iterator_tag iterator_category;

    it() {
        p_ = nullptr;
    }

    it(it const & other) {
        p_ = other.p_;
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

    template<typename X>
    typename enable_if_cast<V, X>::type operator==(it<X, CAST_TO> const & b) const noexcept {
        return p_ == b.p_;
    }

    template<typename X>
    typename enable_if_cast<V, X>::type operator!=(it<X, CAST_TO> const & b) const noexcept {
        return p_ != b.p_;
    }

    V& operator*() const {
        return p_->get_val();
    }

    operator it<const V>() const noexcept {
        return it<const V>(p_);
    }

    V* operator->() const {
        return &(dynamic_cast<node<CAST_TO>*>(p_)->val);
    }

    operator it<const V, CAST_TO>() {
        return it<const V, CAST_TO>(p_);
    }

private:
    it(node_base* p) {
        p_ = p;
    }
    node_base* p_;
};

template <typename V, typename CAST_TO, typename X>
typename enable_if_cast<V, X>::type operator==(it<V, CAST_TO> const & a,
                                               it<X, CAST_TO> const & b) noexcept {
    return a == b;
}

template <typename V, int32_t step = 1, typename CAST_TO, typename X>
typename enable_if_cast<V, X>::type operator!=(it<V, CAST_TO> const & a,
                                               it<X, CAST_TO> const & b) noexcept {
    return !(a == b);
}
