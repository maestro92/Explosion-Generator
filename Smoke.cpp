
#include "Smoke.h"
#define SWAP(x0,x) {double * tmp=x0;x0=x;x=tmp;}


Smoke::Smoke()
{
    N = 8;
    dim1 = (N+2);
    dim2 = (N+2)*(N+2);
    dim3 = (N+2)*(N+2)*(N+2);
    dt = 0.1;
    source = 100.0f;

    xv = new double [dim3];
    yv = new double [dim3];
    zv = new double [dim3];
    xv_prev = new double [dim3];
    yv_prev = new double [dim3];
    zv_prev = new double [dim3];
    dens = new double [dim3];
    dens_prev = new double [dim3];


    InitVector(xv);
    InitVector(yv,1);
    InitVector(zv);
    InitVector(xv_prev);
    InitVector(yv_prev,1);
    InitVector(zv_prev);
    InitVector(dens);
    InitVector(dens_prev);



/*
    xv.resize(dim3);
    yv.resize(dim3);
    zv.resize(dim3);
    xv_prev.resize(dim3);
    yv_prev.resize(dim3);
    zv_prev.resize(dim3);

    dens.resize(dim3);
    dens_prev.resize(dim3);
*/
}

Smoke::~Smoke()
{

    delete[] xv;
    delete[] yv;
    delete[] zv;
    delete[] xv_prev;
    delete[] yv_prev;
    delete[] zv_prev;
    delete[] dens;
    delete[] dens_prev;

}




void Smoke::InitVector(double * temp, double init)
{
    for(int i = 0; i<dim3; i++)
    {
        temp[i] = init;
    }


    cout << endl;
}

/*
/// update functions
void Smoke::update(bool addSmoke)
{
    get_from_UI(addSmoke, 5, 5, 5);
    vel_step();
    dens_step();
}
*/

void Smoke::get_from_UI(bool addSmoke, int x, int y, int z)
{
  //  addDensitySource(5,5,5);
    if(addSmoke)
    {
        cout << "source is " << source << endl;
        cout << "dens_prev[index(x, y, z)] is " << dens_prev[index(x, y, z)] << endl;
        dens_prev[index(x, y, z)] = source;
        cout << "dens_prev[index(x, y, z)] is " << dens_prev[index(x, y, z)] << endl << endl;
    }
}




void Smoke::vel_step()
{

	SWAP ( xv_prev, xv ); diffuse ( xv, xv_prev, viscosity);
	SWAP ( yv_prev, yv ); diffuse ( yv, yv_prev, viscosity);
	SWAP ( zv_prev, zv ); diffuse ( zv, zv_prev, viscosity);
}

void Smoke::dens_step()
{
    addDensitySource();

    // change with dens_temp/prev
    // dens.swap(dens_prev);
    SWAP(dens, dens_prev);
	diffuse ( dens, dens_prev, diffusion_rate);

    // advection
    // dens.swap(dens_prev);
    SWAP(dens, dens_prev);
    advect ( dens, dens_prev, xv, yv, zv,diffusion_rate);
}



int Smoke::index(int x, int y, int z)
{
    return y*dim2 + z*dim1 + x;
}

// coord: coordinate
void Smoke::addVelocityForce(glm::vec3 coord, glm::vec3 force)
{
    xv[index(coord.x,coord.y,coord.z)] = force.x;
    yv[index(coord.x,coord.y,coord.z)] = force.y;
    zv[index(coord.x,coord.y,coord.z)] = force.z;
}

void Smoke::addDensitySource_FromUI(int x, int y, int z)
{
    dens_prev[index(x, y, z)] = source;
}


void Smoke::addDensitySource()
{

    for(int i=0; i<dim3; i++)
    {
        dens[i] += dt*dens_prev[i];
    }


/*
    for(int i=0; i<dens_prev.size(); i++)
    {
        dens[i] += dt*dens_prev[i]
    }

    int i, size=(N+2)*(N+2);
	for ( i=0 ; i<size ; i++ )
        x[i] += dt*s[i];
*/
}


void Smoke::addDensitySource(glm::vec3 coord)
{
    dens_prev[index(coord.x,coord.y,coord.z)] = source;

/*
    for(int i=0; i<dens_prev.size(); i++)
    {
        dens[i] += dt*dens_prev[i]
    }

    int i, size=(N+2)*(N+2);
	for ( i=0 ; i<size ; i++ )
        x[i] += dt*s[i];
*/
}


//( int N, int b, double * x, double * x0, double diff, double dt )


void Smoke::diffuse( double* list, double* list_prev,double diff_rate)
{
	double a=dt * diff_rate * N * N * N;
	lin_solve (list, list_prev, a, 1+6*a );

	// lin_solve ( N, b, x, x0, a, 1+4*a );
}



void Smoke::diffuse( vector<double> list, vector<double> list_prev,double diff_rate)
{
	double a=dt * diff_rate * N * N * N;
	lin_solve (list, list_prev, a, 1+6*a );

	// lin_solve ( N, b, x, x0, a, 1+4*a );
}

void Smoke::advect(double* list, double* list_prev, double* vx, double* vy, double* vz, double diff_rate)
{
    int i, j, k;
    int i0, j0, k0;
    int i1, j1, k1;

    double s0,s1;
    double t0,t1;
    double u0,u1;

    double rate = dt*N;

    for(int y = 1; y <= N; y++)
    {
        for(int z = 1; z <= N; z++)
        {
            for(int x = 1; x <= N; x++)
            {
                int i = index(x,y,z);

                double x0 = x - rate * vx[i];
                double y0 = y - rate * vy[i];
                double z0 = z - rate * vz[i];


                if(x0 < 0.5) x0 = 0.5;
                if(x0 > N + 0.5) x0 = N + 0.5;
                i0 = (int)x0;
                i1 = i0 + 1;

                if(y0 < 0.5) y0 = 0.5;
                if(y0 > N + 0.5) y0 = N + 0.5;
                j0 = (int)y0;
                j1 = j0 + 1;

                if(z0 < 0.5) z0 = 0.5;
                if(z0 > N + 0.5) z0 = N + 0.5;
                k0 = (int)z0;
                k1 = k0 + 1;

                s1 = x0 - i0;
                s0 = 1.0 - s1;
                t1 = y0 - j0;
                t0 = 1.0 - t1;
                u1 = z0 - k0;
                u0 = 1.0 - u1;


                list[i] =

                        s0 * ( t0 * (u0 * list_prev[index(i0, j0, k0)]
                                    +u1 * list_prev[index(i0, j0, k1)])
                            +( t1 * (u0 * list_prev[index(i0, j1, k0)]
                                    +u1 * list_prev[index(i0, j1, k1)])))
                        +s1 * ( t0 * (u0 * list_prev[index(i1, j0, k0)]
                                    +u1 * list_prev[index(i1, j0, k1)])
                            +( t1 * (u0 * list_prev[index(i1, j1, k0)]
                                    +u1 * list_prev[index(i1, j1, k1)])));
            //    list[i] = interp(list_prev, x0, y0, z0);
                // we're dealing with 8 cubes instead of 4 like in 2D
/*
                list[index(x,y,z)] = (list_prev[index(x,y,z)] +
                                    diff_rate * (list[index(x-1,y,z)] + list[index(x+1,y,z)] +
                                                 list[index(x,y-1,z)] + list[index(x,y+1,z)] +
                                                 list[index(x,y,z-1)] + list[index(x,y,z+1)])) * div;
  */
            }
        }
    }


}

double interp(double * list, double x, double y, double z)
{
    // get the integer components, and normalized 0..1 interp factors, of x,y,z:
    /*
    int x0 = (int)x; double xbf = x-(double)x0; double xaf = 1-xbf;
    int y0 = (int)y; double ybf = y-(double)y0; double yaf = 1-ybf;
    int z0 = (int)z; double zbf = z-(double)z0; double zaf = 1-zbf;
    */

    //

}



void Smoke::project()
{


}



void Smoke::lin_solve(double* list, double* list_prev, double diff_rate, double divisor)
{
    double div = 1.0 / divisor;

    for(int iter = 0; iter<20; iter++)
    {
        for(int y = 1; y <= N; y++)
        {

            for(int z = 1; z <= N; z++)
            {
                for(int x = 1; x <= N; x++)
                {

                    list[index(x,y,z)] = (list_prev[index(x,y,z)] +
                                        diff_rate * (list[index(x-1,y,z)] + list[index(x+1,y,z)] +
                                                     list[index(x,y-1,z)] + list[index(x,y+1,z)] +
                                                     list[index(x,y,z-1)] + list[index(x,y,z+1)])) * div;
                }
            }
        }
    }

}


void Smoke::lin_solve(vector<double> list, vector<double> list_prev, double diff_rate, double divisor)
{
    double div = 1.0 / divisor;

    for(int iter = 0; iter<20; iter++)
    {
        for(int y = 1; y <= N; y++)
        {

            for(int z = 1; z <= N; z++)
            {
                for(int x = 1; x <= N; x++)
                {

                    list[index(x,y,z)] = (list_prev[index(x,y,z)] +
                                        diff_rate * (list[index(x-1,y,z)] + list[index(x+1,y,z)] +
                                                     list[index(x,y-1,z)] + list[index(x,y+1,z)] +
                                                     list[index(x,y,z-1)] + list[index(x,y,z+1)])) * div;
                }
            }
        }
    }

}



/*
/// display function
void Smoke::show(int dvel)
{
    if ( dvel )
        draw_velocity ();
    else
        draw_density ();
}
*/

void Smoke::draw_velocity()
{

    double i, j, k;
	double h;

	h = 10*1.0f/N;


    glPushMatrix();
        glDisable( GL_LIGHTING );
    	glColor3f ( 1.0f, 1.0f, 0.0f );

        glLineWidth ( 1.0f );

        glBegin ( GL_LINES );


            for(int y = 1; y <= N; y++)
            {
                j = (y-0.5f)*h;
                for(int z = 1; z <= N; z++)
                {
                    k = (z-0.5f)*h;
                    for(int x = 1; x <= N; x++)
                    {
                        i = (x-0.5f)*h;
                        glVertex3f ( i, j, k );
                        glVertex3f ( i+xv[index(x,y,z)], j+yv[index(x,y,z)], k+zv[index(x,y,z)]);
                    }
                }
            }

        glEnd ();
        glEnable( GL_LIGHTING );
    glPopMatrix();
}


void Smoke::draw_density()
{

    double i, j, k, d000, d001, d010, d011, d100, d101, d110, d111;
    double h;

	h = 10*1.0f/N;

    glPushMatrix();
    glDisable(GL_LIGHTING);
//	glBegin ( GL_QUADS );
	glBegin ( GL_QUADS );


        for(int y = 1; y <= N; y++)
        {
            j = (y-0.5f)*h;
            for(int z = 1; z <= N; z++)
            {
                k = (z-0.5f)*h;
                for(int x = 1; x <= N; x++)
                {
                    i = (x-0.5f)*h;

                    d000 = dens[index(x,y,z)];
                    d001 = dens[index(x,y,z+1)];
                    d010 = dens[index(x,y+1,z)];
                    d011 = dens[index(x,y+1,z+1)];
                    d100 = dens[index(x+1,y,z)];
                    d101 = dens[index(x+1,y,z+1)];
                    d110 = dens[index(x+1,y+1,z)];
                    d111 = dens[index(x+1,y+1,z+1)];


            //        glVertex3f ( i, j, k );
             //       glVertex3f ( i+xv[index(x,y,z)], j+yv[index(x,y,z)], k+zv[index(x,y,z)]);


           //         printf ("0 is %g \n", d000 );
           //         printf ("1 is %g \n", d001 );

               /*     printf ("2 is %g \n", d010 );
                    printf ("3 is %g \n", d011 );
                    printf ("4 is %g \n", d100 );
                    printf ("5 is %g \n", d101 );
                    printf ("6 is %g \n", d110 );
                    printf ("7 is %g \n", d111 );
*/
              //      glDisable(GL_LIGHTING);


     //               glColor3f ( 1.0f, 0.0f, 0.0f ); glVertex3f ( i, j, k );
     //               glColor3f ( 1.0f, 0.0f, 0.0f ); glVertex3f ( i, j, k+h );



                    glColor3f ( d000, d000, d000 ); glVertex3f ( i, j, k );
                    glColor3f ( d001, d001, d001 ); glVertex3f ( i, j, k+h );
                    glColor3f ( d010, d010, d010 ); glVertex3f ( i, j+h, k );
                    glColor3f ( d011, d011, d011 ); glVertex3f ( i, j+h, k+h );

                    glColor3f ( d100, d100, d100 ); glVertex3f ( i+h, j, k );
                    glColor3f ( d101, d101, d101 ); glVertex3f ( i+h, j, k+h );
                    glColor3f ( d110, d110, d110 ); glVertex3f ( i+h, j+h, k );
                    glColor3f ( d111, d111, d111 ); glVertex3f ( i+h, j+h, k+h );


                }
            }
        }
/*
				d00 = dens[IX(i,j)];
				d01 = dens[IX(i,j+1)];
				d10 = dens[IX(i+1,j)];
				d11 = dens[IX(i+1,j+1)];

				glColor3f ( d00, d00, d00 ); glVertex2f ( x, y );
				glColor3f ( d10, d10, d10 ); glVertex2f ( x+h, y );
				glColor3f ( d11, d11, d11 ); glVertex2f ( x+h, y+h );
				glColor3f ( d01, d01, d01 ); glVertex2f ( x, y+h );
*/

	glEnd ();
    glEnable(GL_LIGHTING);

    glPopMatrix();
}



void Smoke::update(bool toggle)
{
    get_from_UI(toggle, 5, 5, 5);
    vel_step();
    dens_step();
}


void Smoke::show(bool toggle)
{
    if ( toggle )
        draw_velocity ();
    else
        draw_density ();
}


void Smoke::Reset()
{

}
