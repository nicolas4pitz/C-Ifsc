#include <stdio.h>

int conta_caractere(char str[], char c) {
    int i = 0;
    int contador = 0;

    while (str[i] != '\0') {
        if (str[i] == c) {
            contador++;
        }
        i++;
    }

    return contador;
}

int main() {
    char str[100];
    char caractere;

    printf("Digite uma string: ");
    scanf(" %s", str);

    printf("Digite um caractere: ");
    scanf(" %c", &caractere); 

    int resultado = conta_caractere(str, caractere);

    printf("O caractere '%c' aparece %d vezes.\n", caractere, resultado);

    return 0;
}