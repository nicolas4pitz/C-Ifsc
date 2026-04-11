/*
 * mini_pipeline.c
 * Equivalente ao comando: ls | wc -l
 *
 * Fluxo:
 *   1. Cria um pipe
 *   2. Fork -> Filho 1: redireciona stdout para fd[1] e executa "ls"
 *   3. Fork -> Filho 2: redireciona stdin para fd[0] e executa "wc -l"
 *   4. Pai fecha as pontas do pipe e aguarda os filhos
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int fd[2];   /* fd[0] = leitura, fd[1] = escrita */
    pid_t pid1, pid2;

    /* 1. Cria o pipe ---------------------------------------------------- */
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    /* 2. Filho 1: executa "ls" ------------------------------------------ */
    pid1 = fork();
    if (pid1 == -1) {
        perror("fork filho 1");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {                  /* --- código do filho 1 --- */
        close(fd[0]);                 /* não usa a ponta de leitura */

        /* redireciona stdout para a ponta de escrita do pipe */
        if (dup2(fd[1], STDOUT_FILENO) == -1) {
            perror("dup2 filho 1");
            exit(EXIT_FAILURE);
        }
        close(fd[1]);                 /* fd[1] já duplicado, pode fechar */

        /* substitui a imagem do processo por "ls" */
        execlp("ls", "ls", NULL);

        /* só chega aqui se execlp falhar */
        perror("execlp ls");
        exit(EXIT_FAILURE);
    }

    /* 3. Filho 2: executa "wc -l" --------------------------------------- */
    pid2 = fork();
    if (pid2 == -1) {
        perror("fork filho 2");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {                  /* --- código do filho 2 --- */
        close(fd[1]);                 /* não usa a ponta de escrita */

        /* redireciona stdin para a ponta de leitura do pipe */
        if (dup2(fd[0], STDIN_FILENO) == -1) {
            perror("dup2 filho 2");
            exit(EXIT_FAILURE);
        }
        close(fd[0]);                 /* fd[0] já duplicado, pode fechar */

        /* substitui a imagem do processo por "wc -l" */
        execlp("wc", "wc", "-l", NULL);

        /* só chega aqui se execlp falhar */
        perror("execlp wc");
        exit(EXIT_FAILURE);
    }

    /* 4. Pai: fecha ambas as pontas e espera os filhos ------------------ */
    /*
     * IMPORTANTE: o pai DEVE fechar fd[1] antes de wait().
     * Se não fechar, "wc -l" nunca recebe EOF e bloqueia para sempre,
     * pois ainda há um descritor de escrita aberto (o do pai).
     */
    close(fd[0]);
    close(fd[1]);

    /* aguarda os dois filhos terminarem (qualquer ordem) */
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return EXIT_SUCCESS;
}