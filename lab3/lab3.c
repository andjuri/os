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

int broj_dretvi = 0; //koliko je trenutno radnih dretvi
int postavljeni_broj_dretvi =1; //koliko ih treba biti

pthread_mutex_t m; //monitor
pthread_cond_t red; //red uvjeta

int radi;
int id[100];

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

    
    pthread_t opisnik[100];

    postavljeni_broj_dretvi = rand() % 100 +2;

    pthread_mutex_init (&m, NULL);
    pthread_cond_init (&red, NULL);

	/* stvori dretve: radnu i mreznu */
	for (int i = 0; i < postavljeni_broj_dretvi; i++) 
        {id[i] = i;}

        if (pthread_create(&opisnik[0], NULL, mreza, &id[0])) 
        {
            fprintf(stderr, "Ne mogu stvoriti novu dretvu!\n");
            exit(1);
        }

    for(int j = 1; j < postavljeni_broj_dretvi; j++)
    {  
        broj_dretvi++;

        if (pthread_create(&opisnik[j], NULL, obrada, &id[j])) 
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
        pthread_mutex_lock (&m);
         while (radi)
           pthread_cond_wait (&red, &m);
        radi = 0; 
        pthread_mutex_unlock (&m);
		

            
	}

	//cekaj na kraj dretve obrada, ali ne i one druge mrezne
     // pthread_join(opisnik[0], NULL);

 pthread_mutex_destroy (&m);
 pthread_cond_destroy (&red);

    return 0;
}

void *obrada(void *p)
{
	/* vise-manje glavni dio iz lab1 */
    int x;
    FILE *obrada;
    int id = *((int *) p);

    obrada = fopen(dat_obrada, "a+");

    while(1)
    { 
        pthread_mutex_lock (&m);

        if(broj_dretvi > postavljeni_broj_dretvi)
        {
            pthread_mutex_unlock (&m);
            pthread_exit(NULL);
        }

        while (radi)
           pthread_cond_wait (&red, &m);
        
        radi= id;
        broj = broj + 1;
        x = broj * broj ;

         pthread_mutex_unlock (&m);

        
        printf("upisujem broj %d, dretva %d\n", x, id);
        fprintf(obrada, "%d\n", x);
        sleep(5);

        pthread_mutex_lock (&m);

        radi = 0;
        pthread_mutex_unlock (&m);
        pthread_cond_broadcast (&red);
        sleep(1);

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

        sleep(1);
    }
	//ako je procitano > 0 onda ga postavi u broj

	return NULL;
}