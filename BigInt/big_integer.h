#include <vector>
#include <string>

typedef uint32_t dig;
typedef uint64_t bi_dig;
typedef __uint128_t tri_dig;

struct big_integer
{
    big_integer();
    big_integer(big_integer const& a);
    big_integer(int a);
    explicit big_integer(std::string const& s);
    ~big_integer();

    big_integer& operator=(big_integer const& a);

    friend bool operator==(big_integer const& a, big_integer const& b);
    friend bool operator!=(big_integer const& a, big_integer const& b);
    friend bool operator<(big_integer const& a, big_integer const& b);
    friend bool operator>(big_integer const& a, big_integer const& b);
    friend bool operator<=(big_integer const& a, big_integer const& b);
    friend bool operator>=(big_integer const& a, big_integer const& b);

    big_integer& operator+=(big_integer const& rhs);
    big_integer& operator-=(big_integer const& rhs);
    big_integer operator+() const;
    big_integer operator-() const;

    big_integer& operator*=(big_integer const& rhs);
    big_integer& operator/=(big_integer const& rhs);
    big_integer& operator%=(big_integer const& rhs);

    big_integer& operator&=(big_integer const& rhs);
    big_integer& operator|=(big_integer const& rhs);
    big_integer& operator^=(big_integer const& rhs);
    big_integer operator~() const;
    big_integer& operator<<=(size_t rhs);
    big_integer& operator>>=(size_t rhs);


    friend void del_nulls(big_integer& a);
    friend bool cmp_abs(big_integer const& a, big_integer const& b);
    friend void short_mul(big_integer& res, big_integer const& rhs, dig digit);
    friend void short_div(big_integer& res, big_integer& a, dig digit);
    friend void short_mod(big_integer& res, big_integer const& a, dig digit);
    friend bi_dig trial(big_integer& r, big_integer& d, size_t m, size_t k);
    friend bool smaller(big_integer const& r, big_integer const& dq, size_t k, size_t m);
    friend void difference(big_integer& r, big_integer const& dq, size_t km, size_t m);
    friend void long_div(big_integer const& x, big_integer const& y, big_integer& q, big_integer& r);
    friend void divide(big_integer& x, big_integer const& y, big_integer& q, big_integer& r);
    friend void to_binary(big_integer &a, size_t size);
    friend void back_binary(big_integer &a);

    friend std::string to_string(big_integer const& a);
    friend std::string to_string_(big_integer const& a);

private:
    std::vector <dig> numbers;
    bool sign;
};

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);

big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);

big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);

big_integer operator<<(big_integer a, size_t b);
big_integer operator>>(big_integer a, size_t b);

std::string to_string(big_integer const& a);






