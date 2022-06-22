#include <omp.h>
#include <stdio.h>
#include <math.h>


#define SIZE       	2000000	// array size -- you get to decide
#define NUMTRIES    1000	// how many times to run the timing -- you get to decide

float A[SIZE];
float B[SIZE];
float C[SIZE];

double runProject(int NUMT){
        omp_set_num_threads( NUMT );
        fprintf( stderr, "Using %d threads\n", NUMT );

        double maxMegaMults = 0.;

        for( int t = 0; t < NUMTRIES; t++ )
        {
                double time0 = omp_get_wtime( );

                #pragma omp parallel for
                for( int i = 0; i < SIZE; i++ )
                {
                        C[i] = A[i] * B[i];
                }

                double time1 = omp_get_wtime( );
                double megaMults = (double)SIZE/(time1-time0)/1000000.;
                if( megaMults > maxMegaMults )
                        maxMegaMults = megaMults;
        }

        printf( "Peak Performance = %8.2lf MegaMults/Sec\n", maxMegaMults );
		
	// note: %lf stands for "long float", which is how printf prints a "double"
	//        %d stands for "decimal integer", not "double"

        return maxMegaMults;
	
}



int main( )
{
	#ifndef _OPENMP
        fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
        return 1;
	#endif

	// inialize the arrays:
	for( int i = 0; i < SIZE; i++ )
	{
		A[i] = 1.;
		B[i] = 2.;
	}
	
	double p4=runProject(4);
	double p1=runProject(1);
	double s;
	s=p4/p1;
	double Fp = (4./3.)*( 1. - (1./s));
	printf("\n p4=%lf \n p1=%lf \n Speed=%lf \n Fp=%lf \n", p4,p1,s,Fp);
}