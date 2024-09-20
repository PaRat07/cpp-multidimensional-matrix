#pragma once

#include <array>
#include <ranges>
#include <stacktrace>

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
class MDMatrixView;

template<typename T, size_t... DimsSize>
class MDMatrixConstView;

template<typename T, size_t... DimsSize>
class MDMatrix {
 public:
    constexpr operator MDMatrixView<T, DimsSize...>() {
        return { data_ };
    }

    constexpr operator MDMatrixConstView<T, DimsSize...>() const {
        return { data_ };
    }

    constexpr decltype(auto) operator[](size_t ind) {
        return data_[ind];
    }

    constexpr decltype(auto) operator[](size_t ind) const {
        return data_[ind];
    }

    constexpr MDMatrix &operator+=(const MDMatrix &rhs) {
        MDMatrixView<T, DimsSize...>(*this) += MDMatrixConstView<T, DimsSize...>(rhs);
        return *this;
    }

    constexpr MDMatrix &operator-=(const MDMatrix &rhs) {
        MDMatrixView<T, DimsSize...>(*this) -= MDMatrixConstView<T, DimsSize...>(rhs);
        return *this;
    }

    constexpr MDMatrix &operator*=(const T &rhs) {
        MDMatrixView<T, DimsSize...>(*this) *= rhs;
        return *this;
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

 private:
    impl::ImplMDM<T, DimsSize...>::value data_;
};

template<typename T, size_t... DimsSize>
class MDMatrixView {
 public:
    constexpr MDMatrixView(impl::ImplMDM<T, DimsSize...>::value &data) : data(data) {}

    constexpr operator MDMatrixConstView<T, DimsSize...>() const {
        return { data };
    }

    constexpr decltype(auto) operator[](size_t ind) {
        if constexpr (sizeof...(DimsSize) == 1) {
            return data[ind];
        } else {
            return [&]<size_t First, size_t... Other>(std::index_sequence<First, Other...>) {
                return MDMatrixView<T, Other...>(data[ind]);
            } (std::index_sequence<DimsSize...>());
        }
    }

    constexpr decltype(auto) operator[](size_t ind) const {
        if constexpr (sizeof...(DimsSize) == 1) {
            return data[ind];
        } else {
            return [&]<size_t First, size_t... Other>(std::index_sequence<First, Other...>) {
                return MDMatrixConstView<T, Other...>(data[ind]);
            } (std::index_sequence<DimsSize...>());
        }
    }

    constexpr MDMatrixView &operator+=(const MDMatrixConstView<T, DimsSize...> &rhs) {
        for (size_t i = 0; i < data.size(); ++i) {
            (*this)[i] += rhs[i];
        }
        return *this;
    }

    constexpr MDMatrixView &operator-=(const MDMatrixConstView<T, DimsSize...> &rhs) {
        for (size_t i = 0; i < data.size(); ++i) {
            (*this)[i] -= rhs[i];
        }
        return *this;
    }

    constexpr MDMatrixView &operator*=(const T &rhs) {
        for (size_t i = 0; i < data.size(); ++i) {
            (*this)[i] *= rhs;
        }
        return *this;
    }

 private:
    impl::ImplMDM<T, DimsSize...>::value &data;
};

template<typename T, size_t... DimsSize>
class MDMatrixConstView {
 public:
    constexpr MDMatrixConstView(const impl::ImplMDM<T, DimsSize...>::value &data) : data(data) {}

    constexpr decltype(auto) operator[](size_t ind) const {
        if constexpr (sizeof...(DimsSize) == 1) {
            return data[ind];
        } else {
            return [&] <size_t First, size_t... Other>(std::index_sequence<First, Other...>) {
                return MDMatrixConstView<T, Other...>(data[ind]);
            } (std::index_sequence<DimsSize...>());
        }
    }

    constexpr auto begin() const {
        return data.begin();
    }

    constexpr auto end() const {
        return data.end();
    }

    constexpr auto cbegin() const {
        return data.begin();
    }

    constexpr auto cend() const {
        return data.end();
    }

 private:
    const impl::ImplMDM<T, DimsSize...>::value &data;
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
