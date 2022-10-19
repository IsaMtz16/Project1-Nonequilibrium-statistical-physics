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
#define N 1000

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

//DEFINIMOS EL VECTOR PARA LA POSICIÓN
typedef struct{
    double x, y;
} Vector;


//DEFINIMOS UN ARRAY PARA LA POSICIÓN DE TODAS LAS PARTÍCULAS
Vector particula[N];
Vector particula0[N];

int main (){
    ini_ran(time(NULL));
    double a,b; //numeros generados con box muller

    double x1, y1; //variables auxiliares para las posiciones

    //PASO DE TIEMPO
    double h=0.1;
    //TIEMPO FINAL
    double T=100*h;
    //TIEMPO EN CADA INSTANTE
    double t=0;

    //LONGITUD DE LA CAJA
    double L=100.0;

    //AMPLITUD DEL RUIDO
    double l=1;

    int i,j;

    //INICIALIZAMOS LA POSICION DE LAS PARTICULAS Y LA ESCRIBIMOS EN UN FICHERO
    FILE*f;
    f=fopen("Trayectoria_inicial.txt","w");
    for(i=0;i<N;i++){
       particula0[i].x=L*Random();
       particula0[i].y=L*Random();
       fprintf(f,"%lf  %lf\n",particula0[i].x,particula0[i].y);
    }
    fclose(f);

    /*FILE*f2;
    f2=fopen("Particula.txt","w");
    fprintf(f2,"%lf  %lf\n",particula0[0].x,particula0[0].y);*/


    //BUCLE DE TIEMPO
    for (j=1;t<=T;j++){
        t=h*j;
    //BUCLE PARA RECORRER TODAS LAS PARTÍCULAS
        for(i=0;i<N;i++){
            Box_Muller(&a,&b);
            particula[i].x=particula0[i].x+sqrt(2*l*h)*a;
            if(particula[i].x>L){
                particula[i].x=particula[i].x-L;
            }
            if(particula[i].x<0){
                particula[i].x=L+particula[i].x;
            }
            particula0[i].x=particula[i].x;

            particula[i].y=particula0[i].y+sqrt(2*l*h)*b;
            if(particula[i].y>L){
                particula[i].y=particula[i].y-L;
            }
            if(particula[i].y<0){
                particula[i].y=L+particula[i].y;
            }
            particula0[i].y=particula[i].y;
            //fprintf(f2,"%lf  %lf\n",particula0[i].x,particula0[i].y);

        }
    }
    //fclose(f2);
    //FICHERO PARA ESCRIBIR LA POSICION FINAL
    FILE*f1;
    f1=fopen("Trayectoria_final.txt","w");
    for(i=0;i<N;i++){
       fprintf(f1,"%lf  %lf\n",particula[i].x,particula[i].y);
    }
    fclose(f1);

    system("Plot_trayectoria_inicial.plt");
    system("Plot_trayectoria_final.plt");

    return 0;

}
