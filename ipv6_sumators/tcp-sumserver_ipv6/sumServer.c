#include "functions.h"

// Socket descriptor
int server_descriptor;

// Function to close server
void sig_handler(int signum)
{
    sleep(1);
    printf("\nClosing server...\n");
    if (close(server_descriptor) == -1)
    {
        perror("Can't close server");
        exit(1);
    }
    exit(0);
}

// Function for clients
void *client_thread(void *arg)
{
    int s = *((int *)arg);

    unsigned long long int number;
    if (!performAction(&number, s))
    {
        sendError(s);
    }
    printf("Closing connection with %i\n", s);
    closeConnection(s);
    printf("------------------------------\n");
    free(arg);
    return NULL;
}

// Function for cleaning if there was some failure
void cleanup_attr(pthread_attr_t *attr)
{
    // Destroying thread
    int errnum = pthread_attr_destroy(attr);
    if (errnum != 0)
    {
        printf("Can't destroy thread\n");
    }
}

void client_handler(int port, int sd)
{
    // Thread attributes
    pthread_attr_t attr;
    int errnum;

    // Address of client
    char client_address[INET_ADDRSTRLEN];

    // Thread attributes initialization
    errnum = pthread_attr_init(&attr);
    if (errnum != 0)
    {
        printf("Can't initialize thread\n");
        return;
    }
    // Setting detach state of new thread 
    errnum = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (errnum != 0)
    {
        printf("Can't set thread detach state \n");
        cleanup_attr(&attr);
    }

    // Main loop
    while (true)
    {
        // Accepting new client
        int client_descriptor = accept(sd, NULL, 0);
        if (client_descriptor == -1)
        {
            break;
        }
        // Allocating memory to the descriptor pointer
        int *descriptor_ptr = (int *)malloc(sizeof(int));
        if (descriptor_ptr == NULL)
        {
            printf("Can't allocate memory to thread argument\n");
            cleanup_attr(&attr);
            break;
        }
        *descriptor_ptr = client_descriptor;
        
        showNewClient(client_descriptor, client_address, port);
        int timeout = setTimeout(client_descriptor);
        if(timeout == -1)
        {
            cleanup_attr(&attr);
            break;
        }
        pthread_t thr;
        // Creating thread for new client and passing client function to this thread
        errnum = pthread_create(&thr, &attr, client_thread, descriptor_ptr);
        if (errnum != 0)
        {
            perror("Can't create a thread");
            closeConnection(client_descriptor);
        }
    }
}

void startServer(int port)
{
    // Creating socket
    server_descriptor = createSocket(port);

    printf("\nServer is created, and listening on this port: %d\n", port);

    // Starting handling clients
    client_handler(port, server_descriptor);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, sig_handler);

    // Port for socket
    int port = 2020;

    if (argc == 2)
    {
        port = strtol(argv[1], NULL, 10);
        if (port == 0) // Check whether conversion to int was successful 
        {
            perror("Wrong input");
            exit(1);
        }
    }
    if (port < 1024) // Check whether port is not small
    {
        printf("\nGiven port is to small\n");
        exit(0);
    }

    startServer(port);

    return 0;
}