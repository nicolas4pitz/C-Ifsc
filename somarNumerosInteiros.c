#include <stdio.h>

int main(){
    int escolha = 0;
    int soma = 0;
    int count = 0;

    while(escolha <= 0){
        printf("Escolha valores para somar ");
        scanf("%d", &escolha);
        int somaAnterior = soma;
        soma += escolha;
        count++;
    }

    printf("A somatoria total foi %d", escolha);


}