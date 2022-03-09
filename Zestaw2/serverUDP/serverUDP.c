/*
    Spróbuj napisać parę klient-serwer komunikującą się przy pomocy protokołu UDP. 
    Pamiętaj o tym, że UDP nie nawiązuje połączeń, 
    więc to klient będzie musiał jako pierwszy wysłać jakiś datagram, a serwer na niego odpowie. 
    
    Sprawdź, czy możliwe jest wysyłanie pustych datagramów (tzn. o długości zero bajtów) — 
    jeśli tak, to może niech klient właśnie taki wysyła?
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int createSocket()
{
    int result = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP);
    if(result == -1)
    {
        perror("Can't create a socket");
        _exit(-1);
    }
    return result;
}

void startUDPServer()
{
    
}

int main(int argc, char const *argv[])
{
    
    return 0;
}
