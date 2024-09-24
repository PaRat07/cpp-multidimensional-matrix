#pragma once

#include <array>
#include <ranges>

template<typename T, size_t... DimsSize>
class MDMatrix;

namespace impl {
template<typename T, size_t... DimsSz>
struct ImplMDM;

template<typename T, size_t First, size_t... dims_sz>
struct ImplMDM<T, First, dims_sz...> {
    using value = std::array<MDMatrix<T, dims_sz...>, First>;
};

template<typename T, size_t DimSz>
struct ImplMDM<T, DimSz> {
    using value = std::array<T, DimSz>;
};
} // namespace impl

template<typename T, size_t... DimsSize>
class MDMatrix {
 public:
    constexpr decltype(auto) operator[](size_t ind) {
        return data_[ind];
    }

    constexpr decltype(auto) operator[](size_t ind) const {
        return data_[ind];
    }

    constexpr decltype(auto) begin() {
        return data_.begin();
    }

    constexpr decltype(auto) end() {
        return data_.end();
    }

    constexpr decltype(auto) begin() const {
        return data_.begin();
    }

    constexpr decltype(auto) end() const {
        return data_.end();
    }

    constexpr decltype(auto) cbegin() const {
        return data_.cbegin();
    }

    constexpr decltype(auto) cend() const {
        return data_.cend();
    }

    constexpr MDMatrix &operator+=(const MDMatrix &rhs) {
        for (size_t i = 0; i < data_.size(); ++i) {
            (*this)[i] += rhs[i];
        }
        return *this;
    }

    constexpr MDMatrix &operator-=(const MDMatrix &rhs) {
        for (size_t i = 0; i < data_.size(); ++i) {
            (*this)[i] -= rhs[i];
        }
        return *this;
    }

    constexpr MDMatrix &operator*=(const T &rhs) {
        for (size_t i = 0; i < data_.size(); ++i) {
            (*this)[i] *= rhs;
        }
        return *this;
    }

 private:
    impl::ImplMDM<T, DimsSize...>::value data_;
};

template<typename T, size_t... DimsSize>
constexpr MDMatrix<T, DimsSize...> operator+(MDMatrix<T, DimsSize...> lhs, const MDMatrix<T, DimsSize...> &rhs) {
    return lhs += rhs;
}

template<typename T, size_t... DimsSize>
constexpr MDMatrix<T, DimsSize...> operator-(MDMatrix<T, DimsSize...> lhs, const MDMatrix<T, DimsSize...> &rhs) {
    return lhs - rhs;
}

template<typename T, size_t... DimsSize>
constexpr MDMatrix<T, DimsSize...> operator*(MDMatrix<T, DimsSize...> lhs, const T &rhs) {
    return lhs * rhs;
}
