
// Bitset with non-constexpr size for mainly competitive programming purposes
// It has the most useful and frequent operators.
// In worst cases works ~1.5 times slower than STL std::bitset

#ifndef BITSET_H
#define BITSET_H

#include "iostream"

using namespace std;

class Bitset {
private:
    size_t N = 0;
    size_t capacity = 0;
    unsigned long long *bits = nullptr;

    const size_t BASE = 64;
    const size_t TWO_BASE = 6;
public:

    Bitset(const size_t N) : N(N) {
        capacity = (N + BASE - 1) >> TWO_BASE;
        bits = new unsigned long long[capacity];
        reset();
    }

    Bitset(const Bitset &other) : N(other.N), capacity(other.capacity) {
        bits = new unsigned long long[capacity];
        for (size_t i = 0; i < capacity; ++i) {
            bits[i] = other.bits[i];
        }
    }

    Bitset(Bitset &&other) noexcept {
        swap(N, other.N);
        swap(capacity, other.capacity);
        swap(bits, other.bits);
    }

    Bitset &operator=(Bitset &&other) noexcept {
        delete[] bits;
        bits = nullptr;
        swap(N, other.N);
        swap(capacity, other.capacity);
        swap(bits, other.bits);
        return *this;
    }

    Bitset &operator=(const Bitset &other) {
        if (this == &other) {
            return *this;
        }
        delete[] bits;
        N = other.N;
        capacity = other.capacity;
        bits = new unsigned long long[capacity];
        for (size_t i = 0; i < capacity; ++i) {
            bits[i] = other.bits[i];
        }
        return *this;
    }

    Bitset &set() noexcept {
        for (size_t i = 0; i < capacity; ++i) {
            if (i + 1 == capacity && (N & (BASE - 1)) != 0) {
                bits[i] = ULLONG_MAX & ((1ull << (N & (BASE - 1))) - 1ull);
            } else bits[i] = ULLONG_MAX;
        }
        return *this;
    }

    Bitset &set(size_t index) {
        bits[index >> TWO_BASE] |= (1ull << (index & (BASE - 1)));
        return *this;
    }

    size_t count() const noexcept {
        size_t res = 0;
        for (size_t i = 0; i < capacity; ++i) {
            if (i + 1 == capacity && (N & (BASE - 1)) != 0) {
                res += __builtin_popcountll(bits[i] & ((1ull << (N & (BASE - 1))) - 1ull));
            } else res += __builtin_popcountll(bits[i]);
        }
        return res;
    }

    bool any() const noexcept {
        return count() > 0;
    }

    bool none() const noexcept {
        return count() == 0;
    }

    Bitset &reset() noexcept {
        for (size_t i = 0; i < capacity; ++i) {
            bits[i] = 0ull;
        }
        return *this;
    }

    Bitset &reset(size_t index) {
        bits[index >> TWO_BASE] &= ~(1ull << (index & (BASE - 1)));
        return *this;
    }

    Bitset &flip() noexcept {
        for (size_t i = 0; i < capacity; ++i) {
            bits[i] = ~bits[i];
        }
        return *this;
    }

    Bitset &flip(size_t index) {
        bits[index >> (TWO_BASE)] ^= (1ull << (index & (BASE - 1)));
        return *this;
    }

    Bitset &operator<<=(const size_t shift) {
        Bitset res(N);
        if (shift >= N) {
            return *this = move(res);
        }
        size_t blocks = shift >> TWO_BASE;
        size_t rem = shift & (BASE - 1ull);
        for (size_t i = blocks; i < capacity; ++i) {
            unsigned long long item = bits[i - blocks];
            if (i + 1 == capacity) {
                if (!rem) {
                    res.bits[i] = item;
                } else {
                    res.bits[i] |= (item << rem);
                }
                if ((N & (BASE - 1ull)) > 0) res.bits[i] &= (1ull << (N & (BASE - 1ull))) - 1ull;
            } else {
                if (!rem) {
                    res.bits[i] = item;
                } else {
                    res.bits[i] |= (item << rem);
                    res.bits[i + 1] |= (item >> (BASE - rem));
                }
            }
        }
        return *this = move(res);
    }

    Bitset operator<<(const size_t shift) const {
        Bitset res = *this;
        res <<= shift;
        return res;
    }

    Bitset &operator>>=(const size_t shift) {
        Bitset res(N);
        if (shift >= N) {
            return *this = move(res);
        }
        size_t blocks = shift >> TWO_BASE;
        size_t rem = shift & (BASE - 1);
        for (int i = capacity - blocks - 1; i >= 0; --i) {
            unsigned long long item = bits[i + blocks];
            if (!rem) {
                res.bits[i] = item;
                continue;
            }
            if (i == 0) {
                res.bits[i] |= (item >> rem);
            } else {
                res.bits[i] |= (item >> rem);
                res.bits[i - 1] |= (item << (BASE - rem));
            }
        }
        return *this = move(res);
    }

    Bitset operator>>(const size_t shift) const {
        Bitset res = *this;
        res >>= shift;
        return res;
    }

    Bitset operator~() const {
        Bitset res = *this;
        res.flip();
        return res;
    }

    Bitset &operator&=(const Bitset &other) noexcept {
        for (size_t i = 0; i < capacity; ++i) {
            this->bits[i] &= other.bits[i];
        }
        return *this;
    }


    Bitset operator&(const Bitset &other) const {
        Bitset res = *this;
        res &= other;
        return res;
    }

    Bitset &operator|=(const Bitset &other) noexcept {
        for (size_t i = 0; i < capacity; ++i) {
            this->bits[i] |= other.bits[i];
        }
        return *this;
    }

    Bitset operator|(const Bitset &other) const {
        Bitset res = *this;
        res |= other;
        return res;
    }

    Bitset &operator^=(const Bitset &other) noexcept {
        for (size_t i = 0; i < capacity; ++i) {
            this->bits[i] ^= other.bits[i];
        }
        return *this;
    }

    Bitset operator^(const Bitset &other) const {
        Bitset res = *this;
        res ^= other;
        return res;
    }

    size_t _Find_first() const noexcept {
        // can be improved
        for (size_t i = 0; i < capacity; ++i) {
            if (bits[i] == 0)continue;
            return __lg(bits[i] ^ (bits[i] - 1)) + i * 64;
        }
        return N;
    }

    size_t _Find_next(size_t index) const noexcept {
        // can be done far better
        if ((index & 63ull) != 63) {
            unsigned long long bit = (bits[index >> 6ull] & ~((1ull << ((index & 63ull) + 1)) - 1));
            if (bit != 0)return (index >> 6ull) * 64 + __lg(bit ^ (bit - 1));
        }
        for (size_t i = (index >> 6ull) + 1; i < capacity; ++i) {
            if (bits[i] == 0)continue;
            return __lg(bits[i] ^ (bits[i] - 1)) + i * 64ull;
        }
        return N;
    }

    bool operator[](size_t index) const {
        return bits[index >> 6ull] & (1ull << (index & 63ull));
    }

    ~Bitset() {
        delete[] bits;
    }
};

#endif //BITSET_H
