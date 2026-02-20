#include <stdio.h>

int main(){
    int escolha = 0;
    int soma = 0;
    int count = 0;

    while(1){
        printf("Escolha valores para somar ");
        scanf("%d", &escolha);
        if(escolha < 0){
            break;
        }
        soma += escolha;
        count++;
    }

    printf("A somatoria total foi %d", escolha);


}