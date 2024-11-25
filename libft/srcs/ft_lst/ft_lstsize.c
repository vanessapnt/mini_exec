/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 14:11:41 by yilin             #+#    #+#             */
/*   Updated: 2024/05/23 15:01:18 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*
#include <stddef.h>
#include <stdlib.h>

typedef struct s_list
{
void *content;
struct s_list *next;
} t_list;
*/

int	ft_lstsize(t_list *lst)
{
	size_t	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}
/* 
t_list *ft_lstnew(void *content)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}
*/
/*
#include <stdio.h>
=================TEST MAIN=======================
int main()
{
	//add nodes into the linked list: 
	//head(NULL)->node1 -> node2 -> node3->end(NULL)
	
	int	list_len;
	t_list *temp;
	t_list *head = ft_lstnew(NULL);
	t_list *node1 = ft_lstnew("1st node content");
	t_list *node2 = ft_lstnew("2nd node content");
	t_list *node3 = ft_lstnew("3rd node content");
	t_list *node4 = ft_lstnew("4th node content");
	
	head = node1;
	node1->next = node2;
	node2->next = node3;
	node3->next = node4;
	
	//it's okay not to explicitly set the next pointer of the last node to NULL 
	//"ft_lstnew(void *content)" => already set end->"NULL", so no need set again!
	
	list_len = ft_lstsize(head);
	printf("List size: %i\n", list_len);
	
	// Free the list nodes to avoid memory leaks.
	//create a "temp" for "head" -> free one by one in order
	
	while (head)
	{
		temp = head;
		head = head->next;
		free(temp);
	}
	return (0);
}*/