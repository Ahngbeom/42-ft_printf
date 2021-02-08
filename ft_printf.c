/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahn <bahn@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 15:21:34 by bahn              #+#    #+#             */
/*   Updated: 2021/02/08 15:53:46 by bahn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static	int	data_type(va_list ap, t_opt *opt)
{
	if (opt->type == 'c')
		return (char_format(va_arg(ap, int), opt));
	else if (opt->type == 's')
		return (str_format(va_arg(ap, char*), opt));
	else if (opt->type == 'i' || opt->type == 'd')
		return (int_format(va_arg(ap, int), opt));
	else if (opt->type == 'p')
		return (pointer_format(va_arg(ap, long long), opt));
	else if (opt->type == 'u' || opt->type == 'x' || opt->type == 'X')
		return (uint_format(va_arg(ap, unsigned int), opt));
	else if (opt->type == '%')
		return (ft_putchar_fd(opt->type, 1));
	else
		return (-1);
}

static	void	set_width_or_prec(va_list ap, t_opt *opt, char ch)
{
	if (ft_isdigit(ch))
	{
		if (opt->prec == -1)
			opt->width = opt->width * 10 + ch - 48;
		else
			opt->prec = opt->prec * 10 + ch - 48;
	}
	else if (ch == '*')
	{
		if (opt->prec == -1)
		{
			if ((opt->width = va_arg(ap, int)) < 0)
			{
				opt->minus = 1;
				opt->width *= -1;
				opt->zero = 0;
			}
		}
		else
		{
			if ((opt->prec = va_arg(ap, int)) < 0)
				opt->prec = -1;
			else
				opt->zero = 0;
		}
	}
}

static	int	find_format(char *fmt, va_list ap)
{
	int	print_len;
	int	i;
	t_opt	*opt;

	i = 0;
	if (!(opt = malloc(sizeof(t_opt))))
		return (-1);
	ft_memset(opt, 0, sizeof(t_opt));
	opt->prec = -1;
	while (!ft_strchr(DTYPE, fmt[i]) && fmt[i] != '\0')
	{
		if (fmt[i] == '-')
		{
			opt->minus = 1;
			if (opt->zero == 1)
				opt->zero = 0;
		}
		else if (fmt[i] == '0' && opt->width == 0 && opt->prec == -1)
			opt->zero = 1;
		else if (fmt[i] == '.')
			opt->prec = 0;
		else if (ft_isdigit(fmt[i]) || fmt[i] == '*')
			set_width_or_prec(ap, opt, fmt[i]);	
		i++;
	}
	if (opt->prec >= 0 && opt->zero > 0)
		opt->zero = 0;
	opt->type = fmt[i];
	print_len = data_type(ap, opt);
	free(opt);
	return (print_len);
}

int			ft_printf(const char *str, ...)
{
	int		print_len;
	int		rtn;
	va_list		ap;

	rtn = 0;
	va_start(ap, str);
	while (*str != '\0')
	{
		if (*str != '%')
		{
			rtn += ft_putchar_fd(*str, 1);
			str++;
		}
		else
		{
			print_len = find_format((char *)++str, ap);
			if (print_len >= 0)
			{
				rtn += print_len;
				str = ft_strchr_set((char *)str, DTYPE) + 1;
			}
		}
	}
	va_end(ap);
	return (rtn);
}
