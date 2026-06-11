#ifndef EZSH_LEXER_H
#define EZSH_LEXER_H

#include "token.h"

struct lexer {
	const char *start;	/* start of current token */
	const char *current;	/* current scan position */
	int line;		/* current line number */
};

void lexer_init(struct lexer *l, const char *source);
struct token next_token(struct lexer *l);
struct token scan_raw_line(struct lexer *l);

#endif /* EZSH_LEXER_H */
