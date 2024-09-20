#pragma once

#include <array>
#include <ranges>

namespace impl {
template<typename T, size_t... DimsSz>
struct ImplArr;

template<typename T, size_t First, size_t... dims_sz>
struct ImplArr<T, First, dims_sz...> {
    using value = std::array<typename ImplArr<T, dims_sz...>::value, First>;
};

template<typename T, size_t sz>
struct ImplArr<T, sz> {
    using value = std::array<T, sz>;
};
} // namespace impl

template<typename T, size_t... DimsSize>
class MDMatrixView;

template<typename T, size_t... DimsSize>
class MDMatrixConstView;

template<typename T, size_t... DimsSize>
class MDMatrixConstViewIterator;

template<typename T, size_t... DimsSize>
class MDMatrixViewIterator;

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
        if constexpr (sizeof...(DimsSize) == 1) {
            return data_[ind];
        } else {
            return MDMatrixView<T, DimsSize...>(data_)[ind];
        }
    }

    decltype(auto) operator[](size_t ind) const {
        if constexpr (sizeof...(DimsSize) > 1) {
            return data_[ind];
        } else {
            return MDMatrixConstView<T, DimsSize...>(data_)[ind];
        }
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
        if constexpr (sizeof...(DimsSize) == 1) {
            return data_.begin();
        } else {
            return MDMatrixConstViewIterator<T, DimsSize...>(data_.begin());
        }
    }

 private:
    impl::ImplArr<T, DimsSize...>::value data_;
};

template<typename T, size_t... DimsSize>
class MDMatrixView {
 public:
    MDMatrixView(impl::ImplArr<T, DimsSize...>::value &data) : data(data) {}

    operator MDMatrixConstView<T, DimsSize...>() const {
        return { data };
    }

    decltype(auto) operator[](size_t ind) {
        if constexpr (sizeof...(DimsSize) == 1) {
            return data[ind];
        } else {
            return [&]<size_t First, size_t... Other>(std::index_sequence<First, Other...>) {
                return MDMatrixView<T, Other...>(data[ind]);
            }(std::index_sequence<DimsSize...>{});
        }
    }

    decltype(auto) operator[](size_t ind) const  {
        if constexpr (sizeof...(DimsSize) == 1) {
            return data[ind];
        } else {
            return [&]<size_t First, size_t... Other>(std::index_sequence<First, Other...>) {
                return MDMatrixConstView<T, Other...>(data[ind]);
            }(std::index_sequence<DimsSize...>{});
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
        constexpr size_t top_dim_sz = []<size_t First, size_t... Other>(std::index_sequence<First, Other...>) {
            return First;
        } (std::index_sequence<DimsSize...>{});
        for (size_t i = 0; i < top_dim_sz; ++i) {
            (*this)[i] -= rhs[i];
        }
        return *this;
    }

    MDMatrixView &operator*=(const T &rhs) {
        constexpr size_t top_dim_sz = []<size_t First, size_t... Other>(std::index_sequence<First, Other...>) {
            return First;
        } (std::index_sequence<DimsSize...>{});
        for (size_t i = 0; i < top_dim_sz; ++i) {
            (*this)[i] *= rhs;
        }
        return *this;
    }

 private:
    impl::ImplArr<T, DimsSize...>::value &data;
};

template<typename T, size_t... DimsSize>
class MDMatrixConstViewIterator {
 public:
    MDMatrixConstViewIterator(impl::ImplArr<T, DimsSize...>::value::iterator data) : data(data) {}

    decltype(auto) operator*() const {
        if constexpr (sizeof...(DimsSize) == 1) {
            return *data;
        } else {
            return [&]<size_t First, size_t... Other>(std::index_sequence<First, Other...>) {
                return MDMatrixConstViewIterator<T, Other...>(*data);
            }(std::index_sequence<DimsSize...>{});
        }
    }

    decltype(auto) operator*()  {
        if constexpr (sizeof...(DimsSize) == 1) {
            return *data;
        } else {
            return [&]<size_t First, size_t... Other>(std::index_sequence<First, Other...>) {
                return MDMatrixConstView<T, Other...>(*data);
            }(std::index_sequence<DimsSize...>{});
        }
    }

    decltype(auto) operator->() const {
        if constexpr (sizeof...(DimsSize) == 1) {
            return data.base();
        } else {
            return [&]<size_t First, size_t... Other>(std::index_sequence<First, Other...>) {
                return MDMatrixConstView<T, Other...>(*data);
            }(std::index_sequence<DimsSize...>{});
        }
    }

    auto operator<=>(const MDMatrixConstViewIterator &rhs) const = default;

 private:
    impl::ImplArr<T, DimsSize...>::value::const_iterator data;
};

template<typename T, size_t... DimsSize>
class MDMatrixConstView {
 public:
    MDMatrixConstView(const impl::ImplArr<T, DimsSize...>::value &data) : data(data) {}
    decltype(auto) operator[](size_t ind) const {
        if constexpr (sizeof...(DimsSize) == 1) {
            return data[ind];
        } else {
            return [&]<size_t First, size_t... Other>(std::index_sequence<First, Other...>) {
                return MDMatrixConstView<T, Other...>(data[ind]);
            }(std::index_sequence<DimsSize...>{});
        }
    }


    auto begin() const {
        if constexpr (sizeof...(DimsSize) == 1) {
            return data.begin();
        } else {
            return MDMatrixConstViewIterator<T, DimsSize...>(data.begin());
        }
    }

    auto end() const {
        if constexpr (sizeof...(DimsSize) == 1) {
            return data.end();
        } else {
            return MDMatrixConstViewIterator<T, DimsSize...>(data.end());
        }
    }

 private:
    const impl::ImplArr<T, DimsSize...>::value &data;
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
