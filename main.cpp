#define GNUPLOT             "gnuplot"
#define GNUPLOT_WIN_WIDTH   1280
#define GNUPLOT_WIN_HEIGHT  720

#include <iostream>
#include "RayleighPlesset.h"

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

#ifdef WIN32
    plotPipeEuler = _popen(GNUPLOT, "w");
    plotPipeRungeKutta = _popen(GNUPLOT, "w");
    plotPipeSteps = _popen(GNUPLOT, "w");
#else
    plotPipeEuler = popen(GNUPLOT, "w");
    plotPipeRungeKutta = popen(GNUPLOT, "w");
    plotPipeSteps = popen(GNUPLOT, "w");
#endif

    if (plotPipeEuler == nullptr || plotPipeRungeKutta == nullptr || plotPipeSteps == nullptr)
    {
        printf("Could not open gnuplot pipe\n");
        return -1;
    }

    calcRayleighPlessetEuler();
    calcRayleighPlessetRungeKutta();

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


    std::cin.clear();
    std::cin.get();

#ifdef WIN32
    _pclose(plotPipeEuler);
    _pclose(plotPipeRungeKutta);
    _pclose(plotPipeSteps);
#else
    pclose(plotPipeEuler);
    pclose(plotPipeRungeKutta);
    pclose(plotPipeSteps);
#endif

    return 0;
}