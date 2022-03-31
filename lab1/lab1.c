#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
 
void obradi_dogadjaj(int sig);
void obradi_sigterm(int sig);
void obradi_sigint(int sig);
 
    int broj;
    FILE *obrada;
    FILE *status;
 
int main()
{
    struct sigaction act;
 
 
    obrada = fopen("obrada.txt", "r+");
    status = fopen("status.txt", "w+");
 
    int n, pom, x;
 
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
 
    printf("Program s PID=%ld krenuo s radom\n", (long) getpid());
 
    fscanf(status, "%d", &broj);
    

    if(broj == 0)
    { int br=0;
      while(fscanf(obrada, "%d\n", &broj) != EOF){br++;}
      broj = br; 
    }
 
    printf("broj je %d\n", broj);
    fprintf(status, "%d", 0);

    fclose(status);
 
    while(1)
    {
      printf("povecavam varijablu broj!\n");
        broj = broj + 1;
        x = broj * broj ;
        fprintf(obrada, "%d\n", x);
        sleep(5);
 
    }
 
    fclose(status);
    fclose(obrada);
    return 0;
}
//na sigusr1: ispiši broj
    void obradi_dogadjaj(int sig)
{
    printf("Trenutni broj u obradi je %d \n", broj);
}
 
    //na sigterm: zapiši broj u status.txt, završi program
    void obradi_sigterm(int sig)
{
    printf("Primio signal SIGTERM, pospremam prije izlaska iz programa\n");
    status = fopen("status.txt", "w+");
    fprintf(status, "%d", broj);
    fclose(status);
    exit(1);
}
 
 
    // na sigint: završi program
    void obradi_sigint(int sig)
{
    printf("Primio signal SIGINT, prekidam rad\n");
    exit(1);
}
 