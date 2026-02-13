#include <stdio.h>

int ehPrimo(int n) {
    if (n < 2)
        return 0;

    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

int main() {
    int num;
    int contador = 0;

    for (int i = 0; i < 20; i++) {
        printf("Digite um numero: ");
        scanf("%d", &num);

        if (num <= 0)
            continue;

        if (ehPrimo(num))
            contador++;
    }

    printf("Quantidade de numeros primos: %d\n", contador);

    return 0;
}
