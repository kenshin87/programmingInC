#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    printf("%s \n", getprotobyname("tcp")); 
}
