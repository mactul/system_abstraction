#include <semaphore.h>
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

    return queue;
}

SA_bool SA_queue_push(SA_Queue* queue, void *value_ptr)
{
    uint32_t index = (queue->queue_input + 1) % queue->array_size;
    if(index == queue->queue_output)
    {
        return SA_FALSE;
    }
    SA_memcpy(queue->array + queue->queue_input * queue->element_size, value_ptr, queue->element_size);
    queue->queue_input = index;
    sem_post(&(queue->queue_sem));

    return SA_TRUE;
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
    SA_memcpy(element, queue->array + queue->queue_output * queue->element_size, queue->element_size);
    queue->queue_output = (queue->queue_output + 1) % queue->array_size;

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
    SA_free(queue);
}