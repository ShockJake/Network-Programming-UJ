/*
    Napisz program sprawdzający czy pewna określona witryna działa poprawnie.
    Program ma się dawać skompilować i uruchomić na spk-ssh.

    Sprawdzenie ma polegać na pobraniu strony spod ustalonego adresu (np. spod http://th.if.uj.edu.pl/).
    Proszę nie zapomnieć o zweryfikowaniu, czy na pewno udało się ją poprawnie pobrać (status 200)
    i czy to jest strona HTML (typ text/html). Następnie należy sprawdzić,
    czy rzeczywiście jest to spodziewana strona, a nie np.
    komunikat o wewnętrznym błędzie serwera WWW —
    to można zweryfikować sprawdzając czy w pobranej treści znajduje się pewien zadany z góry ciąg znaków
    (np. „Institute of Theoretical Physics”).

    Program, w zależności od wyniku sprawdzenia,
    musi zwracać jako wynik funkcji main kod sukcesu (zero) bądź porażki (wartość większa od zera).
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <curl/curl.h>
#include <string.h>

// Variable to check whether phrase is present on page;
static bool phrase_is_found = false;

// Variable for check phrase
static const char *check_phrase;

// Function to print information how to use this programm
void print_usage()
{
    printf("Usage:\n");
    printf("./httpChecker <URL> <PHRASE> <-d/D>\n");
    printf("--- URL - page you want to check\n");
    printf("--- PHRASE - words you want to find on page to verify it's content\n");
    printf("--- -d/D - debug mode flag (\n\tUse -d if you want to get information about error)\n");
    printf("\tUse -D if you want to see each phrase check\n");
}

// Function to print input information
void print_input(int argc, const char *argv[])
{
    printf("URL: %s\n", argv[1]);
    printf("PHRASE: %s\n", argv[2]);
    printf("Debug mode: %s\n", argv[3]);
}

// Function to check input for program
void check_input(int number, const char *args[], int *debugMode)
{
    // Checking number of arguments
    if (number < 3 || number > 4)
    {
        print_usage();
        exit(1);
    }
    // Checking whether debug mode is turned on
    if (number == 4)
    {
        if (strcmp(args[3], "-d"))
        {

            *debugMode = 1;
        }
        if (strcmp(args[3], "-D"))
        {
            *debugMode = 2;
        }
        else
        {
            print_usage();
            exit(1);
        }
    }
}

// Function to check if two strings are equal
bool str_starts_with(const char *str, const char *prefix)
{
    return strncmp(prefix, str, strlen(prefix));
}

// CURL initialization
CURL *initialize_curl()
{
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        perror("CURL Initialization failed:");
        exit(1);
    }
    return curl;
}

// Function for checking page for getting data from it
size_t check_operation(char *buffer, size_t item_size, size_t number_of_items, void *ignored)
{
    size_t bytes = item_size * number_of_items;
    return bytes;
}

bool perform_checks(CURL *curl, int debugMode)
{
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, check_operation);
    CURLcode result = curl_easy_perform(curl);
    if (result != CURLE_OK)
    {
        if (debugMode > 0)
        {
            fprintf(stderr, "Download failed: %s\n", curl_easy_strerror(result));
            return false;
        }
    }

    int html_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &html_code);
    if (html_code != 200)
    {
        if (debugMode > 0)
        {
            printf("No HTTP_OK response : %i\n", html_code);
            return false;
        }
    }

    char *content_type = NULL;
    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);

    if (strcmp(content_type, "text/html") != 0)
    {
        if (debugMode > 0)
        {
            printf("Page content is not 'text/html' : %s\n", content_type);
        }
        return false;
    }

    return true;
}

// Perform action of of CURL object
void perform_action(CURL *curl)
{
    CURLcode result = curl_easy_perform(curl);
    if (result != CURLE_OK)
    {
        fprintf(stderr, "Unnable to perform action: %s\n", curl_easy_strerror(result));
        exit(1);
    }
}

// Function to copy header without <h characters
void copy_header(char *dst, char *src, int lenght)
{
    for (int i = 0; i < lenght; i++)
    {
        dst[i] = src[i + 4];
    }
}

// Function to pass to CURL object and check data
size_t got_data(char *buffer, size_t itemSize, size_t numberOfItems, void *ignored)
{
    size_t bytes = itemSize * numberOfItems;

    char line[2048];
    memset(line, 0, sizeof(line));



    return bytes;
}

int main(int argc, char const *argv[])
{
    int debugMode = 0;

    // print_input(argc, argv);

    // Checking given arguments
    check_input(argc, argv, &debugMode);

    // Initializing CURL object
    CURL *curl = initialize_curl();

    // Url we need to
    const char *input_url = argv[1];

    // Phrase we need to find on the page
    check_phrase = argv[2];

    // Setting operation to get information from page
    curl_easy_setopt(curl, CURLOPT_URL, input_url);
    // Setting operation of Write function of CURL object
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, check_operation);

    // Performing checks
    if (!perform_checks(curl, debugMode))
    {
        curl_easy_cleanup(curl);
        exit(1);
    }

    // Setting operation of Write function of CURL object
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, got_data);

    // Performing setted functions
    perform_action(curl);

    // Cleaning CURL object
    curl_easy_cleanup(curl);

    if (!phrase_is_found)
    {
        return 1;
    }
    if (debugMode > 0)
    {
        printf("Page works...");
    }

    return 0;
}
