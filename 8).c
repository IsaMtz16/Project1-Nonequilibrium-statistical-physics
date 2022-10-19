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
    double x;
} Vector;




//DEFINIMOS UN ARRAY PARA LA POSICIÓN DE TODAS LAS PARTÍCULAS
Vector particula[N];
Vector particula0[N];
Vector velocidad[N];
Vector velocidad0[N];

//DEFINIMOS UN ARRAY PARA LAS POSICIONES INICIALES Y OTRO PARA LAS FINALES
Vector posicion_inicial[N];


int main (){
    ini_ran(time(NULL));
    double a,b; //numeros generados con box muller

    double x1; //variables auxiliares para las posiciones

    //PASO DE TIEMPO
    double h=0.1;
    //TIEMPO FINAL
    double T=1000*h;
    //TIEMPO EN CADA INSTANTE
    double t=0;

    //AMPLITUD DEL RUIDO
    double l=1;

    int i,j,k,m;

    //MEDIAS

    double media_x=0;
    double media_x0=0;

    double chi=0;

    double L=100;

    //FUERZA
    double F=1;
    double e=1; //indica el signo de la fuerza
    double c=0; //numero random con el que elegimos el sentido de la fuerza

    //DESPLAZAMIENTO CUADRADO
    double delta=0;



    //BUCLE DE TIEMPOS PARA LOS QUE GUARDAMOS LOS DESPLAZAMIENTOS
    for(k=0;k<4;k++){
        switch (k){
        case 0:
            F=1;
            break;
        case 1:
            F=0.1;
            break;
        case 2:
            F=0.01;
            break;
        case 3:
            F=0.001;
            break;
        case 4:
            F=10;
            break;
        case 5:
            F=50;
            break;
        }
        //ABRIMOS UN FICHERO PARA ESCRIBIR LOS DESPLAZAMIENTOS
        FILE*f;
        char filename[40];
        sprintf(filename, "desplazamiento_f_%.3lf.txt", F);
        f=fopen(filename, "w");

        media_x0=0;

        //INICIALIZAMOS LA POSICION DE LAS PARTICULAS
        for(i=0;i<N;i++){
            velocidad0[i].x=0;
           particula0[i].x=L*Random();
           //particula0[i].x=0;
           media_x0=media_x0+particula0[i].x;
           posicion_inicial[i].x=particula0[i].x;

        }
        media_x0=media_x0/((double)N);
        t=0;
        //media_x=0;

        chi=0;
        /*FILE*f2;
        f2=fopen("Particula.txt","w");
        fprintf(f2,"%lf     %lf\n",t,particula0[0].x);*/
        //BUCLE DE TIEMPOS
        for (j=1;t<=T;j++){
            t=h*j;
            delta=0;
            media_x=0;
        //BUCLE PARA RECORRER TODAS LAS PARTÍCULAS
            for(i=0;i<N;i++){
                Box_Muller(&a,&b);
                c=Random();
                if(c<=0.5){
                    e=1;
                }
                else{
                    e=-1;
                }
                velocidad[i].x=velocidad0[i].x+h*F*e;
                //printf("%lf\n",e);
                particula[i].x=particula0[i].x+h*velocidad[i].x+sqrt(2*l*h)*a;
                velocidad0[i].x=velocidad[i].x;
                particula0[i].x=particula[i].x;
                //fprintf(f2,"%lf     %lf\n",t,particula0[i].x);
                //printf("%lf    %lf    %lf    %lf\n",t,posicion_inicial[i].x,particula0[i].x,particula[i].x);
                delta=delta+(particula[i].x-posicion_inicial[i].x)*(particula[i].x-posicion_inicial[i].x);
                media_x=media_x+particula[i].x;
            }
            //CALCULAMOS CHI
            //chi=((media_x/((double)N))-(media_x0))/F;
            chi=delta/((double)N*2);
            fprintf(f,"%lf      %lf\n",t,chi);

        }
        fclose(f);

        //fclose(f2);

    }

     system("Plot_chi_t.plt");

 return 0;
}
