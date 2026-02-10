#include <stdio.h>

int main(){
    int escolha = 0;
    int soma = 0;

    while(escolha <= 0){
        printf("Escolha valores para somar ");
        scanf("%d", escolha);
        int somaAnterior = soma;
        soma = escolha + somaAnterior;
    }

    printf("A somatoria total foi %d", escolha);


}