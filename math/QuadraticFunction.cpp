#include "QuadraticFunction.hpp"

#include <cmath>
#include <stdexcept>

namespace Musec::Math
{
QuadraticEquationRoot solveEquation(double a, double b, double c)
{
    double delta = b * b - 4 * a * c;
    if(delta < 0)
    {
        return {NAN, NAN};
    }
    if(delta == 0)
    {
        return {-1 * b / a * 0.5, -1 * b / a * 0.5};
    }
    if(delta > 0)
    {
        auto sqrtDelta = std::sqrt(delta);
        return {(-1 * b + sqrtDelta) / a * 0.5, (-1 * b - sqrtDelta) / a * 0.5};
    }
}

double QuadraticFunction::delta() const
{
    return b * b  - 4 * a * c;
}

double QuadraticFunction::operator()(double x) const
{
    return a * x * x + b * x + c;
}
}


