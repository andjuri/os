#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "signali.h"
#include "datoteke.h"

char *dat_status, *dat_obrada, *dat_mreza;
int broj = 0;
int nije_kraj = 1;


/* funkcije koje rade dretve */
void *obrada(void *);
void *mreza(void *);

int main(int argc, char *argv[])
{
	if (argc < 4) {
		fprintf(stderr, "Koristenje: %s <status-datoteka> "
		 "<datoteka-s-podacima> <cjevovod>\n", argv[0]);
		exit(1);
	}
	dat_status = argv[1];
	dat_obrada = argv[2];
	dat_mreza = argv[3];

	postavi_signale();

	/* dohvati prvi broj i zapisi 0 u status */
	printf("Program s PID=%ld krenuo s radom\n", (long) getpid());

    broj = procitaj_status();

    if(broj == 0)
     {
         broj = pronadji_zadnji_broj(); 
     }
    
    zapisi_status(0);

    int id[3];
    pthread_t opisnik[3];

	/* stvori dretve: radnu i mreznu */
	for (int i = 0; i < 3; i++) 
        {id[i] = i;}

        if (pthread_create(&opisnik[0], NULL, obrada, &id[0])) 
        {
            fprintf(stderr, "Ne mogu stvoriti novu dretvu!\n");
            exit(1);
        }

    for(int j = 1; j < 3; j++)
    {  
        if (pthread_create(&opisnik[j], NULL, mreza, &id[j])) 
        {
            fprintf(stderr, "Ne mogu stvoriti novu dretvu!\n");
            exit(1);
        }
    }

    int b;

	while(nije_kraj) 
    {
		scanf("%d", &b); //procitaj broj iz konzole, npr. sa scanf

		if(b > 0) broj = b ; //ako je > 0 onda ga postavi kao broj

		    else  nije_kraj = 0; //inace prekini s radom => postavi nije_kraj = 0
	}

	//cekaj na kraj dretve obrada, ali ne i one druge mrezne
       pthread_join(opisnik[0], NULL);

    return 0;
}

void *obrada(void *p)
{
	/* vise-manje glavni dio iz lab1 */
    int x;
    FILE *obrada;

    obrada = fopen(dat_obrada, "a+");

    while(1)
    {
        broj = broj + 1;
        x = broj * broj ;
        printf("upisujem broj %d\n", x);
        fprintf(obrada, "%d\n", x);
        sleep(5);
 
    }

    fclose(obrada);

	return NULL;
}

void *mreza(void *p)
{
	int a;  

	while(1)//u petlji cekaj da se nesto pojavi u cijevi
    {
        a = dohvati_iz_cijevi();
        if( a > 0 ) broj = a;
    }
	//ako je procitano > 0 onda ga postavi u broj

	return NULL;
}