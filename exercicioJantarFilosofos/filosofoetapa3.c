/*
 * Jantar dos Filósofos — Etapa 3: Prevenção por Posse Total (Saleiro)
 *
 * Compilar:  gcc -o etapa3 etapa3_saleiro.c -lpthread -Wall -Wextra
 * Executar:  ./etapa3
 *
 * Estratégia: antes de pegar qualquer palito, o filósofo deve obter
 * o "saleiro" — um mutex global. Só com o saleiro em mãos ele tenta
 * pegar os dois palitos. Após comer, libera tudo.
 *
 * Isso elimina a condição de POSSE E ESPERA: um filósofo nunca
 * segura um palito enquanto espera outro, pois só começa a pegar
 * palitos quando já tem o saleiro (garantia de acesso exclusivo
 * à tentativa de montagem do conjunto completo de recursos).
 *
 * Consequência: no máximo 1 filósofo por vez tenta adquirir palitos,
 * tornando deadlock impossível.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_FILOSOFOS  5
#define DURACAO_SEG    30

pthread_mutex_t palitos[NUM_FILOSOFOS];

/*
 * ★ SALEIRO: mutex que controla quem tem permissão de
 *   tentar montar o conjunto completo de recursos.
 *   Somente um filósofo por vez pode tentar pegar palitos.
 */
pthread_mutex_t saleiro;

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

    while (rodando) {

        /* Pensar */
        log_msg(id, "pensando");
        usleep((100 + rand() % 400) * 1000);
        if (!rodando) break;

        /*
         * ★ Pega o SALEIRO antes de qualquer palito.
         *   Enquanto este filósofo não liberar o saleiro,
         *   nenhum outro pode nem tentar pegar palitos.
         *   → Elimina "posse e espera": ou pega tudo, ou não pega nada.
         */
        log_msg(id, "aguardando o saleiro...");
        pthread_mutex_lock(&saleiro);
        log_msg(id, "pegou o saleiro - montando recursos");

        if (!rodando) { pthread_mutex_unlock(&saleiro); break; }

        /* Com o saleiro, pega os dois palitos sem risco de deadlock */
        pthread_mutex_lock(&palitos[esq]);
        snprintf(buf, sizeof buf, "pegou p%d (esquerdo)", esq);
        log_msg(id, buf);

        pthread_mutex_lock(&palitos[dir]);
        snprintf(buf, sizeof buf, "pegou p%d (direito) - comendo!", dir);
        log_msg(id, buf);

        /* Comer */
        usleep((100 + rand() % 200) * 1000);
        refeicoes[id]++;

        /* Libera palitos, depois o saleiro */
        pthread_mutex_unlock(&palitos[dir]);
        pthread_mutex_unlock(&palitos[esq]);
        pthread_mutex_unlock(&saleiro);

        snprintf(buf, sizeof buf,
                 "liberou p%d, p%d e saleiro | refeicoes: %ld",
                 esq, dir, refeicoes[id]);
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
    pthread_mutex_init(&saleiro, NULL);

    printf("=== Etapa 3: Saleiro (Posse Total) — sem deadlock ===\n");
    printf("Somente 1 filosofo por vez tenta adquirir palitos.\n");
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
    printf("  Condicao de Coffman eliminada: POSSE E ESPERA\n");

    for (int i = 0; i < NUM_FILOSOFOS; i++)
        pthread_mutex_destroy(&palitos[i]);
    pthread_mutex_destroy(&saleiro);

    return 0;
}