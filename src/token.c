#include "token.h"

static const char *token_names[] = {
	[TOKEN_LPAREN]       = "LPAREN",
	[TOKEN_RPAREN]       = "RPAREN",
	[TOKEN_LBRACE]       = "LBRACE",
	[TOKEN_RBRACE]       = "RBRACE",
	[TOKEN_LBRACKET]     = "LBRACKET",
	[TOKEN_RBRACKET]     = "RBRACKET",
	[TOKEN_PLUS]         = "PLUS",
	[TOKEN_MINUS]        = "MINUS",
	[TOKEN_STAR]         = "STAR",
	[TOKEN_SLASH]        = "SLASH",
	[TOKEN_PERCENT]      = "PERCENT",
	[TOKEN_COMMA]        = "COMMA",
	[TOKEN_SEMICOLON]    = "SEMICOLON",
	[TOKEN_DOT]          = "DOT",

	[TOKEN_ASSIGN]       = "ASSIGN",
	[TOKEN_EQ]           = "EQ",
	[TOKEN_BANG]         = "BANG",
	[TOKEN_NEQ]          = "NEQ",
	[TOKEN_LT]           = "LT",
	[TOKEN_LE]           = "LE",
	[TOKEN_GT]           = "GT",
	[TOKEN_GE]           = "GE",
	[TOKEN_PIPE]         = "PIPE",
	[TOKEN_APPEND]       = "APPEND",

	[TOKEN_INT]          = "INT",
	[TOKEN_FLOAT]        = "FLOAT",
	[TOKEN_STRING]       = "STRING",
	[TOKEN_RAW_STRING]   = "RAW_STRING",

	[TOKEN_IDENTIFIER]   = "IDENTIFIER",
	[TOKEN_IF]           = "IF",
	[TOKEN_ELSE]         = "ELSE",
	[TOKEN_FOR]          = "FOR",
	[TOKEN_WHILE]        = "WHILE",
	[TOKEN_FN]           = "FN",
	[TOKEN_RETURN]       = "RETURN",
	[TOKEN_TRUE]         = "TRUE",
	[TOKEN_FALSE]        = "FALSE",
	[TOKEN_NIL]          = "NIL",
	[TOKEN_AND]          = "AND",
	[TOKEN_OR]           = "OR",
	[TOKEN_NOT]          = "NOT",
	[TOKEN_SH]           = "SH",

	[TOKEN_NEWLINE]      = "NEWLINE",
	[TOKEN_COMMAND_TEXT]  = "COMMAND_TEXT",
	[TOKEN_EOF]          = "EOF",
	[TOKEN_ERROR]        = "ERROR",
};

const char *token_type_name(enum token_type type)
{
	if (type >= 0 && type <= TOKEN_ERROR)
		return token_names[type];
	return "UNKNOWN";
}
