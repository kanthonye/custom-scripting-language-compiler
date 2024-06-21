#include "parser.hpp"

bool parseIndexedArray( Tree& stmt, Lexer* lexer, Lexer::Token& token );
bool parseStmt( Tree& stmt, Lexer* lexer, Lexer::Token& token );
bool parseE( Tree& stmt, Lexer* lexer, Lexer::Token& token );
bool parseFuncCall( Tree& var, Lexer* lexer, Lexer::Token& token );
bool parseFunction( Tree& stmt, Lexer* lexer, Lexer::Token& token );
bool parseExpr( Tree& expr, Lexer* lexer, Lexer::Token& token );
bool parseScopeBlock( Tree& stmt, Lexer* lexer, Lexer::Token& token );
bool boolTerm( Tree& stmt, Lexer* lexer, Lexer::Token& token );
bool parseWhile( Tree& stmt, Lexer* lexer, Lexer::Token& token );
bool parseFor( Tree& stmt, Lexer* lexer, Lexer::Token& token );
bool parseIf( Tree& stmt, Lexer* lexer, Lexer::Token& token );
bool parseVar( Tree& stmt,  Lexer* lexer, Lexer::Token& token );

bool parseExprId( Tree& stmt, Lexer* lexer, Lexer::Token& token );
bool parseStmtId( Tree& stmt, Lexer* lexer, Lexer::Token& token, int ln );

#define SYNTAX_ERROR(lexer, err, tok) syntaxError( __FUNCTION__, __LINE__, err, lexer->toString( tok ), lexer->getCurrLine() )


void syntaxError( const char* func, int file_line, Lexer::Token token, const char* msg, int line )
{
    const char* err = nullptr;
    std::string errstr = ":  @function -> " + std::string( func ) + " @line ->" + std::to_string( file_line );
    std::string errmsg = std::string( "[ Parser-ERROR ]: " ) + errstr + " : [ issue ]: @ln -> [" +  std::to_string( line ) + "] "  + Lexer::toString( token ) +" -> " + msg;
    throw std::runtime_error( errmsg );
}

int precedence( int op )
{
    switch ( op )
    {
        case Lexer::_OR:
        return 1;

        case Lexer::_AND:
        return 2;

        case Lexer::_LESS:
        case Lexer::_GREATER:
        case Lexer::_NOT_EQUAL:
        case Lexer::_EQUAL_EQUAL:
        case Lexer::_LESS_EQUAL:
        case Lexer::_GREATER_EQUAL:
        return 3;

        case Lexer::_ADD:
        case Lexer::_SUB:
        return 4;

        case Lexer::_MOD:
        return 5;

        case Lexer::_MUL:
        case Lexer::_DIV:
        return 6;

        case Lexer::_EXP:
        case Lexer::_NOT:
        case Lexer::_DECREMENT:
        case Lexer::_INCREMENT:
        return 7;

        default:
        return 0;
    }
}

void resolve( std::stack< Tree >& operators, std::stack< Tree >& operands )
{
    Tree a, b, op;
    if ( operands.size() <= operators.size() )
    {
        // Pop two operands and one operator
        b = operands.top();
        operands.pop();

        op = operators.top();
        operators.pop();

        op->push( b );
        operands.push( op );
    }
    else 
    {
        // Pop two operands and one operator
        b = operands.top();
        operands.pop();

        a = operands.top();
        operands.pop();

        op = operators.top();
        operators.pop();
        
        op->push( a );
        op->push( b );
        operands.push( op );
    }
}

bool parseIndexedArray( Tree& stmt, Lexer* lexer, Lexer::Token& token )
{
    Tree e;
    int ln = lexer->getCurrLine();
    token = lexer->getNextToken();
    if( !parseExpr( e, lexer, token ) )
    {
        return false;
    } 

    if ( token != Lexer::_CLOSE_SQUARE_BRACKET )
    {
        SYNTAX_ERROR( lexer, Lexer::_ERROR_MISSING_TOKEN, Lexer::_CLOSE_SQUARE_BRACKET );
        return false;
    }

    Tree arr = new Tree::Node( ln, Lexer::_ENCLOSE_SQUARE_BRACKET );
    token = lexer->getNextToken();
    stmt->push( arr );
    arr->push( e );
    return true;
}

bool parseOp( Tree& right, Lexer* lexer, Lexer::Token& token )
{
    Tree op = new Tree::Node( lexer->getCurrLine(), token ); 
    Tree left;
    
    if ( !parseExpr( left, lexer, token ) )
    {
        return false;
    }
    
    op->push( right );
    op->push( left ); 
    right = op;
    return true; 
}

bool parseId( Tree& stmt, Lexer* lexer, Lexer::Token& token )
{
    stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_IDENTIFIER, lexer->getLexeme() );
    token = lexer->getNextToken();

    switch( token )
    {   
        case Lexer::_OPEN_PARENTHESIS:
        {
            return parseFuncCall( stmt, lexer, token );
        }   
        break;

        case Lexer::_OPEN_SQUARE_BRACKET:
        {
            return parseIndexedArray( stmt, lexer, token );
        }   
        break;

        case Lexer::_INCREMENT:
        case Lexer::_DECREMENT:
        {
            stmt->push( new Tree::Node( lexer->getCurrLine(), token ) ); 
            token = lexer->getNextToken();
            return true;
        }   
        break;

        default:
        {
            //SYNTAX_ERROR( lexer, Lexer::_ERROR_UNEXPECTED_TOKEN, token );
            return true;
        }
        break;
    }
}

bool parseVar( Tree& var, Lexer* lexer, Lexer::Token& token )
{
    int ln = lexer->getCurrLine();
    switch( token )
    {
        case Lexer::_VAR_INT:
        {
            token = lexer->getNextToken();
            var = new Tree::Node( ln, Lexer::_VAR_INT );
        }
        break;

        case Lexer::_VAR_LONG:
        {
            token = lexer->getNextToken();
            var = new Tree::Node( ln, Lexer::_VAR_LONG );
        }
        break;

        case Lexer::_VAR_FLOAT:
        {
            token = lexer->getNextToken();
            var = new Tree::Node( ln, Lexer::_VAR_FLOAT );
        }
        break;

        case Lexer::_VAR_DOUBLE:
        {
            token = lexer->getNextToken();
            var = new Tree::Node( ln, Lexer::_VAR_DOUBLE );
        }
        break;

        case Lexer::_VAR_STRING:
        {
            token = lexer->getNextToken();
            var = new Tree::Node( ln, Lexer::_VAR_STRING );
        }
        break;

        default:
        {
            SYNTAX_ERROR( lexer, Lexer::_ERROR_UNSUPPORTED_TYPE, token );
            return false;
        }
        break;
    }

    if( token == Lexer::_OPEN_PARENTHESIS )
    {
        Tree v;
        if ( !parseExpr( v, lexer, token ) )
        {
            return false;
        }
        var->push( v );
    }
    return true;
}

bool parseLiteral( Tree& type, Lexer* lexer, Lexer::Token& token )
{
    int ln = lexer->getCurrLine();
    switch( token )
    {
        case Lexer::_NULL:
        {
            token = lexer->getNextToken();
            type = new Tree::Node( ln, Lexer::_NULL );
        }
        break;

        case Lexer::_FALSE:
        {
            type = new Tree::Node( ln, Lexer::_VAR_BOOL, lexer->getLexeme() );
            token = lexer->getNextToken();
        }
        break;

        case Lexer::_TRUE:
        {
            type = new Tree::Node( ln, Lexer::_VAR_BOOL, lexer->getLexeme() );
            token = lexer->getNextToken();
        }
        break;

        case Lexer::_INT: 
        {
            type = new Tree::Node( ln, Lexer::_INT, lexer->getLexeme() );
            token = lexer->getNextToken();
        }
        break;

        case Lexer::_LONG: 
        {
            type = new Tree::Node( ln, Lexer::_LONG, lexer->getLexeme() );
            token = lexer->getNextToken();
        }
        break;

        case Lexer::_FLOAT: 
        {
            type = new Tree::Node( ln, Lexer::_FLOAT, lexer->getLexeme() );
            token = lexer->getNextToken();
        }
        break;

        case Lexer::_DOUBLE: 
        {
            type = new Tree::Node( ln, Lexer::_DOUBLE, lexer->getLexeme() );
            token = lexer->getNextToken();
        }
        break;

        case Lexer::_STRING:
        {
            type = new Tree::Node( ln, Lexer::_STRING, lexer->getLexeme() );
            token = lexer->getNextToken();
        }
        break;

        default:
        {
            SYNTAX_ERROR( lexer, Lexer::_ERROR_UNSUPPORTED_TYPE, token );
            return false;
        }
        break;
    }
    return true;
}

bool parseBool( Tree& type, Lexer* lexer, Lexer::Token& token )
{
    int ln = lexer->getCurrLine();
    switch( token )
    {
        case Lexer::_FALSE:
        {
            type = new Tree::Node( ln, Lexer::_VAR_BOOL, lexer->getLexeme() );
            token = lexer->getNextToken();
        }
        break;

        case Lexer::_TRUE:
        {
            type = new Tree::Node( ln, Lexer::_VAR_BOOL, lexer->getLexeme() );
            token = lexer->getNextToken();
        }
        break;

        default:
        {
            SYNTAX_ERROR( lexer, Lexer::_ERROR_UNSUPPORTED_TYPE, token );
            return false;
        }
        break;
    }
    return true;
}

bool parseBody( Tree& stmt, Lexer* lexer, Lexer::Token& token )
{
    Tree block;
    if( !parseScopeBlock( block, lexer, token ) )
    {
        return false;
    }
    stmt->push( block );
    return true;
}

bool parseFor( Tree& stmt, Lexer* lexer, Lexer::Token& token )
{
    if ( token == Lexer::_OPEN_PARENTHESIS )
    {
        stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_FOR );

        /**
         * for-loop ENCLOSE_PARENTHESISal statements
         **/
        Tree params = new Tree::Node( lexer->getCurrLine(), Lexer::_FORLOOP_PARAMS );
        token = lexer->getNextToken();
        while ( token != Lexer::_CLOSE_PARENTHESIS )
        {
            Tree e;
            if( !parseExpr( e, lexer, token ) )
            {
                return false;
            }

            params->push( e );
            if ( token != Lexer::_CLOSE_PARENTHESIS )
            {
                token = lexer->getNextToken();
            }
        }
        token = lexer->getNextToken();
        stmt->push( params );

        if( !parseBody( stmt, lexer, token ) )
        {
            return false; 
        }
        return true;    
    } 
    SYNTAX_ERROR( lexer, Lexer::_ERROR_MISSING_TOKEN, Lexer::_OPEN_PARENTHESIS );
    return false;
}

bool parseEncloseParenthesis( Tree& stmt, Lexer* lexer, Lexer::Token& token )
{
    int ln =lexer->getCurrLine();
    token = lexer->getNextToken();
    Tree e;
    if ( !parseExpr( e, lexer, token ) )
    {
        return false;
    }
    if ( token != Lexer::_CLOSE_PARENTHESIS )
    {
        SYNTAX_ERROR( lexer, Lexer::_ERROR_MISSING_TOKEN, Lexer::_CLOSE_PARENTHESIS );
        return false;
    }
    Tree ENCLOSE_PARENTHESIS = new Tree::Node( ln, Lexer::_ENCLOSE_PARENTHESIS );
    token = lexer->getNextToken();
    ENCLOSE_PARENTHESIS->push( e );
    stmt->push( ENCLOSE_PARENTHESIS );
    return true;
}

bool parseWhile( Tree& stmt, Lexer* lexer, Lexer::Token& token )
{
    if ( token == Lexer::_OPEN_PARENTHESIS )
    {
        stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_WHILE );
        if ( !parseEncloseParenthesis( stmt, lexer, token ) )
        {
            return false;
        }
        if ( !parseBody( stmt, lexer, token ) )
        {
            return false;
        }
        return true;    
    } 
    SYNTAX_ERROR( lexer, Lexer::_ERROR_MISSING_TOKEN, Lexer::_OPEN_PARENTHESIS );
    return false;
}

bool parseIf( Tree& stmt, Lexer* lexer, Lexer::Token& token )
{
    token = lexer->getNextToken();
    if ( token == Lexer::_OPEN_PARENTHESIS )
    {
        Tree iff = new Tree::Node( lexer->getCurrLine(), Lexer::_IF );
        stmt->push( iff );

        if ( !parseEncloseParenthesis( iff, lexer, token ) )
        {
            return false;
        }
        if ( !parseBody( iff, lexer, token ) )
        {
            return false;
        }
        return true;    
    }
    SYNTAX_ERROR( lexer, Lexer::_ERROR_MISSING_TOKEN, Lexer::_OPEN_PARENTHESIS );
    return false;
}

bool parseElse( Tree& stmt, Lexer* lexer, Lexer::Token& token )
{
    Tree els = new Tree::Node( lexer->getCurrLine(), Lexer::_ELSE );
    token = lexer->getNextToken();
    stmt->push( els );

    if ( token == Lexer::_IF )
    {
        Tree iff = new Tree::Node( lexer->getCurrLine(), Lexer::_IF );
        token = lexer->getNextToken();
        els->push( iff );

        if ( !parseEncloseParenthesis( iff, lexer, token ) )
        {
            return false;
        }
        if ( !parseBody( iff, lexer, token ) )
        {
            return false;
        } 
    }
    else if ( token == Lexer::_OPEN_CURLY_BRACKET )
    {
        if ( !parseBody( els, lexer, token ) )
        {
            return false;
        }
    }
    return true;
}

bool parseFuncCall( Tree& var, Lexer* lexer, Lexer::Token& token )
{
    token = lexer->getNextToken();
    var->node_type = Lexer::_FUNCTION_CALL;
    if ( token != Lexer::_CLOSE_PARENTHESIS )
    {    
        Tree e;
        if ( !parseExpr( e, lexer, token ) )
        {
            return false;
        }
        var->push( e );

        while ( token == Lexer::_COMMA )
        {
            token = lexer->getNextToken();
            if ( !parseExpr( e, lexer, token ) )
            {
                return false;
            }
            var->push( e );
        }

        if ( token != Lexer::_CLOSE_PARENTHESIS ) 
        {
            SYNTAX_ERROR( lexer, Lexer::_ERROR_MISSING_TOKEN, Lexer::_CLOSE_PARENTHESIS );
            return false;
        }
        token = lexer->getNextToken();
    }
    return true;
}

bool parseScopeBlock( Tree& block, Lexer* lexer, Lexer::Token& token )
{
    block = new Tree::Node( lexer->getCurrLine(), Lexer::_ENCLOSE_CURLY_BRACKET );
    if ( token == Lexer::_OPEN_CURLY_BRACKET )
    {
        token = lexer->getNextToken();

        while ( token != Lexer::_CLOSE_CURLY_BRACKET && token != Lexer::_END_STREAM )
        {
            Tree s;
            if( !parseStmt( s, lexer, token ) )
            {
                return false;
            }
            if( s )
            {
                block->push( s );
            }
        }

        if ( token != Lexer::_CLOSE_CURLY_BRACKET )
        {
            SYNTAX_ERROR( lexer, Lexer::_ERROR_MISSING_TOKEN, Lexer::_CLOSE_CURLY_BRACKET );
            return false;
        }
        token = lexer->getNextToken();
    }
    else if ( token == Lexer::_SEMICOLON )
    {
        token = lexer->getNextToken();
    }
    else 
    {
        Tree s;
        if( !parseStmt( s, lexer, token ) )
        {
            return false;
        }
        block->push( s );
    }
    return true;
}

bool parseExprConst( Tree& expr, Lexer* lexer, Lexer::Token& token )
{
    Tree var = new Tree::Node( lexer->getCurrLine(), Lexer::_CONST );
    token = lexer->getNextToken();

    switch ( token )
    {
        case Lexer::_VAR_STRING:
        case Lexer::_VAR_DOUBLE:
        case Lexer::_VAR_FLOAT:
        case Lexer::_VAR_LONG:
        case Lexer::_VAR_INT:
        {
            if( !parseVar( expr, lexer, token ) )
            {
                return false;
            }
            var->push( expr );
            expr = var;
            return true;
        }
        break;

        case Lexer::_FUNCTION:
        {
            if( !parseFunction( expr, lexer, token ) )
            {
                return false;
            }
            var->push( expr );
            expr = var;
            return true;
        }
    
        default:
        {
            SYNTAX_ERROR( lexer, Lexer::_ERROR_UNEXPECTED_TOKEN, token );
            return false;
        }
    }
}

bool parseExprId( Tree& stmt, Lexer* lexer, Lexer::Token& token )
{
    if( parseId( stmt, lexer, token ) )
    {    
        int ln = lexer->getCurrLine();
        switch ( token )
        {
            case Lexer::_EQUAL:
            {
                token = lexer->getNextToken();
                Tree expr;

                if( !parseExpr( expr, lexer, token ) )
                {
                    return false;
                }

                Tree assign = new Tree::Node( ln, Lexer::_ASSIGN );
                assign->push( stmt );
                assign->push( expr );
                stmt = assign;
                return true;
            }

            case Lexer::_COLON:
            {
                token = lexer->getNextToken();
                Tree expr;

                if( !parseExpr( expr, lexer, token ) )
                {
                    return false;
                }

                Tree declar = new Tree::Node( ln, Lexer::_DECLARATION );
                declar->push( stmt );
                declar->push( expr );
                stmt = declar;
                return true;
            }
            
            default:
            {
                return true;
            }
        }
    }
    return false;
}

bool parseExprArray( Tree& stmt, Lexer* lexer, Lexer::Token& token )
{
    stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_ENCLOSE_SQUARE_BRACKET );
    token = lexer->getNextToken();
    Tree e;

    if( token == Lexer::_CLOSE_SQUARE_BRACKET )
    {
        token = lexer->getNextToken();
        return true;
    }

    if( !parseExpr( e, lexer, token ) )
    {
        return false;
    }
    stmt->push( e );

    while ( token == Lexer::_COMMA )
    {
        token = lexer->getNextToken();
        if( !parseExpr( e, lexer, token ) )
        {
            return false;
        }
        stmt->push( e );
    }

    if( token != Lexer::_CLOSE_SQUARE_BRACKET )
    {
        SYNTAX_ERROR( lexer, Lexer::_ERROR_MISSING_TOKEN, Lexer::_CLOSE_SQUARE_BRACKET );
        return false;
    }

    token = lexer->getNextToken();
    return true;
}

bool parseExprParen( Tree& stmt, Lexer* lexer, Lexer::Token& token )
{
    int ln = lexer->getCurrLine();
    token = lexer->getNextToken();
    Tree e;

    if( !parseExpr( e, lexer, token ) )
    {
        return false;
    }

    stmt = new Tree::Node( ln, Lexer::_ENCLOSE_PARENTHESIS );
    if( token == Lexer::_EQUAL )
    {
        Tree r;
        token = lexer->getNextToken();
        if( !parseExpr( r, lexer, token ) )
        {
            return false;
        }

        if ( token != Lexer::_CLOSE_PARENTHESIS )
        {
            SYNTAX_ERROR( lexer, Lexer::_ERROR_MISSING_TOKEN, Lexer::_CLOSE_PARENTHESIS );
            return false;
        }

        ln = lexer->getCurrLine();
        token = lexer->getNextToken();
        
        Tree assign = new Tree::Node( ln, Lexer::_ASSIGN );
        assign->push( e );
        assign->push( r );
        stmt->push( assign );

        return true;
    }
    else
    {
        stmt->push( e );
    }


    while ( token == Lexer::_COMMA )
    {
        token = lexer->getNextToken();
        if( !parseExpr( e, lexer, token ) )
        {
            return false;
        }
        stmt->push( e );
    }

    if( token != Lexer::_CLOSE_PARENTHESIS )
    {
        SYNTAX_ERROR( lexer, Lexer::_ERROR_MISSING_TOKEN, Lexer::_CLOSE_PARENTHESIS );
        return false;
    }

    token = lexer->getNextToken();
    return true;
}


bool parseParam( Tree& stmt, Lexer* lexer, Lexer::Token& token )
{
    int ln = lexer->getCurrLine();
    switch ( token )
    {
        case Lexer::_IDENTIFIER:
        {
            return parseExprId( stmt, lexer, token );
        }

        case Lexer::_ADDRESS:
        {
            token = lexer->getNextToken();
            if ( !parseParam( stmt, lexer, token ) )
            {
                return false;
            }
            Tree addr = new Tree::Node( ln, Lexer::_ADDRESS );
            addr->push( stmt );
            stmt = addr;
            return true;
        }

        case Lexer::_CONST:
        {
            token = lexer->getNextToken();
            if ( !parseParam( stmt, lexer, token ) )
            {
                return false;
            }
            Tree cstmt = new Tree::Node( ln, Lexer::_CONST );
            cstmt->push( stmt );
            stmt = cstmt;
            return true;
        }

        default:
        break;
    }
    SYNTAX_ERROR( lexer, Lexer::_ERROR_UNEXPECTED_TOKEN, token );
    return false;
}

bool parseParams( Tree& stmt, Lexer* lexer, Lexer::Token& token )
{
    int ln = lexer->getCurrLine();
    token = lexer->getNextToken();
    Tree e;

    if( !parseParam( e, lexer, token ) )
    {
        return false;
    }

    Tree params = new Tree::Node( ln, Lexer::_ENCLOSE_PARENTHESIS );
    stmt->push( params );
    params->push( e );

    while ( token == Lexer::_COMMA )
    {
        token = lexer->getNextToken();
        if( !parseParam( e, lexer, token ) )
        {
            return false;
        }
        params->push( e );
    }

    if( token != Lexer::_CLOSE_PARENTHESIS )
    {
        SYNTAX_ERROR( lexer, Lexer::_ERROR_MISSING_TOKEN, Lexer::_CLOSE_PARENTHESIS );
        return false;
    }

    token = lexer->getNextToken();
    return true;
}

bool parseFunction( Tree& function, Lexer* lexer, Lexer::Token& token )
{
    function = new Tree::Node( lexer->getCurrLine(), Lexer::_FUNCTION );
    token = lexer->getNextToken();

    if( parseParams( function, lexer, token ) )
    {
        if( !parseBody( function, lexer, token ) )
        {
            return false;
        }
        return true;
    }
    SYNTAX_ERROR( lexer, Lexer::_ERROR_UNEXPECTED_TOKEN, token );
    return false;
}

bool parseHost( Tree& function, Lexer* lexer, Lexer::Token& token )
{
    function = new Tree::Node( lexer->getCurrLine(), Lexer::_HOST );
    token = lexer->getNextToken();

    Tree params;
    if( !parseExprParen( params, lexer, token ) )
    {
        return false;
    }
    function->push( params );
    return true;
}

bool parseExpr( Tree& expr, Lexer* lexer, Lexer::Token& token )
{
    std::stack< Tree > operators; // Stack to hold operators
    std::stack< Tree > operands;  // Stack to hold operands
    int ln = lexer->getCurrLine();
    bool op = false;

    Tree e;
    while 
    ( 
        token != Lexer::_COMMA &&
        token != Lexer::_CLOSE_SQUARE_BRACKET &&
        token != Lexer::_CLOSE_CURLY_BRACKET && 
        token != Lexer::_CLOSE_PARENTHESIS &&
        token != Lexer::_OPEN_CURLY_BRACKET && 
        token != Lexer::_SEMICOLON && 
        token != Lexer::_EQUAL && 
        token != Lexer::_END_STREAM
    )
    {
        switch ( token )
        {
            default:
            {
                token = lexer->getNextToken();
                ln = lexer->getCurrLine();
            }
            break;

            case Lexer::_IDENTIFIER:
            {
                if( !parseExprId( e, lexer, token ) )
                {
                    return false;
                }
                operands.push( e );
                op = false;
            }
            break;

            case Lexer::_CONST:
            {
                if( !parseExprConst( e, lexer, token ) )
                {
                    return false;
                }
                operands.push( e );
                op = false;
            }
            break;

            case Lexer::_FUNCTION:
            {
                if( !parseFunction( e, lexer, token ) )
                {
                    return false;
                }
                operands.push( e );
                op = false;
            }
            break;

            case Lexer::_HOST:
            {
                if ( !parseHost( e, lexer, token ) )
                {
                    return false;
                }
                operands.push( e );
                op = false;
            }
            break;

            case Lexer::_OPEN_SQUARE_BRACKET:
            {
                if ( !parseExprArray( e, lexer, token ) )
                {
                    return false;
                }
                operands.push( e );
                op = false;
            }
            break;

            case Lexer::_OPEN_PARENTHESIS:
            {
                if ( !parseExprParen( e, lexer, token ) )
                {
                    return false;
                }
                operands.push( e );
                op = false;
            }
            break;

            case Lexer::_NULL:
            {
                ln = lexer->getCurrLine();
                token = lexer->getNextToken();
                operands.push( new Tree::Node( ln, Lexer::_NULL ) );
            }
            break;

            case Lexer::_VAR_STRING:
            case Lexer::_VAR_DOUBLE:
            case Lexer::_VAR_FLOAT:
            case Lexer::_VAR_LONG:
            case Lexer::_VAR_INT:
            {
                if( !parseVar( e, lexer, token ) )
                {
                    return false;
                }
                operands.push( e );
                op = false;
            }
            break;

            case Lexer::_STRING:
            case Lexer::_DOUBLE:
            case Lexer::_FLOAT:
            case Lexer::_LONG:
            case Lexer::_INT:
            {
                if( !parseLiteral( e, lexer, token ) )
                {
                    return false;
                }
                operands.push( e );
                op = false;
            }
            break;

            case Lexer::_FALSE:
            case Lexer::_TRUE:
            {
                if( !parseBool( e, lexer, token ) )
                {
                    return false;
                }
                operands.push( e );
                op = false;
            }
            break;

            case Lexer::_INVERT:
            case Lexer::_NOT:
            {
                Tree n = new Tree::Node( ln, token );
                token = lexer->getNextToken();
                ln = lexer->getCurrLine();

                Tree e;
                if( !parseExpr( e, lexer, token ) )
                {
                    return false;
                }

                n->push( e );
                operands.push( n );
                op = false;
            }
            break;

            /**
             * OPERATORS
             **/
            case Lexer::_MOD:
            case Lexer::_EXP:
            case Lexer::_ADD:
            case Lexer::_SUB:
            case Lexer::_MUL:
            case Lexer::_DIV:
            case Lexer::_AND:
            case Lexer::_OR:
            case Lexer::_LESS:
            case Lexer::_LESS_EQUAL:
            case Lexer::_EQUAL_EQUAL:
            case Lexer::_GREATER:
            case Lexer::_GREATER_EQUAL:
            case Lexer::_NOT_EQUAL:
            case Lexer::_DECREMENT:
            case Lexer::_INCREMENT:
            {
                if ( !op )
                {
                    while (!operators.empty() && precedence( operators.top()->node_type ) >= precedence( token ) )
                    {
                        resolve( operators, operands );
                    }
                }
                
                operators.push( new Tree::Node( ln, token ) );
                token = lexer->getNextToken();
                ln = lexer->getCurrLine();
                op = true;
            }
            break;
        }
    }

    // While the operator stack is not empty
    while (!operators.empty()) 
    {
        resolve( operators, operands );
    }
    expr = operands.top();
    return true;
}

bool parseRightSide( Tree& expr, Lexer* lexer, Lexer::Token& token )
{
    if ( token == Lexer::_FUNCTION || token == Lexer::_HOST )
    {
        if( !parseExpr( expr, lexer, token ) )
        {
            return false;
        }
    }
    else 
    {
        if( !parseExpr( expr, lexer, token ) )
        {
            return false;
        }
        if ( token != Lexer::_SEMICOLON )
        {
            SYNTAX_ERROR( lexer, Lexer::_ERROR_MISSING_TOKEN, Lexer::_SEMICOLON );
            return false;
        }
        token = lexer->getNextToken();
    }
    return true;
}

bool parseAssign( Tree& id, Lexer* lexer, Lexer::Token& token )
{
    Tree stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_ASSIGN );
    token = lexer->getNextToken();

    if ( token == Lexer::_FUNCTION )
    {
        Tree func;
        if( !parseFunction( func, lexer, token ) )
        {
            return false;
        }
        stmt->push( id );
        stmt->push( func );
    }
    else 
    {
        Tree expr;
        if( !parseExpr( expr, lexer, token ) )
        {
            return false;
        }
        stmt->push( id );
        stmt->push( expr );
        
        if ( token != Lexer::_SEMICOLON )
        {
            SYNTAX_ERROR( lexer, Lexer::_ERROR_MISSING_TOKEN, Lexer::_SEMICOLON );
            return false;
        }
        token = lexer->getNextToken();
    }
    id = stmt;
    return true;
}

bool parseDeclaration( Tree& id, Lexer* lexer, Lexer::Token& token )
{
    Tree stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_DECLARATION );
    token = lexer->getNextToken();

    if ( token == Lexer::_FUNCTION )
    {
        Tree func;
        if( !parseFunction( func, lexer, token ) )
        {
            return false;
        }
        stmt->push( id );
        stmt->push( func );
    }
    else 
    {
        Tree expr;
        if( !parseExpr( expr, lexer, token ) )
        {
            return false;
        }
        stmt->push( id );
        stmt->push( expr );

        if ( token != Lexer::_SEMICOLON )
        {
            SYNTAX_ERROR( lexer, Lexer::_ERROR_MISSING_TOKEN, Lexer::_SEMICOLON );
            return false;
        }
        token = lexer->getNextToken();
    }
    id = stmt;
    return true;
}

bool parseStmtId( Tree& stmt, Lexer* lexer, Lexer::Token& token )
{
    if( parseId( stmt, lexer, token ) )
    {    
        switch ( token )
        {
            case Lexer::_EQUAL:
            {
                return parseAssign( stmt, lexer, token );
            }

            case Lexer::_COLON:
            {
                return parseDeclaration( stmt, lexer, token );
            }

            case Lexer::_INVERT_EQUAL:
            case Lexer::_EQUAL_EQUAL:
            case Lexer::_ADD_EQUAL:
            case Lexer::_SUB_EQUAL:
            case Lexer::_MUL_EQUAL:
            case Lexer::_DIV_EQUAL:
            case Lexer::_EXP_EQUAL:
            case Lexer::_AND_EQUAL:
            case Lexer::_NOT_EQUAL:
            case Lexer::_OR_EQUAL:
            case Lexer::_GREATER_EQUAL:
            case Lexer::_GREATER:
            case Lexer::_LESS_EQUAL:
            case Lexer::_LESS:
            {
                return parseOp( stmt, lexer, token );
            }

            case Lexer::_SEMICOLON:
            {
                token = lexer->getNextToken();
                return true;
            }
            
            case Lexer::_COMMA:
            {
                return true;
            }
        
            default:
            {
                SYNTAX_ERROR( lexer, Lexer::_ERROR_UNEXPECTED_TOKEN, token );
                return false;
            }
        }
    }
    return false;
}

bool parseRetn( Tree& stmt, Lexer* lexer, Lexer::Token& token )
{
    int ln = lexer->getCurrLine();
    Tree id;
    token = lexer->getNextToken(); 

    if( !parseExpr( id, lexer, token ) )
    {
        return false;
    }
    
    stmt = new Tree::Node( ln, Lexer::_RETURN );
    stmt->push( id );

    if ( token != Lexer::_SEMICOLON )
    {
        SYNTAX_ERROR( lexer, Lexer::_ERROR_MISSING_TOKEN, Lexer::_SEMICOLON );
        return false;
    }
    token = lexer->getNextToken(); 
    return true;
}

/**
 * I defined block statement to be statements that do not end with a semicolon.
 * Statements like for, while, if, functions, and scopes. The reason for this is
 * so that i know when a state should end with or with out a semicolon.
 **/
bool parseStmtBlock( Tree& stmt, Lexer* lexer, Lexer::Token& token )
{
    Tree node;
    switch ( token )
    {
        case Lexer::_IF:
        {   
            stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_IF_STATEMENT );
            if( !parseIf( stmt, lexer, token ) )
            {
                return false;
            }    
            while ( token == Lexer::_ELSE )
            {
                if ( !parseElse( stmt, lexer, token ) )
                {
                    return false;
                }
            }
        }
        break;

        case Lexer::_FOR:
        {
            token = lexer->getNextToken();
            if( !parseFor( stmt, lexer, token ) )
            {
                return false;
            }          
        }
        break;

        case Lexer::_WHILE:
        {  
            token = lexer->getNextToken();
            if( !parseWhile( stmt, lexer, token ) )
            {
                return false;
            }    
        }
        break;

        case Lexer::_OPEN_CURLY_BRACKET:
        {
            if( !parseScopeBlock( stmt, lexer, token ) )
            {
                return false;
            }    
        }
        break;

        default:
        {
            SYNTAX_ERROR( lexer, Lexer::_ERROR_UNEXPECTED_TOKEN, token );
            return false;
        }
        break;
    }      
    return true;  
}

bool parseStmtConst( Tree& stmt, Lexer* lexer, Lexer::Token& token )
{
    int ln = lexer->getCurrLine();
    token = lexer->getNextToken();
    if ( token == Lexer::_IDENTIFIER )
    {
        Tree id;
        if( !parseStmtId( id, lexer, token ) )
        {
            return false;
        }
        stmt = new Tree::Node( ln, Lexer::_CONST );
        stmt->push( id );
        return true;
    }
    else
    {
        SYNTAX_ERROR( lexer, Lexer::_ERROR_MISSING_TOKEN, Lexer::_IDENTIFIER );
        return false;
    }
}

bool parseStmt( Tree& stmt, Lexer* lexer, Lexer::Token& token )
{
    Tree node;

    switch ( token )
    {
        case Lexer::_IDENTIFIER:
        {   
            return parseStmtId( stmt, lexer, token );
        }

        case Lexer::_CONST:
        {   
            return parseStmtConst( stmt, lexer, token );
        }

        case Lexer::_RETURN:
        {
            return parseRetn( stmt, lexer, token );   
        }

        case Lexer::_BREAK:
        case Lexer::_CONTINUE:
        {
            stmt = new Tree::Node( lexer->getCurrLine(), token );
            return true;  
        }

        case Lexer::_SEMICOLON:
        {
            stmt = new Tree::Node( lexer->getCurrLine(), token );
            token = lexer->getNextToken();
            return true;  
        }

        case Lexer::_IF:
        case Lexer::_FOR:
        case Lexer::_WHILE:
        case Lexer::_OPEN_CURLY_BRACKET:
        {  
            if( !parseStmtBlock( stmt, lexer, token ) )
            {
                return false;
            }    
        }
        break;

        default:
        {
            SYNTAX_ERROR( lexer, Lexer::_ERROR_UNEXPECTED_TOKEN, token );
            return false;
        }
        break;
    }      
    return true;  
}

void printNode(int tabs, Tree& ref)
{
    for(int i=0; i<tabs; i++) std::cout <<"- ";
    std::cout <<Lexer::toString( ref->node_type ) <<": " << ref->id <<"\n";
    for(int i=0; i<ref->nodes.size(); i++)
    {
        printNode( tabs + 1, ref->nodes[ i ] );
    }
}

void Parser::printTree()
{
    for(int i=0; i<_parse_tree->nodes.size(); i++)
    {
        printNode( 0, _parse_tree->nodes[ i ] );
    }
}

Tree& Parser::getTree()
{
    return _parse_tree;
}

bool Parser::parse( Lexer* lexer )
{
    _parse_tree = new Tree::Node();

    try
    {
        Tree stmt;
        Lexer::Token token = lexer->getNextToken();
        while ( token != Lexer::_END_STREAM )
        { 
            if ( !parseStmt( stmt, lexer, token ) )
            {
                break;
            }

            if( stmt )
            {
                _parse_tree->push( stmt );
            }
        }
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }

    lexer->clear();
    return true;
}

void Parser::clear()
{
    _parse_tree.clear();
}

Parser::~Parser()
{
    clear();
}

Parser::Parser()
{}
    