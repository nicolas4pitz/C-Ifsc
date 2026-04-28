/*
 * Opção 3 – Escritor e múltiplos leitores
 * Disciplina: Sistemas Operacionais – IFSC
 *
 * Compilação:
 *   gcc opcao3_leitores_escritor.c -o opcao3 -pthread
 *
 * Descrição dos mecanismos usados:
 *   - mutex_contador : protege o acesso ao contador de leitores ativos.
 *   - mutex_recurso  : garante acesso exclusivo do escritor ao recurso.
 *                      O PRIMEIRO leitor o trava; o ÚLTIMO o libera.
 *                      Leitores intermediários não precisam tocá-lo.
 *   - Dessa forma múltiplos leitores coexistem, mas o escritor sempre
 *     fica sozinho na região crítica.
 *
 * Estrutura do programa:
 *   - 1 thread escritora  (faz NUM_ESCRITAS escritas)
 *   - 4 threads leitoras  (cada uma faz NUM_LEITURAS leituras)
 *   - 1 variável compartilhada inteira: dado_compartilhado
 *   - 1 contador inteiro:               num_leitores
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/* ── Parâmetros da simulação ─────────────────────────────────── */
#define NUM_LEITORES   4
#define NUM_LEITURAS   5   /* cada leitor lê esta quantidade de vezes  */
#define NUM_ESCRITAS   4   /* o escritor escreve esta quantidade de vezes */

/* ── Variável e contador compartilhados ─────────────────────── */
int dado_compartilhado = 0;   /* recurso que será lido e escrito         */
int num_leitores       = 0;   /* quantos leitores estão ativos agora     */

/* ── Dois mutexes ────────────────────────────────────────────── */
pthread_mutex_t mutex_contador;  /* protege num_leitores                 */
pthread_mutex_t mutex_recurso;   /* protege dado_compartilhado vs escrita */

/* ────────────────────────────────────────────────────────────── *
 *  THREAD LEITORA                                                *
 * ────────────────────────────────────────────────────────────── */
void *leitor(void *arg)
{
    int id = *(int *)arg;

    for (int i = 0; i < NUM_LEITURAS; i++) {

        /* ── Protocolo de ENTRADA ──────────────────────────────
         * Incrementa o contador de leitores com exclusão mútua.
         * Se for o PRIMEIRO leitor, trava o recurso para impedir
         * que um escritor entre enquanto há leitura em andamento. */
        pthread_mutex_lock(&mutex_contador);
        num_leitores++;
        if (num_leitores == 1) {
            /* Primeiro leitor: bloqueia o escritor */
            pthread_mutex_lock(&mutex_recurso);
            printf("[Leitor  %d] PRIMEIRO leitor – escritor BLOQUEADO."
                   " Leitores ativos: %d\n", id, num_leitores);
        } else {
            printf("[Leitor  %d] Entrou na leitura."
                   " Leitores ativos: %d\n", id, num_leitores);
        }
        pthread_mutex_unlock(&mutex_contador);

        /* ── Região crítica de LEITURA ─────────────────────────
         * Vários leitores podem estar aqui simultaneamente.       */
        printf("[Leitor  %d] Lendo dado = %d\n", id, dado_compartilhado);
        usleep(150000 + rand() % 200000);   /* simula tempo de leitura  */

        /* ── Protocolo de SAÍDA ────────────────────────────────
         * Decrementa o contador. Se for o ÚLTIMO leitor, libera
         * o recurso para que o escritor possa entrar.             */
        pthread_mutex_lock(&mutex_contador);
        num_leitores--;
        if (num_leitores == 0) {
            /* Último leitor: libera o escritor */
            pthread_mutex_unlock(&mutex_recurso);
            printf("[Leitor  %d] ULTIMO leitor – escritor LIBERADO."
                   " Leitores ativos: %d\n", id, num_leitores);
        } else {
            printf("[Leitor  %d] Saiu da leitura."
                   " Leitores ativos: %d\n", id, num_leitores);
        }
        pthread_mutex_unlock(&mutex_contador);

        usleep(100000 + rand() % 300000);   /* pausa entre leituras      */
    }

    printf("[Leitor  %d] Encerrado apos %d leituras.\n", id, NUM_LEITURAS);
    return NULL;
}

/* ────────────────────────────────────────────────────────────── *
 *  THREAD ESCRITORA                                              *
 * ────────────────────────────────────────────────────────────── */
void *escritor(void *arg)
{
    (void)arg;

    for (int i = 0; i < NUM_ESCRITAS; i++) {

        /* ── Protocolo de ENTRADA ──────────────────────────────
         * Tenta adquirir o mutex_recurso. Se há leitores ativos,
         * o mutex já está travado pelo primeiro leitor: o escritor
         * ficará bloqueado AQUI (sem espera ocupada) até o último
         * leitor liberá-lo.                                       */
        printf("[Escritor] Quer escrever – aguardando acesso exclusivo...\n");
        pthread_mutex_lock(&mutex_recurso);

        /* ── Região crítica de ESCRITA ─────────────────────────
         * Apenas um escritor por vez; nenhum leitor ativo.        */
        dado_compartilhado++;
        printf("[Escritor] >> ENTROU na regiao critica. Escreveu dado = %d\n",
               dado_compartilhado);
        usleep(250000 + rand() % 200000);   /* simula tempo de escrita  */

        /* ── Protocolo de SAÍDA ────────────────────────────────*/
        printf("[Escritor] << SAIU da regiao critica.\n");
        pthread_mutex_unlock(&mutex_recurso);

        usleep(200000 + rand() % 400000);   /* pausa entre escritas      */
    }

    printf("[Escritor] Encerrado apos %d escritas.\n", NUM_ESCRITAS);
    return NULL;
}

/* ────────────────────────────────────────────────────────────── *
 *  MAIN                                                          *
 * ────────────────────────────────────────────────────────────── */
int main(void)
{
    pthread_t t_escritor;
    pthread_t t_leitores[NUM_LEITORES];
    int       ids[NUM_LEITORES];

    srand(42);

    /* Inicializa os dois mutexes */
    pthread_mutex_init(&mutex_contador, NULL);
    pthread_mutex_init(&mutex_recurso,  NULL);

    printf("=== Opcao 3: Escritor e multiplos leitores ===\n");
    printf("Leitores: %d | Leituras por leitor: %d | Escritas: %d\n\n",
           NUM_LEITORES, NUM_LEITURAS, NUM_ESCRITAS);

    /* Cria a thread escritora */
    pthread_create(&t_escritor, NULL, escritor, NULL);

    /* Pequena pausa para o escritor tentar entrar primeiro (demonstra bloqueio) */
    usleep(50000);

    /* Cria as threads leitoras */
    for (int i = 0; i < NUM_LEITORES; i++) {
        ids[i] = i + 1;
        pthread_create(&t_leitores[i], NULL, leitor, &ids[i]);
    }

    /* Aguarda todas as threads terminarem */
    pthread_join(t_escritor, NULL);
    for (int i = 0; i < NUM_LEITORES; i++)
        pthread_join(t_leitores[i], NULL);

    /* Destroi os mutexes */
    pthread_mutex_destroy(&mutex_contador);
    pthread_mutex_destroy(&mutex_recurso);

    printf("\n=== Programa encerrado corretamente. Valor final: %d ===\n",
           dado_compartilhado);
    return 0;
}
