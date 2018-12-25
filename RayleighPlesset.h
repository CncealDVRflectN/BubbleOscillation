#ifndef BUBBLEOSCILLATION_RAYLEIGHPLESSET_H
#define BUBBLEOSCILLATION_RAYLEIGHPLESSET_H

#include "util.h"

inline const char* RAYLEIGH_PLESSET_EULER_OUTPUT = "rayleigh-plesset-euler.dat";
inline const char* RAYLEIGH_PLESSET_RUNGE_KUTTA_OUTPUT = "rayleigh-plesset-runge-kutta.dat";

void calcRayleighPlessetEuler();

void calcRayleighPlessetRungeKutta();

#endif //BUBBLEOSCILLATION_RAYLEIGHPLESSET_H
