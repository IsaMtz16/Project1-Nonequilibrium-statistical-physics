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


int main (){

    ini_ran(time(NULL));

    double a,b;



    //PASO DE TIEMPO
    double h=0.1;
    //TIEMPO FINAL
    double T=100*h;
    //TIEMPO EN CADA INSTANTE
    double t=0;
    int k;

    //AMPLITUD DEL RUIDO
    double l=1;

    //POSISCIONES ANTERIORES Y POSTERIORES
    double x0,y0,x,y;
    x0=0; y0=0;

    //ABRIMOS EL FICHERO
    FILE*f1;
    f1=fopen("Trayectoria.txt","w");
    fprintf(f1,"%lf  %lf   %lf\n",t,x0,y0);

    for(k=1;t<=T;k++){
        t=h*k;
        Box_Muller(&a,&b);
        x=x0+sqrt(2*l*h)*a;
        y=y0+sqrt(2*l*h)*b;
        x0=x;
        y0=y;
        fprintf(f1,"%lf  %lf   %lf\n",t,x,y);
    }


    fclose(f1);
    system("Plot_trayectoria.plt");
    return 0;
}
