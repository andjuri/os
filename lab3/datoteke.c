#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "datoteke.h"

/* implementacija funkcija, vec opisanih u lab1 + dohvati_iz_cijevi */

int procitaj_status() 
{ 
    FILE *status;
    int broj;
    status = fopen(dat_status,"r+");
    fscanf(status, "%d", &broj);
    fclose(status);
    return broj; 
}

void zapisi_status(int broj) 
{
    FILE *status;
    status = fopen(dat_status,"w+");
    fprintf(status, "%d", broj);
    fclose(status);
}

void dodaj_broj(int broj) 
{ 
    FILE *obrada;
    obrada = fopen(dat_obrada, "a+");
    fprintf(obrada, "%d", broj);
    fclose(obrada);
    
}

int pronadji_zadnji_broj() 
{ 
    FILE *obrada;
    int broj;
    obrada = fopen(dat_obrada, "r+");

    int br=0;
    while(fscanf(obrada, "%d\n", &broj) != EOF){br++;}
    broj = br; 
    

 fclose (obrada);

    return broj;
}

int dohvati_iz_cijevi()
{
	FILE *fp;
	int x, y = -1;

	fp = fopen(dat_mreza, "r");
	if (!fp) {
		fprintf(stderr, "Ne mogu otvoriti cijev %s\n", dat_mreza);
		exit(1);
	}
	if (fscanf(fp, "%d", &x) > 0)
		y = x;
	fclose(fp);

	return y;
}