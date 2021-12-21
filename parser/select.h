//
// Created by Einar on 18-12-2021.
//

#pragma once

#include <type_traits>

template<bool B, typename T>
struct cond {
    static constexpr bool value = B;
    using type = T;
};

template<typename T, typename... Ts>
struct select_cond {
    using type = typename std::conditional_t<T::value, T, select_cond<Ts...>>::type;
};

template<typename T>
struct select_cond<T> {
    using type = T;
};

template<bool B, typename T>
struct select_cond<cond<B, T>> {
    static_assert(B);
    using type = T;
};

template<typename T, typename... Ts>
using select_t = typename select_cond<T, Ts...>::type;