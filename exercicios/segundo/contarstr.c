#include <stdio.h>

int main() {
    char str[100];
    int i = 0;
    int contador = 0;

    printf("Digite uma string: ");
    scanf(" %s", str);

    while (str[i] != '\0') {
        if (str[i] == 'a') {
            contador++;
        }
        i++;
    }

    printf("Quantidade de 'a': %d\n", contador);

    return 0;
}