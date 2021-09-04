#ifndef __MYCONTAINER_H
#define __MYCONTAINER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct file_node
{
    char *fname;
    struct file_node *next;
};

void clear_all_found_files(struct file_node *head)
{
    if (head == NULL)
        return;
    clear_all_found_files(head->next);
    if (head->fname != NULL)
    {
        free(head->fname);
    }
    free(head);
}

struct file_node *create_file_node(char *fname)
{
    struct file_node *current = (struct file_node *)malloc(sizeof(struct file_node));
    memset(current, 0, sizeof(struct file_node));
    current->fname = (char *)malloc(strlen(fname) + 5);
    strcpy(current->fname, fname);
    return current;
}

struct file_container
{
    struct file_node *head; // dunmmy head
    int size;
};

struct file_container *create_file_container()
{
    struct file_container *container = (struct file_container *)malloc(sizeof(struct file_container));
    memset(container, 0, sizeof(struct file_container));
    struct file_node *head = create_file_node("dummy");
    head->next = NULL;
    container->head = head;
    return container;
}

void free_file_container(struct file_container *container)
{
    clear_all_found_files(container->head);
    free(container);
}

void add_file_to_container(struct file_container *container, char *fname)
{
    struct file_node *current = create_file_node(fname);
    current->next = container->head->next;
    container->head->next = current;
    container->size++;
}

void print_file_counter(struct file_container *container)
{
    struct file_node *p = container->head->next;
    while (p != NULL)
    {
        printf("file name: %s\n", p->fname);
        p = p->next;
    }
}

#endif