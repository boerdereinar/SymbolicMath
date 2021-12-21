//
// Created by Einar on 19-12-2021.
//

#pragma once

#include "../symbolicMath.h"

template<auto value>
struct Const {
    static double eval(double x) { return value; }
    using derivative = Const<0>;
};

struct Var {
    static double eval(double x) { return x; }
    using derivative = Const<1>;
};