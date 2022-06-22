#include "functions.h"

bool is_number(char input, unsigned short int *space_counter)
{
    if (input == ' ')
    {
        // printf("[+] Detected space = %i\n", *space_counter);
        *space_counter += 1;
        return true;
    }
    if (input == '\r' || input == '\n')
    {
        return true;
    }
    // Checking if a character is a number
    if (input < 58 && input > 47)
    {
        // printf("[+] Detected number %c(%i), spaces = %i\n", input, (int)input, *space_counter);
        *space_counter = 0;
        return true;
    }
    return false;
}

void send_error(int asd)
{
    const char error_msg[7] = "ERROR\r\n";
    // Sending error to the client
    ssize_t byteN = write(asd, error_msg, sizeof(error_msg));
    if (byteN == -1)
    {
        perror("Can't send error");
        exit(1);
    }
}

void close_server(int server_descriptor)
{
    if (close(server_descriptor) == -1)
    {
        perror("Can't close server");
        exit(1);
    }
}

void add_ending(char *data)
{
    strcat(data, "\r\n");
}

void fill_sockaddrs_struct(int port, struct sockaddr_in *addr)
{
    memset(addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    addr->sin_addr.s_addr = htonl(INADDR_ANY);
}

void bind_and_listen(int server_descriptor, const struct sockaddr *addr)
{
    if (bind(server_descriptor, addr, sizeof(*addr)) == -1)
    {
        close_server(server_descriptor);
        perror("Can't bind socket");
        exit(1);
    }
    if (listen(server_descriptor, 10) == -1)
    {
        close_server(server_descriptor);
        perror("Can't make the socket listen");
        exit(1);
    }
}

int create_socket(int port)
{
    // Socket creation with TCP specifications
    int server_descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_descriptor == -1)
    {
        perror("Can't create socket");
        exit(1);
    }
    // Structure for server data
    struct sockaddr_in address;
    fill_sockaddrs_struct(port, &address);
    // Cast to const struct sockaddr for passing into functions
    const struct sockaddr *addr = (const struct sockaddr *)&address;
    bind_and_listen(server_descriptor, addr);

    return server_descriptor;
}

unsigned long long int sum_numbers(char *data)
{
    unsigned long long int number = 0;
    unsigned long long int result = 0;
    // Separating buffer to tokens
    char *numberStr = strtok(data, " ");
    while (numberStr != NULL)
    {
        // Converting char* -> unsigned long long int
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
        // Moving through the buffer
        numberStr = strtok(NULL, " ");
    }
    return result;
}

void send_data(unsigned long long int answer_int, int clientDescriptor)
{
    ssize_t byteN;
    char answer_ch[12];
    // Converting char* -> unsigned long long int
    byteN = sprintf(answer_ch, "%lld", answer_int);
    if (byteN == -1)
    {
        perror("Can't convert data");
        send_error(clientDescriptor);
        return;
    }
    add_ending(answer_ch);
    printf("Result: %s", answer_ch);

    // Length of answer
    int length = strlen(answer_ch);

    // Sending answer
    byteN = write(clientDescriptor, answer_ch, length);
    if (byteN == -1)
    {
        perror("Can't send data");
    }
    printf("Answer was sent successfully\n============================\n");
}

bool check_data(char *message)
{
    unsigned short int space_counter = 0;

    // Variable to check if the message has \r in it
    bool hasR = false;

    unsigned short int length = strlen(message);
    if (length <= 0)
    {
        return false;
    }

    // printf("[+] Checking data: %s| with length: %i\n", message, length);

    // Checking data
    for (int i = 0; i < length; i++)
    {
        if (is_number(message[i], &space_counter))
        {
            if (space_counter >= 2)
            {
                printf("[-] To many spaces: %i\n", space_counter);
                return false;
            }
        }
        if (!is_number(message[i], &space_counter))
        {
            printf("[-] Wrong data: %i : %i\n", (int)message[i], i);
            return false;
        }
        if (message[i] == '\n' && space_counter > 0)
        {
            printf("[-n] To many spaces: %i\n", space_counter);
            return false;
        }
        if (message[i] == '\r')
        {
            hasR = true;
            message[i] = ' ';
            continue;
        }
        if (message[i] == '\n' && !hasR)
        {
            printf("[-] Lack of \\r character\n");
            return false;
        }
        if (message[i] == '\n')
        {
            if (message[0] == ' ' || message[i - 2] == ' ')
            {
                printf("[-] Space in the beginning or in the end of message\n");
                return false;
            }
        }
    }
    return true;
}

bool perform_action(unsigned long long int *number, int cd)
{
    // unsigned short int space_counter = 0;
    char input[2] = {0, 0};
    char message[1024];
    ssize_t byteN;
    memset(message, 0, sizeof(message));

    // Reading data from client
    while ((byteN = read(cd, input, sizeof(input) - 1)) != 0)
    {
        if (byteN == -1)
        {
            perror("Can't read data");
            return false;
        }
        strncat(message, input, 1);
        if (input[0] == '\n')
        {
            printf("============================\n");
            printf("Message form the client - %i:\n:: %s\n", cd, message);
            if (!check_data(message))
            {
                printf("Sending error...\n");
                // Sending errors
                send_error(cd);
            }
            else
            {
                // summing numbers
                *number = sum_numbers(message);

                // Sending data to the client
                send_data(*number, cd);
            }
            memset(input, 0, sizeof(input));
            memset(message, 0, sizeof(input));
        }
    }
    return true;
}

int close_connection(int con)
{
    int result = close(con);
    if (result == -1)
    {
        perror("Can't close the connection");
    }
    return result;
}

void show_new_client(int client_descriptor, char *client_addr, int port)
{
    // Structure for holding informations about client
    struct sockaddr_in _addr;
    socklen_t length = sizeof(_addr);

    // Getting name of client
    if (getpeername(client_descriptor, (struct sockaddr *)&_addr, &length) == -1)
    {
        perror("Can't get name of peer");
        exit(1);
    }

    // Converting ip from binary to text and showing it's address
    const char *client_ip = inet_ntop(AF_INET, &(_addr.sin_addr), client_addr, INET_ADDRSTRLEN);
    if (client_ip != NULL)
    {
        printf("-------------------------------\n");
        printf("Connected with: %s:%d|\ndescriptor number: %i\n\n", client_addr, port, client_descriptor);
    }
}

int set_timeout(int socket)
{
    // Structure for timeout specification
    struct timeval timeout;
    timeout.tv_sec = 20;
    timeout.tv_usec = 0.0;

    // Setting timeout for the client
    int result = setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    if (result == -1)
    {
        perror("Can't set timeout");
        return -1;
    }
    return result;
}