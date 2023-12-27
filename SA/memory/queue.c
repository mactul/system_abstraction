#include <semaphore.h>
#include <pthread.h>
#include "SA/memory/mem_alloc.h"
#include "SA/strings/strings.h"
#include "SA/memory/queue.h"

struct _sa_queue {
    SA_byte* array;
    uint32_t queue_input;
    uint32_t queue_output;
    uint32_t element_size;
    uint32_t array_size;
    sem_t queue_sem;
    pthread_mutex_t mutex;
};

SA_Queue* SA_queue_create(uint32_t element_size, uint32_t array_size)
{
    SA_Queue* queue = SA_malloc(sizeof(SA_Queue));
    if(queue == NULL)
    {
        return NULL;
    }

    queue->array = SA_malloc(element_size * array_size);
    if(queue->array == NULL)
    {
        SA_free(&queue);
        return NULL;
    }
    queue->array_size = array_size;
    queue->element_size = element_size;
    queue->queue_input = 0;
    queue->queue_output = 0;
    sem_init(&(queue->queue_sem), 0, 0);
    pthread_mutex_init(&(queue->mutex), NULL);

    return queue;
}

SA_bool SA_queue_push(SA_Queue* queue, void *value_ptr)
{
    uint32_t index;
    SA_bool ret = SA_TRUE;

    pthread_mutex_lock(&(queue->mutex));
    index = (queue->queue_input + 1) % queue->array_size;
    if(index == queue->queue_output)
    {
        ret = SA_FALSE;
        goto UNLOCK;
    }
    SA_memcpy(queue->array + queue->queue_input * queue->element_size, value_ptr, queue->element_size);
    queue->queue_input = index;
    sem_post(&(queue->queue_sem));

UNLOCK:
    pthread_mutex_unlock(&(queue->mutex));
    return ret;
}

SA_bool SA_queue_pull(SA_Queue* queue, void *element, SA_bool wait_for_element)
{
    if(wait_for_element)
    {
        sem_wait(&(queue->queue_sem));
    }
    if(queue->queue_output == queue->queue_input)
    {
        return SA_FALSE;
    }
    if(!wait_for_element)
    {
        sem_wait(&(queue->queue_sem));  // This will decrease the semaphore, but in theory will not block.
    }

    pthread_mutex_lock(&(queue->mutex));
    SA_memcpy(element, queue->array + queue->queue_output * queue->element_size, queue->element_size);
    queue->queue_output = (queue->queue_output + 1) % queue->array_size;
    pthread_mutex_unlock(&(queue->mutex));

    return SA_TRUE;
}

void SA_queue_free(SA_Queue** queue)
{
    if(*queue == NULL)
    {
        return;
    }
    SA_free(&((*queue)->array));
    sem_destroy(&((*queue)->queue_sem));
    pthread_mutex_destroy(&((*queue)->mutex));
    SA_free(queue);
}