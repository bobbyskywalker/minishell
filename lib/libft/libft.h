/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 13:02:15 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/12 11:17:19 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

// SECTION: INCLUDES
# include <assert.h>
# include <limits.h>
# include <stdarg.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

// SECTION: PROTOTYPES
int		ft_isalpha(int c);
int		ft_isalnum(int c);
int		ft_isspace(int c);
size_t	ft_strlen(const char *str);
int		ft_isascii(int c);
int		ft_isdigit(int c);
int		ft_isprint(int c);
void	*ft_memset(void *s, int c, size_t n);
void	ft_bzero(void *s, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
int		ft_toupper(int c);
int		ft_tolower(int c);
int		ft_atoi(const char *nptr);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strnstr(const char *big, const char *little, size_t len);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strdup(const char *s);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_itoa(int n);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char *));
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);

// SECTION PRINTF:
int		ft_print_hex(uintptr_t arg, int case_p);
int		ft_print_int(int nb);
int		ft_print_uint(unsigned int nb);
int		ft_print_ptr(uintptr_t ptr);
int		ft_print_str(char *str);
int		ft_print_char(char c);
int		ft_printf(const char *format, ...);

// SECTION: new
void	ft_slc_sort(int *arr, int len);
void	ft_arr2d_free(char **arr);
char	**ft_2d_strdup(char **arr);
int		ft_2d_strlen(char **arr);
void	ft_2d_strprintf(char **str);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
char	**ft_2dstr_realloc(char **arr, size_t old_rows, size_t new_rows);
#endif
