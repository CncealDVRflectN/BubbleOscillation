#include "util.h"
#include <cmath>

double calcGasPressure(double radius, double time)
{
    return (PRESSURE_DEFAULT + 2.0 * SURFACE_TENSION_COEF / RADIUS_DEFAULT) * pow(RADIUS_DEFAULT / radius, 3.0 * SPECIFIC_HEAT_COEF);
}

double calcAcousticWavePressure(double time)
{
    return -AMPLITUDE * PRESSURE_DEFAULT * sin(2.0 * M_PI * FREQUENCY * time);
}

void calcEulerOneStep(void (*calcDerivatives)(double, const CalcUnit*, CalcUnit*),
                        double time, double step, const CalcUnit* value, CalcUnit* result)
{
    calcDerivatives(time, value, result);
    result->derivR = value->derivR + step * result->derivR;
    result->secondDerivR = value->secondDerivR + step * result->secondDerivR;
}

void calcRungeKuttaOneStep(void (*calcDerivatives)(double, const CalcUnit*, CalcUnit*),
                            double time, double step, const CalcUnit* value, CalcUnit *result)
{
    static CalcUnit k1 = {0.0, 0.0};
    static CalcUnit k2 = {0.0, 0.0};
    static CalcUnit k3 = {0.0, 0.0};
    static CalcUnit k4 = {0.0, 0.0};
    static CalcUnit tmp = {0.0, 0.0};

    tmp.derivR = value->derivR;
    tmp.secondDerivR = value->secondDerivR;
    calcDerivatives(time, &tmp, &k1);

    tmp.derivR = value->derivR + 0.5 * step * k1.derivR;
    tmp.secondDerivR = value->secondDerivR + 0.5 * step * k1.secondDerivR;
    calcDerivatives(time + 0.5 * step, &tmp, &k2);

    tmp.derivR = value->derivR + 0.5 * step * k2.derivR;
    tmp.secondDerivR = value->secondDerivR + 0.5 * step * k2.secondDerivR;
    calcDerivatives(time + 0.5 * step, &tmp, &k3);

    tmp.derivR = value->derivR + step * k3.derivR;
    tmp.secondDerivR = value->secondDerivR + step * k3.secondDerivR;
    calcDerivatives(time + step, &tmp, &k4);

    result->derivR = value->derivR + step * (k1.derivR + 2.0 * (k2.derivR + k3.derivR) + k4.derivR) / 6.0;
    result->secondDerivR = value->secondDerivR + step * (k1.secondDerivR + 2.0 * (k2.secondDerivR + k3.secondDerivR) +
                                                         k4.secondDerivR) / 6.0;
}

double calcError(const CalcUnit* stepValue, const CalcUnit* doubleStepValue)
{
    double derivDif = stepValue->derivR - doubleStepValue->derivR;
    double secondDerivDif = stepValue->secondDerivR - doubleStepValue->secondDerivR;
    return sqrt(derivDif * derivDif + secondDerivDif * secondDerivDif);
}
