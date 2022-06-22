#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.
// setting the number of threads:
#ifndef NUMT
#define NUMT		    2
#endif

// setting the number of trials in the monte carlo simulation:
#ifndef NUMTRIALS
#define NUMTRIALS	500
#endif

#ifndef NUMNODES
#define NUMNODES	5000
#endif

// The code to evaluate the height at a given <i>iu</i> and <i>iv</i> is:

const float N = 2.5f;
const float R = 1.2f;

float Height( int iu, int iv )	// iu,iv = 0 .. NUMNODES-1
{
	float x = -1.  +  2.*(float)iu /(float)(NUMNODES-1);	// -1. to +1.
	float y = -1.  +  2.*(float)iv /(float)(NUMNODES-1);	// -1. to +1.

	float xn = pow( fabs(x), (double)N );
	float yn = pow( fabs(y), (double)N );
	float rn = pow( fabs(R), (double)N );
	float r = rn - xn - yn;
	if( r <= 0. )
	        return 0.;
	float height = pow( r, 1./(double)N );
	return height;
}

int main( int argc, char *argv[ ] )
{
	#ifndef _OPENMP
	fprintf(stderr, "OPENMP is not supported here--sorry.\n");
	return 1;
	#endif
	// the area of a single full-sized tile:
	// (not all tiles are full-sized, though)
	
	double fullTileArea = (  ( ( XMAX - XMIN )/(float)(NUMNODES-1) )  * ( ( YMAX - YMIN )/(float)(NUMNODES-1) )  );

	double volume=0.0;
	double maxvolume=0.0;
	double maxPerformance=0.0;
	omp_set_num_threads( NUMT );

	for(int i =0;i<NUMTRIALS;i++){
		// sum up the weighted heights into the variable "volume"
		// using an OpenMP for-loop and a reduction:
		maxPerformance=0.0;
		double time0 = omp_get_wtime();
		#pragma omp parallel for collapse(2) default(none) shared(fullTileArea) reduction(+:volume)
		for( int iv = 0; iv < NUMNODES; iv++ )
		{
			for( int iu = 0; iu < NUMNODES; iu++ )
			{
				float z = Height( iu, iv );
			
				if((iu==0 && iv==0)||(iu==NUMNODES-1 && iv==NUMNODES-1)||(iu==0 && iv==NUMNODES-1)||(iu==NUMNODES-1 && iv==0)){
					volume+=(1/4)*(fullTileArea)*z*2;
				}
				else if(iu==0 || iv==NUMNODES-1 || iv==0 || iu==NUMNODES-1){
					volume+=(1/2)*(fullTileArea)*z*2;
				}
				else{
					volume+=fullTileArea*z*2;
				}
			}
		}
		double time1 = omp_get_wtime();
		double MegaTrialsPerSecond = (double)NUMNODES*NUMNODES/ (time1-time0)/1000000;
		if (MegaTrialsPerSecond > maxPerformance)
			{
				maxPerformance = MegaTrialsPerSecond;
			}
		if (volume > maxvolume)
			{
				maxvolume = volume;
			}
		volume=0;
	}
	printf("%f,%d,%d,%lf\n", maxvolume, NUMT, NUMNODES, maxPerformance);
}