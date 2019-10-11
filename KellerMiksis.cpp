#include "KellerMiksis.h"
#include <cstdio>


void calcDerivativesKellerMiksis(double time, const CalcUnit& value, CalcUnit& result)
{
    result.derivR = value.secondDerivR;
    result.secondDerivR = ((1.0 + value.secondDerivR / SOUND_VELOCITY) * (calcBoundaryPressure(value, time) - PRESSURE_DEFAULT) / DENSITY -
            1.5 * value.secondDerivR * value.secondDerivR * (1.0 - value.secondDerivR / (3.0 * SOUND_VELOCITY)) +
            value.derivR * calcOmega(value, time) / (SOUND_VELOCITY * DENSITY)) /
            (value.derivR * (1.0 - value.secondDerivR / SOUND_VELOCITY) + 4.0 * VISCOSITY / (SOUND_VELOCITY * DENSITY));
}

void calcKellerMiksisEuler()
{
    FILE* resultOutput;

    fopen_s(&resultOutput, KELLER_MIKSIS_EULER_OUTPUT.c_str(), "w");

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

    printf("Starting Keller-Miksis calculation using Euler method\n");

    while (time < TIME_MAX)
    {
        calcEulerOneStep(&calcDerivativesKellerMiksis, time, step, result, stepValueTmp);
        calcEulerOneStep(&calcDerivativesKellerMiksis, time + step, step, stepValueTmp, stepValue);
        calcEulerOneStep(&calcDerivativesKellerMiksis, time, 2.0 * step, result, doubleStepValue);

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

void calcKellerMiksisRungeKutta()
{
    FILE* resultOutput;

    fopen_s(&resultOutput, KELLER_MIKSIS_RUNGE_KUTTA_OUTPUT.c_str(), "w");

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

    printf("Starting Keller-Miksis calculation using Runge-Kutta method\n");

    while (time < TIME_MAX)
    {
        calcRungeKuttaOneStep(&calcDerivativesKellerMiksis, time, step, result, stepValueTmp);
        calcRungeKuttaOneStep(&calcDerivativesKellerMiksis, time + step, step, stepValueTmp, stepValue);
        calcRungeKuttaOneStep(&calcDerivativesKellerMiksis, time, 2.0 * step, result, doubleStepValue);

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
