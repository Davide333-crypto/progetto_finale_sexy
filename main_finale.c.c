#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>

#include "inotify.h"

int main(int argc, char *argv[])
{
    int inotifyFd;                  

    char buffer[BUF_LEN];           

    char *p;                        

    ssize_t numRead;                

    struct inotify_event *event;    

    // maschera eventi da controllare
    uint32_t mask =
        IN_CREATE |                 
        IN_DELETE |                 
        IN_MOVED_FROM |             
        IN_MOVED_TO |               
        IN_ISDIR |                  
        IN_DELETE_SELF |            
        IN_IGNORED;                 

    // controllo argomenti
    if (argc < 2)
    {
        printf("uso: %s <percorso>\n", argv[0]);
        return 1;
    }

    // inizializza inotify
    inotifyFd = inotify_init();

    // controllo errore inizializzazione
    if (inotifyFd == -1)
    {
        printf("errore inizializzazione inotify\n");
        return 1;
    }

    /* WATCH */

    // aggiunge la directory/file da osservare
    int wd = inotify_add_watch(inotifyFd,
                               argv[1],
                               mask);

    // controllo errore watch
    if (wd == -1)
    {
        printf("errore aggiunta watch\n");
        return 1;
    }

    // array dove salvare il percorso assoluto
    char abs_path[PATH_MAX];

    // converte percorso relativo in assoluto
    if (getAbsPath(argv[1], abs_path) == NULL)
    {
        printf("errore percorso assoluto\n");
        return 1;
    }

    // stampa watch attiva
    printf("watch attivo su %s wd=%d\n",
           abs_path,
           wd);

    // ciclo infinito
    for (;;)
    {
        // legge eventi dal kernel
        numRead = read(inotifyFd,
                       buffer,
                       BUF_LEN);

        // controllo errore lettura
        if (numRead <= 0)
        {
            printf("errore lettura eventi\n");
            return 1;
        }

        // stampa byte letti
        printf("letti %ld byte da inotify\n", numRead);

        // scorre tutti gli eventi presenti nel buffer
        for (p = buffer; p < buffer + numRead;)
        {
            // converte il buffer in evento
            event = (struct inotify_event *) p;

            // stampa evento
            displayEvent(event);

            // passa all'evento successivo
            p += sizeof(struct inotify_event)
                 + event->len;
        }
    }

    // chiude file descriptor
    close(inotifyFd);

    return 0;
}