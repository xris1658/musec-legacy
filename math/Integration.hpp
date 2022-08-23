#ifndef MUSEC_MATH_INTEGRATION
#define MUSEC_MATH_INTEGRATION

#include <math/QuadraticFunction.hpp>

namespace Musec
{
namespace Math
{
double constantIntegration(double c, double from, double to);

double linearFunctionIntegration(double k, double b, double from, double to);

double linearFunctionInvertIntegration(double k, double b, double from, double to);

double quadraticFunctionIntegration(const QuadraticFunction& function, double from, double to);

double quadraticFunctionInvertIntegration(const QuadraticFunction& function, double from, double to);
}
}

#endif //MUSEC_MATH_INTEGRATION
