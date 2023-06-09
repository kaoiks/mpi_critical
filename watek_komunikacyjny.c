#include "main.h"
#include "watek_komunikacyjny.h"

/* wątek komunikacyjny; zajmuje się odbiorem i reakcją na komunikaty */
void *startKomWatek(void *ptr)
{
    MPI_Status status;
    int is_message = FALSE;
    packet_t pakiet;
    /* Obrazuje pętlę odbierającą pakiety o różnych typach */
    while ( stan!=InFinish ) {
	debug("czekam na recv");
        MPI_Recv( &pakiet, 1, MPI_PAKIET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        pthread_mutex_lock(&mutex);
        if (pakiet.ts > timestamp){
            timestamp = pakiet.ts;
        }
        ++timestamp;
        pthread_mutex_unlock(&mutex);
        
        switch ( status.MPI_TAG ) {
	    case REQUEST: 
            queue[pakiet.src] = pakiet.ts;
            sendPacket( 0, status.MPI_SOURCE, ACK, timestamp);
            pthread_mutex_lock(&mutex);
            ++timestamp;
            pthread_mutex_unlock(&mutex);
        break;
        case RELEASE:
            queue[pakiet.src] = INT_MAX;

	    break;
	    case ACK: 
                debug("Dostałem ACK od %d, mam już %d", status.MPI_SOURCE, ackCount);
	        ackCount++; /* czy potrzeba tutaj muteksa? Będzie wyścig, czy nie będzie? Zastanówcie się. */
	    break;
	    default:
	    break;
        }
    }
}
