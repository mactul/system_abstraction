#ifndef SA_QUEUE
    #define SA_QUEUE
    #include "SA/global/global.h"


    typedef struct _sa_queue SA_Queue;

    #ifdef __cplusplus
    extern "C"{
    #endif


    SA_Queue* SA_queue_create(uint32_t element_size, uint32_t array_size);
    SA_bool SA_queue_push(SA_Queue* queue, void *value_ptr);
    SA_bool SA_queue_pull(SA_Queue* queue, void *element, SA_bool wait_for_element);
    void SA_queue_free(SA_Queue** queue);

    #ifdef __cplusplus
    }
    #endif
#endif