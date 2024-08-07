#ifndef lexer_token_hpp
#define lexer_token_hpp


#define LEXER_TOKENS \
DEFINE_LEXER_TOKEN( ERROR ), \
DEFINE_LEXER_TOKEN( END_STREAM ), \
DEFINE_LEXER_TOKEN( END_TOKEN ), \
DEFINE_LEXER_TOKEN( START_TOKEN ), \
DEFINE_LEXER_TOKEN( AT_SIGN ), \
DEFINE_LEXER_TOKEN( BAR ), \
DEFINE_LEXER_TOKEN( AMPERSAND ), \
DEFINE_LEXER_TOKEN( ADD ), \
DEFINE_LEXER_TOKEN( ADD_EQUAL ), \
DEFINE_LEXER_TOKEN( SUB ), \
DEFINE_LEXER_TOKEN( SUB_EQUAL ), \
DEFINE_LEXER_TOKEN( MUL ), \
DEFINE_LEXER_TOKEN( MUL_EQUAL ), \
DEFINE_LEXER_TOKEN( DIV ), \
DEFINE_LEXER_TOKEN( DIV_EQUAL ), \
DEFINE_LEXER_TOKEN( MOD ), \
DEFINE_LEXER_TOKEN( MOD_EQUAL ), \
DEFINE_LEXER_TOKEN( EXP ), \
DEFINE_LEXER_TOKEN( EXP_EQUAL ), \
DEFINE_LEXER_TOKEN( ADDRESS ), \
DEFINE_LEXER_TOKEN( OPEN_SQUARE_BRACKET ), \
DEFINE_LEXER_TOKEN( CLOSE_SQUARE_BRACKET ), \
DEFINE_LEXER_TOKEN( OPEN_CURLY_BRACKET ), \
DEFINE_LEXER_TOKEN( CLOSE_CURLY_BRACKET ), \
DEFINE_LEXER_TOKEN( OPEN_PARENTHESIS ), \
DEFINE_LEXER_TOKEN( CLOSE_PARENTHESIS ), \
DEFINE_LEXER_TOKEN( SEMICOLON ), \
DEFINE_LEXER_TOKEN( COLON ), \
DEFINE_LEXER_TOKEN( COMMA ), \
DEFINE_LEXER_TOKEN( LESS ), \
DEFINE_LEXER_TOKEN( LESS_EQUAL ), \
DEFINE_LEXER_TOKEN( GREATER ), \
DEFINE_LEXER_TOKEN( GREATER_EQUAL ), \
DEFINE_LEXER_TOKEN( AND ), \
DEFINE_LEXER_TOKEN( AND_EQUAL ), \
DEFINE_LEXER_TOKEN( OR ), \
DEFINE_LEXER_TOKEN( OR_EQUAL ), \
DEFINE_LEXER_TOKEN( NOT ), \
DEFINE_LEXER_TOKEN( NOT_EQUAL ), \
DEFINE_LEXER_TOKEN( EQUAL ), \
DEFINE_LEXER_TOKEN( EQUAL_EQUAL ), \
DEFINE_LEXER_TOKEN( ASSIGN ), \
DEFINE_LEXER_TOKEN( XOR ), \
DEFINE_LEXER_TOKEN( XOR_EQUAL ), \
DEFINE_LEXER_TOKEN( INVERT ), \
DEFINE_LEXER_TOKEN( INVERT_EQUAL ), \
DEFINE_LEXER_TOKEN( SHIFT_RIGHT ), \
DEFINE_LEXER_TOKEN( SHIFT_RIGHT_EQUAL ), \
DEFINE_LEXER_TOKEN( SHIFT_LEFT ), \
DEFINE_LEXER_TOKEN( SHIFT_LEFT_EQUAL ), \
DEFINE_LEXER_TOKEN( INCREMENT ), \
DEFINE_LEXER_TOKEN( DECREMENT ), \
\
DEFINE_LEXER_TOKEN( BOOL ), \
DEFINE_LEXER_TOKEN( FLOAT ), \
DEFINE_LEXER_TOKEN( INT ), \
DEFINE_LEXER_TOKEN( DOUBLE ), \
DEFINE_LEXER_TOKEN( LONG ), \
DEFINE_LEXER_TOKEN( STRING ), \
\
DEFINE_LEXER_TOKEN( FLOAT_E ), \
DEFINE_LEXER_TOKEN( FLOAT_EX ), \
DEFINE_LEXER_TOKEN( DOUBLE_E ), \
DEFINE_LEXER_TOKEN( DOUBLE_EX ), \
DEFINE_LEXER_TOKEN( STRING_ESC ), \
DEFINE_LEXER_TOKEN( NULL ), \
DEFINE_LEXER_TOKEN( CONST ), \
DEFINE_LEXER_TOKEN( TRUE ), \
DEFINE_LEXER_TOKEN( FALSE ), \
DEFINE_LEXER_TOKEN( IF ), \
DEFINE_LEXER_TOKEN( ELSE ), \
DEFINE_LEXER_TOKEN( ELSE_IF ), \
DEFINE_LEXER_TOKEN( BREAK ), \
DEFINE_LEXER_TOKEN( CONTINUE ), \
DEFINE_LEXER_TOKEN( FOR ), \
DEFINE_LEXER_TOKEN( WHILE ), \
DEFINE_LEXER_TOKEN( SWITCH_DEFAULT ), \
DEFINE_LEXER_TOKEN( SWITCH_CASE ), \
DEFINE_LEXER_TOKEN( SWITCH ), \
DEFINE_LEXER_TOKEN( RETURN ), \
DEFINE_LEXER_TOKEN( FUNCTION ), \
DEFINE_LEXER_TOKEN( HOST ), \
DEFINE_LEXER_TOKEN( BLOCK_COMMENT ), \
DEFINE_LEXER_TOKEN( INLINE_COMMENT ),\
\
DEFINE_LEXER_TOKEN( OPERATOR ), \
DEFINE_LEXER_TOKEN( KEYWORD ), \
DEFINE_LEXER_TOKEN( IDENTIFIER ), \
DEFINE_LEXER_TOKEN( STATIC_IDENTIFIER ), \
DEFINE_LEXER_TOKEN( CONST_IDENTIFIER ), \
DEFINE_LEXER_TOKEN( CONST_STATIC_IDENTIFIER ), \
DEFINE_LEXER_TOKEN( NUMERIC ), \
DEFINE_LEXER_TOKEN( DELIMITER ), \
\
DEFINE_LEXER_TOKEN( VAR_INT ), \
DEFINE_LEXER_TOKEN( VAR_LONG ), \
DEFINE_LEXER_TOKEN( VAR_FLOAT ), \
DEFINE_LEXER_TOKEN( VAR_DOUBLE ), \
DEFINE_LEXER_TOKEN( VAR_STRING ), \
DEFINE_LEXER_TOKEN( VAR_BOOL ), \
DEFINE_LEXER_TOKEN( VAR_FUNCTION ), \
DEFINE_LEXER_TOKEN( ARRAY ), \
DEFINE_LEXER_TOKEN( ARRAY_INDEXED ), \
DEFINE_LEXER_TOKEN( ARRAY_SIZE ), \
DEFINE_LEXER_TOKEN( SCOPE_BLOCK ), \
\
DEFINE_LEXER_TOKEN( ENCLOSE_SQUARE_BRACKET ), \
DEFINE_LEXER_TOKEN( ENCLOSE_CURLY_BRACKET ), \
DEFINE_LEXER_TOKEN( ENCLOSE_PARENTHESIS ), \
DEFINE_LEXER_TOKEN( IF_STATEMENT ), \
DEFINE_LEXER_TOKEN( DECLARATION ), \
DEFINE_LEXER_TOKEN( CONDITIONS ), \
DEFINE_LEXER_TOKEN( BRANCHING ), \
DEFINE_LEXER_TOKEN( PARAMETERS ), \
DEFINE_LEXER_TOKEN( PARAMETER ), \
DEFINE_LEXER_TOKEN( EXPRESSION ), \
DEFINE_LEXER_TOKEN( FUNCTION_CALL ), \
DEFINE_LEXER_TOKEN( VARIABLE ), \
DEFINE_LEXER_TOKEN( VALUE ), \
DEFINE_LEXER_TOKEN( ARITHMATIC_VALUE ), \
\
DEFINE_LEXER_TOKEN( CONST_BOOL ), \
DEFINE_LEXER_TOKEN( CONST_INT ), \
DEFINE_LEXER_TOKEN( CONST_LONG ), \
DEFINE_LEXER_TOKEN( CONST_FLOAT ), \
DEFINE_LEXER_TOKEN( CONST_DOUBLE ), \
DEFINE_LEXER_TOKEN( CONST_STRING ), \
DEFINE_LEXER_TOKEN( CONST_FUNCTION ), \
DEFINE_LEXER_TOKEN( CONST_ARRAY ), \
\
DEFINE_LEXER_TOKEN( FAILED ), \
DEFINE_LEXER_TOKEN( SUCCESS ), \
DEFINE_LEXER_TOKEN( ERROR_LEXER ), \
DEFINE_LEXER_TOKEN( ERROR_INVALID_OPERATION ), \
DEFINE_LEXER_TOKEN( ERROR_UNSUPPORTED_TYPE ), \
DEFINE_LEXER_TOKEN( ERROR_MISSING_TOKEN ), \
DEFINE_LEXER_TOKEN( ERROR_UNEXPECTED_TOKEN ), \
DEFINE_LEXER_TOKEN( ERROR_UNDEFINED_STATE ), \
DEFINE_LEXER_TOKEN( ERROR_FILE_IO ), \

#endif