//
// Created by Einar on 19-12-2021.
//

#pragma once

#include <cmath>
#include "../symbolicMath.h"

template<typename F> struct Neg;
template<typename F> struct Abs;
template<typename F> struct Sqrt;
template<typename F> struct Sin;
template<typename F> struct Asin;
template<typename F> struct Csc;
template<typename F> struct Acsc;
template<typename F> struct Cos;
template<typename F> struct Acos;
template<typename F> struct Sec;
template<typename F> struct Asec;
template<typename F> struct Tan;
template<typename F> struct Atan;
template<typename F> struct Cot;
template<typename F> struct Acot;

template<typename F>
struct Neg {
    static double eval(double x) { return -1 * F::eval(x); }
    using derivative = Neg<typename F::derivative>;
};

template<typename F>
struct Abs {
    static double eval(double x) { return abs(F::eval(x)); }
    using derivative = Abs<typename F::derivative>;
};

template<typename F>
struct Sqrt {
    static double eval(double x) { return sqrt(x); }
    using derivative = Div<Const<1>, Mul<Const<2>, Sqrt<F>>>;
};

template<typename F>
struct Sin {
    static double eval(double x) { return sin(F::eval(x)); }
    using derivative = Mul<typename F::derivative, Cos<F>>;
};

template<typename F>
struct Asin {
    static double eval(double x) { return asin(F::eval(x)); }
    using derivative = Div<typename F::derivative, Sqrt<Sub<Const<1>, Pow<Var, Const<2>>>>>;
};

template<typename F>
struct Csc {
    static double eval(double x) { return 1/sin(F::eval(x)); }
    using derivative = Mul<typename F::derivative, Neg<Mul<Cot<F>, Csc<F>>>>;
};

template<typename F>
struct Acsc {
    static double eval(double x) { return asin(1/F::eval(x)); }
    using derivative = Neg<Div<typename F::derivative, Sqrt<Mul<Abs<F>, Sub<Pow<F, Const<2>>, Const<1>>>>>>;
};

template<typename F>
struct Cos {
    static double eval(double x) { return cos(F::eval(x)); }
    using derivative = Mul<typename F::derivative, Neg<Sin<F>>>;
};

template<typename F>
struct Acos {
    static double eval(double x) { return acos(F::eval(x)); }
    using derivative = Neg<Div<typename F::derivative, Sqrt<Sub<Const<1>, Pow<Var, Const<2>>>>>>;
};

template<typename F>
struct Sec {
    static double eval(double x) { return 1/cos(F::eval(x)); }
    using derivative = Mul<typename F::derivative, Mul<Tan<F>, Sec<F>>>;
};

template<typename F>
struct Asec {
    static double eval(double x) { return acos(1/F::eval(x)); }
    using derivative = Div<typename F::derivative, Sqrt<Mul<Abs<F>, Sub<Pow<F, Const<2>>, Const<1>>>>>;
};

template<typename F>
struct Tan {
    static double eval(double x) { return tan(F::eval(x)); }
    using derivative = Mul<typename F::derivative, Mul<Sec<F>, Sec<F>>>;
};

template<typename F>
struct Atan {
    static double eval(double x) { return atan(F::eval(x)); }
    using derivative = Div<typename F::derivative, Add<Const<1>, Pow<Var, Const<2>>>>;
};

template<typename F>
struct Cot {
    static double eval(double x) { return 1/tan(F::eval(x)); }
    using derivative = Mul<typename F::derivative, Neg<Mul<Cot<F>, Csc<F>>>>;
};

template<typename F>
struct Acot {
    static double eval(double x) { return atan(1/F::eval(x)); }
    using derivative = Neg<Div<typename F::derivative, Add<Const<1>, Pow<Var, Const<2>>>>>;
};

template<char c, char... chars>
struct Function {
    template<class F> using function = Const<-1>;
};

template<> struct Function<'a','b','s'> {
    template<class F> using function = Abs<F>;
};

template<> struct Function<'s','q','r','t'> {
    template<class F> using function = Sqrt<F>;
};

template<> struct Function<'s','i','n'> {
    template<class F> using function = Sin<F>;
};

template<> struct Function<'a','s','i','n'> {
    template<class F> using function = Asin<F>;
};

template<> struct Function<'c','s','c'> {
    template<class F> using function = Csc<F>;
};

template<> struct Function<'a','c','s','c'> {
    template<class F> using function = Acsc<F>;
};

template<> struct Function<'c','o','s'> {
    template<class F> using function = Cos<F>;
};

template<> struct Function<'a','c','o','s'> {
    template<class F> using function = Acos<F>;
};

template<> struct Function<'s','e','c'> {
    template<class F> using function = Sec<F>;
};

template<> struct Function<'a','s','e','c'> {
    template<class F> using function = Asec<F>;
};

template<> struct Function<'t','a','n'> {
    template<class F> using function = Tan<F>;
};

template<> struct Function<'a','t','a','n'> {
    template<class F> using function = Atan<F>;
};

template<> struct Function<'c','o','t'> {
    template<class F> using function = Cot<F>;
};

template<> struct Function<'a','c','o','t'> {
    template<class F> using function = Acot<F>;
};

template<typename T> struct is_function : std::false_type {};
template<char... chars> struct is_function<Function<chars...>> : std::true_type {};

template<typename T> inline constexpr bool is_function_v = is_function<T>::value;