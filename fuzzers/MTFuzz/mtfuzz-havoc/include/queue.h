#ifndef __MYQUEUE_H
#define __MYQUEUE_H

struct queue_entry
{
    char *fname;
    unsigned len;
    unsigned bitmap_size;
    unsigned long long exec_us, handicap, depth;
    unsigned execs;
    struct queue_entry *next;
};

struct queue
{
    struct queue_entry *head; // dummy head
    int size;
};

struct queue *create_queue()
{
    struct queue *queue = (struct queue *)malloc(sizeof(struct queue));
    memset(queue, 0, sizeof(struct queue));
    struct queue_entry *head = (struct queue_entry *)malloc(sizeof(struct queue_entry));
    head->next = NULL;
    queue->head = head;
    return queue;
}

void add_entry_to_queue(struct queue *queue, struct queue_entry *entry)
{
    entry->next = queue->head->next;
    queue->head->next = entry;
    queue->size++;
}

void print_queue(struct queue * _queue)
{
    struct queue_entry *p = _queue->head->next;
    while (p != NULL)
    {
        printf("queue entry: %s\n", p->fname);
        p = p->next;
    }
}


void print_queue_entry(struct queue_entry* entry) {
    printf("fname       = %s\n", entry->fname);
    printf("len         = %u\n", entry->len);
    printf("bitmap_size = %u\n", entry->bitmap_size);
    printf("exec_us     = %llu\n", entry->exec_us);
    printf("handicap    = %llu\n", entry->handicap);
    printf("depth       = %llu\n", entry->depth);
}

#endif