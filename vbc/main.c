#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

static const char *input; /* current parse pointer */
static int	has_err = 0;   /* error flag */

static int	parse_expr(void);
static int	parse_term(void);
static int	parse_factor(void);

static void	error_end(void)
{
	write(2, "unexpected end of input\n", 24);
	has_err = 1;
}

static void	error_tok(char c)
{
	write(2, "unexpected token '", 17);
	write(2, &c, 1);
	write(2, "'\n", 2);
	has_err = 1;
}

static int	parse_factor(void)
{
	int	value;

	if (has_err)
		return (0);
	if (*input == '\0')
	{
		error_end();
		return (0);
	}
	if (*input == '(')
	{
		input++;
		value = parse_expr();
		if (has_err)
			return (0);
		if (*input == ')')
		{
			input++;
			return (value);
		}
		if (*input == '\0')
			error_end();
		else
			error_tok(*input);
		return (0);
	}
	if (isdigit((unsigned char)*input))
	{
		value = *input - '0';
		input++;
		return (value);
	}
	error_tok(*input);
	return (0);
}

static int	parse_term(void)
{
	int	factor_value;
	int	result;

	factor_value = parse_factor();
	while (!has_err)
	{
		if (*input == '*')
		{
			input++;
			result = parse_factor();
			factor_value *= result;
		}
		else if (*input == '(' || isdigit((unsigned char)*input))
		{
			result = parse_factor();
			factor_value *= result;
		}
		else
			break ;
	}
	return (factor_value);
}

static int	parse_expr(void)
{
	int	term_value;
	int	result;

	if (has_err)
		return (0);
	term_value = parse_term();
	while (!has_err && *input == '+')
	{
		input++;
		result = parse_term();
		term_value += result;
	}
	return (term_value);
}

int	main(int argc, char *argv[])
{
	int result;

	if (argc != 2)
		return (1);
	input = argv[1];
	result = parse_expr();
	if (!has_err && *input != '\0')
	{
		if (*input == ')')
			error_tok(')');
		else
			error_tok(*input);
	}
	if (has_err)
		return (1);
	printf("%d\n", result);
	return (0);
}