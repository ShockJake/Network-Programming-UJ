#include "functions.h"

// Socket descriptor
int server_descriptor;

// Function to close server
void sig_handler(int signum)
{
    sleep(1);
    printf("\nClosing server...\n");
    close_server(server_descriptor);
    exit(0);
}

// Function for clients
void *client_thread(void *arg)
{
    int client_descriptor = *((int *)arg);
    unsigned long long int number;
    if (!perform_action(&number, client_descriptor))
    {
        send_error(client_descriptor);
    }
    printf("Closing connection with %i\n", client_descriptor);
    close_connection(client_descriptor);
    printf("------------------------------\n");
    free(arg);
    return NULL;
}

// Function for cleaning thread attributes if there was some failure
void cleanup_attr(pthread_attr_t *attr)
{
    // Destroying thread
    int errnum = pthread_attr_destroy(attr);
    if (errnum != 0)
    {
        printf("Can't destroy thread\n");
    }
}

void set_detach_state_of_thread(pthread_attr_t *attr)
{
    int errnum = pthread_attr_setdetachstate(attr, PTHREAD_CREATE_DETACHED);
    if (errnum != 0)
    {
        printf("Can't detach thread state\n");
        cleanup_attr(attr);
    }
}

void create_thread_for_client(pthread_attr_t *attr, int *descriptor_ptr, int client_descriptor)
{
    pthread_t new_thread;
    // Creating thread for new client and passing client function to this thread
    int errnum = pthread_create(&new_thread, attr, client_thread, descriptor_ptr);
    if (errnum != 0)
    {
        perror("Can't create a thread");
        close_connection(client_descriptor);
    }
}

int set_timeout_to_a_new_client(int client_descriptor, pthread_attr_t *attr)
{
    int timeout = set_timeout(client_descriptor);
    if (timeout == -1)
    {
        cleanup_attr(attr);
        return 1;
    }
    return 0;
}

int initialize_thread_attrs(pthread_attr_t *attr)
{
    int error_num = pthread_attr_init(attr);
    if (error_num != 0)
    {
        printf("Can't initialize thread\n");
        return 1;
    }
    return 0;
}

void client_handler(int port, int sd)
{
    // Thread attributes
    pthread_attr_t attr;
    // Address of client
    char client_address[INET_ADDRSTRLEN];
    if (initialize_thread_attrs(&attr))
    {
        return;
    }
    set_detach_state_of_thread(&attr);
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

        show_new_client(client_descriptor, client_address, port);
        if (set_timeout_to_a_new_client(client_descriptor, &attr))
        {
            break;
        }
        create_thread_for_client(&attr, descriptor_ptr, client_descriptor);
    }
}

void start_server(int port)
{
    server_descriptor = create_socket(port);
    printf("\nServer is created, and listening on this port: %d\n", port);
    client_handler(port, server_descriptor);
    close_server(server_descriptor);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, sig_handler);
    int port = 2020;
    if (argc == 2)
    {
        port = strtol(argv[1], NULL, 10);
        if (port == 0)
        {
            perror("Wrong input");
            exit(1);
        }
    }
    if (port < 1024)
    {
        printf("\nGiven port is to small\n");
        exit(0);
    }
    start_server(port);
    return 0;
}