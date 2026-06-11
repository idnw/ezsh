#include <stdio.h>

#include "lexer.h"
#include "token.h"

static void print_token(struct token *tok)
{
	printf("%4d  %-14s '%.*s'\n",
	       tok->line, token_type_name(tok->type),
	       tok->length, tok->start);
}

static void tokenize(const char *source)
{
	struct lexer l;
	struct token tok;

	lexer_init(&l, source);
	for (;;) {
		tok = next_token(&l);
		print_token(&tok);
		if (tok.type == TOKEN_EOF)
			break;
	}
}

int main(void)
{
	const char *test =
		"x = 42\n"
		"name = \"hello {world}\"\n"
		"raw = 'no interpolation'\n"
		"if x >= 10 {\n"
		"    fn greet(a, b) {\n"
		"        return a + b\n"
		"    }\n"
		"}\n"
		"# this is a comment\n"
		"result = sh(ls -la)\n"
		"3.14 != 2\n";

	printf("--- Tokenizing test input ---\n\n");
	tokenize(test);

	printf("\n--- Testing scan_raw_line ---\n\n");

	struct lexer l;
	struct token tok;

	lexer_init(&l, "  ls -la /tmp | grep foo > out.txt\n");
	tok = scan_raw_line(&l);
	print_token(&tok);

	return 0;
}
