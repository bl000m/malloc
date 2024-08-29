#include "libft.h"

void	ft_itoa_fd(size_t nb, char base, int fd, bool prefix)
{
	char *str;

	str = "0123456789ABCDEFGHIJKLMNOPQRSTUIVWXYZ";
	if (nb / base)
		ft_itoa_fd(nb / base, base, fd, prefix);
	else if (prefix)
		write(fd, "0x", 2);
	write(fd, &str[nb % base], 1);
}

void	ft_itoa_base(size_t nb, char base, char length, bool prefix)
{
	char	*str;

	str = "0123456789ABCDEFGHIJKLMNOPQRSTUIVWXYZ";
	if (nb / base)
		ft_itoa_base(nb / base, base, length - 1, prefix);
	else
	{
		if (prefix)
			ft_putstr("0x");
		while (--length > 0)
		{
			ft_putstr("0");
		}
	}
	write(1, &str[nb % base], 1);
}
