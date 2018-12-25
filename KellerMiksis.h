#ifndef BUBBLEOSCILLATION_KELLERMIKSIS_H
#define BUBBLEOSCILLATION_KELLERMIKSIS_H

#include "util.h"

inline const char* KELLER_MIKSIS_EULER_OUTPUT = "keller-miksis-euler.dat";
inline const char* KELLER_MIKSIS_RUNGE_KUTTA_OUTPUT = "keller-miksis-runge-kutta.dat";

void calcKellerMiksisEuler();

void calcKellerMiksisRungeKutta();

#endif //BUBBLEOSCILLATION_KELLERMIKSIS_H
