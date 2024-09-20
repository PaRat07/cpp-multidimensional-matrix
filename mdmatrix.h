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
    operator MDMatrixView<T, DimsSize...>() {
        return { data_ };
    }

    operator MDMatrixConstView<T, DimsSize...>() const {
        return { data_ };
    }

    decltype(auto) operator[](size_t ind) {
        return data_[ind];
    }

    decltype(auto) operator[](size_t ind) const {
        return data_[ind];
    }

    MDMatrix &operator+=(const MDMatrix &rhs) {
        MDMatrixView<T, DimsSize...>(*this) += MDMatrixConstView<T, DimsSize...>(rhs);
        return *this;
    }

    MDMatrix &operator-=(const MDMatrix &rhs) {
        MDMatrixView<T, DimsSize...>(*this) -= MDMatrixConstView<T, DimsSize...>(rhs);
        return *this;
    }

    MDMatrix &operator*=(const T &rhs) {
        MDMatrixView<T, DimsSize...>(*this) *= rhs;
        return *this;
    }

    decltype(auto) begin() {
        return data_.begin();
    }

    decltype(auto) end() {
        return data_.end();
    }

    decltype(auto) begin() const {
        return data_.begin();
    }

    decltype(auto) end() const {
        return data_.end();
    }

    decltype(auto) cbegin() const {
        return data_.cbegin();
    }

    decltype(auto) cend() const {
        return data_.cend();
    }

 private:
    impl::ImplMDM<T, DimsSize...>::value data_;
};

template<typename T, size_t... DimsSize>
class MDMatrixView {
 public:
    MDMatrixView(impl::ImplMDM<T, DimsSize...>::value &data) : data(data) {}

    operator MDMatrixConstView<T, DimsSize...>() const {
        return { data };
    }

    decltype(auto) operator[](size_t ind) {
        if constexpr (sizeof...(DimsSize) == 1) {
            return data[ind];
        } else {
            return [&]<size_t First, size_t... Other>(std::index_sequence<First, Other...>) {
                return MDMatrixView<T, Other...>(data[ind]);
            } (std::index_sequence<DimsSize...>());
        }
    }

    decltype(auto) operator[](size_t ind) const {
        if constexpr (sizeof...(DimsSize) == 1) {
            return data[ind];
        } else {
            return [&]<size_t First, size_t... Other>(std::index_sequence<First, Other...>) {
                return MDMatrixConstView<T, Other...>(data[ind]);
            } (std::index_sequence<DimsSize...>());
        }
    }

    MDMatrixView &operator+=(const MDMatrixConstView<T, DimsSize...> &rhs) {
        constexpr size_t top_dim_sz = []<size_t First, size_t... Other>(std::index_sequence<First, Other...>) {
            return First;
        } (std::index_sequence<DimsSize...>{});
        for (size_t i = 0; i < top_dim_sz; ++i) {
            (*this)[i] += rhs[i];
        }
        return *this;
    }

    MDMatrixView &operator-=(const MDMatrixConstView<T, DimsSize...> &rhs) {
        for (size_t i = 0; i < data.size(); ++i) {
            (*this)[i] -= rhs[i];
        }
        return *this;
    }

    MDMatrixView &operator*=(const T &rhs) {
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
    MDMatrixConstView(const impl::ImplMDM<T, DimsSize...>::value &data) : data(data) {}

    decltype(auto) operator[](size_t ind) const {
        if constexpr (sizeof...(DimsSize) == 1) {
            return data[ind];
        } else {
            return [&] <size_t First, size_t... Other>(std::index_sequence<First, Other...>) {
                return MDMatrixConstView<T, Other...>(data[ind]);
            } (std::index_sequence<DimsSize...>());
        }
    }

    auto begin() const {
        return data.begin();
    }

    auto end() const {
        return data.end();
    }

 private:
    const impl::ImplMDM<T, DimsSize...>::value &data;
};

template<typename T, size_t... DimsSize>
MDMatrix<T, DimsSize...> operator+(MDMatrix<T, DimsSize...> lhs, const MDMatrix<T, DimsSize...> &rhs) {
    return lhs += rhs;
}

template<typename T, size_t... DimsSize>
MDMatrix<T, DimsSize...> operator-(MDMatrix<T, DimsSize...> lhs, const MDMatrix<T, DimsSize...> &rhs) {
    return lhs - rhs;
}

template<typename T, size_t... DimsSize>
MDMatrix<T, DimsSize...> operator*(MDMatrix<T, DimsSize...> lhs, const T &rhs) {
    return lhs * rhs;
}
