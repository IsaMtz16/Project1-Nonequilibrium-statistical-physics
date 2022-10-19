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
typedef struct
{
    double x, y;
} Vector;




//DEFINIMOS UN ARRAY PARA LA POSICIÓN DE TODAS LAS PARTÍCULAS
Vector particula[N];
Vector particula0[N];

//DEFINIMOS UN ARRAY PARA LAS POSICIONES INICIALES Y OTRO PARA LAS FINALES
Vector posicion_inicial[N];


int main (){
    ini_ran(time(NULL));
    double a,b; //numeros generados con box muller

    double x1, y1; //variables auxiliares para las posiciones

    //PASO DE TIEMPO
    double h=0.1;
    //TIEMPO FINAL
    double T=1000*h;
    //TIEMPO EN CADA INSTANTE
    double t=0;


    //AMPLITUD DEL RUIDO
    double l=0.1;

    int i,j,k,m;


    double delta=0,delta_x=0,delta_y=0;
    double D;

    //INICIALIZAMOS LA POSICION DE LAS PARTICULAS A 0
    for(i=0;i<N;i++){
       particula0[i].x=0;
       particula0[i].y=0;
       posicion_inicial[i].x=particula0[i].x;
       posicion_inicial[i].y=particula0[i].y;
    }

    //ABRIMOS UN FICHERO PARA ESCRIBIR D vs Gamma
    FILE*f;
    f=fopen("D_Gamma.txt","w");

    //BUCLE DE GAMMA
    for(k=0;k<8;k++){
        switch (k){
        case 0:
            l=0.1;
            break;
        case 1:
            l=1;
            break;
        case 2:
            l=3;
            break;
        case 3:
            l=10;
            break;
        case 4:
            l=30;
            break;
        case 5:
            l=100;
            break;
        case 6:
            l=300;
            break;
        case 7:
            l=1000;
            break;
        }
        t=0;
        //BUCLE DE TIEMPO
        for (j=1;t<=T;j++){
            t=h*j;
        //BUCLE PARA RECORRER TODAS LAS PARTÍCULAS
            for(i=0;i<N;i++){
                Box_Muller(&a,&b);
                particula[i].x=particula0[i].x+sqrt(2*l*h)*a;
                particula0[i].x=particula[i].x;

                particula[i].y=particula0[i].y+sqrt(2*l*h)*b;
                particula0[i].y=particula[i].y;
            }
        }


        //CALCULAMOS EL MEAN SQUARE DISPLACEMENTE
        for(m=0;m<N;m++){
        delta_x=(particula[m].x-posicion_inicial[m].x);
        delta_y=(particula[m].y-posicion_inicial[m].y);
        delta=delta+(pow(delta_x,2)+pow(delta_y,2));
        }
        delta=delta/N;
        D=delta/(4*t);
        //fprintf(f,"%lf   %lf\n",log(l),log(D));
        fprintf(f,"%lf   %lf\n",l,D);
    }
fclose(f);
system("Plot_D_Gamma.plt");
 return 0;
}
