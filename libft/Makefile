# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fbelfort <fbelfort@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/07 13:34:03 by mpagani           #+#    #+#              #
#    Updated: 2023/02/21 16:19:51 by fbelfort         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS  = ft_isalpha.c ft_isdigit.c ft_isalnum.c \
		ft_bzero.c ft_isascii.c ft_isprint.c \
		ft_memcpy.c ft_memset.c ft_strlen.c \
		ft_memmove.c ft_strlcpy.c ft_strlcat.c\
		ft_strncmp.c ft_toupper.c ft_tolower.c \
		ft_strchr.c ft_strrchr.c ft_memchr.c \
		ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c \
		ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c \
		ft_split.c ft_itoa.c ft_strmapi.c \
		ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c \
		ft_putnbr_fd.c ft_striteri.c \
		ft_printf.c ft_check.c ft_check_tag.c ft_output_c.c \
		ft_output_s.c ft_output_x.c ft_output_p.c ft_putnbr_base.c \
		ft_output_d.c ft_output_xmaj.c ft_output_u.c ft_putnbr.c \
		ft_output_perc.c get_next_line_utils.c \
		get_next_line.c ft_lstdelone_bonus.c ft_lstsize_bonus.c\
		ft_lstnew_bonus.c ft_lstadd_front_bonus.c \
		ft_lstclear_bonus.c ft_lstiter_bonus.c \
		ft_lstadd_back_bonus.c ft_lstlast_bonus.c \
		ft_lstmap_bonus.c
OBJS  = ${SRCS:.c=.o}
NAME  = libft.a
CC    = cc
AR    = ar
RM    = rm -f
FLAGS  = -Wall -Wextra -Werror
HEADER_FILE = libft.h get_next_line_bonus.h

all : ${NAME}

${NAME} : ${OBJS}
	${AR} -rcs ${NAME} ${OBJS}

%.o: %.c ${HEADER_FILE} Makefile
	${CC} ${FLAGS} -c $< -o $@

clean :
	${RM} -f ${OBJS}

fclean : clean
	${RM} -f ${NAME}

re : fclean all

.PHONY: all clean fclean re
