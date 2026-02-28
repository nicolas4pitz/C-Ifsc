#include <stdio.h>
#include <string.h>

struct livro {
    char titulo[80];
    char autor[80];
    char editora[80];
    int ano;
};

struct livro acervo[] = {
    {"Fundamentos de Redes", "Tanenbaum", "Pearson", 2011},
    {"Sistemas Operacionais Modernos", "Tanenbaum", "Pearson", 2015},
    {"Código Limpo", "Robert Martin", "Alta Books", 2008},
    {"Engenharia de Software", "Sommerville", "Addison-Wesley", 2018}
};


struct livro* buscar_livro(struct livro *vet, int n, const char *titulo){
    int i;
    struct livro *ptr = vet;
    for (i = 0; i < n; i++)
    {
        if (strcmp(titulo, ptr->titulo))
        {
            return *ptr->titulo;
        }

        ptr++;

    }

    return NULL;
    
};



void atualizar_editora(struct livro *l, const char *nova_editora){

    int i;
    struct livro *ptr = l;

    strcpy(l->editora, nova_editora);
    
}

int main(int argc, char *argv[]){
    

    struct livro *book;
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            if (argv[1]){
                book = buscar_livro(acervo, 4, argv[1]);
            }

            if(argv[2]){
                atualizar_editora(book, argv[2]);
            }
        }
    }
    return 0;
    
}