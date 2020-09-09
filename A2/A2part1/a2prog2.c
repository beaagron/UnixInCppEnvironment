#include <dirent.h>
#include <stdio.h>

int main(void)
{
    struct dirent *de;
    DIR *dr = opendir(".");

    if(dr == NULL)
    {
        printf("error");
        return 0;
    }

    while((de = readdir(dr)) != NULL)
    {
        printf("%s\n", de->d_name);

    }
    closrdir(dr);

    return 0;
}