#define GNUPLOT             "gnuplot"
#define GNUPLOT_WIN_WIDTH   1280
#define GNUPLOT_WIN_HEIGHT  720

#include <iostream>
#include "RayleighPlesset.h"
#include "KellerMiksis.h"

#include <stdio.h>
#ifdef WIN32
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

    FILE* plotPipeEuler;
    FILE* plotPipeRungeKutta;
    FILE* plotPipeSteps;

    FILE* plotPipeKellerMiksisEuler;
    FILE* plotPipeKellerMiksisRungeKutta;
    FILE* plotPipeKellerMiksisEulerSteps;
    FILE* plotPipeKellerMiksisRungeKuttaSteps;

#ifdef WIN32
    plotPipeEuler = _popen(GNUPLOT, "w");
    plotPipeRungeKutta = _popen(GNUPLOT, "w");
    plotPipeSteps = _popen(GNUPLOT, "w");
    plotPipeKellerMiksisEuler = _popen(GNUPLOT, "w");
    plotPipeKellerMiksisRungeKutta = _popen(GNUPLOT, "w");
    plotPipeKellerMiksisEulerSteps = _popen(GNUPLOT, "w");
    plotPipeKellerMiksisRungeKuttaSteps = _popen(GNUPLOT, "w");
#else
    plotPipeEuler = popen(GNUPLOT, "w");
    plotPipeRungeKutta = popen(GNUPLOT, "w");
    plotPipeSteps = popen(GNUPLOT, "w");
    plotPipeKellerMiksisEuler = popen(GNUPLOT, "w");
    plotPipeKellerMiksisRungeKutta = popen(GNUPLOT, "w");
    plotPipeKellerMiksisEulerSteps = popen(GNUPLOT, "w");
    plotPipeKellerMiksisRungeKuttaSteps = popen(GNUPLOT, "w");
#endif

    if (plotPipeEuler == nullptr || plotPipeRungeKutta == nullptr || plotPipeSteps == nullptr)
    {
        printf("Could not open gnuplot pipe\n");
        return -1;
    }

    calcRayleighPlessetEuler();
    calcRayleighPlessetRungeKutta();
    calcKellerMiksisEuler();
    calcKellerMiksisRungeKutta();

    fprintf(plotPipeEuler, "set term wxt size %d, %d\n", GNUPLOT_WIN_WIDTH, GNUPLOT_WIN_HEIGHT);
    fprintf(plotPipeEuler, "set title \"Rayleigh-Plesset model (Euler method)\"\n");
    fprintf(plotPipeEuler, "set xlabel \"Time\"\n");
    fprintf(plotPipeEuler, "set ylabel \"Radius, m\"\n");
    fprintf(plotPipeEuler, "plot '%s\\%s' using 1:2 with line notitle\n", workingDir, RAYLEIGH_PLESSET_EULER_OUTPUT);
    fflush(plotPipeEuler);


    fprintf(plotPipeRungeKutta, "set term wxt size %d, %d\n", GNUPLOT_WIN_WIDTH, GNUPLOT_WIN_HEIGHT);
    fprintf(plotPipeRungeKutta, "set title \"Rayleigh-Plesset model (Runge-Kutta method)\"\n");
    fprintf(plotPipeRungeKutta, "set xlabel \"Time\"\n");
    fprintf(plotPipeRungeKutta, "set ylabel \"Radius, m\"\n");
    fprintf(plotPipeRungeKutta, "plot '%s\\%s' using 1:2 with line notitle\n", workingDir, RAYLEIGH_PLESSET_RUNGE_KUTTA_OUTPUT);
    fflush(plotPipeRungeKutta);


    fprintf(plotPipeSteps, "set term wxt size %d, %d\n", GNUPLOT_WIN_WIDTH, GNUPLOT_WIN_HEIGHT);
    fprintf(plotPipeSteps, "set title \"Steps\"\n");
    fprintf(plotPipeSteps, "set xlabel \"Time\"\n");
    fprintf(plotPipeSteps, "set ylabel \"Step\"\n");
    fprintf(plotPipeSteps, "plot '%s\\%s' using 1:3 with line title \"Euler method\", "
                                "'%s\\%s' using 1:3 with line title \"Runge-Kutta method\"\n",
                                workingDir, RAYLEIGH_PLESSET_EULER_OUTPUT, workingDir, RAYLEIGH_PLESSET_RUNGE_KUTTA_OUTPUT);
    fflush(plotPipeSteps);

    fprintf(plotPipeKellerMiksisEuler, "set term wxt size %d, %d\n", GNUPLOT_WIN_WIDTH, GNUPLOT_WIN_HEIGHT);
    fprintf(plotPipeKellerMiksisEuler, "set title \"Keller-Miksis model (Euler method)\"\n");
    fprintf(plotPipeKellerMiksisEuler, "set xlabel \"Time\"\n");
    fprintf(plotPipeKellerMiksisEuler, "set ylabel \"Radius, m\"\n");
    fprintf(plotPipeKellerMiksisEuler, "plot '%s\\%s' using 1:2 with line notitle\n", workingDir, KELLER_MIKSIS_EULER_OUTPUT);
    fflush(plotPipeKellerMiksisEuler);


    fprintf(plotPipeKellerMiksisRungeKutta, "set term wxt size %d, %d\n", GNUPLOT_WIN_WIDTH, GNUPLOT_WIN_HEIGHT);
    fprintf(plotPipeKellerMiksisRungeKutta, "set title \"Keller-Miksis model (Runge-Kutta method)\"\n");
    fprintf(plotPipeKellerMiksisRungeKutta, "set xlabel \"Time\"\n");
    fprintf(plotPipeKellerMiksisRungeKutta, "set ylabel \"Radius, m\"\n");
    fprintf(plotPipeKellerMiksisRungeKutta, "plot '%s\\%s' using 1:2 with line notitle\n", workingDir, KELLER_MIKSIS_RUNGE_KUTTA_OUTPUT);
    fflush(plotPipeKellerMiksisRungeKutta);


    fprintf(plotPipeKellerMiksisEulerSteps, "set term wxt size %d, %d\n", GNUPLOT_WIN_WIDTH, GNUPLOT_WIN_HEIGHT);
    fprintf(plotPipeKellerMiksisEulerSteps, "set title \"Steps Keller-Miksis (Euler)\"\n");
    fprintf(plotPipeKellerMiksisEulerSteps, "set xlabel \"Time\"\n");
    fprintf(plotPipeKellerMiksisEulerSteps, "set ylabel \"Step\"\n");
    fprintf(plotPipeKellerMiksisEulerSteps, "plot '%s\\%s' using 1:3 with line notitle\n",
            workingDir, KELLER_MIKSIS_EULER_OUTPUT);
    fflush(plotPipeKellerMiksisEulerSteps);


    fprintf(plotPipeKellerMiksisRungeKuttaSteps, "set term wxt size %d, %d\n", GNUPLOT_WIN_WIDTH, GNUPLOT_WIN_HEIGHT);
    fprintf(plotPipeKellerMiksisRungeKuttaSteps, "set title \"Steps Keller-Miksis (Runge-Kutta)\"\n");
    fprintf(plotPipeKellerMiksisRungeKuttaSteps, "set xlabel \"Time\"\n");
    fprintf(plotPipeKellerMiksisRungeKuttaSteps, "set ylabel \"Step\"\n");
    fprintf(plotPipeKellerMiksisRungeKuttaSteps, "plot '%s\\%s' using 1:3 with line notitle\n",
            workingDir, KELLER_MIKSIS_RUNGE_KUTTA_OUTPUT);
    fflush(plotPipeKellerMiksisRungeKuttaSteps);


    std::cin.clear();
    std::cin.get();

#ifdef WIN32
    _pclose(plotPipeEuler);
    _pclose(plotPipeRungeKutta);
    _pclose(plotPipeSteps);
    _pclose(plotPipeKellerMiksisEuler);
    _pclose(plotPipeKellerMiksisRungeKutta);
    _pclose(plotPipeKellerMiksisEulerSteps);
    _pclose(plotPipeKellerMiksisRungeKuttaSteps);
#else
    pclose(plotPipeEuler);
    pclose(plotPipeRungeKutta);
    pclose(plotPipeSteps);
    pclose(plotPipeKellerMiksisEuler);
    pclose(plotPipeKellerMiksisRungeKutta);
    pclose(plotPipeKellerMiksisEulerSteps);
    pclose(plotPipeKellerMiksisRungeKuttaSteps);
#endif

    return 0;
}