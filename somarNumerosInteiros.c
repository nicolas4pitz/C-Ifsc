#include <stdio.h>

int main(){
    int escolha;
    int soma = 0;

    while(escolha < 0){
        printf("Escolha valores para somar ");
        scanf("%d", escolha);
        soma = soma + escolha;
    }

    printf("A somatoria total foi %d", escolha);


}