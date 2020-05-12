#include <iostream>
#include <string>
#include <vector>

template <class T>
T max(T x, T y) {
    return x >= y ? x : y;
}

template <class T>
T min(T x, T y) {
    return x < y ? x : y;
}

class BigInteger {
private:
    static const signed char base = 10;
    bool positive;
    std::vector<signed char> digits;

    void unsigned_add(const BigInteger& x);
    void unsigned_sub(const BigInteger& x);
    void unsigned_mul(const BigInteger& x);
    void unsigned_div(const BigInteger& x);

    friend BigInteger karatsuba_mul(const BigInteger& x, const BigInteger& y);
    BigInteger shift(int x) const ;
    BigInteger& shift_this(int x) & ;
    BigInteger first_half(int mid) const;
    BigInteger second_half(int mid) const;

    void remove_leading_zeros();
    void reverse();

    int unsigned_less(const BigInteger& other) const;
    void ensure_zero();

public:
    BigInteger();
    BigInteger(int x);
    BigInteger(const std::string& s);
    BigInteger(const BigInteger& another) = default;
    BigInteger(BigInteger&& another) = default;
    ~BigInteger() = default;

    BigInteger& operator=(const BigInteger& other) &;
    BigInteger& operator=(BigInteger&& other) &;

    BigInteger& operator+=(const BigInteger& other) &;
    BigInteger& operator-=(const BigInteger& other) &;
    BigInteger& operator*=(const BigInteger& other) &;
    BigInteger& operator/=(const BigInteger& other) &;
    BigInteger& operator%=(const BigInteger& other) &;

    friend BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs);
    friend BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs);
    friend BigInteger operator*(const BigInteger& lhs, const BigInteger& rhs);
    friend BigInteger operator/(const BigInteger& lhs, const BigInteger& rhs);
    friend BigInteger operator%(const BigInteger& lhs, const BigInteger& rhs);

    BigInteger& invert() &;
    BigInteger operator-();
    BigInteger& operator++() &;
    BigInteger operator++(int);
    BigInteger& operator--() &;
    BigInteger operator--(int);

    friend bool operator==(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator!=(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator<=(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator>=(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator<(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator>(const BigInteger& lhs, const BigInteger& rhs);

    friend std::ostream& operator<<(std::ostream& out, const BigInteger& x);
    friend std::istream& operator>>(std::istream& in, BigInteger& x);

    std::string toString() const;

    explicit operator bool() const;
};

static const BigInteger zero(0);
static const BigInteger one(1);

BigInteger::BigInteger(): BigInteger(0) {}
BigInteger::BigInteger(int x) {
    positive = x >= 0;
    x = abs(x);
    do {
        digits.push_back(x % base);
        x /= base;
    } while (x > 0);
}

BigInteger::BigInteger(const std::string& s) {
    bool has_sign = false;
    positive = true;
    if (s[0] == '+') {
        positive = true;
        has_sign = true;
    }
    if (s[0] == '-') {
        positive = false;
        has_sign = true;
    }
    int s_size = s.size();
    int size = s_size - has_sign;
    digits = std::vector<signed char>(size);
    for (int i = 0; i < size; i++) {
        digits[i] = s[s_size - i - 1] - '0';
    }
    ensure_zero();
}

void BigInteger::ensure_zero() {
    if (unsigned_less(zero) == 0) {
        positive = true;
    }
}

void BigInteger::reverse() {
    int size = digits.size();
    int till = size / 2;
    for (int i = 0; i < till; ++i) {
        auto t = digits[i];
        digits[i] = digits[size - i - 1];
        digits[size - i - 1] = t;
    }
}

void BigInteger::unsigned_div(const BigInteger& x) {
    BigInteger result;
    BigInteger divisor = x.shift(digits.size() - x.digits.size());
    int this_size = digits.size() + 1;
    for (int i = x.digits.size(); i < this_size; ++i) {
        BigInteger current = divisor;
        signed char digit = 1;
        while (current.unsigned_less(*this) >= 0) {
            current.unsigned_add(divisor);
            digit++;
        }
        current.unsigned_sub(divisor);
        unsigned_sub(current);
        result.digits.push_back(digit - 1);
        divisor.shift_this(-1);
    }
    result.reverse();
    *this = result;
    remove_leading_zeros();
}

void BigInteger::unsigned_add(const BigInteger& x) {
    int size = digits.size();
    int x_size = x.digits.size();
    int common_part = min(size, x_size);

    int carry = 0;
    for (int i = 0; i < common_part; ++i) {
        digits[i] += x.digits[i] + carry;
        if (digits[i] >= base) {
            digits[i] -= base;
            carry = 1;
        } else {
            carry = 0;
        }
    }

    if (size > x_size) {
        for (int i = common_part; i < size && carry != 0; ++i) {
            digits[i] += carry;
            if (digits[i] >= base) {
                digits[i] -= base;
                carry = 1;
            } else {
                carry = 0;
            }
        }
    } else if (size < x_size) {
        digits.resize(x_size);
        for (int i = common_part; i < x_size; ++i) {
            digits[i] = x.digits[i] + carry;
            if (digits[i] >= base) {
                digits[i] -= base;
                carry = 1;
            } else {
                carry = 0;
            }
        }
    }
    if (carry == 1) {
        digits.push_back(1);
    }
}

void BigInteger::unsigned_sub(const BigInteger& x) {
    const BigInteger* great_p;
    const BigInteger* less_p;
    if (unsigned_less(x) == 1) {
        great_p = &x;
        less_p = this;
        positive = false;
    } else {
        great_p = this;
        less_p = &x;
        positive = true;
    }

    const BigInteger& great = *great_p;
    const BigInteger& less = *less_p;

    int g_size = great.digits.size();
    int l_size = less.digits.size();
    int common_part = min(g_size, l_size);

    int carry = 0;
    for (int i = 0; i < common_part; ++i) {
        digits[i] = great.digits[i] - less.digits[i] + carry;
        if (digits[i] < 0) {
            digits[i] += base;
            carry = -1;
        } else {
            carry = 0;
        }
    }
    if (less_p == this) {
        digits.resize(g_size);
        for (int i = common_part; i < g_size; ++i) {
            digits[i] = great.digits[i] + carry;
            if (digits[i] < 0) {
                digits[i] += base;
                carry = -1;
            } else {
                carry = 0;
            }
        }
    } else {
        for (int i = common_part; i < g_size && carry != 0; ++i) {
            digits[i] += carry;
            if (digits[i] < 0) {
                digits[i] += base;
                carry = -1;
            } else {
                carry = 0;
            }
        }
    }
    if (carry != 0) {
        std::cerr << "Fuck\n";
    }
    remove_leading_zeros();
}

void BigInteger::unsigned_mul(const BigInteger& x) {
    *this = karatsuba_mul(*this, x);
    remove_leading_zeros();
}

BigInteger karatsuba_mul(const BigInteger& x, const BigInteger& y) {
    if (x == zero || y == zero) {
        return zero;
    }
    if (x.digits.size() == 1 && y.digits.size() == 1) {
        return BigInteger(x.digits[0] * y.digits[0]);
    }
    int mid = max(x.digits.size(), y.digits.size()) / 2;

    BigInteger a = x.first_half(mid);
    BigInteger b = x.second_half(mid);
    BigInteger c = y.first_half(mid);
    BigInteger d = y.second_half(mid);

    BigInteger ac = karatsuba_mul(a, c);
    BigInteger bd = karatsuba_mul(b, d);
    BigInteger abcd = karatsuba_mul(a + b, c + d);
    BigInteger result = ac.shift(2*mid) + (abcd - ac - bd).shift(mid) + bd;

    return result;
}

BigInteger BigInteger::shift(int x) const {
    BigInteger result = *this;
    result.shift_this(x);
    return result;
}

BigInteger& BigInteger::shift_this(int x) & {
    int size = digits.size() + x;
    if (*this == zero) {
        return *this;
    }
    if (x > 0) {
        digits.resize(size);
        for (int i = size - 1; i >= x; --i) {
            digits[i] = digits[i - x];
        } 
        for (int i = 0; i < x; ++i){
            digits[i] = 0;
        }
    } else {
        if (size <= 0) {
            *this = zero;
        } else {
            for (int i = 0; i < size; ++i) {
                digits[i] = digits[i-x];
            }
            int old_size = digits.size();
            for (int i = size; i < old_size; ++i) {
                digits[i] = 0;
            }
            remove_leading_zeros();
        }
    }
    return *this;
}

BigInteger BigInteger::first_half(int mid) const {
    int size = digits.size();
    if (size <= mid) {
        return zero;
    }
    BigInteger result;
    result.positive = true;
    result.digits.resize(size - mid);
    for (int i = mid; i < size; ++i) {
        result.digits[i - mid] = digits[i];
    }
    return result;
}

BigInteger BigInteger::second_half(int mid) const {
    int size = digits.size();
    int new_size = min(size, mid);
    BigInteger result;
    result.positive = true;
    result.digits.resize(new_size);
    for (int i = 0; i < new_size; ++i) {
        result.digits[i] = digits[i];
    }
    return result;
}

void BigInteger::remove_leading_zeros() {
    int i = digits.size() - 1;
    while (digits[i] == 0 && i > 0) {
        digits.pop_back();
        --i;
    }
}

BigInteger& BigInteger::operator=(const BigInteger& other) & {
    if (&other == this) {
        return *this;
    }
    positive = other.positive;
    digits = other.digits;
    return *this;
}

BigInteger& BigInteger::operator=(BigInteger&& other) & {
    if (&other == this) {
        return *this;
    }
    positive = other.positive;
    digits = std::move(other.digits);
    return *this;
}

BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger result = lhs;
    result += rhs;
    return result;
}

BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger result = lhs;
    result -= rhs;
    return result;
}

BigInteger operator*(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger result = lhs;
    result *= rhs;
    return result;
}

BigInteger operator/(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger result = lhs;
    result /= rhs;
    return result;
}

BigInteger operator%(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger result = lhs;
    result %= rhs;
    return result;
}

BigInteger& BigInteger::operator+=(const BigInteger& other) & {
    if (positive && other.positive) {
        unsigned_add(other);
    }
    if (!positive && !other.positive) {
        unsigned_add(other);
    }
    if (!positive && other.positive) {
        unsigned_sub(other);
        invert();
    }
    if (positive && !other.positive) {
        unsigned_sub(other);
    }
    ensure_zero();
    return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) & {
    if (positive && !other.positive) {
        unsigned_add(other);
    }
    if (!positive && other.positive) {
        unsigned_add(other);
    }
    if (!positive && !other.positive) {
        unsigned_sub(other);
        invert();
    }
    if (positive && other.positive) {
        unsigned_sub(other);
    }
    ensure_zero();
    return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& other) & {
    bool new_positive = !(positive ^ other.positive);
    unsigned_mul(other);
    positive = new_positive;
    ensure_zero();
    return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& other) & {
    bool new_positive = !(positive ^ other.positive);
    if (other == zero) {
        throw std::runtime_error("division by zero");
    }
    unsigned_div(other);
    positive = new_positive;
    ensure_zero();
    return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& other) & {
    *this -= (*this / other) * other;
    ensure_zero();
    return *this;
}

BigInteger& BigInteger::invert() & {
    positive = !positive;
    ensure_zero();
    return *this;
}

BigInteger BigInteger::operator-() {
    BigInteger answer = *this;
    answer.invert();
    return answer;
}

BigInteger& BigInteger::operator++() & {
    *this += one;
    ensure_zero();
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger answer = *this;
    *this += one;
    return answer;
}

BigInteger& BigInteger::operator--() & {
    *this -= one;
    ensure_zero();
    return *this;
}

BigInteger BigInteger::operator--(int) {
    BigInteger answer = *this;
    *this -= one;
    return answer;
}

bool operator<(const BigInteger& lhs, const BigInteger& rhs) {
    if (!lhs.positive && rhs.positive) {
        return true;
    }
    if (lhs.positive && !rhs.positive) {
        return false;
    }
    if (lhs.positive && rhs.positive) {
        return lhs.unsigned_less(rhs) == 1;
    }
    if (!lhs.positive && !rhs.positive) {
        return lhs.unsigned_less(rhs) == -1;
    }
    return false;
}

bool operator==(const BigInteger& lhs, const BigInteger& rhs) {
    if (lhs.positive != rhs.positive) {
        return false;
    }
    return lhs.unsigned_less(rhs) == 0;
}

bool operator!=(const BigInteger& lhs, const BigInteger& rhs) {
    return !(lhs == rhs);
}

bool operator<=(const BigInteger& lhs, const BigInteger& rhs) {
    return !(rhs < lhs);
}

bool operator>=(const BigInteger& lhs, const BigInteger& rhs) {
    return !(lhs < rhs);
}

bool operator>(const BigInteger& lhs, const BigInteger& rhs) {
    return rhs < lhs;
}

int BigInteger::unsigned_less(const BigInteger& other) const {
    if (digits.size() < other.digits.size()) {
        return 1;
    }
    if (digits.size() > other.digits.size()) {
        return -1;
    }
    for (int i = digits.size() - 1; i >= 0; --i) {
        if (digits[i] < other.digits[i]) {
            return 1;
        }
        if (digits[i] > other.digits[i]) {
            return -1;
        }
    }
    return 0;
}

std::istream& operator>>(std::istream& in, BigInteger& x) {
    std::string s;
    in >> s;
    BigInteger new_int(s);
    x = std::move(new_int);
    return in;
}

std::ostream& operator<<(std::ostream& out, const BigInteger& x) {
    out << x.toString();
    return out;
}

std::string BigInteger::toString() const {
    int size = digits.size();
    int s_size = digits.size() + !positive;
    std::string answer(s_size, 0);
    for (int i = 0; i < size; i++) {
        answer[s_size - 1 - i] = digits[i] + '0';
    }
    if (!positive) {
        answer[0] = '-';
    }
    return answer;
}

BigInteger::operator bool() const {
    return *this != zero;
}

