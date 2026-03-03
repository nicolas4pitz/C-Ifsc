#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct sensor {
    char id[20];
    float leituras[5];
    char *mensagem;
} sensor;


struct sensor* buscar_sensor(struct sensor *vet, int n, const char *id) {


    int i;

    struct sensor *psensor;
    
    for (i = 0; i < n; i++) {
        if(strcmp(vet[i].id, id) == 1){
            *psensor = vet[i];
            return psensor;
        }
    }

    return NULL;
    
}

float media_leituras(const struct sensor *s){
    
    int i;

    float media = 0;

    for (i = 0; i < sizeof(s); i++)
    {
        for (i = 0; i < sizeof(s->leituras); i++)
        {
            media += *s->leituras;
        }
    }
    

    return media / sizeof(s->leituras);
    

}

int definir_mensagem(struct sensor *s, const char *msg){


    //struct sensor *sen;
    char *pf;


    pf = (char *)malloc(strlen(msg)+1);


    strcpy(pf, s->mensagem);

    if(pf == NULL){
        return 0;
    } else{
        return 1;
    }



}


int main(int argc, char* argv[]){

    struct sensor sensores[] = {
        {"S1", {22.5, 23.0, 22.8, 23.1, 22.9}, NULL},
        {"S2", {30.1, 29.8, 30.0, 30.2, 29.9}, NULL},
        {"S3", {18.4, 18.6, 18.5, 18.7, 18.6}, NULL}
    };

    char *idSensor;

    char *msg;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            idSensor = (argv[i + 1]);
            i++;
        }

        if (strcmp(argv[i], "-m") == 2 && i + 1 < argc) {
            msg = argv[i + 1];
            i++;
        }
    }


    
    buscar_sensor(sensores, sizeof(sensores), idSensor);
    
    media_leituras(sensores);

    definir_mensagem(sensores, msg);
    
    return 0;
}