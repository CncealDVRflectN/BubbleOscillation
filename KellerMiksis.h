#ifndef BUBBLEOSCILLATION_KELLERMIKSIS_H
#define BUBBLEOSCILLATION_KELLERMIKSIS_H

#include "util.h"
#include <string>

const std::string KELLER_MIKSIS_EULER_OUTPUT = "keller-miksis-euler.dat";
const std::string KELLER_MIKSIS_RUNGE_KUTTA_OUTPUT = "keller-miksis-runge-kutta.dat";

void calcKellerMiksisEuler();

void calcKellerMiksisRungeKutta();

#endif //BUBBLEOSCILLATION_KELLERMIKSIS_H
