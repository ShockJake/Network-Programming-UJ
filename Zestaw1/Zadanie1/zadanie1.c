#include <stdio.h>

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
            arr[i] = input;
        }
    }

    for (int i = 0; i < SIZE; i++)
    {
        if(arr[i] < 100 && arr[i] > 10)
        {
            printf("%d\n", arr[i]);
        }
    }
    
    return 0;
}