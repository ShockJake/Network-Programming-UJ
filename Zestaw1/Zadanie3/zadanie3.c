#include <stdio.h>
#include <stdbool.h>

bool drukowalne(const void *buf, int len)
{
    const char* char_buf = (char *)buf;
    for (int i = 0; i < len; i++)
    {
        if (char_buf[i] < 32 || char_buf[i] > 126)
        {
            return false;
        }
    }
    return true;
}

bool drukowalneP(const void *buf, int len)
{
    const char* char_buf = (char *)buf;
    for (int i = 0; i < len; i++)
    {
        char var = *(char_buf + i);
        if (*(char_buf + i) < 32 || *(char_buf + i) > 126)
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

    if (drukowalne(vp, SIZE))
    {
        printf("Works!\n");
    }
    else
    {
        printf("Not works ;(\n");
    }
    
    return 0;
}
