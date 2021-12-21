//
// Created by Einar on 19-12-2021.
//

#pragma once

#include <cmath>
#include "../symbolicMath.h"

template<typename F, typename G> struct Add;
template<typename F, typename G> struct Sub;
template<typename F, typename G> struct Mul;
template<typename F, typename G> struct Div;
template<typename F, typename G> struct Pow;

template<typename F, typename G>
struct Add {
    static double eval(double x) { return F::eval(x) + G::eval(x); }
    using derivative =  Add<typename F::derivative, typename G::derivative>;
};

template<typename F, typename G>
struct Sub {
    static double eval(double x) { return F::eval(x) - G::eval(x); }
    using derivative = Sub<typename F::derivative, typename G::derivative>;
};

template<typename F, typename G>
struct Mul {
    static double eval(double x) { return F::eval(x) * G::eval(x); }
    using derivative = Add<Mul<typename F::derivative, G>, Mul<F, typename G::derivative>>;
};

template<typename F, typename G>
struct Div {
    static double eval(double x) { return F::eval(x) / G::eval(x); }
    using derivative = Div<Sub<Mul<typename F::derivative, G>, Mul<F, typename G::derivative>>, Mul<G, G>>;
};

template<typename F, int E>
struct Pow<F, Const<E>> {
    static double eval(double x) { return pow(F::eval(x), E); }
    using derivative = Mul<Const<E>, Pow<F, Const<E - 1>>>;
};

template<typename F>
struct Pow<F, Const<0>> {
    static double eval(double x) { return 1; }
    using derivative = Const<0>;
};

enum Associativity {
    Left, Right, None
};

template<int p, Associativity a>
struct OperatorBase {
    static const int precedence = p;
    static const Associativity associativity = a;
};

template<char c>
struct Operator : OperatorBase<-1, Associativity::None> {};

template<> struct Operator<'+'> : OperatorBase<2, Associativity::Left> {
    template<class T, class U> using op = Add<T, U>;
};
template<> struct Operator<'-'> : OperatorBase<2, Associativity::Left> {
    template<class T, class U> using op = Sub<T, U>;
};
template<> struct Operator<'*'> : OperatorBase<3, Associativity::Left> {
    template<class T, class U> using op = Mul<T, U>;
};
template<> struct Operator<'/'> : OperatorBase<3, Associativity::Left> {
    template<class T, class U> using op = Div<T, U>;
};
template<> struct Operator<'^'> : OperatorBase<4, Associativity::Right> {
    template<class T, class U> using op = Pow<T, U>;
};

constexpr bool is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}