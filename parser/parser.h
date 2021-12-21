//
// Created by Einar on 18-12-2021.
//

#pragma once

#include <iostream>
#include <type_traits>
#include <utility>
#include "../symbolicMath.h"
#include "fixedString.h"
#include "stack.h"
#include "select.h"

template<typename T, typename U> struct stop_condition : std::true_type {};
template<char o1, char o2>
struct stop_condition<Operator<o1>, Operator<o2>> : std::integral_constant<bool,
        (Operator<o1>::precedence >= Operator<o2>::precedence && (Operator<o1>::precedence != Operator<o2>::precedence || Operator<o1>::associativity != Associativity::Left))> {};
template<typename T, typename U> inline constexpr bool stop_condition_v = stop_condition<T, U>::value;

// Parse operation
template<typename S, typename T, typename U, typename Enable = void> struct parse_operation : std::true_type {};

template<char o, typename T, typename U>
struct parse_operation<Operator<o>, T, U, std::enable_if_t<is_operator(o), void>> {
    using type = typename Operator<o>::template op<T, U>;
};

template<typename S, typename T, typename U> using parse_operation_t = typename parse_operation<S, T, U>::type;

// Parse function
template<typename F, typename G> struct parse_function : std::true_type {};

template<typename G, char... chars>
struct parse_function<Function<chars...>, G> {
    using type = typename Function<chars...>::template function<G>;
};

template<typename F, typename G>
using parse_function_t = typename parse_function<F, G>::type;

// Function stack
template<typename U, char... chars>
struct stack<Function<chars...>, U> {
    using peek = Function<chars...>;
    using pop = U;
    template<class V> using push = stack<V, stack<Function<chars...>, U>>;
    template<char c> using push_function = stack<Function<chars..., c>, U>;
};

/// Compile-time implementation of the Shunting-yard algorithm.
/// \tparam Operators operator stack
/// \tparam Operands operand stack
/// \tparam T fixed_string input
template<typename Operators, typename Operands, typename T> struct Parser;

/// Base-case: operator stack is empty and input is empty.
/// \tparam Operands operand stack
template<typename Operands>
struct Parser<Operands, stack_end, fixed_string<>> {
    using parse = typename Operands::peek;
};

/// Base-case: input is empty.
/// \tparam Operands operand stack
/// \tparam Operators operator stack
template<typename Operands, typename Operators>
struct Parser<Operands, Operators, fixed_string<>> {
    using parse = typename Parser<
            typename Operands::pop::pop::template push<parse_operation_t<typename Operators::peek, typename Operands::pop::peek, typename Operands::peek>>,
            typename Operators::pop,
            fixed_string<>>::parse;
};

/// Body: input still contains characters.
/// \tparam Operands operand stack
/// \tparam Operators operator stack
/// \tparam c first character of input
/// \tparam chars left-over characters in the input
template<typename Operands, typename Operators, char c, char... chars>
struct Parser<Operands, Operators, fixed_string<c, chars...>> {
    using parse = typename Parser<
            select_t<
                cond<c == ' ', Operands>,
                cond<'0' <= c && c <= '9', typename Operands::template push<Const<static_cast<int>(c - '0')>>>,
                cond<c == 'x', typename Operands::template push<Var>>,
                cond<c == '(', Operands>,
                cond<c == ')', std::conditional_t<
                        std::is_same_v<typename Operators::peek, Operator<'('>>,
                            std::conditional_t<
                                    is_function_v<typename Operators::pop::peek>,
                                        typename Operands::pop::template push<parse_function_t<typename Operators::pop::peek, typename Operands::peek>>,
                                        Operands>,
                            typename Operands::pop::pop::template push<parse_operation_t<typename Operators::peek, typename Operands::pop::peek, typename Operands::peek>>>>,
                cond<is_operator(c), std::conditional_t<
                        stop_condition_v<Operator<c>, typename Operators::peek>,
                            Operands,
                            typename Operands::pop::pop::template push<parse_operation_t<typename Operators::peek, typename Operands::pop::peek, typename Operands::peek>>>>,
                Operands
            >,
            select_t<
                cond<c == ' ', Operators>,
                cond<'0' <= c && c <= '9', Operators>,
                cond<c == 'x', Operators>,
                cond<c == '(', typename Operators::template push<Operator<c>>>,
                cond<c == ')', std::conditional_t<
                        (std::is_same_v<typename Operators::peek, Operator<'('>> && is_function_v<typename Operators::pop::peek>),
                            typename Operators::pop::pop,
                            typename Operators::pop>>,
                cond<is_operator(c), std::conditional_t<
                        stop_condition_v<Operator<c>, typename Operators::peek>,
                            typename Operators::template push<Operator<c>>,
                            typename Operators::pop>>,
                typename Operators::template push_function<c>
            >,
            select_t<
                cond<c == ' ', fixed_string<chars...>>,
                cond<'0' <= c && c <= '9', fixed_string<chars...>>,
                cond<c == 'x', fixed_string<chars...>>,
                cond<c == '(', fixed_string<chars...>>,
                cond<c == ')', std::conditional_t<
                        std::is_same_v<typename Operators::peek, Operator<'('>>,
                            fixed_string<chars...>,
                            fixed_string<c, chars...>>>,
                cond<is_operator(c), std::conditional_t<
                        stop_condition_v<Operator<c>, typename Operators::peek>,
                            fixed_string<chars...>,
                            fixed_string<c, chars...>>>,
                fixed_string<chars...>
            >>::parse;
};