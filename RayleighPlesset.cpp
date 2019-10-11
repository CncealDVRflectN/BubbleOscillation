#include "RayleighPlesset.h"
#include <cstdio>

void calcDerivativesRayleighPlesset(double time, const CalcUnit& value, CalcUnit& result)
{
    result.derivR = value.secondDerivR;
    result.secondDerivR = ((calcGasPressure(value.derivR, time) - PRESSURE_DEFAULT - calcAcousticWavePressure(time) -
                            2.0 * SURFACE_TENSION_COEF / value.derivR - 4.0 * VISCOSITY * value.secondDerivR / value.derivR) / DENSITY -
                            1.5 * value.secondDerivR * value.secondDerivR) / value.derivR;
}


void calcRayleighPlessetEuler()
{
    FILE* resultOutput;

    fopen_s(&resultOutput, RAYLEIGH_PLESSET_EULER_OUTPUT.c_str(), "w");

    if (resultOutput == nullptr)
    {
        printf("Could not open output file\n");
        return;
    }

    double error = 0.0;
    double delta = 0.0;
    double nextStep;
    double prevStep;
    double time = TIME_DEFAULT;
    double step = STEP_DEFAULT;
    long long iterations = 0;
    CalcUnit result = { RADIUS_DEFAULT, 0.0 };
    CalcUnit prevResult = { RADIUS_DEFAULT, 0.0 };
    CalcUnit stepValueTmp = { 0.0, 0.0 };
    CalcUnit stepValue = { 0.0, 0.0 };
    CalcUnit doubleStepValue = { 0.0, 0.0 };

    printf("Starting Rayleigh-Plesset calculation using Euler method\n");

    while (time < TIME_MAX)
    {
        calcEulerOneStep(&calcDerivativesRayleighPlesset, time, step, result, stepValueTmp);
        calcEulerOneStep(&calcDerivativesRayleighPlesset, time + step, step, stepValueTmp, stepValue);
        calcEulerOneStep(&calcDerivativesRayleighPlesset, time, 2.0 * step, result, doubleStepValue);

        error = calcError(stepValue, doubleStepValue);
        delta = sqrt(EPSILON / fabs(error));

        nextStep = 0.9 * delta * step;
        if (delta < 1.0)
        {
            step = nextStep;
        }
        else if (delta > 1.0)
        {
            time += 2.0 * step;
            prevStep = step;

            prevResult.derivR = result.derivR;
            prevResult.secondDerivR = result.secondDerivR;
            result.derivR = stepValue.derivR;
            result.secondDerivR = stepValue.secondDerivR;

            double tmpStep = (TIME_MAX - time) / 2.0;
            step = (tmpStep < nextStep) ? tmpStep : nextStep;

            if (iterations % 1000 == 0 && fabs(prevResult.derivR - result.derivR) > 2E-12)
            {
                fprintf(resultOutput, "%e %e %e\n", time, result.derivR, prevStep);
                printf("%e : %e\n", time, result.derivR);
            }

            iterations++;
        }
    }

    fclose(resultOutput);
    printf("Iterations num: %lld\n", iterations);
}


void calcRayleighPlessetRungeKutta()
{
    FILE* resultOutput;

    fopen_s(&resultOutput, RAYLEIGH_PLESSET_RUNGE_KUTTA_OUTPUT.c_str(), "w");

    if (resultOutput == nullptr)
    {
        printf("Could not open output file\n");
        return;
    }

    double error = 0.0;
    double delta = 0.0;
    double nextStep;
    double prevStep;
    double time = TIME_DEFAULT;
    double step = STEP_DEFAULT;
    long long iterations = 0;
    CalcUnit result = { RADIUS_DEFAULT, 0.0 };
    CalcUnit prevResult = { RADIUS_DEFAULT, 0.0 };
    CalcUnit stepValueTmp = { 0.0, 0.0 };
    CalcUnit stepValue = { 0.0, 0.0 };
    CalcUnit doubleStepValue = { 0.0, 0.0 };

    printf("Starting Rayleigh-Plesset calculation using Runge-Kutta method\n");

    while (time < TIME_MAX)
    {
        calcRungeKuttaOneStep(&calcDerivativesRayleighPlesset, time, step, result, stepValueTmp);
        calcRungeKuttaOneStep(&calcDerivativesRayleighPlesset, time + step, step, stepValueTmp, stepValue);
        calcRungeKuttaOneStep(&calcDerivativesRayleighPlesset, time, 2.0 * step, result, doubleStepValue);

        error = calcError(stepValue, doubleStepValue) / 15.0;
        delta = pow(EPSILON / fabs(error), 0.2);

        nextStep = 0.9 * delta * step;
        if (delta < 1.0)
        {
            step = nextStep;
        }
        else if (delta > 1.0)
        {
            time += 2.0 * step;
            prevStep = step;

            prevResult.derivR = result.derivR;
            prevResult.secondDerivR = result.secondDerivR;
            result.derivR = stepValue.derivR;
            result.secondDerivR = stepValue.secondDerivR;

            double tmpStep = (TIME_MAX - time) / 2.0;
            step = (tmpStep < nextStep) ? tmpStep : nextStep;

            fprintf(resultOutput, "%e %e %e\n", time, result.derivR, prevStep);
            printf("%e : %e\n", time, result.derivR);

            iterations++;
        }
    }

    fclose(resultOutput);
    printf("Iterations num: %lld\n", iterations);
}