/*
 * Jantar dos Filósofos — Etapa 2: Prevenção por Ordenação Global
 *
 * Compilar:  gcc -o etapa2 etapa2_ordenacao.c -lpthread -Wall -Wextra
 * Executar:  ./etapa2
 *
 * Estratégia: cada filósofo adquire os dois palitos em ordem crescente
 * de índice. Isso quebra a ESPERA CIRCULAR, a 4ª condição de Coffman.
 *
 * Exemplo crítico — Filósofo 4 (palitos p4 e p0):
 *   Ordem natural seria p4 → p0, mas p0 < p4, então pega p0 → p4.
 *   Isso evita que ele "feche o ciclo" da espera circular.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_FILOSOFOS  5
#define DURACAO_SEG    30   /* tempo total de execução */

pthread_mutex_t palitos[NUM_FILOSOFOS];

/* Contadores de refeições por filósofo (para estatística) */
long refeicoes[NUM_FILOSOFOS];

volatile int rodando = 1;
static long  t0_ms   = 0;

long ms_agora(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000L + ts.tv_nsec / 1000000L) - t0_ms;
}

void log_msg(int id, const char *msg)
{
    printf("[%6ld ms] Filosofo %d %s\n", ms_agora(), id, msg);
    fflush(stdout);
}

/* ------------------------------------------------------------------ */
void *filosofo(void *arg)
{
    int id  = *(int *)arg;
    int esq = id;
    int dir = (id + 1) % NUM_FILOSOFOS;
    char buf[80];

    /*
     * ★ ORDENAÇÃO GLOBAL: sempre pega o palito de menor índice primeiro.
     *   Garante que nunca haverá ciclo no grafo de espera.
     */
    int primeiro  = (esq < dir) ? esq : dir;
    int segundo   = (esq < dir) ? dir : esq;

    snprintf(buf, sizeof buf,
             "iniciado | palitos: p%d e p%d | ordem de aquisicao: p%d -> p%d",
             esq, dir, primeiro, segundo);
    log_msg(id, buf);

    while (rodando) {

        /* Pensar */
        log_msg(id, "pensando");
        usleep((100 + rand() % 400) * 1000);
        if (!rodando) break;

        /* Pega palito de MENOR índice primeiro */
        snprintf(buf, sizeof buf, "pegando p%d (1o)", primeiro);
        log_msg(id, buf);
        pthread_mutex_lock(&palitos[primeiro]);
        snprintf(buf, sizeof buf, "pegou p%d", primeiro);
        log_msg(id, buf);

        if (!rodando) { pthread_mutex_unlock(&palitos[primeiro]); break; }

        /* Pega palito de MAIOR índice depois */
        snprintf(buf, sizeof buf, "pegando p%d (2o)", segundo);
        log_msg(id, buf);
        pthread_mutex_lock(&palitos[segundo]);
        snprintf(buf, sizeof buf, "pegou p%d - comendo!", segundo);
        log_msg(id, buf);

        /* Comer */
        usleep((100 + rand() % 200) * 1000);
        refeicoes[id]++;

        /* Libera (ordem inversa da aquisição, boa prática) */
        pthread_mutex_unlock(&palitos[segundo]);
        pthread_mutex_unlock(&palitos[primeiro]);

        snprintf(buf, sizeof buf,
                 "liberou p%d e p%d | refeicoes: %ld",
                 primeiro, segundo, refeicoes[id]);
        log_msg(id, buf);
    }

    return NULL;
}

/* ------------------------------------------------------------------ */
int main(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    t0_ms = ts.tv_sec * 1000L + ts.tv_nsec / 1000000L;

    pthread_t threads[NUM_FILOSOFOS];
    int ids[NUM_FILOSOFOS];

    srand((unsigned)time(NULL));

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_init(&palitos[i], NULL);
        refeicoes[i] = 0;
    }

    printf("=== Etapa 2: Ordenacao Global — sem deadlock ===\n");
    printf("Filosofo 4 (caso especial): palitos p4 e p0 -> adquire p0 -> p4\n");
    printf("Rodando por %d segundos...\n\n", DURACAO_SEG);
    fflush(stdout);

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, filosofo, &ids[i]);
    }

    sleep(DURACAO_SEG);
    rodando = 0;

    for (int i = 0; i < NUM_FILOSOFOS; i++)
        pthread_join(threads[i], NULL);

    printf("\n=== Resultado apos %d segundos ===\n", DURACAO_SEG);
    long total = 0;
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        printf("  Filosofo %d comeu %ld vezes\n", i, refeicoes[i]);
        total += refeicoes[i];
    }
    printf("  Total de refeicoes: %ld\n", total);
    printf("  Condicao de Coffman eliminada: ESPERA CIRCULAR\n");

    for (int i = 0; i < NUM_FILOSOFOS; i++)
        pthread_mutex_destroy(&palitos[i]);

    return 0;
}