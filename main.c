
#include <stdio.h>
#include "get_next_line.h"
#include "libft.h"

int main()
{
    char *temp = get_next_line(0);
    while (temp)
    {
        printf("%s\n", temp);
        free(temp);
        temp = get_next_line(0);
    }
    return 0;
}