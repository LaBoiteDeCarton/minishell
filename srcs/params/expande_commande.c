#include "minishell.h"
#include "ast.h"
static int	itoasize(int n)
{
	int	size;

	if (n < 0)
		size = 2;
	else
		size = 1;
	while (n / 10)
	{
		n /= 10;
		size++;
	}
	return (size);
}

static int		first_expanded_char_size(char *str)
{
	int		expanded_size;
	int		dquote_open;
	int		squote_open;

	expanded_size = 0;
	dquote_open = 0;
	squote_open = 0;
	while (*str)
	{
		if (*str == '\"' && !squote_open)
			dquote_open = (dquote_open + 1) % 2;
		if (*str == '\'' && !dquote_open)
			squote_open = (squote_open + 1) % 2;
		if (!squote_open && !ft_strncmp(str, "$?", 2))
		{
			expanded_size += itoasize(cenv.exit_status);
			str++;
		}
		else if (*str == '$' && !squote_open)
		{
			expanded_size += ft_strlen(get_value(str + 1));
			str += var_name_size(str + 1);
		}
		else
			expanded_size++;
		str++;
	}
	return (expanded_size + 1);
}

char *first_expand(char *str)
{
	char	*expanded_char;
	int		i;
	int		dquote_open;
	int		squote_open;
	char	*itoachar;

	dquote_open = 0;
	squote_open = 0;
	expanded_char = (char *)malloc(sizeof(char) * first_expanded_char_size(str));
	if (!expanded_char)
	{
		handle_errors("expande failed: ");
		return (NULL);
	}
	i = 0;
	while (*str)
	{
		if (*str == '\"' && !squote_open)
			dquote_open = (dquote_open + 1) % 2;
		if (*str == '\'' && !dquote_open)
			squote_open = (squote_open + 1) % 2;
		if (!squote_open && !ft_strncmp(str, "$?", 2))
		{
			str++;
			itoachar = get_value(str);
			ft_memcpy(expanded_char + i, itoachar, ft_strlen(itoachar));
			i += ft_strlen(itoachar);
			free(itoachar);
		}
		else if (*str == '$' && !squote_open)
		{
			str++;
			ft_memcpy(expanded_char + i, get_value(str), ft_strlen(get_value(str)));
			i += ft_strlen(get_value(str));
			str += var_name_size(str) - 1;
		}
		else
		{
			expanded_char[i] = *str;
			i++;
		}
		str++;
	}
	expanded_char[i] = '\0';
	return (expanded_char);
}

t_list	*lst_expande_char(char	*str)
{
	t_list	*lst_res;
	char	*expanded_char;
	// size_t	n_c_size;

	lst_res = NULL;
	expanded_char = first_expand(str);
	printf("char is : %s\nfirst expand is : %s\n", str, expanded_char);
	return (NULL);
	return (lst_res);
}

char	**create_chartab_from_lst(t_list *lst)
{
	char	**chartab;
	int		i;

	chartab = (char **)malloc(sizeof(char *) * ft_lstsize(lst) + 1);
	if (!chartab)
	{
		handle_errors("malloc");
		return (NULL);
	}
	i = 0;
	while (lst)
	{
		chartab[i] = (char *)lst->content;
		lst = lst->next;
		i++;
	}
	chartab[i] = NULL;
	return (chartab);
}

void	expande_commande(t_cmd *node)
{
	// char	*name_save;
	// char	*arg_save;
	int		i;
	t_list	*lst_excmd;

	lst_excmd = NULL;
	i = 0;
	while (node->cmd_arg[i])
	{
		ft_lstadd_back(&lst_excmd, lst_expande_char(node->cmd_arg[i]));
		i++;
	}
	free_chartab(node->cmd_arg);
	node->cmd_arg = create_chartab_from_lst(lst_excmd);
	if (node->cmd_arg)
		node->cmd_name = node->cmd_arg[0];
	else
		node->cmd_name = NULL;
}

// void	expande_commande(t_cmd *node)
// {
// 	char	*name_save;
// 	char	*arg_save;
// 	int		i;

// //si vide
// 	name_save = node->cmd_name;
// 	node->cmd_name = expande_char(node->cmd_name);
// 	free(name_save);
// 	node->cmd_arg[0] = node->cmd_name;
// 	i = 1;
// 	while (node->cmd_arg[i])
// 	{
// 		//ici faire un truc compliquer
// 		arg_save = node->cmd_arg[i];
// 		node->cmd_arg[i] = expande_char(node->cmd_arg[i]);
// 		free(arg_save);
// 		i++;
// 	}
// }