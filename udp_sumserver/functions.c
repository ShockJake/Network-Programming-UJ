#include "functions.h"

void signal_handler(int signum)
{
    close_server();
    exit(0);
}

void close_server()
{
    printf("\nClosing server...\n");
    if (close(sd) == -1)
    {
        perror("Can't close the socket");
        exit(1);
    }
}

int create_socket()
{
    // Socket creation with UDP specifications
    int result = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (result == -1)
    {
        perror("Can't create socket");
        exit(1);
    }
    return result;
}

bool is_number(char input)
{
    // Checking if a character is a number
    if (input > 57 || input < 48)
    {
        return false;
    }
    return true;
}

bool check_input(char *data, int length, bool *isRN)
{
    for (int i = 0; i < length; i++)
    {
        if (data[i] == ' ')
        {
            continue;
        }
        if (data[i] == '\n')
        {
            *isRN = true;
            continue;
        }
        if (!is_number(data[i]))
        {
            return false;
        }
    }
    return true;
}

unsigned long long int perform_action(char *data, int length)
{
    unsigned long long int number = 0;
    unsigned long long int result = 0;

    char *numberStr = strtok(data, " ");

    while (numberStr != NULL)
    {
        number = strtoull(numberStr, NULL, 10);
        if (number == ULLONG_MAX && errno == ERANGE)
        {
            return -1;
        }
        if (number > INT_MAX - result)
        {
            return -1;
        }
        result += number;
        numberStr = strtok(NULL, " ");
    }

    return result;
}

void add_ending(char *data)
{
    strcat(data, "\r\n");
}

void send_error(char *msg, size_t msg_len, struct sockaddr *clientAddr, socklen_t len)
{
    int bytesToSent = sendto(sd, msg, msg_len, MSG_CONFIRM, clientAddr, len);
    if (bytesToSent == -1)
    {
        perror("Can't send an error");
        close_server();
        exit(1);
    }
}

void fill_sock_addr_struct(struct sockaddr_in *server_addr, struct sockaddr_in *client_addr, int port)
{
    memset(server_addr, 0, sizeof(*server_addr));
    memset(client_addr, 0, sizeof(*client_addr));
    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = INADDR_ANY;
    server_addr->sin_port = htons(port);
}

void bind_server_socket(struct sockaddr *server_addr)
{
    // Binding socket to the address
    if (bind(sd, server_addr, sizeof(*server_addr)) == -1)
    {
        perror("Can't bind the socket");
        close_server();
        exit(1);
    }
}

void clear_buffers_and_variables(char *answer, char *buff, char *result_char, bool *isRN)
{
    // Clearing buffers and variables
    memset(answer, 0, 12);
    memset(buff, 0, MAX_LINE);
    memset(result_char, 0, 10);
    *isRN = false;
}

int receive_data(char *buff, struct sockaddr *client_addr, socklen_t *len)
{
    // Receiving data and checking it
    int bytesFromClient = recvfrom(sd, buff, MAX_LINE, MSG_WAITALL, client_addr, len);
    if (bytesFromClient == -1)
    {
        perror("Can't receive data");
        close_server();
        exit(1);
    }
    return bytesFromClient;
}

bool convert_result(char *answer, unsigned long long int *result, struct sockaddr *client_addr, char *error_msg)
{
    if (sprintf(answer, "%lld", *result) == -1)
    {
        perror("Can't convert result to string");
        // Informing client that error occurred
        send_error(error_msg, strlen(error_msg), (struct sockaddr *)&client_addr, sizeof(client_addr));
        return false;
    }
    return true;
}

void send_data(char *answer, struct sockaddr *client_addr, socklen_t *len)
{
    // Sending answer
    if (!sendto(sd, answer, strlen(answer), MSG_CONFIRM, client_addr, *len))
    {
        perror("Can't send a message");
    }
    else
    {
        printf("\n - Answer was sent successfully...\n");
    }
}

void start_sum_server(int port)
{
    sd = create_socket();
    char main_buffer[MAX_LINE];
    bool isRN; // Variable for state if incoming message has \r\n in the end of line.
    char error_msg[9] = "ERROR";
    // Buffer for the server answer (size = 12,
    // because of space that maximum UINT number can have + space for \r\n)
    char answer[12];

    int bytesFromClient;
    unsigned long long int result;
    char result_char[10];

    struct sockaddr_in server_addr, client_addr; // Structures for server and client
    fill_sock_addr_struct(&server_addr, &client_addr, port);
    bind_server_socket((struct sockaddr *)&server_addr);
    socklen_t len = sizeof(client_addr);

    // Main loop
    while (true)
    {
        clear_buffers_and_variables(answer, main_buffer, result_char, &isRN);
        bytesFromClient = receive_data(main_buffer, (struct sockaddr *)&client_addr, &len);

        // Checking input if it's readable by server
        if (check_input(main_buffer, bytesFromClient, &isRN))
        {
            printf("\nData from client: %s", main_buffer);
        }
        else
        {
            // If not readable informing client
            send_error(error_msg, strlen(error_msg), (struct sockaddr *)&client_addr, len);
            printf("\nUnable to read data from client\n");
            continue;
        }

        // Getting result
        result = perform_action(main_buffer, bytesFromClient - 1);
        if (result == -1)
        {
            // If data is too big server informs client that this data provokes overflow
            printf("\nLarge data has come from client and overflow happened");
            send_error(error_msg, strlen(error_msg), (struct sockaddr *)&client_addr, len);
            continue;
        }
        printf("Result: %lld\n", result);
        // Converting result from int to char*
        if (!convert_result(answer, &result, (struct sockaddr *)&client_addr, error_msg))
        {
            continue;
        }
        // Preparing answer
        if (isRN)
        {
            add_ending(answer);
        }
        send_data(answer, (struct sockaddr *)&client_addr, &len);
    }
}