#include "not_libft.h"

static int	ft_str_to_nbr(const char *start, char *end, int sign)
{
	long	res;
	long	exp;
	long	prev;
	char	*ptr;

	exp = 1 * sign;
	res = 0;
	ptr = end;
	while (end >= start)
	{
		prev = res;
		res += (*end - '0') * exp;
		if (prev != res % exp && ptr != end)
		{
			if (sign == 1)
				return (-1);
			else
				return (0);
		}
		exp *= 10;
		end--;
	}
	return ((int)res);
}

int			ft_atoi(char *s)
{
	int		sign;
	char	*start;

	sign = 1;
	while ((*s >= 9 && *s <= 13) || *s == ' ')
		s++;
	if (*s == '+' && *(s + 1) >= '0' && *(s + 1) <= '9')
		s++;
	else if (*s == '-' && *(s + 1) >= '0' && *(s + 1) <= '9')
	{
		sign = -1;
		s++;
	}
	else if (!(*s >= '0' || *s <= '9'))
		return (0);
	start = s;
	while (*s >= '0' && *s <= '9')
		s++;
	return (ft_str_to_nbr(start, s - 1, sign));
}
