#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// ALGORITMO P-R
extern float Random(void);
#define NormRANu (2.3283063671E-10)
extern void ini_ran(int SEMILLA);
unsigned int irr[256];
unsigned int ir1;
unsigned char ind_ran, ig1, ig2, ig3;

#define PI 3.14159265

// GENERACION DE NUMEROS
float Random(void)	// generar numero aleatorio
{
    float r;
    ig1=ind_ran-24;
    ig2=ind_ran-55;
    ig3=ind_ran-61;
    irr[ind_ran]=irr[ig1]+irr[ig2];
    ir1=(irr[ind_ran]^irr[ig3]);
    ind_ran++;
    r=ir1*NormRANu;
    return r;
}

void ini_ran(int SEMILLA)
{
    int INI,FACTOR,SUM,i;
    srand(SEMILLA);
    INI=SEMILLA;
    FACTOR=67397;
    SUM=7364893;
    for(i=0; i<256; i++)
    {
        INI=(INI*FACTOR+SUM);
        irr[i]=INI;
    }
    ind_ran=ig1=ig2=ig3=0;
}


void Box_Muller (double *g1, double *g2)
{
    double d1, d2;
    d1=sqrt(-2.0*log(Random()));
    d2=2.0*PI*Random();
    *g1=-d1*cos(d2);
    *g2=-d1*sin(d2);
}

//DECLARAMOS UN VARIABLE PARA CALCULAR LA MEDIA
double media=0;

//PARA CALCULAR LA VARIANZA
double media_2=0;
double varianza=0;

#define N 10000

int main (){

    ini_ran(time(NULL));

    double a,b;

    //ABRIMOS EL FICHERO
    FILE *f;
    f=fopen("Random_gaussiana.txt", "w");
    int i;
    int j;    j=N/2;
    for(i=0;i<(N/2);i++){
        Box_Muller(&a,&b);
        media=media+a+b;
        media_2=media_2+a*a+b*b;
        fprintf(f,"%lf\n%lf\n",a,b);
    }
    fclose(f);

    //CALCULO DE LA MEDIA
    media=media/(double)N;
    //CALCULO DE LA VARIANZA
    media_2=media_2/(double)N;
    varianza=media_2-(media*media);

    printf("Media: %lf\n  Varianza: %lf\n",media,varianza);

    system("Plot_histograma.plt");
    system("Plot_valores.plt");


    return 0;
}
