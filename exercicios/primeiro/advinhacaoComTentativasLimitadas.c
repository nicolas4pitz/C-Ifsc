#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int escolha, chute;

    srand(time(NULL));
    escolha = rand() % 50 + 1;

    for (int i = 1; i <= 5; i++) {
        printf("Tentativa %d - Digite um numero: ", i);
        scanf("%d", &chute);

        if (chute == escolha) {
            printf("Certo !!\n");
            break;
        }

        if (chute > escolha)
            printf("O Numero escolhido é menor\n");
        else
            printf("O Numero escolhido é maior\n");
            
    }

    printf("Fim! O numero era %d\n", escolha);
    return 0;
}
