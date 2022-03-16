#ifndef MUSEC_MATH_INTEGRATION
#define MUSEC_MATH_INTEGRATION

#include <math/QuadraticFunction.hpp>

namespace Musec
{
namespace Math
{
// 常数函数求定积分
double constantIntegration(double c, double from, double to);
// 一次函数求定积分
double linearFunctionIntegration(double k, double b, double from, double to);
// 一次函数取倒求定积分
double linearFunctionInvertIntegration(double k, double b, double from, double to);
// 二次函数求定积分
double quadraticFunctionIntegration(const QuadraticFunction& function, double from, double to);
// 二次函数取倒求定积分
double quadraticFunctionInvertIntegration(const QuadraticFunction& function, double from, double to);
}
}

#endif //MUSEC_MATH_INTEGRATION
