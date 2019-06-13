#include "big_integer.h"

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

dig const MAX_DIGIT = static_cast<dig>(4294967295);
int32_t const SIZE_BITS = 32;
bi_dig const SIZE = (static_cast<bi_dig>(1) << SIZE_BITS);

big_integer::big_integer()
{
    sign = true;
}

big_integer::big_integer(big_integer const& a)
{
    sign = a.sign;
    numbers = a.numbers;
}

big_integer::big_integer(int a)
{
    sign = a >= 0;
    if (a == 0)
        return;
    numbers.push_back(std::abs(a));
}

big_integer::big_integer(std::string const& s)
{
    std::string s_tmp = s;
    big_integer power = 1;
    bool tmp_sign = true;
    sign = true;
    bool st = false;
    if (s_tmp[0] == '-'){
        st = true;
        tmp_sign = false;
    }
    for (size_t i = st; i < s_tmp.size(); ++i) {
        *this += big_integer(s_tmp[s_tmp.size() - i + st - 1] - '0') * power;
        power *= 10;
    }
    sign = tmp_sign;
    if (numbers.size() == 0)
        sign = true;
}

big_integer::~big_integer()
{}

big_integer& big_integer::operator=(big_integer const& a)
{
    sign = a.sign;
    numbers = a.numbers;
    return *this;
}

bool isCarry(dig a, dig b){
    return MAX_DIGIT - a < b;
}

void del_nulls(big_integer& a) {
    while (a.numbers.size() > 0 && a.numbers.back() == 0)
        a.numbers.pop_back();
}

big_integer& big_integer::operator+=(big_integer const& rhs)
{
    if (sign != rhs.sign)
    {
        if (!sign) {
            big_integer tmp = rhs - (-*this);
            *this = tmp;
        }
        else
        {
            *this -= -rhs;
        }
        return *this;
    }

    //a*b>=0
    bool carry = false;
    for (size_t i = 0; i < std::max(numbers.size(), rhs.numbers.size()); i++)
    {
        if (numbers.size() == i)
            numbers.push_back(0);

        dig a = numbers[i];
        dig b = 0;
        if(rhs.numbers.size() > i)
            b = rhs.numbers[i];

        bool newcarry = false;

        if(a < MAX_DIGIT) {
            a += carry;
        } else {
            if(b < MAX_DIGIT) {
                b += carry;
            } else {
                newcarry = true;
                a = 0;
            }
        }
        newcarry = newcarry || isCarry(a,b);
        a += b;
        carry = newcarry;
        numbers[i] = a;
    }
    if (carry)
        numbers.push_back(carry);
    del_nulls(*this);
    return *this;
}

//return |a|>|b|
bool cmp_abs(big_integer const& a, big_integer const& b)
{
    if (a.numbers.size() != b.numbers.size())
        return a.numbers.size() > b.numbers.size();
    for (size_t i = a.numbers.size(); i > 0; i--)
    {
        if (a.numbers[i-1] != b.numbers[i-1])
            return a.numbers[i-1] > b.numbers[i-1];
    }
    //|a| == |b|
    return false;
}

big_integer& big_integer::operator-=(big_integer const& rhs)
{
    if (sign != rhs.sign)
    {
        *this += -rhs;
        return *this;
    }
    if (cmp_abs(rhs, *this))
    {
        big_integer tmp = rhs - *this;
        *this = tmp;
        sign = !sign;
        return *this;
    }
    bool carry = false;
    for (size_t i = 0; i < numbers.size(); i++)
    {
        dig a = numbers[i];
        dig b = 0;
        if (i < rhs.numbers.size())
            b = rhs.numbers[i];
        bool newcarry = false;
        if (b > a) {
            newcarry = true;
            b = b - a;
            a = 0;
            if (b != MAX_DIGIT || !carry) {
                b += carry;
                a = MAX_DIGIT - b + 1;
            } else {
                a = 0;
            }
        } else if (b == a && carry) {
            a = MAX_DIGIT;
            newcarry = true;
        } else {
            a = a - b - carry;
        }
        numbers[i] = a;
        carry = newcarry;

    }
    del_nulls(*this);
    return *this;
}

big_integer big_integer::operator+() const
{
    return *this;
}

big_integer big_integer::operator-() const
{
    big_integer r = *this;
    r.sign = !r.sign;
    return r;
}

big_integer& big_integer::operator*=(big_integer const& rhs)
{
    sign = (sign == rhs.sign);
    dig carry = 0;
    while (numbers.size() < rhs.numbers.size())
        numbers.push_back(0);
    size_t res_size = numbers.size() + rhs.numbers.size();
    std::vector <dig> next(res_size, 0);
    for (size_t i = 0; i < rhs.numbers.size(); i++)
    {

        bi_dig sum;
        for (size_t j = 0; j < numbers.size(); j++)
        {
            sum = static_cast<bi_dig>(numbers[j]) * rhs.numbers[i] + carry + next[i + j];
            carry = sum >> SIZE_BITS;
            sum %= SIZE;
            next[i + j] = sum;
        }
        if (carry)
        {
            sum = next[i + numbers.size()] + carry;
            carry = sum >> SIZE_BITS;
            sum %= SIZE;
            next[i + numbers.size()] = sum;
        }
    }

    while (numbers.size() < res_size)
        numbers.push_back(0);
    for (size_t i = 0; i < res_size; i++){
        numbers[i] = next[i];
    }
    del_nulls(*this);
    std::vector<dig>().swap(next);

    return *this;
}

void short_mul(big_integer& res, big_integer const& a, dig digit){
    dig carry = 0;
    res = 0;
    for (size_t i = 0; i < a.numbers.size(); i++) {
        bi_dig sum = static_cast<bi_dig>(a.numbers[i]) * digit + carry;
        carry = sum >> SIZE_BITS;
        sum %= SIZE;
        res.numbers.push_back(sum);
    }
    if (carry){
        res.numbers.push_back(carry);
    }
}

void short_div(big_integer& res, big_integer& a, dig digit){
    dig carry = 0;
    bi_dig tmp = 0;
    size_t sz = a.numbers.size();
    res.numbers.resize(sz, 0);
    for (int i = sz - 1; i > -1; i--){
        tmp = static_cast<bi_dig>(carry) * SIZE + a.numbers[i];
        res.numbers[i] = tmp / digit;
        carry = tmp % digit;
    }
}

void short_mod(big_integer& res, big_integer const& a, dig digit){
    dig carry = 0;
    for (int i = a.numbers.size() - 1; i > -1; i--){
        carry = static_cast<bi_dig>(carry * SIZE + a.numbers[i]) % digit;
    }
    res = carry;
}

bi_dig trial(big_integer& r, big_integer& d, size_t k, size_t m){
    size_t km = k + m;
    size_t sz_r = r.numbers.size();
    for (size_t i = sz_r; i < km + 1; i++)
        r.numbers.push_back(0);
    size_t sz_d = d.numbers.size();
    for (size_t i = sz_d; i < m; i++)
        d.numbers.push_back(0);
    tri_dig r3 = (static_cast<tri_dig>(r.numbers[km]) * SIZE + r.numbers[km - 1]) * SIZE + r.numbers[km - 2];
    tri_dig d2 = static_cast<tri_dig>(d.numbers[m - 1]) * SIZE + d.numbers[m - 2];
    tri_dig t = r3 / d2;
    if (t > SIZE - 1)
        return SIZE - 1;
    return t;
}

bool smaller(big_integer const& r, big_integer const& dq, size_t k, size_t m){
    size_t i = m;
    while (i > 0 && r.numbers[i + k] == dq.numbers[i])
        i--;
    return r.numbers[i + k] < dq.numbers[i];
}

void difference(big_integer& r, big_integer const& dq, size_t k, size_t m){
    int32_t borrow = 0;
    bi_dig df = 0;
    for (size_t i = 0; i <= m; i++){
        df = SIZE + r.numbers[i + k] - dq.numbers[i] - borrow;
        r.numbers[i + k] = df % SIZE;
        borrow = 1 - (df >> SIZE_BITS);
    }
}

void long_div(big_integer const& x, big_integer const& y, big_integer& q, big_integer& r){
    size_t n = x.numbers.size();
    size_t m = y.numbers.size();
    dig f = SIZE / (y.numbers[m - 1] + 1);
    short_mul(r, x, f);
    big_integer d = 0;
    short_mul(d, y, f);
    bi_dig qt;
    big_integer dq = 0;
    q.numbers.resize(n - m + 1, 0);
    for (int k = n - m; k > -1; k--){
        qt = trial(r, d, k, m);
        short_mul(dq, d, qt);
        dq.numbers.push_back(0);
        if (smaller(r, dq, k, m)){
            qt--;
            short_mul(dq, d, qt);
        }
        q.numbers[k] = qt;
        difference(r, dq, k, m);
    }
    short_div(r, r, f);
}

void divide(big_integer& x, big_integer const& y, big_integer& q, big_integer& r){
    size_t sz_y = y.numbers.size();
    if (sz_y == 1){
        dig a = y.numbers[0];
        short_div(q, x, a);
        short_mod(r, x, a);
    }
    else {
        size_t sz_x = x.numbers.size();
        if (sz_y > sz_x) {
            q = 0;
            r = x;
        }
        else
            long_div(x, y, q, r);
    }
}

big_integer& big_integer::operator/=(big_integer const& rhs)
{
    if (*this == 0)
        return *this;
    big_integer rhs_tmp = rhs.sign ? rhs : -rhs;
    big_integer tmp = sign ? *this : -*this;
    bool tmp_sign = (rhs.sign == sign);
    big_integer q = 0;
    big_integer r = 0;
    divide(tmp, rhs_tmp, q, r);
    *this = q;
    del_nulls(*this);
    sign = tmp_sign;
    return *this;
}

big_integer& big_integer::operator%=(big_integer const& rhs)
{
    if (*this == 0)
        return *this;
    big_integer rhs_tmp = rhs.sign ? rhs : -rhs;
    big_integer tmp = sign ? *this : -*this;
    big_integer q = 0;
    big_integer r = 0;
    divide(*this, rhs, q, r);
    *this = r;
    del_nulls(*this);
    return *this;
}

void to_binary(big_integer &a, size_t size) {
    while (a.numbers.size() < size + 1)
        a.numbers.push_back(0);
    if (!a.sign) {
        for (size_t i = 0; i < a.numbers.size(); i++) {
            a.numbers[i] = ~a.numbers[i];
        }
        a.sign = true;
        a += 1;
    }
}

void back_binary(big_integer &a) {
    a.sign = true;
    if (a.numbers.back()) {
        a -= 1;
        a.sign = false;
        for (size_t i = 0; i < a.numbers.size() - 1; i++) {
            a.numbers[i] = ~a.numbers[i];
        }
        a.numbers[a.numbers.size() - 1] = 0;
    }
    del_nulls(a);
}

big_integer& big_integer::operator&=(big_integer const& rhs)
{
    to_binary(*this, std::max(numbers.size(), rhs.numbers.size()) + 1);
    big_integer tmp = rhs;
    to_binary(tmp, std::max(numbers.size(), rhs.numbers.size()) + 1);
    for (size_t i = 0; i < numbers.size(); i++) {
        numbers[i] &= tmp.numbers[i];
    }
    back_binary(*this);
    return *this;
}

big_integer& big_integer::operator|=(big_integer const& rhs)
{
    to_binary(*this, std::max(numbers.size(), rhs.numbers.size()) + 1);
    big_integer tmp = rhs;
    to_binary(tmp, std::max(numbers.size(), rhs.numbers.size()) + 1);
    for (size_t i = 0; i < numbers.size(); i++) {
        numbers[i] |= tmp.numbers[i];
    }
    back_binary(*this);
    return *this;
}

big_integer& big_integer::operator^=(big_integer const& rhs)
{
    *this = ((~*this & rhs) | (*this & ~rhs));
    return *this;
}

big_integer big_integer::operator~() const
{
    big_integer r = *this;
    r.sign = !r.sign;
    r = r - 1;
    return r;
}

big_integer& big_integer::operator<<=(size_t rhs)
{
    size_t shift = rhs / SIZE_BITS;
    size_t small_shift = rhs % SIZE_BITS;
    std::reverse(numbers.begin(), numbers.end());
    for (size_t i = 0; i < shift; i++)
        numbers.push_back(0);
    std::reverse(numbers.begin(), numbers.end());
    if (small_shift){
        numbers.push_back(0);
        for (size_t i = numbers.size() - 1; i > shift; i--) {
            dig digit = numbers[i - 1];
            numbers[i - 1] <<= small_shift;
            digit >>= SIZE_BITS - small_shift;
            numbers[i] |= digit;
        }
    }
    del_nulls(*this);
    return *this;
}

big_integer& big_integer::operator>>=(size_t rhs)
{
    to_binary(*this, numbers.size() + 1);
    size_t shift = rhs / SIZE_BITS;
    size_t small_shift = rhs % SIZE_BITS;
    if (shift >= numbers.size()) {
        *this = 0;
        return *this;
    }
    for (size_t i = 0; i + shift < numbers.size(); i++) {
        numbers[i] = numbers[i + shift];
    }
    for (size_t i = 0; i < shift; i++)
        numbers.pop_back();
    numbers[0] >>= small_shift;
    for (size_t i = 1; i < numbers.size(); i++) {
        dig digit = numbers[i];
        numbers[i] >>= small_shift;
        digit <<= SIZE_BITS - small_shift;
        numbers[i-1] |= digit;
    }
    back_binary(*this);
    return *this;
}

std::string to_string(big_integer const& a)
{
    std::string res = "";
    if (a.numbers.size() == 0) {
        return "0";
    }
    if (a.numbers.size() == 1) {
        return a.sign ? std::to_string(a.numbers[0]) : "-" + std::to_string(a.numbers[0]);
    }
    big_integer tmp = a.sign ? a : -a;
    while (tmp > 0)
    {
        res += to_string(tmp % 10);
        tmp /= 10;
    }
    if (!a.sign)
        res += "-";
    std::reverse(res.begin(), res.end());
    return res;

}

std::string to_string_(big_integer const& a)
{
    std::string res = a.sign ? "(+)" : "(-)";
    for (size_t i = 0; i < a.numbers.size(); i++)
        res += std::to_string(a.numbers[i]) + " ";
    return res;
}

bool operator==(big_integer const& a, big_integer const& b)
{
    if (a.numbers.size() != b.numbers.size())
        return false;
    for (size_t i = a.numbers.size(); i > 0; i--)
    {
        if (a.numbers[i-1] != b.numbers[i-1])
            return false;
    }
    return true;
}

bool operator!=(big_integer const& a, big_integer const& b)
{
    return !(a == b);
}

bool operator<(big_integer const& a, big_integer const& b)
{
    if (a.numbers.size() == b.numbers.size() && a.numbers.size() == 0)
        return false;
    if (a.sign != b.sign)
        return b.sign;
    if (a.sign)
        return cmp_abs(b, a);
    return cmp_abs(a, b);
}

bool operator>(big_integer const& a, big_integer const& b)
{
    return (b < a);
}

bool operator<=(big_integer const& a, big_integer const& b)
{
    return !(a > b);
}

bool operator>=(big_integer const& a, big_integer const& b)
{
    return !(a < b);
}

big_integer operator+(big_integer a, big_integer const& b)
{
    return a += b;
}

big_integer operator-(big_integer a, big_integer const& b)
{
    return a -= b;
}

big_integer operator*(big_integer a, big_integer const& b)
{
    return a *= b;
}

big_integer operator/(big_integer a, big_integer const& b)
{
    return a /= b;
}

big_integer operator%(big_integer a, big_integer const& b)
{
    return a %= b;
}

big_integer operator&(big_integer a, big_integer const& b)
{
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const& b)
{
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const& b)
{
    return a ^= b;
}

big_integer operator<<(big_integer a, size_t b)
{
    return a <<= b;
}

big_integer operator>>(big_integer a, size_t b)
{
    return a >>= b;
}

