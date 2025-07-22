#ifndef LIST_H
#define LIST_H

# include <stdlib.h>

typedef struct	s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*next;
}				t_list;

t_list			*ft_lstnew(void const *content, size_t content_size);
t_list			*ft_lstnew_nodup(void *content, size_t content_size);
size_t			ft_lstsize(const t_list *lst);
void			ft_lstdelone(t_list **alst, void (*del)(void*, size_t));
void			ft_lstdel(t_list **alst, void (*del)(void*, size_t));
void			ft_lstadd(t_list **alst, t_list *new);
void			ft_lstpush(t_list **lst, t_list *new);
t_list			*ft_lstget(t_list *lst, const size_t index);
t_list			*ft_lstback(t_list *lst);
void			ft_lstiter(t_list *lst, void (*f)(t_list *elem));
t_list			*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem));
void			ft_lstrev(t_list **lst);
void			default_del(void *content, size_t content_size);

#endif
