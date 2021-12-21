//
// Created by Einar on 19-12-2021.
//

#pragma once

#include "../symbolicMath.h"

struct Pi {
    static double eval(double x) { return 3.14159265358979323846; }
    using derivative = Const<0>;
};

struct E {
    static double eval(double x) { return 2.71828182845904523536; }
    using derivative = Const<0>;
};