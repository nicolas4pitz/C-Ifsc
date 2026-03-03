#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char titulo[100];
    int userID;
    int status; 
    float nota; 
} TCC;

void consultarTCC(TCC tccs[], int quantidade, int idBuscado) {
    int encontrado = 0;
    
    for (int i = 0; i < quantidade; i++) {
        if (tccs[i].userID == idBuscado) {
            encontrado = 1;
            
            printf("Título: %s\n", tccs[i].titulo);
            
            printf("userID: %d\n", tccs[i].userID);
            
            printf("Status: ");
            if (tccs[i].status == 1) {
                printf("Em andamento\n");
            } else if (tccs[i].status == 2) {
                printf("Defendido\n");
            } else if (tccs[i].status == 3) {
                printf("Suspenso\n");
            }
            
            printf("Nota: ");
            if (tccs[i].nota == -1) {
                printf("N/A\n");
            } else {
                printf("%.1f\n", tccs[i].nota);
            }
            
            break;
        }
    }
    
    if (encontrado == 0) {
        printf("TCC com userID %d não encontrado!\n", idBuscado);
    }
}

int main(int argc, char* argv[]) {
    TCC tccs[5] = {
        {"Desenvolvimento de App Mobile", 101, 1, -1},
        {"Sistema de Monitoramento Ambiental", 103, 1, -1},
        {"Chatbot com Inteligência Artificial", 102, 2, 8.5},
        {"Plataforma de E-commerce", 104, 3, -1},
        {"Análise de Dados com Machine Learning", 105, 2, 9.0}
    };
    
    int userID = -1;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-u") == 0 && i + 1 < argc) {
            userID = atoi(argv[i + 1]);
            i++;
        }
    }
    
    if (userID == -1) {
        printf("É usado com: ./tcc -u <userID>\n");
        printf("Exemplo: ./tcc -u 103\n");
        return 1;
    }
    
    consultarTCC(tccs, 5, userID);
    
    return 0;
}
