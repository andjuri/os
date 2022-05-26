/* Operacije nad signalima */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "signali.h"
#include "datoteke.h"


/* implementacija funkcija, vec opisanih u lab1 + postavi_signale() */
void postavi_signale() 
{   
    struct sigaction act;
    /* 1. maskiranje signala SIGUSR1 */
    act.sa_handler = obradi_dogadjaj; /* kojom se funkcijom signal obrađuje */
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGTERM); /* blokirati i SIGTERM za vrijeme obrade */
    act.sa_flags = 0; /* naprednije mogućnosti preskočene */
    sigaction(SIGUSR1, &act, NULL); /* maskiranje signala preko sučelja OS-a */
 
    /* 2. maskiranje signala SIGTERM */
    act.sa_handler = obradi_sigterm;
    sigemptyset(&act.sa_mask);
    sigaction(SIGTERM, &act, NULL);
 
    /* 3. maskiranje signala SIGINT */
    act.sa_handler = obradi_sigint;
    sigaction(SIGINT, &act, NULL);
 
}

void obradi_dogadjaj(int sig) 
{
    printf("Trenutni broj u obradi je %d \n", broj);
}

void obradi_sigterm(int sig) 
{
    FILE *status;
    printf("Primio signal SIGTERM, pospremam prije izlaska iz programa\n");
    status = fopen(dat_status, "w+");
    fprintf(status, "%d", broj);
    fclose(status);
    nije_kraj = 0;
    exit(1);
}

void obradi_sigint(int sig) 
{
    printf("Primio signal SIGINT, prekidam rad\n");
    exit(1);
}