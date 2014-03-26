#ifndef _SMOKE_H_
#define _SMOKE_H_

#include <vector>
#include <iostream>
#include <sstream> //You'll need to include this header to use 'std::stringstream'.
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>

#include "gl/gl.h"
#include "gl/glu.h"

#include "define.h"

using namespace std;


class Smoke
{
    public:
        // side size
        int N;
        int dim1;
        int dim2;
        int dim3;

        double dt, diffusion_rate, viscosity;
        double force, source;

/*
        // velocity
        vector<double> xv, xv_prev;
        vector<double> yv, yv_prev;
        vector<double> zv, zv_prev;

        // density
        vector<double> dens, dens_prev;
*/


        double* xv;
        double* xv_prev;
        double* yv;
        double* yv_prev;
        double* zv;
        double* zv_prev;
        double* dens;
        double* dens_prev;



        Smoke();
        ~Smoke();

        void InitVector(double * temp, double init=0);

        void update(bool addSmoke);
        void get_from_UI(bool addSmoke, int x, int y, int z);
        void vel_step();
        void dens_step();



        void show(int dvel);
        void draw_density();
        void draw_velocity();

        void addVelocityForce(glm::vec3 coord, glm::vec3 force);
        void addDensitySource(int x, int y, int z);
        void addDensitySource(glm::vec3 coord);
        void addDensitySource();
        void addDensitySource_FromUI(int x, int y, int z);


        void diffuse(double* list, double* list_prev,double diff_rate);
        void diffuse( vector<double> list, vector<double> list_prev,double diff_rate);
        void advect(double* list, double* list_prev, double* vx, double* vy, double* vz, double diff_rate);

        double interp(double * list, double x, double y, double z);


        void project();
        void lin_solve(double* list, double* list_prev, double diff_rate, double divisor);
        void lin_solve(vector<double> list, vector<double> list_prev, double diff_rate, double divisor);

        // returns the index
        int index(int x, int y, int z);


};









#endif
