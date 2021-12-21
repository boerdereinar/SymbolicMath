# Symbolic Math

Compile-time math equation evaluator and parser.

## Example
```c++
double y = Parser<stack_end, stack_end, decltype("x^2 + 2*x + 5"_tstr)>::parse::eval(2);
// y = 13
```

## Supported operations
`+`,
`-`,
`*`,
`/`,
`^`,
`x`,
`(...)`,
`abs(...)`,
`sqrt(...)`,
`sin(...)`,
`asin(...)`,
`csc(...)`,
`acsc(...)`,
`cos(...)`,
`acos(...)`,
`sec(...)`,
`asec(...)`,
`tan(...)`,
`atan(...)`,
`cot(...)`,
`acot(...)`