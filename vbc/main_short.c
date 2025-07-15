#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int	parse_plus(char *input, int *pos);
int	parse_mult(char *input, int *pos);
int	parse_factor(char *input, int *pos);

void	error_handling(char *msg, char unexp_c)
{
	if (unexp_c)
		printf("unexpected token '%c'\n", unexp_c);
	else
		printf("%s\n", msg);
	exit(1);
}

int	parse_factor(char *input, int *pos)
{
	int	result;

	if (input[*pos] == '\0')
		error_handling("unexpected end of input", 0);
	if (isdigit(input[*pos]))
	{
		result = input[*pos] - '0';
		(*pos)++;
		if (isdigit(input[*pos]))
			error_handling("unexpected token %c\n", input[*pos]);
		return (result);
	}
	if (input[*pos] == '(')
	{
		(*pos)++;
		result = parse_plus(input, pos);
		if (input[*pos] != ')')
		{
			if (input[*pos] == '\0')
				error_handling("unexpected end of input", 0);
			else
				error_handling("expected ')'", input[*pos]);
		}
		(*pos)++;
		return (result);
	}
	error_handling("unexpected token %c\n", input[*pos]);
	return (0);
}

int	parse_mult(char *input, int *pos)
{
	int	result;

	result = parse_factor(input, pos);
	while (input[*pos] == '*' || isdigit(input[*pos]))
	{
		if (input[*pos] == '*')
		{
			(*pos)++;
			if (input[*pos] == '\0')
				error_handling("unexpected end of input", 0);
		}
		result *= parse_factor(input, pos);
	}
	return (result);
}

int	parse_plus(char *input, int *pos)
{
	int	result;

	result = parse_mult(input, pos);
	while (input[*pos] == '+')
	{
		(*pos)++;
		if (input[*pos] == '\0')
			error_handling("unexpected end of input", 0);
		result += parse_mult(input, pos);
	}
	return (result);
}

int	main(int argc, char **argv)
{
	char	*input;
	int	pos;
	int	result;

	if (argc != 2)
		return (1);
	pos = 0;
	input = argv[1];
	result = parse_plus(input, &pos);
	if (input[pos] != '\0')
		error_handling("unexpected token %c\n", input[pos]);
	printf("%d\n", result);
	return (0);
}
