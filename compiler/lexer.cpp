#include "lexer.hpp"

bool charIsNumeric( char c )
{ 
    return ( c >= '0' && '9' >= c ); 
}

bool charIsWhiteSpace( char c )
{ 
    return c == ' ' || c == '\t'; 
}

bool charIsId( char c )
{ 
    return 
        ( c >= 'A' && 'Z' >= c ) || 
        ( c >= 'a' && 'z' >= c ) || 
        ( c >= '0' && '9' >= c ) || 
        c == '_'
    ; 
}

bool charIsDelimiter( char c )
{ 
    switch (c)
    {
        case ',':
        case '[':
        case ']':
        case '{':
        case '}':
        case '(':
        case ')':
        case ';':
        case ':':
        case ' ':
        case '\t':
        case '\n':
        case '\r':
        return true;
        break;
    
        default:
        return false;
        break;
    } 
}

bool charIsOperator( char c )
{ 
    switch (c)
    {
        case '=':
        case '+':
        case '-':
        case '/':
        case '*':
        case '^':
        case '%':
        case '<':
        case '>':
        case '!':
        case '~':
        case '&':
        case '|':
        return true;
        break;
    
        default:
        return false;
        break;
    } 
}

Lexer::Token getOpFinalState( char c )
{ 
    if ( charIsId( c ) || charIsDelimiter( c ) )
    {
        return Lexer::_END_TOKEN;
    }
    return Lexer::_ERROR;
}

#define DEFINE_LEXER_TOKEN( s ) #s
const char* Lexer::toString( Lexer::Token tok )
{
    static const char *tokens[] = { LEXER_TOKENS };
    return tokens[ tok ];
};

void Lexer::error( const char* file, const char* func, int ln, Token token, const char* msg )
{
    const char* err = nullptr;
    int line = preprocessor.getCurrentLineNumber();
    switch( token )
    {
        case _ERROR_FILE_IO:
        err = "UNKNOWN_ERROR: file open failed ";
        break;

        default:
        case _ERROR_LEXER:
        err = "UNKNOWN_ERROR";
        break;

        case _ERROR_INVALID_OPERATION:
        err = "INVALID_OPERATION";
        break;

        case _ERROR_MISSING_TOKEN:
        err = "MISSING_TOKEN";
        break;

        case _ERROR_UNEXPECTED_TOKEN:
        err = "UNEXPECTED_TOKEN";
        break;

        case _ERROR_UNDEFINED_STATE:
        err = "UNDEFINED_STATE";
        break;

        case _ERROR_UNSUPPORTED_TYPE:
        err = "UNSUPPORTED_TYPE";
        break;
    }

    std::string errstr = "@file -> " + std::string( file ) + ":  @function -> " + std::string( func ) + " @line ->" + std::to_string( ln );
    std::string errmsg = std::string( "[ ERROR ]: " ) + errstr + " : [ issue ]: @ln -> [" +  std::to_string( line ) + "] "  + err +" -> "+ + msg;
    _error = errmsg;
}


bool Lexer::load( const char* filename )
{
    if( !preprocessor.load( filename ) )
    {
        error( __FILE__, __FUNCTION__, __LINE__, _ERROR_FILE_IO, filename );
        return false;
    }
    preprocessor.startFromBeginnig();
    return true;
}

void Lexer::print()
{
    Token tok = _START_TOKEN;
    const char* lex;
    while ( tok != _END_STREAM )
    {
        tok = getNextToken();
        lex = lexeme.data();
        printf( "%*s | %s \n", 28, toString( tok ), lex ); 
    }
}

Lexer::Token Lexer::getNextToken()
{
    Token token       = _END_STREAM;
    Token state       = _START_TOKEN;
    Token category    = _START_TOKEN;
    int   index       = 0;
    bool  record_char = false;
    char  curr_char;
    char  prev_char;
    err_code = 0;

    preprocessor.setCheckPoint();

    while ( *preprocessor != 0 && state != _END_TOKEN && state != _ERROR )
    {
        prev_char = curr_char;
        curr_char = *preprocessor;
        record_char = true;

        switch ( state )
        {
            case _START_TOKEN:
            {
                switch ( curr_char )
                {
                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9':
                    category = _NUMERIC;
                    state = _INT;
                    token = _INT;
                    break;

                    case '.': 
                    state = _DOUBLE;
                    category = _NUMERIC;
                    break;

                    case '\"': 
                    category = _STRING;
                    state = _STRING;
                    record_char = false;
                    preprocessor++;
                    break;

                    case '@': 
                    state = _AT_SIGN;
                    category = _OPERATOR;
                    break;

                    case '~': 
                    state = _INVERT;
                    category = _OPERATOR;
                    break;

                    case '|': 
                    state = _BAR;
                    category = _OPERATOR;
                    break;

                    case '&': 
                    state = _AMPERSAND;
                    category = _OPERATOR;
                    break;

                    case '>': 
                    state = _GREATER;
                    category = _OPERATOR;
                    break;

                    case '<': 
                    state = _LESS;
                    category = _OPERATOR;
                    break;

                    case '!': state = _NOT;
                    category = _OPERATOR;
                    break;

                    case '=': 
                    state = _EQUAL;
                    category = _OPERATOR;
                    break;

                    case '-': 
                    state = _SUB;
                    category = _OPERATOR;
                    break;

                    case '+': 
                    state = _ADD;
                    category = _OPERATOR;
                    break;

                    case '*': 
                    state = _MUL;
                    category = _OPERATOR;
                    break;

                    case '/': 
                    state = _DIV;
                    category = _OPERATOR;
                    break;

                    case '%': 
                    state = _MOD;
                    category = _OPERATOR;
                    break;

                    case '^': 
                    state = _EXP;
                    category = _OPERATOR;
                    break;

                    case ',': 
                    state = _COMMA;
                    token = _COMMA;
                    category = _DELIMITER;
                    break;

                    case ':': 
                    state = _COLON;
                    token = _COLON;
                    category = _DELIMITER;
                    break;

                    case ';': 
                    state = _SEMICOLON;
                    token = _SEMICOLON;
                    category = _DELIMITER;
                    break;

                    case '{': 
                    state = _OPEN_CURLY_BRACKET;
                    token = _OPEN_CURLY_BRACKET;
                    category = _DELIMITER;
                    break;

                    case '}': 
                    state = _CLOSE_CURLY_BRACKET;
                    token = _CLOSE_CURLY_BRACKET;
                    category = _DELIMITER;
                    break;

                    case '[': 
                    state = _OPEN_SQUARE_BRACKET;
                    token = _OPEN_SQUARE_BRACKET;
                    category = _DELIMITER;
                    break;

                    case ']': 
                    state = _CLOSE_SQUARE_BRACKET;
                    token = _CLOSE_SQUARE_BRACKET;
                    category = _DELIMITER;
                    break;

                    case '(': 
                    state = _OPEN_PARENTHESIS;
                    token = _OPEN_PARENTHESIS;
                    category = _DELIMITER;
                    break;

                    case ')': 
                    state = _CLOSE_PARENTHESIS;
                    token = _CLOSE_PARENTHESIS;
                    category = _DELIMITER;
                    break;

                    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': 
                    case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
                    case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
                    case 'v': case 'w': case 'x': case 'y': case 'z':
                    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': 
                    case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
                    case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
                    case 'V': case 'W': case 'X': case 'Y': case 'Z':
                    case '_':
                    category = _IDENTIFIER;
                    state = _IDENTIFIER;
                    token = _IDENTIFIER;
                    break;

                    case ' ': case '\t':
                    case '\n': case '\r':
                    state = _START_TOKEN;
                    record_char = false;
                    break;
                    
                    default:
                    state = _ERROR;
                    err_code = 0;
                    break;
                }
            }
            break;

            case _EXP:
            {
                category = _OPERATOR; 
                if ( prev_char == '^' && curr_char == '=' )
                {
                    state = _EXP_EQUAL;
                    token = _EXP_EQUAL;
                }
                else 
                {
                    state = _END_TOKEN;
                    token = _EXP;
                }
            }
            break;

            case _MOD:
            {
                category = _OPERATOR; 
                if ( prev_char == '%' && curr_char == '=' )
                {
                    state = _MOD_EQUAL;
                    token = _MOD_EQUAL;
                }
                else 
                {
                    state = _END_TOKEN;
                    token = _MOD;
                }
            }
            break;

            case _XOR:
            {
                if ( prev_char == '^' && curr_char == '=' )
                {
                    state = _XOR_EQUAL;
                    token = _XOR_EQUAL;
                }
                else 
                {
                    state = _END_TOKEN;
                    token = _XOR;
                }
            }
            break;

            case _INVERT:
            {
                if ( prev_char == '~' && curr_char == '=' )
                {
                    state = _INVERT_EQUAL;
                    token = _INVERT_EQUAL;
                }
                else 
                {
                    state = _END_TOKEN;
                    token = _INVERT;
                }
            }
            break;

            case _AT_SIGN:
            {
                if ( charIsId( curr_char ) )
                {
                    state = _END_TOKEN;
                    record_char = false;
                    token = _ADDRESS;
                }
                else 
                {
                    state = _ERROR;
                    token = _END_STREAM;
                }
            }
            break;

            case _LESS:
            {
                if ( prev_char == '<' && curr_char == '=' )
                {
                    state = _LESS_EQUAL;
                    token = _LESS_EQUAL;
                }
                else if ( prev_char == '<' && curr_char == '<' )
                {
                    state = _SHIFT_LEFT;
                    token = _SHIFT_LEFT_EQUAL;
                    category = _OPERATOR;
                }
                else 
                {
                    state = _END_TOKEN;
                    token = _LESS;
                }
            }
            break;

            case _SHIFT_LEFT:
            {
                if ( curr_char == '=' )
                {
                    state = _SHIFT_LEFT_EQUAL;
                    token = _SHIFT_LEFT_EQUAL;
                }
                else 
                {
                    state = _END_TOKEN;
                    token = _SHIFT_LEFT;
                }
            }
            break;

            case _GREATER:
            {
                if ( prev_char == '>' && curr_char == '=' )
                {
                    state = _GREATER_EQUAL;
                    token = _GREATER_EQUAL;
                }
                else if ( prev_char == '>' && curr_char == '>' )
                {
                    state = _SHIFT_RIGHT;
                    token = _SHIFT_RIGHT_EQUAL;
                    category = _OPERATOR;
                }
                else
                {
                    state = _END_TOKEN;
                    token = _GREATER;
                }
            }
            break;

            case _SHIFT_RIGHT:
            {
                if ( curr_char == '=' )
                {
                    state = _SHIFT_RIGHT_EQUAL;
                    token = _SHIFT_RIGHT_EQUAL;
                }
                else 
                {
                    state = _END_TOKEN;
                    token = _SHIFT_RIGHT;
                }
            }
            break;

            case _EQUAL:
            {
                if ( prev_char == '=' && curr_char == '=' )
                {
                    state = _EQUAL_EQUAL;
                    token = _EQUAL_EQUAL;
                    category = _OPERATOR;
                }
                else
                {
                    state = _END_TOKEN;
                    record_char = false;
                    token = _EQUAL;
                }
            }
            break;

            case _AMPERSAND:
            {
                if ( prev_char == '&' && curr_char == '=' )
                {
                    category = _OPERATOR;
                    state = _AND_EQUAL;
                    token = _AND_EQUAL;
                }
                else if ( prev_char == '&' && curr_char == '&' )
                {
                    category = _OPERATOR;
                    state = _AND;
                    token = _AND;
                }
                else
                {
                    category = _OPERATOR;
                    state = _END_TOKEN;
                    token = _AMPERSAND;
                }
            }
            break;

            case _BAR:
            {
                if ( prev_char == '|' && curr_char == '=' )
                {
                    category = _OPERATOR;
                    state = _OR_EQUAL;
                    token = _OR_EQUAL;
                }
                if ( prev_char == '|' && curr_char == '|' )
                {
                    category = _OPERATOR;
                    state = _OR;
                    token = _OR;
                }
                else
                {
                    category = _OPERATOR;
                    state = _END_TOKEN;
                    token = _BAR;
                }
            }
            break;

            case _NOT:
            {
                if ( prev_char == '!' && curr_char == '=' )
                {
                    category = _OPERATOR;
                    state = _NOT_EQUAL;
                    token = _NOT_EQUAL;
                }
                else
                {
                    category = _OPERATOR;
                    state = _END_TOKEN;
                    token = _NOT;
                }
            }
            break;

            case _INT:
            {
                if ( charIsNumeric( curr_char ) )
                {
                    state = _INT;
                }
                else if ( curr_char == '.' )
                {
                    token = _DOUBLE;
                    state = _DOUBLE;
                }
                else
                {
                    state = _END_TOKEN;
                }
            }
            break;

            case _DOUBLE:
            {
                if ( charIsNumeric( curr_char ) )
                {
                    state = _DOUBLE;
                }
                else if ( curr_char == 'e' || curr_char == 'E' )
                {
                    state = _DOUBLE_E;
                }
                else if ( curr_char == 'f' )
                {
                    token = _FLOAT;
                    state = _FLOAT;
                }
                else
                {
                    state = _END_TOKEN;
                }
            }
            break;

            case _DOUBLE_E:
            {
                token = _FLOAT;
                if ( charIsNumeric( curr_char ) )
                {
                    state = _DOUBLE_E;
                }
                else if ( curr_char == '+' || curr_char == '-' )
                {
                    state = _DOUBLE_EX;
                }
                else
                {
                    state = _END_TOKEN;
                }
            }

            case _DOUBLE_EX:
            {
                token = _FLOAT;
                if ( charIsNumeric( curr_char ) )
                {
                    state = _DOUBLE_EX;
                }
                else if ( curr_char == 'f' )
                {
                    state = _FLOAT;
                }
                else
                {
                    state = _END_TOKEN;
                }
            }
            break;

            case _FLOAT:
            {
                if ( charIsDelimiter( curr_char ) )
                {
                    state = _END_TOKEN;
                    token = _FLOAT;
                }
                else 
                {
                    state = _ERROR;
                }
            }
            break;

            case _BLOCK_COMMENT:
            {
                if ( prev_char == '*' || curr_char == '/' )
                {
                    state = _START_TOKEN;
                }
                else 
                {
                    state = _BLOCK_COMMENT;
                }
                record_char = false;
                if ( curr_char != '\n' ) curr_char = ' ';
            }
            break;

            case _INLINE_COMMENT:
            {
                state = _INLINE_COMMENT;
                if ( curr_char == '\n' )
                {
                    state = _START_TOKEN;
                }
                else 
                { 
                    curr_char = ' ';
                }
                record_char = false;
            }
            break;

            case _DIV:
            {
                if ( prev_char == '/' && curr_char == '=' )
                {
                    token = _DIV_EQUAL;
                    state = _DIV_EQUAL;
                }
                else if ( curr_char == '/' && prev_char == '/' )
                {
                    state = _INLINE_COMMENT;
                    record_char = false;
                    curr_char = '\"';
                    index = 0;
                }
                else if ( prev_char == '/' && curr_char == '*' )
                {
                    state = _BLOCK_COMMENT;
                    record_char = false;
                    curr_char = '\"';
                    index = 0;
                }
                else 
                {
                    token = _DIV;
                    state = getOpFinalState( curr_char );
                }
            }
            break;

            case _MUL:
            {
                if ( prev_char == '*' && curr_char == '=' )
                {
                    token = _MUL_EQUAL;
                    state = _MUL_EQUAL;
                }
                if ( charIsOperator( curr_char ) )
                {
                    token = _MUL_EQUAL;
                    state = _ERROR;
                }
                else 
                {
                    token = _MUL;
                    state = getOpFinalState( curr_char );
                }
            }
            break;

            case _ADD:
            {
                if ( curr_char == '.' )
                {
                    state = _DOUBLE;
                }
                else if ( curr_char == '+' )
                {
                    token = _INCREMENT;
                    state = _INCREMENT;
                }
                else if ( curr_char == '=' )
                {
                    token = _ADD_EQUAL;
                    state = _ADD_EQUAL;
                }
                else if ( charIsNumeric( curr_char ) )
                {
                    state = _INT;
                    token = _INT;
                }
                else 
                {
                    token = _ADD;
                    state = getOpFinalState( curr_char );
                }
            }
            break;

            case _SUB:
            {
                if ( curr_char == '.' )
                {
                    state = _DOUBLE;
                }
                else if ( curr_char == '-' )
                {
                    token = _DECREMENT;
                    state = _DECREMENT;
                }
                else if ( curr_char == '=' )
                {
                    token = _SUB_EQUAL;
                    state = _SUB_EQUAL;
                }
                else if ( charIsNumeric( curr_char ) )
                {
                    state = _INT;
                    token = _INT;
                }
                else 
                {
                    token = _SUB;
                    state = getOpFinalState( curr_char );
                }
            }
            break;

            case _IDENTIFIER:
            {
                if ( charIsId( curr_char ) )
                {
                    state = _IDENTIFIER;
                }
                else if ( charIsDelimiter( curr_char ) )
                {
                    state = _END_TOKEN;
                }
                else if ( charIsOperator( curr_char ) )
                {
                    state = _END_TOKEN;
                }
                else 
                {
                    state = _ERROR;
                }
            }
            break;

            case _STRING:
            {
                if( prev_char == '\\' && curr_char == '\"' ) 
                {
                    state = _STRING_ESC;
                }
                else if( curr_char == '\"') 
                {
                    state = _END_TOKEN;
                    token = _STRING;
                    record_char = false;
                    preprocessor++;
                }
            }
            break;

            case _STRING_ESC:
            {
                if( prev_char == '\\' && curr_char == '\"' ) 
                {
                    state = _STRING;
                }
            }
            break;

            case _EXP_EQUAL:
            case _SHIFT_RIGHT_EQUAL:
            case _SHIFT_LEFT_EQUAL:
            case _INVERT_EQUAL:
            case _AND_EQUAL:
            case _OR_EQUAL:
            case _XOR_EQUAL:
            case _EQUAL_EQUAL:
            case _NOT_EQUAL:
            case _LESS_EQUAL:
            case _GREATER_EQUAL:
            case _AND:
            case _OR:
            case _ADD_EQUAL:
            case _SUB_EQUAL:
            case _MUL_EQUAL:
            case _DIV_EQUAL:
            case _MOD_EQUAL:
            case _INCREMENT:
            case _DECREMENT:
            case _COMMA:
            case _COLON:
            case _SEMICOLON:
            case _OPEN_CURLY_BRACKET:
            case _CLOSE_CURLY_BRACKET:
            case _OPEN_PARENTHESIS:
            case _CLOSE_PARENTHESIS:
            case _OPEN_SQUARE_BRACKET:
            case _CLOSE_SQUARE_BRACKET:
            {
                state = _END_TOKEN;
            }
            break;

            default: 
            state = _ERROR; 
            break;
        }

        if( record_char && state != _END_TOKEN )
        {
            lexeme[ index++ ] = curr_char;
            preprocessor++;
        }
        else if( charIsWhiteSpace( curr_char ) )
        {
            preprocessor++;
        }
        else if( curr_char == '\n' || curr_char == '\r' )
        {
            line_count++;
            preprocessor++;
        }
    }
    lexeme[ index ] = 0;

    if ( state == _ERROR )
    {
        error( __FILE__, __FUNCTION__, __LINE__, _ERROR_UNDEFINED_STATE );
    }
    
    if( token == _IDENTIFIER )
    {
        Token* t = token_table.find( lexeme.data() );
        if( t != nullptr )
        {
            category = _KEYWORD;
            token = *t;
        }
    }
    return token;
}

void Lexer::revertToken()
{
    preprocessor.gotoCheckPoint();
}

int Lexer::getCurrLine()
{
    return preprocessor.getCurrentLineNumber();
}

const char* Lexer::getLexeme()
{
    return lexeme.data();
}

void Lexer::clear()
{
    lexeme.clear();
    preprocessor.clear();
    file_bufr.clear();
    token_table.clear();
}

Lexer:: ~Lexer()
{
    clear();
}

Lexer::Lexer()
{
    lexeme.resize( 128 );
    token_table[ "null"     ] = _NULL;
    token_table[ "const"    ] = _CONST;
    token_table[ "int"      ] = _VAR_INT;
    token_table[ "long"     ] = _VAR_LONG;
    token_table[ "float"    ] = _VAR_FLOAT;
    token_table[ "double"   ] = _VAR_DOUBLE;
    token_table[ "string"   ] = _VAR_STRING;
    token_table[ "true"     ] = _TRUE;
    token_table[ "false"    ] = _FALSE;
    token_table[ "if"       ] = _IF;
    token_table[ "else"     ] = _ELSE;
    token_table[ "break"    ] = _BREAK;
    token_table[ "continue" ] = _CONTINUE;
    token_table[ "for"      ] = _FOR;
    token_table[ "while"    ] = _WHILE;
    token_table[ "function" ] = _FUNCTION;
    token_table[ "return"   ] = _RETURN;
    token_table[ "host"     ] = _HOST;
    token_table[ "switch"   ] = _SWITCH;
    token_table[ "case"     ] = _SWITCH_CASE;
    token_table[ "default"  ] = _SWITCH_DEFAULT;
}