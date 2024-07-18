/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbelfort <fbelfort@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 14:37:46 by mpagani           #+#    #+#             */
/*   Updated: 2023/02/23 12:27:15 by fbelfort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>
# include <stdint.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# ifndef OPEN_MAX
#  define OPEN_MAX 256
# endif

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_gnl
{
	char			*content;
	struct s_gnl	*next;
}	t_gnl;

void	ft_bzero(void *s, size_t n);
int		ft_isalnum(int c);
int		ft_isalpha(int c);
int		ft_isascii(int c);
int		ft_isdigit(int n);
int		ft_isprint(int c);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_memset(void *b, int c, size_t len);
size_t	ft_strlen(const char *str);
void	*ft_memmove(void *dst, const void *src, size_t len);
size_t	ft_strlcpy(char *dest, char *src, size_t size);
size_t	ft_strlcat(char *dest, const char *src, size_t size);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_toupper(int c);
int		ft_tolower(int c);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
int		ft_atoi(const char *str);
void	*ft_calloc(size_t count, size_t size);
char	*ft_strdup(const char *s1);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
char	*ft_itoa(int n);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
int		ft_printf(const char *format, ...);
int		ft_check(va_list args, const char *format, int size);
int		ft_check_tag(va_list args, char c, int size);
int		ft_output_c(int c, int size);
int		ft_output_s(char *s, int size);
int		ft_output_p(va_list args, int size);
int		ft_output_x(va_list args, int size);
int		ft_output_xmaj(va_list args, int size);
int		ft_output_d(va_list args, int size);
int		ft_output_u(va_list args, int size);
int		ft_output_perc(int size);
void	ft_putnbr(int nb);
int		ft_putnbr_base(unsigned long long nb, unsigned int base, char *digits);
int		check_eol(t_gnl *stash);
void	ft_add_pattern(t_gnl **lst, char *buffer, int characters);
int		ft_line_size(t_gnl *record);
char	*get_next_line(int fd);
t_gnl	*ft_gnllast(t_gnl *lst);
void	ft_free_list(t_gnl *record);
void	clean_record(t_gnl **record);
void	compose_line(char **line, t_gnl *record);
void	read_stock(t_gnl **record, int fd);

#endif
