//
// Created by Einar on 20-12-2021.
//

#pragma once

template<typename T, typename U> struct stack;

struct stack_end {
    using peek = stack_end;
    using pop = stack_end;
    template<class T> using push = stack<T, stack_end>;
    template<char c> using push_function = push<Function<c>>;
};

template<typename T, typename U>
struct stack {
    using peek = T;
    using pop = U;
    template<class V> using push = stack<V, stack<T, U>>;
    template<char c> using push_function = push<Function<c>>;
};