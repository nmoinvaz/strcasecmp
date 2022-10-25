#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main() {
    char string1[256] = {0};
    char string2[256] = {0};

    for (int i = 0; i < sizeof(string1) - 1; i++)
        string1[i] = 'a' + (rand() % ('z' - 'a'));
    for (int i = 0; i < sizeof(string2)- 1; i++)
        string2[i] = (i % 7 == 0) ? string1[i]: toupper(string1[i]);

    printf("char *string1 = \"%s\";\n", string1);
    printf("char *string2 = \"%s\";\n", string2);

    float y = 0;
    for (int x = 0; x < sizeof(string1) -1; x++) {
        if (string1[x] == string2[x])
            y++;
    }

    printf("same percentage %.02f%%\n", (y/(sizeof(string1)-1)) * 100);
    return 0;
}