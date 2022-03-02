#include <stdio.h>
#include <stdbool.h>

bool drukowalne(const char *buf)
{
    const int SIZE = sizeof(buf) - sizeof(buf[0]);
    for (int i = 0; i < SIZE; i++)
    {
        if (buf[i] < 32 || buf[i] > 126)
        {
            return false;
        }
    }
    return true;
}

bool drukowalneP(const char *buf)
{
    const int SIZE = sizeof(buf) - sizeof(buf[0]);
    for (int i = 0; i < SIZE; i++)
    {
        if (*(buf + i) < 32 || *(buf + i) > 126)
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char const *argv[])
{
    const int SIZE = 50;
    int arr[SIZE];
    int i = 0;
    int input;

    printf("Write 50 numbers:\n");
    for (int i = 0; i < SIZE; i++)
    {
        scanf("%d", &input);
        if (input == 0)
        {
            break;
        }
        else
        {
            *(arr + i) = input;
        }
    }

    const void* vp = arr;

    if (drukowalne(vp))
    {
        printf("Works!\n");
    }
    else
    {
        printf("Not works ;(\n");
    }
    
    return 0;
}
