#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include"pomiar_czasu.h"
#define PRZEDZIAL_A 0
#define PRZEDZIAL_B 10
typedef struct {
    int a;
    int b;
    int id;
} Przedzial;

int ROZMIAR = 60;
int LICZBA_W = 4;


pthread_mutex_t muteks;
pthread_t *watki;//[LICZBA_W];
double *local_suma;//[LICZBA_W]; // muszê zainicjalizowaæ o wartosci 0
Przedzial *p;//[LICZBA_W];
int *indeksy;//[LICZBA_W];
double f(double x) {
    return ((2*x + 5)*x)-2;
    //return 2*x*x+5*x-2;
}

double suma_trapez(double x1, double x2) {
    return (f(x1)+f(x2))*(x2-x1)/2;
}


double licz_calke_sekwencyjnie() {
    double dx = ((double)PRZEDZIAL_B - (double)PRZEDZIAL_A) / (double)ROZMIAR;
    double x1;
    int i = 0;
    double suma = 0;
    while(i < ROZMIAR) {
        x1 = (double)PRZEDZIAL_A + (double)i * dx;
        suma += suma_trapez(x1,x1 + dx);
        i++;
        if(i >= ROZMIAR) { //
            break;      //
        }
    }
    return suma;
}

// Dok³adnoæ siê poprawi jeli bêde mia³a 100% pewnoci ¿e jestem na n-1 kroku iteracji i koñcowa wartoæ to PRZEDZIAL_B

void * licz_calke_cyklicznie(int id) {
    double dx = ((double)PRZEDZIAL_B - (double)PRZEDZIAL_A) / (double)ROZMIAR;
    double x1;
    int i = id;
    while(i < ROZMIAR) {
        x1 = (double)PRZEDZIAL_A + (double)i * dx;
        
        local_suma[id] += suma_trapez(x1,x1 + dx);
        i += LICZBA_W;
        if(i >= ROZMIAR) {
            break;
        }
    }
    pthread_exit( (void *)0);
}

void * licz_calke_blokowo(void *arg_wsk) {
    Przedzial p = *((Przedzial*) arg_wsk);
    double dx = ((double)PRZEDZIAL_B - (double)PRZEDZIAL_A) / (double)ROZMIAR;
    double x1, x2;
    int i = p.a;
    while(i < p.b) {
        x1 = (double)PRZEDZIAL_A + (double)i * dx;
        x2 = x1 + dx;
        local_suma[p.id] += suma_trapez(x1,x2);
        i++;
        if(i >= ROZMIAR) {
            break;
        }
    }
}


int main () {
    //Inicjalizacja obliczeñ sekwencyjnych


    watki = (pthread_t*) malloc(LICZBA_W * sizeof(*watki));
    local_suma = (double*) malloc(LICZBA_W * sizeof(*local_suma));
    p = (Przedzial*) malloc(LICZBA_W * sizeof(*p));
    indeksy = (int*) malloc(LICZBA_W * sizeof(*indeksy));


    int it;
    
    //LICZBA_W = w;
    //ROZMIAR = n;
    printf("Rozpoczecie obliczen dla %d watkow i dokladnosci %d\n", LICZBA_W, ROZMIAR);


    printf("Obliczenia cyklicznie\n"); // Dekompozycja sterowania

    

    double suma = 0;
    for(it = 0; it < LICZBA_W; it++) {
        indeksy[it] = it;
        local_suma[it] = 0;
        pthread_create( &watki[it], NULL, licz_calke_cyklicznie, (void *) indeksy[it] );
    }
    for(it = 0; it < LICZBA_W; it++) {
        pthread_join( watki[it], NULL );
        suma += local_suma[it];
    }

    
    

    printf("Koniec obliczen, suma wynosi %lf\n\n", suma);

    printf("Obliczenia cyklicznie\n"); // Dekompozycja blokowa
    suma = 0;
    

    for(it = 0; it < LICZBA_W; it++) {
        local_suma[it] = 0;
        p[it].id = it;
        p[it].a = it * ROZMIAR / LICZBA_W;
        p[it].b = (it+1) * ROZMIAR / LICZBA_W;
        if(p[it].b > ROZMIAR) {
            p[it].b = ROZMIAR;
        }
        pthread_create( &watki[it], NULL, licz_calke_blokowo, (void *) &p[it]);
    }
    for(it = 0; it < LICZBA_W; it++) {
        pthread_join( watki[it], NULL );
        suma += local_suma[it];
    }
    
    
    printf("Koniec obliczen, suma wynosi %lf\n\n", suma);


    printf("Obliczenia sekwencyjne\n");
    
    suma = licz_calke_sekwencyjnie();
    
    
    printf("Koniec obliczen, suma wynosi %lf\n\n", suma);

    free(watki);// = (pthread_t*) malloc(w * sizeof(*watki));
    free(local_suma);// = (double*) malloc(w * sizeof(*local_suma));
    free(p);// = (Przedzial*) malloc(w * sizeof(*p));
    free(indeksy); 

    return 0;

}
/*
int main()
{
    
	int rdzenie[8];
        int dokladn[2];
	int x;
	double *czasy;
        czasy = (double*) malloc((8*2*3) * sizeof(*czasy));

	int i,j;
	for(i=0;i<8;i++)
	{
		rdzenie[i] = i+1;
	}
	dokladn[0] = 10000;
	dokladn[1] = 10000000;
        for(i=0;i<2;i++)
        {
		for(j=0;j<8;j++)
		{
			calki(rdzenie[j],dokladn[i],j+i*8);
		}
	}
	
	for(i=0;i<(2*8*3);i++)
	{
		printf("%lf\n",czasy[i]);
	}


    watki = (pthread_t*) malloc(LICZBA_W * sizeof(*watki));
    local_suma = (double*) malloc(LICZBA_W * sizeof(*local_suma));
    p = (Przedzial*) malloc(LICZBA_W * sizeof(*p));
    indeksy = (int*) malloc(LICZBA_W * sizeof(*indeksy));


    int it;
 
    //printf("Rozpoczecie obliczen dla %d watkow i dokladnosci %d\n", LICZBA_W, ROZMIAR);


    //printf("Obliczenia cyklicznie\n"); // Dekompozycja sterowania

    

    double suma = 0;
    for(it = 0; it < LICZBA_W; it++) {
        indeksy[it] = it;
        local_suma[it] = 0;
        pthread_create( &watki[it], NULL, licz_calke_cyklicznie, (void *) indeksy[it] );
    }
    for(it = 0; it < LICZBA_W; it++) {
        pthread_join( watki[it], NULL );
        suma += local_suma[it];
    }

    
    

    //printf("Koniec obliczen, suma wynosi %lf\n\n", suma);

    //printf("Obliczenia cyklicznie\n"); // Dekompozycja blokowa
    suma = 0;
    

    for(it = 0; it < LICZBA_W; it++) {
        local_suma[it] = 0;
        p[it].id = it;
        p[it].a = it * ROZMIAR / LICZBA_W;
        p[it].b = (it+1) * ROZMIAR / LICZBA_W;
        if(p[it].b > ROZMIAR) {
            p[it].b = ROZMIAR;
        }
        pthread_create( &watki[it], NULL, licz_calke_blokowo, (void *) &p[it]);
    }
    for(it = 0; it < LICZBA_W; it++) {
        pthread_join( watki[it], NULL );
        suma += local_suma[it];
    }
    
    
    //printf("Koniec obliczen, suma wynosi %lf\n\n", suma);


    //printf("Obliczenia sekwencyjne\n");
    
    suma = licz_calke_sekwencyjnie();
    
    
    //printf("Koniec obliczen, suma wynosi %lf\n\n", suma);

    free(watki);// = (pthread_t*) malloc(w * sizeof(*watki));
    free(local_suma);// = (double*) malloc(w * sizeof(*local_suma));
    free(p);// = (Przedzial*) malloc(w * sizeof(*p));
    free(indeksy); 


	return 0;
}
*/
