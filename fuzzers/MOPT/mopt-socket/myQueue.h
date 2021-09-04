#ifndef __MYQUEUE_H
#define __MYQUEUE_H

#include <malloc.h>
#include <stdio.h>
#include <string.h>
struct qentry {
  char *fname;
  struct qentry *next;
};

struct queue {
  struct qentry *head;  // dummy head
  struct qentry *tail;
  int size;
};

struct queue *create_queue() {
  struct queue *queue = (struct queue *)malloc(sizeof(struct queue));
  memset(queue, 0, sizeof(struct queue));
  struct qentry *head = (struct qentry *)malloc(sizeof(struct qentry));
  head->next = NULL;
  queue->head = head;
  queue->tail = head;
  queue->size = 0;
  return queue;
}

void add_entry_to_queue(struct queue *queue, struct qentry *entry) {
  queue->tail->next = entry;
  queue->tail = entry;
  queue->size++;
}

struct queue *renew_queue(struct queue *queue) {
  struct qentry *tmp = queue->head->next;
  while (tmp != NULL) {
    struct qentry *nxt = tmp->next;
    free(tmp->fname);
    free(tmp);
    tmp = nxt;
  }
  free(queue->head);
  free(queue);
  return create_queue();
}

void print_queue(struct queue *_queue) {
  struct qentry *p = _queue->head->next;
  while (p != NULL) {
    printf("queue entry: %s\n", p->fname);
    p = p->next;
  }
}

void print_queue_entry(struct qentry *entry) {
  printf("fname = %s\n", entry->fname);
}

#endif