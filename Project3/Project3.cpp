#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int  NowYear;           // 2022 - 2028
int  NowMonth;          // 0 - 11

float NowPrecip;        // inches of rain per month
float NowTemp;          // temperature this month
float NowHeight;        // grain height in inches
int   NowNumDeer;       // current deer population
int   NowNumFox;     // current wolf population

//unsigned int seed;

const float GRAIN_GROWS_PER_MONTH =             10.0;
const int   DEER_GROWS_PER_MONTH =              15;
const int   FOX_GROWS_PER_MONTH =               5;

const float ONE_DEER_EATS_PER_MONTH =           0.12;
const float ONE_FOX_EATS_PER_MONTH =            0.25;
const float AVG_PRECIP_PER_MONTH =              8.0;
const float AMP_PRECIP_PER_MONTH =              8.0;
const float RANDOM_PRECIP =                     3.0;
const float AVG_TEMP =                          40.0;
const float AMP_TEMP =                          30.0;
const float RANDOM_TEMP =                       20.0;
const float MIDTEMP =                           30.0;
const float MIDPRECIP =                         20.0;

float Ranf( float low, float high, unsigned int* seed )
{
    float r = (float) rand_r(seed);      // 0 - RAND_MAX
    return( low + r * ( high - low ) / (float)RAND_MAX );
}



void Deer()
{
	while (NowYear < 2028) {
    int tempDeer = NowNumDeer;
    float gf = 1 - exp(-pow((NowHeight - 0.11) / 10., 2));
    tempDeer += gf * DEER_GROWS_PER_MONTH;
    if (tempDeer > ONE_FOX_EATS_PER_MONTH * NowNumFox && tempDeer > NowHeight / ONE_DEER_EATS_PER_MONTH)
        tempDeer -= ONE_FOX_EATS_PER_MONTH * NowNumFox;
    else
        tempDeer = tempDeer * 1.0;
#pragma omp barrier
    NowNumDeer = tempDeer;
#pragma omp barrier
#pragma omp barrier
	}
}

void GrainGrowth()
{
	while (NowYear <= 2027) {
    float tf = exp(-pow((NowTemp - MIDTEMP) / 10., 2));
    float pf = exp(-pow((NowPrecip - MIDPRECIP) / 10., 2));

    float tempHeight  = NowHeight;
    tempHeight += tf * pf * GRAIN_GROWS_PER_MONTH;
    tempHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH ;

    if(tempHeight <= 0.12)
        tempHeight = 0.12;
#pragma omp barrier
    NowHeight = tempHeight;
#pragma omp barrier
#pragma omp barrier
	}
}

float FarToCesls(float f){
    return (5./9.)*(f-32);
}

float ToCm(float h){
    return h*2.54;
}

void Watcher()
{
	while (NowYear < 2028) {
    #pragma omp barrier
    
    #pragma omp barrier
    printf("%d, %.2f, %.2f, %.2f, %d, %d\n", NowMonth+1, FarToCesls(NowTemp), NowPrecip, ToCm(NowHeight), NowNumDeer, NowNumFox);
    
    NowMonth++;
    NowYear = 2022 + NowMonth/12;
    
    unsigned int seed = omp_get_thread_num() * 42;
    float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );
    float temp = AVG_TEMP - AMP_TEMP * cos( ang );
    NowTemp = temp + Ranf( -RANDOM_TEMP, RANDOM_TEMP, &seed );
    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
    NowPrecip = precip + Ranf( -RANDOM_PRECIP, RANDOM_PRECIP, &seed );
    
    if( NowPrecip < 0. )
        NowPrecip = 0.;
    #pragma omp barrier
	}
}

void FOX()
{
	while (NowYear < 2028) {

    int tempFox = NowNumFox;
    float df = 1 - exp(-pow((NowNumDeer - 45) / 10., 2));
    if (df > 0.5 && NowNumDeer > 10)
        tempFox += FOX_GROWS_PER_MONTH;
    if (tempFox > NowNumDeer / ONE_FOX_EATS_PER_MONTH / 5.)
        tempFox = tempFox * 0.5;


    
#pragma omp barrier
    NowNumFox = tempFox;
#pragma omp barrier
#pragma omp barrier
	}
}

int main( )
{
    //  setup global variables
    NowNumDeer = 50;
    NowNumFox = 2;
    NowHeight =  1.;
    NowMonth =    0;
    NowYear  = 2022;
    
    unsigned int seed = time(NULL);
    
    float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );
    float temp = AVG_TEMP - AMP_TEMP * cos( ang );
    NowTemp = temp + Ranf( -RANDOM_TEMP, RANDOM_TEMP, &seed );
    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
    NowPrecip = precip + Ranf( -RANDOM_PRECIP, RANDOM_PRECIP, &seed );
    
#ifndef _OPENMP
    fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
    return 1;
#endif
    
    omp_set_num_threads(4);
    
    
#pragma omp parallel sections
{
#pragma omp section
            {
                Deer();
            }
#pragma omp section
            {
                FOX();
            }

#pragma omp section
            {
                GrainGrowth();
            }
#pragma omp section
            {
                Watcher();
            }
}

    return 0;
}