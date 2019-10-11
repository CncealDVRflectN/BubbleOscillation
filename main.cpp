#define GNUPLOT             "gnuplot"
#define GNUPLOT_WIN_WIDTH   1280
#define GNUPLOT_WIN_HEIGHT  720

#include <iostream>
#include "RayleighPlesset.h"
#include "KellerMiksis.h"

#include <stdio.h>
#if defined(WIN32) || defined(_WIN32)
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

int main(int argc, char* argv[])
{
    char workingDir[FILENAME_MAX];
    int size = sizeof(workingDir);
    if (!GetCurrentDir(workingDir, size))
    {
        printf("Could not find working directory\n");
        return errno;
    }
    workingDir[size - 1] = '\0';
    printf("The current working directory is %s\n", workingDir);

    FILE* plotPipeRayleighPlessetEuler;
    FILE* plotPipeRayleighPlessetRungeKutta;
    FILE* plotPipeRayleighPlessetSteps;

    FILE* plotPipeKellerMiksisEuler;
    FILE* plotPipeKellerMiksisRungeKutta;
    FILE* plotPipeKellerMiksisEulerSteps;
    FILE* plotPipeKellerMiksisRungeKuttaSteps;

#if defined(WIN32) || defined(_WIN32)
    plotPipeRayleighPlessetEuler = _popen(GNUPLOT, "w");
    plotPipeRayleighPlessetRungeKutta = _popen(GNUPLOT, "w");
    plotPipeRayleighPlessetSteps = _popen(GNUPLOT, "w");
    plotPipeKellerMiksisEuler = _popen(GNUPLOT, "w");
    plotPipeKellerMiksisRungeKutta = _popen(GNUPLOT, "w");
    plotPipeKellerMiksisEulerSteps = _popen(GNUPLOT, "w");
    plotPipeKellerMiksisRungeKuttaSteps = _popen(GNUPLOT, "w");
#else
    plotPipeRayleighPlessetEuler = popen(GNUPLOT, "w");
    plotPipeRayleighPlessetRungeKutta = popen(GNUPLOT, "w");
    plotPipeRayleighPlessetSteps = popen(GNUPLOT, "w");
    plotPipeKellerMiksisEuler = popen(GNUPLOT, "w");
    plotPipeKellerMiksisRungeKutta = popen(GNUPLOT, "w");
    plotPipeKellerMiksisEulerSteps = popen(GNUPLOT, "w");
    plotPipeKellerMiksisRungeKuttaSteps = popen(GNUPLOT, "w");
#endif

    if (plotPipeRayleighPlessetEuler == nullptr || plotPipeRayleighPlessetRungeKutta == nullptr || plotPipeRayleighPlessetSteps == nullptr ||
        plotPipeKellerMiksisEuler == nullptr || plotPipeKellerMiksisRungeKutta == nullptr || plotPipeKellerMiksisEulerSteps == nullptr || 
        plotPipeKellerMiksisRungeKuttaSteps == nullptr)
    {
        printf("Could not open gnuplot pipe\n");
        return -1;
    }

    calcRayleighPlessetEuler();
    calcRayleighPlessetRungeKutta();
    calcKellerMiksisEuler();
    calcKellerMiksisRungeKutta();

    fprintf(plotPipeRayleighPlessetEuler, "set term wxt size %d, %d\n", GNUPLOT_WIN_WIDTH, GNUPLOT_WIN_HEIGHT);
    fprintf(plotPipeRayleighPlessetEuler, "set title \"Rayleigh-Plesset model (Euler method)\"\n");
    fprintf(plotPipeRayleighPlessetEuler, "set xlabel \"Time\"\n");
    fprintf(plotPipeRayleighPlessetEuler, "set ylabel \"Radius, m\"\n");
    fprintf(plotPipeRayleighPlessetEuler, "plot '%s\\%s' using 1:2 with line notitle\n",
            workingDir, RAYLEIGH_PLESSET_EULER_OUTPUT.c_str());
    fflush(plotPipeRayleighPlessetEuler);


    fprintf(plotPipeRayleighPlessetRungeKutta, "set term wxt size %d, %d\n", GNUPLOT_WIN_WIDTH, GNUPLOT_WIN_HEIGHT);
    fprintf(plotPipeRayleighPlessetRungeKutta, "set title \"Rayleigh-Plesset model (Runge-Kutta method)\"\n");
    fprintf(plotPipeRayleighPlessetRungeKutta, "set xlabel \"Time\"\n");
    fprintf(plotPipeRayleighPlessetRungeKutta, "set ylabel \"Radius, m\"\n");
    fprintf(plotPipeRayleighPlessetRungeKutta, "plot '%s\\%s' using 1:2 with line notitle\n",
            workingDir, RAYLEIGH_PLESSET_RUNGE_KUTTA_OUTPUT.c_str());
    fflush(plotPipeRayleighPlessetRungeKutta);


    fprintf(plotPipeRayleighPlessetSteps, "set term wxt size %d, %d\n", GNUPLOT_WIN_WIDTH, GNUPLOT_WIN_HEIGHT);
    fprintf(plotPipeRayleighPlessetSteps, "set title \"Steps\"\n");
    fprintf(plotPipeRayleighPlessetSteps, "set xlabel \"Time\"\n");
    fprintf(plotPipeRayleighPlessetSteps, "set ylabel \"Step\"\n");
    fprintf(plotPipeRayleighPlessetSteps, "plot '%s\\%s' using 1:3 with line title \"Euler method\", "
                                "'%s\\%s' using 1:3 with line title \"Runge-Kutta method\"\n",
                                workingDir, RAYLEIGH_PLESSET_EULER_OUTPUT.c_str(), workingDir, 
                                RAYLEIGH_PLESSET_RUNGE_KUTTA_OUTPUT.c_str());
    fflush(plotPipeRayleighPlessetSteps);

    system("pause");

#if defined(WIN32) || defined(_WIN32)
    _pclose(plotPipeRayleighPlessetEuler);
    _pclose(plotPipeRayleighPlessetRungeKutta);
    _pclose(plotPipeRayleighPlessetSteps);
#else
    pclose(plotPipeRayleighPlessetEuler);
    pclose(plotPipeRayleighPlessetRungeKutta);
    pclose(plotPipeRayleighPlessetSteps);
#endif

    fprintf(plotPipeKellerMiksisEuler, "set term wxt size %d, %d\n", GNUPLOT_WIN_WIDTH, GNUPLOT_WIN_HEIGHT);
    fprintf(plotPipeKellerMiksisEuler, "set title \"Keller-Miksis model (Euler method)\"\n");
    fprintf(plotPipeKellerMiksisEuler, "set xlabel \"Time\"\n");
    fprintf(plotPipeKellerMiksisEuler, "set ylabel \"Radius, m\"\n");
    fprintf(plotPipeKellerMiksisEuler, "plot '%s\\%s' using 1:2 with line notitle\n", 
            workingDir, KELLER_MIKSIS_EULER_OUTPUT.c_str());
    fflush(plotPipeKellerMiksisEuler);


    fprintf(plotPipeKellerMiksisRungeKutta, "set term wxt size %d, %d\n", GNUPLOT_WIN_WIDTH, GNUPLOT_WIN_HEIGHT);
    fprintf(plotPipeKellerMiksisRungeKutta, "set title \"Keller-Miksis model (Runge-Kutta method)\"\n");
    fprintf(plotPipeKellerMiksisRungeKutta, "set xlabel \"Time\"\n");
    fprintf(plotPipeKellerMiksisRungeKutta, "set ylabel \"Radius, m\"\n");
    fprintf(plotPipeKellerMiksisRungeKutta, "plot '%s\\%s' using 1:2 with line notitle\n", 
            workingDir, KELLER_MIKSIS_RUNGE_KUTTA_OUTPUT.c_str());
    fflush(plotPipeKellerMiksisRungeKutta);


    fprintf(plotPipeKellerMiksisEulerSteps, "set term wxt size %d, %d\n", GNUPLOT_WIN_WIDTH, GNUPLOT_WIN_HEIGHT);
    fprintf(plotPipeKellerMiksisEulerSteps, "set title \"Steps Keller-Miksis (Euler)\"\n");
    fprintf(plotPipeKellerMiksisEulerSteps, "set xlabel \"Time\"\n");
    fprintf(plotPipeKellerMiksisEulerSteps, "set ylabel \"Step\"\n");
    fprintf(plotPipeKellerMiksisEulerSteps, "plot '%s\\%s' using 1:3 with line notitle\n",
            workingDir, KELLER_MIKSIS_EULER_OUTPUT.c_str());
    fflush(plotPipeKellerMiksisEulerSteps);


    fprintf(plotPipeKellerMiksisRungeKuttaSteps, "set term wxt size %d, %d\n", GNUPLOT_WIN_WIDTH, GNUPLOT_WIN_HEIGHT);
    fprintf(plotPipeKellerMiksisRungeKuttaSteps, "set title \"Steps Keller-Miksis (Runge-Kutta)\"\n");
    fprintf(plotPipeKellerMiksisRungeKuttaSteps, "set xlabel \"Time\"\n");
    fprintf(plotPipeKellerMiksisRungeKuttaSteps, "set ylabel \"Step\"\n");
    fprintf(plotPipeKellerMiksisRungeKuttaSteps, "plot '%s\\%s' using 1:3 with line notitle\n",
            workingDir, KELLER_MIKSIS_RUNGE_KUTTA_OUTPUT.c_str());
    fflush(plotPipeKellerMiksisRungeKuttaSteps);


    system("pause");

#if defined(WIN32) || defined(_WIN32)
    _pclose(plotPipeKellerMiksisEuler);
    _pclose(plotPipeKellerMiksisRungeKutta);
    _pclose(plotPipeKellerMiksisEulerSteps);
    _pclose(plotPipeKellerMiksisRungeKuttaSteps);
#else
    pclose(plotPipeKellerMiksisEuler);
    pclose(plotPipeKellerMiksisRungeKutta);
    pclose(plotPipeKellerMiksisEulerSteps);
    pclose(plotPipeKellerMiksisRungeKuttaSteps);
#endif

    return 0;
}