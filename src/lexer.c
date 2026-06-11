#include <ctype.h>
#include <string.h>

#include "lexer.h"

void lexer_init(struct lexer *l, const char *source)
{
	l->start = source;
	l->current = source;
	l->line = 1;
}

static int is_at_end(struct lexer *l)
{
	return *l->current == '\0';
}

static char advance(struct lexer *l)
{
	l->current++;
	return l->current[-1];
}

static char peek(struct lexer *l)
{
	return *l->current;
}

static char peek_next(struct lexer *l)
{
	if (is_at_end(l))
		return '\0';
	return l->current[1];
}

static int match(struct lexer *l, char expected)
{
	if (is_at_end(l))
		return 0;
	if (*l->current != expected)
		return 0;
	l->current++;
	return 1;
}

static struct token make_token(struct lexer *l, enum token_type type)
{
	struct token tok;

	tok.type = type;
	tok.start = l->start;
	tok.length = (int)(l->current - l->start);
	tok.line = l->line;
	return tok;
}

static struct token error_token(struct lexer *l, const char *msg)
{
	struct token tok;

	tok.type = TOKEN_ERROR;
	tok.start = msg;
	tok.length = (int)strlen(msg);
	tok.line = l->line;
	return tok;
}

static void skip_whitespace(struct lexer *l)
{
	for (;;) {
		char c = peek(l);

		switch (c) {
		case ' ':
		case '\t':
		case '\r':
			advance(l);
			break;
		case '#':
			while (peek(l) != '\n' && !is_at_end(l))
				advance(l);
			break;
		default:
			return;
		}
	}
}

static int is_ident_start(char c)
{
	return isalpha(c) || c == '_';
}

static int is_ident_char(char c)
{
	return isalnum(c) || c == '_';
}

static enum token_type check_keyword(struct lexer *l, int start, int length,
				     const char *rest, enum token_type type)
{
	if (l->current - l->start == start + length &&
	    memcmp(l->start + start, rest, length) == 0)
		return type;
	return TOKEN_IDENTIFIER;
}

static enum token_type identifier_type(struct lexer *l)
{
	switch (l->start[0]) {
	case 'a':
		return check_keyword(l, 1, 2, "nd", TOKEN_AND);
	case 'e':
		return check_keyword(l, 1, 3, "lse", TOKEN_ELSE);
	case 'f':
		if (l->current - l->start > 1) {
			switch (l->start[1]) {
			case 'a':
				return check_keyword(l, 2, 3, "lse", TOKEN_FALSE);
			case 'n':
				if (l->current - l->start == 2)
					return TOKEN_FN;
				break;
			case 'o':
				return check_keyword(l, 2, 1, "r", TOKEN_FOR);
			}
		}
		break;
	case 'i':
		return check_keyword(l, 1, 1, "f", TOKEN_IF);
	case 'n':
		if (l->current - l->start > 1) {
			switch (l->start[1]) {
			case 'i':
				return check_keyword(l, 2, 1, "l", TOKEN_NIL);
			case 'o':
				return check_keyword(l, 2, 1, "t", TOKEN_NOT);
			}
		}
		break;
	case 'o':
		return check_keyword(l, 1, 1, "r", TOKEN_OR);
	case 'r':
		return check_keyword(l, 1, 5, "eturn", TOKEN_RETURN);
	case 's':
		return check_keyword(l, 1, 1, "h", TOKEN_SH);
	case 't':
		return check_keyword(l, 1, 3, "rue", TOKEN_TRUE);
	case 'w':
		return check_keyword(l, 1, 4, "hile", TOKEN_WHILE);
	}
	return TOKEN_IDENTIFIER;
}

static struct token scan_identifier(struct lexer *l)
{
	while (is_ident_char(peek(l)))
		advance(l);
	return make_token(l, identifier_type(l));
}

static struct token scan_number(struct lexer *l)
{
	enum token_type type = TOKEN_INT;

	while (isdigit(peek(l)))
		advance(l);

	if (peek(l) == '.' && isdigit(peek_next(l))) {
		type = TOKEN_FLOAT;
		advance(l); /* consume '.' */
		while (isdigit(peek(l)))
			advance(l);
	}

	return make_token(l, type);
}

static struct token scan_string(struct lexer *l)
{
	while (peek(l) != '"' && !is_at_end(l)) {
		if (peek(l) == '\n')
			l->line++;
		if (peek(l) == '\\' && peek_next(l) != '\0')
			advance(l); /* skip escaped char */
		advance(l);
	}

	if (is_at_end(l))
		return error_token(l, "unterminated string");

	advance(l); /* closing quote */
	return make_token(l, TOKEN_STRING);
}

static struct token scan_raw_string(struct lexer *l)
{
	while (peek(l) != '\'' && !is_at_end(l)) {
		if (peek(l) == '\n')
			l->line++;
		advance(l);
	}

	if (is_at_end(l))
		return error_token(l, "unterminated string");

	advance(l); /* closing quote */
	return make_token(l, TOKEN_RAW_STRING);
}

struct token next_token(struct lexer *l)
{
	char c;

	skip_whitespace(l);
	l->start = l->current;

	if (is_at_end(l))
		return make_token(l, TOKEN_EOF);

	c = advance(l);

	if (c == '\n') {
		l->line++;
		return make_token(l, TOKEN_NEWLINE);
	}

	if (is_ident_start(c))
		return scan_identifier(l);
	if (isdigit(c))
		return scan_number(l);

	switch (c) {
	case '(':  return make_token(l, TOKEN_LPAREN);
	case ')':  return make_token(l, TOKEN_RPAREN);
	case '{':  return make_token(l, TOKEN_LBRACE);
	case '}':  return make_token(l, TOKEN_RBRACE);
	case '[':  return make_token(l, TOKEN_LBRACKET);
	case ']':  return make_token(l, TOKEN_RBRACKET);
	case '+':  return make_token(l, TOKEN_PLUS);
	case '-':  return make_token(l, TOKEN_MINUS);
	case '*':  return make_token(l, TOKEN_STAR);
	case '/':  return make_token(l, TOKEN_SLASH);
	case '%':  return make_token(l, TOKEN_PERCENT);
	case ',':  return make_token(l, TOKEN_COMMA);
	case ';':  return make_token(l, TOKEN_SEMICOLON);
	case '.':  return make_token(l, TOKEN_DOT);
	case '|':  return make_token(l, TOKEN_PIPE);
	case '!':
		return make_token(l, match(l, '=') ? TOKEN_NEQ : TOKEN_BANG);
	case '=':
		return make_token(l, match(l, '=') ? TOKEN_EQ : TOKEN_ASSIGN);
	case '<':
		return make_token(l, match(l, '=') ? TOKEN_LE : TOKEN_LT);
	case '>':
		if (match(l, '>'))
			return make_token(l, TOKEN_APPEND);
		return make_token(l, match(l, '=') ? TOKEN_GE : TOKEN_GT);
	case '"':
		return scan_string(l);
	case '\'':
		return scan_raw_string(l);
	}

	return error_token(l, "unexpected character");
}

struct token scan_raw_line(struct lexer *l)
{
	/* skip leading whitespace on the line */
	while (peek(l) == ' ' || peek(l) == '\t')
		advance(l);

	l->start = l->current;

	/* consume until newline or end */
	while (!is_at_end(l) && peek(l) != '\n')
		advance(l);

	return make_token(l, TOKEN_COMMAND_TEXT);
}
