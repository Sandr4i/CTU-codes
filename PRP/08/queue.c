#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "queue.h"

queue_t* create_queue(int capacity)
{
   queue_t *q = malloc(sizeof(queue_t));
   if (!q)
   {
      fprintf(stderr, "Malloc ERROR");
      return NULL;
   }
   q->capacity = capacity;
   q->data = malloc(sizeof(void*)*(capacity));
   q->start = q->end = q->len = 0;
   return q;
}

void delete_queue(queue_t *queue)
{
   free(queue->data);
   free(queue);
}

bool push_to_queue(queue_t *queue, void *data)
{
   if (queue->len!= queue->capacity)
   {
      queue->data[queue->end] = data;
      queue->end=(queue->end+1)%queue->capacity;
      queue->len++;
      return true;
   }
   else return false;
}

void* pop_from_queue(queue_t *queue)
{
   if (queue->len!=0)
   {
      void *el = queue->data[queue->start];
      queue->start=(queue->start+1)%queue->capacity;
      queue->len--;
      return el;
   }
   else return NULL;
}

void* get_from_queue(queue_t *queue, int idx)
{
   if (queue->len != 0 && (idx<queue->len))
   {
      return queue->data[idx];
   }
   else return NULL;
}

int get_queue_size(queue_t *queue)
{
   return queue->len;
}
