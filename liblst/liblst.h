#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct s_list {
    void *content;
    size_t content_size;
    struct s_list *next;
} list_t;

list_t *ft_lstnew(void const *content, size_t content_size);
list_t *ft_lstnew_nodup(void *content, size_t content_size);
size_t ft_lstsize(const list_t *lst);
void ft_lstdelone(list_t **alst, void (*del)(void *, size_t));
void ft_lstdel(list_t **alst, void (*del)(void *, size_t));
void ft_lstadd(list_t **alst, list_t *new);
void ft_lstpush(list_t **lst, list_t *new);
list_t *ft_lstget(list_t *lst, const size_t index);
list_t *ft_lstback(list_t *lst);
void ft_lstiter(list_t *lst, void (*f)(list_t *elem));
list_t *ft_lstmap(list_t *lst, list_t *(*f)(list_t *elem));
void ft_lstrev(list_t **lst);
void default_del(void *content, size_t content_size);

#endif
