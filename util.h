#ifndef BUBBLEOSCILLATION_UTIL_H
#define BUBBLEOSCILLATION_UTIL_H

#define _USE_MATH_DEFINES
#include <math.h>


typedef struct
{
    double derivR;
    double secondDerivR;
} CalcUnit;

typedef struct
{
    double time;
    double radius;
    double step;
} ResultUnit;

const static double RADIUS_DEFAULT = 0.000005;
const static double PRESSURE_DEFAULT = 101400.0;
const static double SURFACE_TENSION_COEF = 0.0725;
const static double FREQUENCY = 20000.0;
const static double DENSITY = 1000.0;
const static double VISCOSITY = 0.003;
const static double SPECIFIC_HEAT_COEF = 1.4;
const static double EPSILON = 1E-8;
const static double AMPLITUDE = 1.28;
const static double ANGULAR_FREQUENCY = 125663.706144;
const static double SOUND_VELOCITY = 1500;

const static double TIME_DEFAULT = 0.0;
const static double TIME_MAX = 0.00025;
const static double STEP_DEFAULT = 1E-9;

double calcGasPressure(double radius, double time);

double calcAcousticWavePressure(double time);

double calcBoundaryPressure(const CalcUnit& value, double time);

double calcOmega(const CalcUnit& value, double time);

void calcEulerOneStep(void (*calcDerivatives)(double, const CalcUnit&, CalcUnit&),
                        double time, double step, const CalcUnit& value, CalcUnit& result);

void calcRungeKuttaOneStep(void (*calcDerivatives)(double, const CalcUnit&, CalcUnit&),
                            double time, double step, const CalcUnit& value, CalcUnit& result);

double calcError(const CalcUnit& stepValue, const CalcUnit& doubleStepValue);

#endif //BUBBLEOSCILLATION_UTIL_H
