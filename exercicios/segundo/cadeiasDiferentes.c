#include <stdio.h>

int compararStrings(char str1[], char str2[]) {
    int i = 0;

    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return 0;
        }
        i++;
    }

    if (str1[i] == '\0' && str2[i] == '\0') {
        return 1;
    }

    return 0;
}

int main() {
    char str1[100];
    char str2[100];

    printf("Primeira string: ");
    scanf(" %s", str1);

    printf("Segunda string: ");
    scanf(" %s", str2);

    if (compararStrings(str1, str2)) {
        printf("Iguais.\n");
    } else {
        printf("Diferentes.\n");
    }

    return 0;
}