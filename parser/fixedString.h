//
// Created by Einar on 21-12-2021.
//

#pragma once

template<char... chars>
using fixed_string = std::integer_sequence<char, chars...>;

template<typename T, T... chars>
constexpr fixed_string<chars...> operator""_tstr() { return {}; };