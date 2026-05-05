/*
 * Jantar dos Filósofos — Etapa 1: Produzindo o Impasse (Deadlock)
 *
 * Compilar:  gcc -o etapa1 etapa1_deadlock.c -lpthread -Wall -Wextra
 * Executar:  ./etapa1
 *
 * Estratégia para forçar deadlock de forma confiável:
 *   1. pthread_barrier_t sincroniza todos os filósofos antes de pegarem
 *      qualquer palito — todos partem ao mesmo instante.
 *   2. Atraso de 200 ms após pegar o palito esquerdo garante que todos
 *      os 5 segurem um palito antes de qualquer um tentar o direito.
 *   3. Resultado: espera circular inevitável na primeira rodada.
 *
 * Condições de Coffman presentes:
 *   1. Exclusão mútua  – pthread_mutex_t por palito
 *   2. Posse e espera  – segura esq enquanto espera dir
 *   3. Não preempção   – ninguém toma palito à força
 *   4. Espera circular – 0→p1, 1→p2, 2→p3, 3→p4, 4→p0
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_FILOSOFOS 5
#define ATRASO_ENTRE_PALITOS_MS 200  /* ms entre pegar esq e tentar dir */

/* Palitos: um mutex por recurso */
pthread_mutex_t palitos[NUM_FILOSOFOS];

/*
 * ★ BARREIRA DE SINCRONIZAÇÃO
 * Garante que todos os 5 filósofos cheguem ao ponto de pegar
 * o palito esquerdo ao mesmo tempo, tornando o deadlock inevitável.
 */
pthread_barrier_t barreira;

static long t0_ms = 0;

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

    log_msg(id, "pronto, aguardando os outros...");

    /*
     * ★ Todos os filósofos ficam aqui até o último chegar.
     *   Só então todos avançam juntos para pegar o palito esquerdo.
     */
    pthread_barrier_wait(&barreira);

    /* ── Pega palito ESQUERDO ── */
    snprintf(buf, sizeof buf, "pegou p%d", esq);
    pthread_mutex_lock(&palitos[esq]);
    log_msg(id, buf);

    /*
     * ★ Atraso longo após segurar o palito esquerdo.
     *   Com 200 ms aqui, todos os 5 certamente seguram seu palito
     *   esquerdo antes de qualquer um tentar o direito.
     */
    usleep(ATRASO_ENTRE_PALITOS_MS * 1000);

    /* ── Tenta pegar palito DIREITO — ponto de deadlock ── */
    snprintf(buf, sizeof buf, "esperando p%d", dir);
    log_msg(id, buf);

    pthread_mutex_lock(&palitos[dir]);  /* ← nunca retorna (deadlock) */

    /* Código abaixo jamais é atingido no deadlock */
    log_msg(id, "ERRO: nao deveria chegar aqui!");
    pthread_mutex_unlock(&palitos[dir]);
    pthread_mutex_unlock(&palitos[esq]);

    return NULL;
}

/* ------------------------------------------------------------------ */
int main(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    t0_ms = ts.tv_sec * 1000L + ts.tv_nsec / 1000000L;

    pthread_t threads[NUM_FILOSOFOS];
    int       ids[NUM_FILOSOFOS];

    /* Inicializa palitos */
    for (int i = 0; i < NUM_FILOSOFOS; i++)
        pthread_mutex_init(&palitos[i], NULL);

    /* Inicializa barreira para NUM_FILOSOFOS threads */
    pthread_barrier_init(&barreira, NULL, NUM_FILOSOFOS);

    printf("=== Jantar dos Filosofos - Etapa 1: Deadlock Garantido ===\n");
    printf("Barreira sincroniza todos antes de pegar qualquer palito.\n");
    printf("%d ms de atraso apos o palito esquerdo -> espera circular.\n\n",
           ATRASO_ENTRE_PALITOS_MS);
    fflush(stdout);

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, filosofo, &ids[i]);
    }

    /*
     * Aguarda alguns segundos para o deadlock se estabelecer,
     * depois imprime diagnóstico e encerra.
     * (As threads bloqueadas são destruídas com o processo.)
     */
    sleep(3);

    printf("\n[monitor] 3s sem progresso = DEADLOCK confirmado.\n");
    printf("[monitor] Cada filosofo segura um palito e espera o proximo.\n");
    printf("[monitor] Espera circular: 0->p1, 1->p2, 2->p3, 3->p4, 4->p0\n");

    pthread_barrier_destroy(&barreira);
    for (int i = 0; i < NUM_FILOSOFOS; i++)
        pthread_mutex_destroy(&palitos[i]);

    return 0;
}