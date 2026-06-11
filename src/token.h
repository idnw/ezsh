#ifndef EZSH_TOKEN_H
#define EZSH_TOKEN_H

enum token_type {
	/* single-char tokens */
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_LBRACE,
	TOKEN_RBRACE,
	TOKEN_LBRACKET,
	TOKEN_RBRACKET,
	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_STAR,
	TOKEN_SLASH,
	TOKEN_PERCENT,
	TOKEN_COMMA,
	TOKEN_SEMICOLON,
	TOKEN_DOT,

	/* one or two char tokens */
	TOKEN_ASSIGN,		/* = */
	TOKEN_EQ,		/* == */
	TOKEN_BANG,		/* ! */
	TOKEN_NEQ,		/* != */
	TOKEN_LT,		/* < */
	TOKEN_LE,		/* <= */
	TOKEN_GT,		/* > */
	TOKEN_GE,		/* >= */
	TOKEN_PIPE,		/* | */
	TOKEN_APPEND,		/* >> */

	/* literals */
	TOKEN_INT,
	TOKEN_FLOAT,
	TOKEN_STRING,		/* "..." with interpolation */
	TOKEN_RAW_STRING,	/* '...' no interpolation */

	/* identifiers and keywords */
	TOKEN_IDENTIFIER,
	TOKEN_IF,
	TOKEN_ELSE,
	TOKEN_FOR,
	TOKEN_WHILE,
	TOKEN_FN,
	TOKEN_RETURN,
	TOKEN_TRUE,
	TOKEN_FALSE,
	TOKEN_NIL,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_NOT,
	TOKEN_SH,

	/* special */
	TOKEN_NEWLINE,
	TOKEN_COMMAND_TEXT,	/* raw command line text */
	TOKEN_EOF,
	TOKEN_ERROR,
};

struct token {
	enum token_type type;
	const char *start;	/* pointer into source */
	int length;
	int line;
};

const char *token_type_name(enum token_type type);

#endif /* EZSH_TOKEN_H */
