#ifndef MUSEC_MATH_QUADRATICFUNCTION
#define MUSEC_MATH_QUADRATICFUNCTION

namespace Musec
{
namespace Math
{
struct QuadraticEquationRoot
{
    double x1;
    double x2;
};

QuadraticEquationRoot solveEquation(double a, double b, double c);

struct QuadraticFunction
{
    double a;
    double b;
    double c;
    double delta() const;
    double operator()(double x) const;
};
}
}

#endif //MUSEC_MATH_QUADRATICFUNCTION
