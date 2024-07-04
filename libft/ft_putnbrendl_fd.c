#include <unistd.h>



static void	ft_recur_nbr(long n, int fd)
{
	int	c;
	if (!n)
		return ;
	ft_recur_nbr((int)(n / 10), fd);
	c = (int)(n % 10) + '0';
	write(fd, &c, 1);
}

void	ft_putnbrendl_fd(int n, int fd)
{
	long	temp;

	temp = n;
	if (n < 0)
	{
		temp *= -1;
		write(fd, "-", 1);
	}
	if (n == 0)
		write(fd, "0", 1);
	else
		ft_recur_nbr(temp, fd);
	write(fd, "\n", 1);
}