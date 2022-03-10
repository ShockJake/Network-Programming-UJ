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
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int createSocket()
{
    int result = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(result == -1)
    {
        perror("Can't create a socket");
        _exit(-1);
    }
    return result;
}

bool verifyConnection(int sockD)
{
    const char* data = "o";
    while (true)
    {
        sendto(sockD, );
    }
    
}

void startUDPServer(int port)
{
    // Tworzenie deskryptora dla soketu
    int sd = createSocket();

    struct sockaddr_in server_addr, client_addr;

    // Ilość opracowanych bajtów
    ssize_t byteS;
    ssize_t byteR;

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // Wypełnianie informacji o serverze
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    

    int n, len;



}

int main(int argc, char const *argv[])
{
    
    return 0;
}
