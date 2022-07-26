#include "../include/disco.h"

t_pedido_disco* crear_pedido_escribir(int id, int dir_marco, int num_pag)
{
    t_pedido_disco *p = malloc(sizeof(t_pedido_disco));
    p->operacion_disco = ESCRIBIR_ARCHIVO_SWAP;
    p->argumentos[0] = id;
    p->argumentos[1] = dir_marco;
    p->argumentos[2] = num_pag;
    sem_init(&(p->pedido_listo), 0, 0);

    pthread_mutex_lock(&mutex_cola_pedidos);
    queue_push(pedidos_disco, p);
    pthread_mutex_unlock(&mutex_cola_pedidos);

    sem_post(&lista_tiene_pedidos);
    return p;
}

t_pedido_disco* crear_pedido_lectura(int id, int dir_marco, int num_pag)
{
    t_pedido_disco *p = malloc(sizeof(t_pedido_disco));
    p->operacion_disco = LEER_ARCHIVO_SWAP;
    p->argumentos[0] = id;
    p->argumentos[1] = dir_marco;
    p->argumentos[2] = num_pag;
    sem_init(&(p->pedido_listo), 0, 0);

    pthread_mutex_lock(&mutex_cola_pedidos);
    queue_push(pedidos_disco, p);
    pthread_mutex_unlock(&mutex_cola_pedidos);

    sem_post(&lista_tiene_pedidos);
    return p;
}

void eliminar_pedido_disco(t_pedido_disco *p)
{
    // Ver de liberar los argumentos individualmente
    free(p);
}

t_pedido_disco* crear_pedido_crear_archivo(int id)
{
    t_pedido_disco *p = malloc(sizeof(t_pedido_disco));
    p->operacion_disco = CREAR_ARCHIVO_SWAP;
    p->argumentos[0] = id;
    sem_init(&(p->pedido_listo), 0,0);

    pthread_mutex_lock(&mutex_cola_pedidos);
    queue_push(pedidos_disco, p);
    pthread_mutex_unlock(&mutex_cola_pedidos);

    sem_post(&lista_tiene_pedidos);
    return p;
}
