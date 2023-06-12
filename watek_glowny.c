#include "main.h"
#include "watek_glowny.h"

int beforeMeInLine;

void mainLoop()
{
    srandom(time(NULL) + rank);
    int tag;
    int perc;

    while (stan != InFinish) {
	switch (stan) {
	    case InRun: 
			perc = random()%100;
			if ( perc < 25 ) {
				debug("Perc: %d", perc);
				println("Ubiegam się o sekcję krytyczną")
				debug("Zmieniam stan na wysyłanie");
				packet_t *pkt = malloc(sizeof(packet_t));
				pkt->data = perc;
				ackCount = 0;

				queue[rank] = timestamp;
				
				for (int i=0;i<=size-1;i++)
				if (i!=rank)
					sendPacket( pkt, i, REQUEST, timestamp);
				pthread_mutex_lock(&mutex);
				++timestamp;	
				pthread_mutex_unlock(&mutex);
				changeState( InWant );
				free(pkt);
			}
			debug("Skończyłem myśleć");
		break;
	    case InWant:{
			// println("Czekam na wejście do sekcji krytycznej")
			beforeMeInLine = 0;
			for(int i = 0; i < size; ++i){
				if(queue[i] < queue[rank]){
					++beforeMeInLine;
				}
			}

			if ( ackCount == size - 1 && beforeMeInLine < B) 
				changeState(InSection);
			break;
		}
	    case InSection:
		// tutaj zapewne jakiś muteks albo zmienna warunkowa
		println("Jestem w sekcji krytycznej %d", beforeMeInLine)
		    sleep(5);
		//if ( perc < 25 ) {
		    debug("Perc: %d", perc);
		    println("Wychodzę z sekcji krytyczneh")
		    debug("Zmieniam stan na wysyłanie");
		    packet_t *pkt = malloc(sizeof(packet_t));
		    pkt->data = perc;
		    for (int i=0;i<=size-1;i++)
			if (i!=rank)
			    sendPacket( pkt, (rank+1)%size, RELEASE, timestamp);
			pthread_mutex_lock(&mutex);
			++timestamp;	
			pthread_mutex_unlock(&mutex);
			queue[rank] = INT_MAX;
			
		    changeState( InRun );
		    free(pkt);
		//}
		break;
	    default: 
		break;
            }
        sleep(SEC_IN_STATE);
    }
}
