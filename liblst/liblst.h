#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

/**
 * @brief A generic list structure.
 *
 * This struct represents a node in a linked list.
 */
typedef struct s_list {
	void *content;       /**< The content of the node. */
	size_t content_size; /**< The size of the content. */
	struct s_list *next; /**< The next node in the list. */
} list_t;

/**
 * @brief Creates a new list node.
 *
 * @param content The content of the node.
 * @param content_size The size of the content.
 * @return The new list node.
 */
list_t *ft_lstnew(void const *content, size_t content_size);
/**
 * @brief Creates a new list node without duplicating content.
 *
 * @param content The content of the node.
 * @param content_size The size of the content.
 * @return The new list node.
 */
list_t *ft_lstnew_nodup(void *content, size_t content_size);
/**
 * @brief Gets the size of the list.
 *
 * @param lst The beginning of the list.
 * @return The size of the list.
 */
size_t ft_lstsize(const list_t *lst);
/**
 * @brief Deletes a single node from the list.
 *
 * @param alst A pointer to the list.
 * @param del The function to delete the content.
 */
void ft_lstdelone(list_t **alst, void (*del)(void *, size_t));
/**
 * @brief Deletes the entire list.
 *
 * @param alst A pointer to the list.
 * @param del The function to delete the content.
 */
void ft_lstdel(list_t **alst, void (*del)(void *, size_t));
/**
 * @brief Adds a node to the front of the list.
 *
 * @param alst A pointer to the list.
 * @param new The node to add.
 */
void ft_lstadd(list_t **alst, list_t *new);
/**
 * @brief Adds a node to the back of the list.
 *
 * @param lst A pointer to the list.
 * @param new The node to add.
 */
void ft_lstpush(list_t **lst, list_t *new);
/**
 * @brief Gets a node at a specific index.
 *
 * @param lst The beginning of the list.
 * @param index The index of the node to get.
 * @return The node at the specified index.
 */
list_t *ft_lstget(list_t *lst, const size_t index);
/**
 * @brief Gets the last node of the list.
 *
 * @param lst The beginning of the list.
 * @return The last node of the list.
 */
list_t *ft_lstback(list_t *lst);
/**
 * @brief Iterates over the list and applies a function.
 *
 * @param lst The beginning of the list.
 * @param f The function to apply to each node.
 */
void ft_lstiter(list_t *lst, void (*f)(list_t *elem));
/**
 * @brief Creates a new list by applying a function to each node.
 *
 * @param lst The beginning of the list.
 * @param f The function to apply to each node.
 * @return The new list.
 */
list_t *ft_lstmap(list_t *lst, list_t *(*f)(list_t *elem));
/**
 * @brief Reverses the list.
 *
 * @param lst A pointer to the list.
 */
void ft_lstrev(list_t **lst);
/**
 * @brief The default delete function.
 *
 * @param content The content to delete.
 * @param content_size The size of the content.
 */
void default_del(void *content, size_t content_size);

#endif
