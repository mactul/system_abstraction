#ifndef SA_QUEUE
    #define SA_QUEUE
    #include "SA/global/global.h"


    typedef struct _sa_queue SA_Queue;

    #ifdef __cplusplus
    extern "C"{
    #endif

    /**
     * @brief Create a new queue that can contains `array_size` elements of size `element_size`
     * 
     * @param element_size The size of one element of the queue in byte. sizeof(int) for a queue of integers for example.
     * @param array_size The maximum number of elements that can be stored in the queue. will determine the size of the queue in memory.
     * @return A handler to a SA_Queue.
     */
    SA_Queue* SA_queue_create(uint32_t element_size, uint32_t array_size);

    /**
     * @brief Copy the element under value_ptr to the queue if the queue isn't full.
     * 
     * @param queue The handler returned by `SA_queue_create`
     * @param value_ptr The address of the value to add. It will often be a pointer to a structure.
     * @return - SA_TRUE if the element was inserted.
     * - SA_FALSE if the queue is full.
     */
    SA_bool SA_queue_push(SA_Queue* queue, void *value_ptr);

    /**
     * @brief Copy the oldest element of the queue in the memory under `element`.  
     * @brief It can wait for the queue to have at least one element.
     * 
     * @param queue The handler returned by `SA_queue_create`
     * @param element A pointer to the memory to fill.
     * @param wait_for_element If true, the function will block until the queue have at least one element.
     * @return - SA_TRUE if the element was set.
     * - SA_FALSE if there is nothing in the queue (only in non-blocking mode with `wait_for_element = SA_FALSE`).
     */
    SA_bool SA_queue_pull(SA_Queue* queue, void *element, SA_bool wait_for_element);

    /**
     * @brief Free the underlying SA_Queue behind the pointer queue and set the SA_Queue handler to NULL.
     * 
     * @param queue The address of the handler returned by `SA_queue_create`.
     */
    void SA_queue_free(SA_Queue** queue);

    #ifdef __cplusplus
    }
    #endif
#endif