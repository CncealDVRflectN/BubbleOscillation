#ifndef BUBBLEOSCILLATION_RAYLEIGHPLESSET_H
#define BUBBLEOSCILLATION_RAYLEIGHPLESSET_H

#include "util.h"
#include <string>

const std::string RAYLEIGH_PLESSET_EULER_OUTPUT = "rayleigh-plesset-euler.dat";
const std::string RAYLEIGH_PLESSET_RUNGE_KUTTA_OUTPUT = "rayleigh-plesset-runge-kutta.dat";


void calcRayleighPlessetEuler();

void calcRayleighPlessetRungeKutta();

#endif //BUBBLEOSCILLATION_RAYLEIGHPLESSET_H
