#include "parser.hpp"

Tree parseExpression( Lexer* lexer, Lexer::Token& token );
Tree parseIfStatement( Lexer* lexer, Lexer::Token& token );
Tree parseForLoop( Lexer* lexer, Lexer::Token& token );
Tree parseScopeBlock( Lexer* lexer, Lexer::Token& token );
Tree parseFunction( Lexer* lexer, Lexer::Token& token );
Tree parseValue( Lexer* lexer, Lexer::Token& token );
Tree parseDataType( Lexer* lexer, Lexer::Token& token );
Tree parseIdentifier( Lexer* lexer, Lexer::Token& token );
Tree parseConstant( Lexer* lexer, Lexer::Token& token );

enum
{
    EXPECTED_TOKEN,
    MISSING_TOKEN,
    UNEXPECTED_TOKEN,
    UNSUPPORTED_TYPE,
    EMPTY_ARRAY_OBJECT,
    UNDETERMIND_STATIC_TYPE,
    UNSUPPORTED_FUNCTION_PARAMETER,
};
void throwPaserError( int error_code, int line, Lexer::Token token, const char* funct )
{
    std::string msg;
    switch ( error_code )
    {
        case UNEXPECTED_TOKEN:
        {
            msg += "error in " + std::string( funct );
            msg += std::string( ": UNEXPECTED_TOKEN -> " ) + Lexer::toString( token );
            msg += std::string( ": at line# " ) + std::to_string( line );
        }
        break;

        case MISSING_TOKEN:
        {
            msg += "error in " + std::string( funct );
            msg += std::string( ": MISSING_TOKEN -> " ) + Lexer::toString( token );
            msg += std::string( ": at line# " ) + std::to_string( line );
        }
        break;

        case EXPECTED_TOKEN:
        {
            msg += "error in " + std::string( funct );
            msg += std::string( ": EXPECTED_TOKEN -> " ) + Lexer::toString( token );
            msg += std::string( ": at line# " ) + std::to_string( line );
        }
        break;

        case UNSUPPORTED_TYPE:
        {
            msg += "error in " + std::string( funct );
            msg += std::string( ": UNSUPPORTED_TYPE -> " ) + Lexer::toString( token );
            msg += std::string( ": at line# " ) + std::to_string( line );
        }
        break;

        case EMPTY_ARRAY_OBJECT:
        {
            msg += "error detected by " + std::string( funct );
            msg += ": EMPTY_ARRAY_OBJECT -> " + std::string( ": at line# " ) + std::to_string( line );
        }
        break;

        case UNDETERMIND_STATIC_TYPE:
        {
            msg += "error detected by " + std::string( funct );
            msg += ": UNDETERMIND_STATIC_TYPE -> " + std::string( ": at line# " ) + std::to_string( line );
        }
        break;

        case UNSUPPORTED_FUNCTION_PARAMETER:
        {
            msg += "error detected by " + std::string( funct );
            msg += ": UNSUPPORTED_FUNCTION_PARAMETER -> " + std::string( ": at line# " ) + std::to_string( line );
        }
        break;
    
        default:
        break;
    }
    throw std::runtime_error( msg );
}
#define THROW_PARSING_ERROR( code, line, token ) throwPaserError( code, line, token, __FUNCTION__ )

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

        case Lexer::_ADD_EQUAL:
        case Lexer::_SUB_EQUAL:
        case Lexer::_DIV_EQUAL:
        case Lexer::_MUL_EQUAL:
        return 7;

        case Lexer::_EXP_EQUAL:
        return 8;

        case Lexer::_EXP:
        case Lexer::_NOT:
        case Lexer::_DECREMENT:
        case Lexer::_INCREMENT:
        case Lexer::_INVERT_EQUAL:
        return 9;

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

Tree parseValue( Lexer* lexer, Lexer::Token& token )
{
    int ln = lexer->getCurrLine();
    Tree val;

    switch( token )
    {
        case Lexer::_BOOL:
        {
            val = new Tree::Node( ln, Lexer::_BOOL, lexer->getLexeme() );
        }
        break;

        case Lexer::_INT:
        {
            val = new Tree::Node( ln, Lexer::_INT, lexer->getLexeme() );
        }
        break;

        case Lexer::_LONG:
        {
            val = new Tree::Node( ln, Lexer::_LONG, lexer->getLexeme());
        }
        break;

        case Lexer::_FLOAT:
        {
            val = new Tree::Node( ln, Lexer::_FLOAT, lexer->getLexeme() );
        }
        break;

        case Lexer::_DOUBLE:
        {
            val = new Tree::Node( ln, Lexer::_DOUBLE, lexer->getLexeme() );
        }
        break;

        case Lexer::_STRING:
        {
            val = new Tree::Node( ln, Lexer::_STRING, lexer->getLexeme() );
        }
        break;

        default:
        {
            THROW_PARSING_ERROR( UNSUPPORTED_TYPE, lexer->getCurrLine(), token );
        }
        break;
    }

    token = lexer->getNextToken();
    return val;
}

Tree parseDataType( Lexer* lexer, Lexer::Token& token )
{
    int ln = lexer->getCurrLine();
    Tree var;

    switch( token )
    {
        case Lexer::_VAR_BOOL:
        {
            var = new Tree::Node( ln, Lexer::_VAR_BOOL );
        }
        break;

        case Lexer::_VAR_INT:
        {
            var = new Tree::Node( ln, Lexer::_VAR_INT );
        }
        break;

        case Lexer::_VAR_LONG:
        {
            var = new Tree::Node( ln, Lexer::_VAR_LONG );
        }
        break;

        case Lexer::_VAR_FLOAT:
        {
            var = new Tree::Node( ln, Lexer::_VAR_FLOAT );
        }
        break;

        case Lexer::_VAR_DOUBLE:
        {
            var = new Tree::Node( ln, Lexer::_VAR_DOUBLE );
        }
        break;

        case Lexer::_VAR_STRING:
        {
            var = new Tree::Node( ln, Lexer::_VAR_STRING );
        }
        break;

        default:
        {
            THROW_PARSING_ERROR( UNSUPPORTED_TYPE, ln, token );
        }
        break;
    }

    token = lexer->getNextToken();
    if( token == Lexer::_OPEN_PARENTHESIS )
    {
        if( token != Lexer::_CLOSE_PARENTHESIS )
        {
            token = lexer->getNextToken();
            var->push( parseExpression( lexer, token ) );
            if( token != Lexer::_CLOSE_PARENTHESIS )
            {
                THROW_PARSING_ERROR( EXPECTED_TOKEN, ln, Lexer::_CLOSE_PARENTHESIS );
            }
            token = lexer->getNextToken();
        }
    }
    return var;
}

Tree parseArrayObject( Lexer* lexer, Lexer::Token& token )
{
    Tree stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_ARRAY );
    token = lexer->getNextToken();
    Tree e;

    if( token == Lexer::_CLOSE_SQUARE_BRACKET )
    {
        token = lexer->getNextToken();
        if( token == Lexer::_OPEN_PARENTHESIS )
        {
            token = lexer->getNextToken();
            Tree size = new Tree::Node( lexer->getCurrLine(), Lexer::_ARRAY_SIZE );
            size->push( parseExpression( lexer, token ) );
            stmt->push( size );
            if( token != Lexer::_CLOSE_PARENTHESIS )
            {
                THROW_PARSING_ERROR( MISSING_TOKEN, lexer->getCurrLine(), Lexer::_CLOSE_PARENTHESIS );
            }
            token = lexer->getNextToken();
        }
        if( token != Lexer::_SEMICOLON )
        {
            THROW_PARSING_ERROR( MISSING_TOKEN, lexer->getCurrLine(), Lexer::_SEMICOLON );
        }
        return stmt;
    }

    stmt->push( parseExpression( lexer, token ) );
    while ( token == Lexer::_COMMA )
    {
        token = lexer->getNextToken();
        stmt->push( parseExpression( lexer, token ) );
    }

    if( token != Lexer::_CLOSE_SQUARE_BRACKET )
    {
        THROW_PARSING_ERROR( MISSING_TOKEN, lexer->getCurrLine(), Lexer::_CLOSE_SQUARE_BRACKET );
    }
    token = lexer->getNextToken();
    return stmt;
}

Tree getBodyStmt( Lexer* lexer, Lexer::Token& token )
{
    if ( token == Lexer::_OPEN_CURLY_BRACKET )
    {
        return parseScopeBlock( lexer, token );  
    } 
    else 
    {
        return parseIdentifier( lexer, token );  
    } 
}

Tree parseForLoopCondition( Lexer* lexer, Lexer::Token& token )
{
    Tree condition_list = new Tree::Node( lexer->getCurrLine(), Lexer::_CONDITIONS );
    token = lexer->getNextToken();

    Tree conditions[ 3 ];
    for (size_t i = 0; i < 3; i++)
    {
        conditions[ i ] = new Tree::Node( lexer->getCurrLine(), Lexer::_PARAMETERS );
        conditions[ i ]->push( parseExpression( lexer, token ) );
        while ( token != Lexer::_SEMICOLON && token == Lexer::_COMMA )
        {
            token = lexer->getNextToken();
            conditions[ i ]->push( parseExpression( lexer, token ) );
        }
        if ( token != Lexer::_SEMICOLON && token != Lexer::_CLOSE_PARENTHESIS )
        {
            THROW_PARSING_ERROR( UNEXPECTED_TOKEN, lexer->getCurrLine(), token );
        }
        token = lexer->getNextToken();
        condition_list->push( conditions[ i ] );
    }
    return condition_list;
}

Tree parseForLoop( Lexer* lexer, Lexer::Token& token )
{
    Tree stmt;
    if ( token == Lexer::_OPEN_PARENTHESIS )
    {
        stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_FOR );
        stmt->push( parseForLoopCondition( lexer, token ) );
        stmt->push( getBodyStmt( lexer, token ) );
    }
    return stmt;
}

Tree parseWhileLoop( Lexer* lexer, Lexer::Token& token )
{
    Tree stmt;
    if ( token == Lexer::_OPEN_PARENTHESIS )
    {
        stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_WHILE );
        
        Tree conditions = new Tree::Node( lexer->getCurrLine(), Lexer::_CONDITIONS );
        conditions->push( parseExpression( lexer, token ) );
        stmt->push( conditions );

        stmt->push( getBodyStmt( lexer, token ) );
    }
    return stmt;
}

Tree parseIfStatement( Lexer* lexer, Lexer::Token& token )
{
    Tree stmt;
    if ( token == Lexer::_OPEN_PARENTHESIS )
    {
        stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_IF );
        Tree conditions = new Tree::Node( lexer->getCurrLine(), Lexer::_CONDITIONS );
        token = lexer->getNextToken();

        conditions->push( parseExpression( lexer, token ) );
        stmt->push( conditions );

        if ( token != Lexer::_CLOSE_PARENTHESIS )
        {
            THROW_PARSING_ERROR( UNEXPECTED_TOKEN, lexer->getCurrLine(), token );
        }
        token = lexer->getNextToken();
        stmt->push( getBodyStmt( lexer, token ) );
    }
    return stmt;
}

Tree parseElseStatement( Lexer* lexer, Lexer::Token& token )
{
    Tree els = new Tree::Node( lexer->getCurrLine(), Lexer::_ELSE );
    token = lexer->getNextToken();

    if ( token == Lexer::_IF )
    {
        token = lexer->getNextToken();
        els->push( parseIfStatement( lexer,  token ) );
    }
    else 
    {
        els->push( getBodyStmt( lexer, token ) );
    }
    return els;
}

Tree parseScopeBlock( Lexer* lexer, Lexer::Token& token )
{
    Tree stmts = new Tree::Node( lexer->getCurrLine(), Lexer::_SCOPE_BLOCK );
    token = lexer->getNextToken();

    if ( token == Lexer::_CLOSE_CURLY_BRACKET )
    {
        token = lexer->getNextToken();
        return stmts;
    }

    while ( token != Lexer::_CLOSE_CURLY_BRACKET )
    {
        switch ( token )
        {
            case Lexer::_IDENTIFIER:
            {   
                stmts->push( parseIdentifier( lexer, token ) );
            }
            break;

            case Lexer::_CONST:
            {   
                token = lexer->getNextToken();
                stmts->push( parseConstant( lexer, token ) );
            }
            break;

            case Lexer::_IF:
            {   
                token = lexer->getNextToken();
                stmts->push( parseIfStatement( lexer, token ) );
                while ( token == Lexer::_ELSE )
                {
                    stmts->push( parseElseStatement( lexer, token ) );
                }
            }
            break;

            case Lexer::_FOR:
            {
                token = lexer->getNextToken();
                stmts->push( parseForLoop( lexer, token ) );          
            }
            break;

            case Lexer::_WHILE:
            {  
                token = lexer->getNextToken();
                stmts->push( parseWhileLoop( lexer, token ) );
            }
            break;

            case Lexer::_OPEN_CURLY_BRACKET:
            {
                stmts->push( parseScopeBlock( lexer, token ) ); 
            }
            break;

            case Lexer::_CONTINUE:
            case Lexer::_BREAK:
            {
                stmts->push( new Tree::Node( lexer->getCurrLine(), token ) ); 
                token = lexer->getNextToken();
                if ( token != Lexer::_SEMICOLON )
                {
                    THROW_PARSING_ERROR( EXPECTED_TOKEN, lexer->getCurrLine(), Lexer::_SEMICOLON );
                }
                token = lexer->getNextToken();
            }
            break;

            default:
            {
                token = lexer->getNextToken();
            }
            break;
        }      
    }
    token = lexer->getNextToken();
    return stmts;
}

Tree getFunctParam( Lexer* lexer, Lexer::Token& token )
{
    Tree stmt;
    switch ( token )
    {
        case Lexer::_VAR_STRING:
        case Lexer::_VAR_DOUBLE:
        case Lexer::_VAR_FLOAT:
        case Lexer::_VAR_LONG:
        case Lexer::_VAR_INT:
        case Lexer::_VAR_BOOL:
        {
            stmt = parseDataType( lexer, token );
            if ( token != Lexer::_COMMA && token != Lexer::_CLOSE_PARENTHESIS )
            {
                THROW_PARSING_ERROR( UNEXPECTED_TOKEN, lexer->getCurrLine(), token );
            }
        }
        break;

        case Lexer::_FUNCTION:
        {
            stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_VAR_FUNCTION );
            if ( token != Lexer::_COMMA && token != Lexer::_CLOSE_PARENTHESIS )
            {
                THROW_PARSING_ERROR( UNEXPECTED_TOKEN, lexer->getCurrLine(), token );
            }
        }
        break;

        case Lexer::_OPEN_SQUARE_BRACKET:
        {
            stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_ARRAY );
            token = lexer->getNextToken();
            if ( token != Lexer::_CLOSE_SQUARE_BRACKET )
            {
                THROW_PARSING_ERROR( EXPECTED_TOKEN, lexer->getCurrLine(), Lexer::_CLOSE_SQUARE_BRACKET );
            }
            token = lexer->getNextToken();
            if ( token != Lexer::_COMMA && token != Lexer::_CLOSE_PARENTHESIS )
            {
                THROW_PARSING_ERROR( UNEXPECTED_TOKEN, lexer->getCurrLine(), token );
            }
        }
        break;

        default:
        {
            THROW_PARSING_ERROR( UNSUPPORTED_FUNCTION_PARAMETER, lexer->getCurrLine(), token );
        }
        break;
    }
    return stmt;
}

Tree parseFunctParameter( Lexer* lexer, Lexer::Token& token )
{
    Tree expr;
    switch ( token )
    {
        case Lexer::_IDENTIFIER:
        {
            Tree identifier = new Tree::Node( lexer->getCurrLine(), Lexer::_IDENTIFIER, lexer->getLexeme() );
            token = lexer->getNextToken();

            switch ( token )
            {
                case Lexer::_EQUAL:
                {
                    token = lexer->getNextToken();
                    expr = new Tree::Node( lexer->getCurrLine(), Lexer::_ASSIGN );
                    expr->push( identifier );
                    expr->push( getFunctParam( lexer, token ) );
                }
                break;

                case Lexer::_COLON:
                {
                    token = lexer->getNextToken();
                    expr = new Tree::Node( lexer->getCurrLine(), Lexer::_DECLARATION );
                    expr->push( identifier );
                    expr->push( getFunctParam( lexer, token ) );
                }
                break;

                case Lexer::_COMMA:
                {
                    expr = identifier;
                }
                break;
            
                default:
                {
                    THROW_PARSING_ERROR( UNEXPECTED_TOKEN, lexer->getCurrLine(), token );
                }
                break;
            }
        }
        break;

        case Lexer::_ADDRESS:
        {
            expr = new Tree::Node( lexer->getCurrLine(), Lexer::_ADDRESS );
            token = lexer->getNextToken();
            expr->push( parseFunctParameter( lexer, token ) );
        }
        break;

        case Lexer::_CONST:
        {
            expr = new Tree::Node( lexer->getCurrLine(), Lexer::_CONST );
            token = lexer->getNextToken();

            switch ( token )
            {
                case Lexer::_IDENTIFIER:
                {
                    expr->push( parseFunctParameter( lexer, token ) );
                }
                break;

                case Lexer::_ADDRESS:
                {
                    expr->push( parseFunctParameter( lexer, token ) );
                }
                break;
            
                default:
                {
                    THROW_PARSING_ERROR( UNEXPECTED_TOKEN, lexer->getCurrLine(), token );
                }
                break;
            }
        }
        break;

        default:
        {
            THROW_PARSING_ERROR( UNEXPECTED_TOKEN, lexer->getCurrLine(), token );
        }
        break;
    }
    return expr;
}

Tree parseFunctionParameters( Lexer* lexer, Lexer::Token& token )
{
    Tree parameters = new Tree::Node( lexer->getCurrLine(), Lexer::_PARAMETERS );
    token = lexer->getNextToken();

    if( token == Lexer::_CLOSE_PARENTHESIS )
    {
        token = lexer->getNextToken();
        return parameters;
    }

    parameters->push( parseFunctParameter( lexer, token ) );
    while ( token == Lexer::_COMMA )
    {
        token = lexer->getNextToken();
        parameters->push( parseFunctParameter( lexer, token ) );
    }

    if( token != Lexer::_CLOSE_PARENTHESIS )
    {
        THROW_PARSING_ERROR( MISSING_TOKEN, lexer->getCurrLine(), Lexer::_CLOSE_PARENTHESIS );
    }

    token = lexer->getNextToken();
    return parameters;
}

Tree parseFunction( Lexer* lexer, Lexer::Token& token )
{
    Tree function = new Tree::Node( lexer->getCurrLine(), Lexer::_FUNCTION );
    token = lexer->getNextToken();
    function->push( parseFunctionParameters( lexer, token ) );
    function->push( parseScopeBlock( lexer, token ) );
    return function;
}

Tree parseHost( Lexer* lexer, Lexer::Token& token )
{
    Tree function = new Tree::Node( lexer->getCurrLine(), Lexer::_HOST );
    token = lexer->getNextToken();

    Tree params = parseFunctionParameters( lexer, token );
    for(int i=0; i<params->nodes.size(); i++)
    {
        function->push( params->nodes[ i ] );
    }
    return function;
}

Tree parseExpressionIdentifier( Lexer* lexer, Lexer::Token& token )
{   
    Tree identifier = new Tree::Node( Lexer::_IDENTIFIER, lexer->getLexeme() );
    token = lexer->getNextToken();
    Tree stmt;

    switch ( token )
    {
        case Lexer::_OPEN_PARENTHESIS: // function call
        {
            stmt = identifier;
            stmt->node_type = Lexer::_FUNCTION_CALL;
            stmt->line = lexer->getCurrLine();
            token = lexer->getNextToken();

            if ( token != Lexer::_CLOSE_PARENTHESIS )
            {
                stmt->push( parseExpression( lexer, token ) );
                while( token == Lexer::_COMMA )
                {
                    token = lexer->getNextToken();
                    stmt->push( parseExpression( lexer, token ) );
                }
                if ( token != Lexer::_CLOSE_PARENTHESIS )
                {
                    THROW_PARSING_ERROR( UNEXPECTED_TOKEN, lexer->getCurrLine(), token );
                }
                token = lexer->getNextToken();
            }
        }
        break;

        case Lexer::_OPEN_SQUARE_BRACKET: // array index accessing
        {
            stmt = identifier;
            stmt->node_type = Lexer::_ARRAY_INDEXED;
            stmt->line = lexer->getCurrLine();
            token = lexer->getNextToken();

            stmt->push( parseExpression( lexer, token ) );
            if ( token != Lexer::_CLOSE_SQUARE_BRACKET )
            {
                THROW_PARSING_ERROR( UNEXPECTED_TOKEN, lexer->getCurrLine(), token );
            }
            token = lexer->getNextToken();
        }
        break;

        case Lexer::_EQUAL:
        {
            stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_ASSIGN );
            token = lexer->getNextToken();
            stmt->push( identifier );
            stmt->push( parseExpression( lexer, token ) );
        }
        break;

        case Lexer::_ADD_EQUAL:
        case Lexer::_SUB_EQUAL:
        case Lexer::_MUL_EQUAL:
        case Lexer::_DIV_EQUAL:
        case Lexer::_EXP_EQUAL:
        case Lexer::_MOD_EQUAL:
        case Lexer::_INVERT_EQUAL:
        {
            stmt = new Tree::Node( lexer->getCurrLine(), token ); 
            token = lexer->getNextToken();

            stmt->push( identifier );
            stmt->push( parseExpression( lexer, token ) ); 
        }
        break;

        default:
        {
            stmt = identifier;
        }
        break;
    }
    
    return stmt;
}

Tree parseExpression( Lexer* lexer, Lexer::Token& token )
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
                operands.push( parseExpressionIdentifier( lexer, token ) );
                op = false;
            }
            break;

            case Lexer::_OPEN_SQUARE_BRACKET:
            {
                operands.push( parseArrayObject( lexer, token ) );
                op = false;
            }
            break;

            case Lexer::_OPEN_PARENTHESIS:
            {
                e = new Tree::Node( lexer->getCurrLine(), Lexer::_ENCLOSE_PARENTHESIS );
                token = lexer->getNextToken();

                if ( token == Lexer::_CLOSE_PARENTHESIS )
                {
                    // todo: error 
                }

                e->push( parseExpression( lexer, token ) );
                if ( token != Lexer::_CLOSE_PARENTHESIS )
                {
                    // todo: error
                }

                token = lexer->getNextToken();
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
                operands.push( parseDataType( lexer, token ) );
                op = false;
            }
            break;

            case Lexer::_STRING:
            case Lexer::_DOUBLE:
            case Lexer::_FLOAT:
            case Lexer::_LONG:
            case Lexer::_INT:
            case Lexer::_BOOL:
            {
                operands.push( parseValue( lexer, token ) );
                op = false;
            }
            break;

            case Lexer::_INVERT:
            case Lexer::_NOT:
            {
                Tree n = new Tree::Node( ln, token );
                token = lexer->getNextToken();
                ln = lexer->getCurrLine();

                n->push( parseExpression( lexer, token ) );
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
            case Lexer::_ADD_EQUAL:
            case Lexer::_SUB_EQUAL:
            case Lexer::_MUL_EQUAL:
            case Lexer::_DIV_EQUAL:
            case Lexer::_EXP_EQUAL:
            case Lexer::_INVERT_EQUAL:
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

    return operands.top();
}

Tree parseRightside( Lexer* lexer, Lexer::Token& token )
{
    Tree stmt;
    switch ( token )
    {
        case Lexer::_FUNCTION:
        {
            stmt = parseFunction( lexer, token );
        }
        break;

        case Lexer::_CONST:
        {
            stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_CONST, lexer->getLexeme() );
            token = lexer->getNextToken();

            switch ( token )
            {
                case Lexer::_FUNCTION:
                {
                    stmt->push( parseFunction( lexer, token ) );
                }
                break;

                case Lexer::_VAR_STRING:
                case Lexer::_VAR_DOUBLE:
                case Lexer::_VAR_FLOAT:
                case Lexer::_VAR_LONG:
                case Lexer::_VAR_INT:
                {
                    stmt->push( parseDataType( lexer, token ) );

                    if ( token != Lexer::_SEMICOLON )
                    {
                        THROW_PARSING_ERROR( EXPECTED_TOKEN, lexer->getCurrLine(), Lexer::_SEMICOLON );
                    }
                    token = lexer->getNextToken();
                }
                break;

                case Lexer::_STRING:
                case Lexer::_DOUBLE:
                case Lexer::_FLOAT:
                case Lexer::_LONG:
                case Lexer::_INT:
                case Lexer::_BOOL:
                {
                    stmt = parseValue( lexer, token );

                    if ( token != Lexer::_SEMICOLON )
                    {
                        THROW_PARSING_ERROR( EXPECTED_TOKEN, lexer->getCurrLine(), Lexer::_SEMICOLON );
                    }
                    token = lexer->getNextToken();
                }
                break;

                default:
                {
                    THROW_PARSING_ERROR( UNEXPECTED_TOKEN, lexer->getCurrLine(), token );
                }
                break;
            }
            
        }
        break;
        
        default:
        {
            stmt = parseExpression( lexer, token );

            if ( token != Lexer::_SEMICOLON )
            {
                THROW_PARSING_ERROR( EXPECTED_TOKEN, lexer->getCurrLine(), Lexer::_SEMICOLON );
            }
            token = lexer->getNextToken();
        }
        break;
    }
    return stmt;
}

Tree parseDeclaration( Lexer* lexer, Lexer::Token& token )
{
    Tree stmt;
    switch ( token )
    {
        case Lexer::_FUNCTION:
        {
            stmt = parseFunction( lexer, token );
        }
        break;

        case Lexer::_HOST:
        {
            stmt = parseHost( lexer, token );

            if ( token != Lexer::_SEMICOLON )
            {
                THROW_PARSING_ERROR( EXPECTED_TOKEN, lexer->getCurrLine(), Lexer::_SEMICOLON );
            }
            token = lexer->getNextToken();
        }
        break;

        case Lexer::_CONST:
        {
            stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_CONST, lexer->getLexeme() );
            token = lexer->getNextToken();
            stmt->push( parseDeclaration( lexer, token ) );
        }
        break;

        // case Lexer::_STRUCT: TODO: for the future
        // {}
        // break;
        
        default:
        {
            stmt = parseExpression( lexer, token );
            if ( token != Lexer::_SEMICOLON )
            {
                THROW_PARSING_ERROR( EXPECTED_TOKEN, lexer->getCurrLine(), Lexer::_SEMICOLON );
            }
            token = lexer->getNextToken();
        }
        break;
    }
    
    return stmt;
}

Tree parseIdentifier( Lexer* lexer, Lexer::Token& token )
{   
    Tree identifier = new Tree::Node( Lexer::_IDENTIFIER, lexer->getLexeme() );
    token = lexer->getNextToken();
    Tree stmt;

    switch ( token )
    {
        case Lexer::_EQUAL:
        {
            stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_ASSIGN );
            token = lexer->getNextToken();
            stmt->push( identifier );
            stmt->push( parseRightside( lexer, token ) );
        }
        break;

        case Lexer::_COLON:
        {
            stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_DECLARATION );
            token = lexer->getNextToken();
            stmt->push( identifier );
            stmt->push( parseDeclaration( lexer, token ) );
        }
        break;

        case Lexer::_OPEN_PARENTHESIS: // function call
        {
            stmt = identifier;
            stmt->node_type = Lexer::_FUNCTION_CALL;
            stmt->line = lexer->getCurrLine();
            token = lexer->getNextToken();

            if ( token != Lexer::_CLOSE_PARENTHESIS )
            {
                stmt->push( parseExpression( lexer, token ) );
                while( token == Lexer::_COMMA )
                {
                    token = lexer->getNextToken();
                    stmt->push( parseExpression( lexer, token ) );
                }

                if ( token != Lexer::_CLOSE_PARENTHESIS )
                {
                    THROW_PARSING_ERROR( UNEXPECTED_TOKEN, lexer->getCurrLine(), token );
                }
                token = lexer->getNextToken();

                if ( token != Lexer::_SEMICOLON )
                {
                    THROW_PARSING_ERROR( EXPECTED_TOKEN, lexer->getCurrLine(), Lexer::_SEMICOLON );
                }
                token = lexer->getNextToken();
            }
        }
        break;

        case Lexer::_OPEN_SQUARE_BRACKET: // array index accessing
        {
            stmt = identifier;
            stmt->node_type = Lexer::_ARRAY_INDEXED;
            stmt->line = lexer->getCurrLine();
            token = lexer->getNextToken();

            stmt->push( parseExpression( lexer, token ) );

            if ( token != Lexer::_ENCLOSE_SQUARE_BRACKET )
            {
                THROW_PARSING_ERROR( UNEXPECTED_TOKEN, lexer->getCurrLine(), token );
            }
            token = lexer->getNextToken();

            if ( token != Lexer::_SEMICOLON )
            {
                THROW_PARSING_ERROR( EXPECTED_TOKEN, lexer->getCurrLine(), Lexer::_SEMICOLON );
            }
            token = lexer->getNextToken();
        }
        break;

        case Lexer::_ADD_EQUAL:
        case Lexer::_SUB_EQUAL:
        case Lexer::_MUL_EQUAL:
        case Lexer::_DIV_EQUAL:
        case Lexer::_EXP_EQUAL:
        case Lexer::_MOD_EQUAL:
        case Lexer::_INVERT_EQUAL:
        {
            stmt = new Tree::Node( lexer->getCurrLine(), token ); 
            token = lexer->getNextToken();

            stmt->push( identifier );
            stmt->push( parseExpression( lexer, token ) ); 
            if ( token != Lexer::_SEMICOLON )
            {
                THROW_PARSING_ERROR( EXPECTED_TOKEN, lexer->getCurrLine(), Lexer::_SEMICOLON );
            }
            token = lexer->getNextToken();
        }
        break;

        case Lexer::_SEMICOLON:
        {
            token = lexer->getNextToken();
            stmt = identifier;
        }
        break;
        
        default:
        {
            THROW_PARSING_ERROR( UNEXPECTED_TOKEN, lexer->getCurrLine(), token );
        }
        break;
    }
    
    return stmt;
}

Tree parseConstant( Lexer* lexer, Lexer::Token& token )
{
    Tree stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_CONST, lexer->getLexeme() );
    token = lexer->getNextToken();
    stmt->push( parseIdentifier( lexer, token ) );
    return stmt;
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
            switch ( token )
            {
                case Lexer::_IDENTIFIER:
                {   
                    _parse_tree->push( parseIdentifier( lexer, token ) );
                }
                break;

                case Lexer::_CONST:
                {   
                    _parse_tree->push( parseConstant( lexer, token ) );
                    if ( token == Lexer::_SEMICOLON )
                    {
                        token = lexer->getNextToken();
                    }
                }
                break;

                default:
                {
                    THROW_PARSING_ERROR( UNEXPECTED_TOKEN, lexer->getCurrLine(),  token );
                }
                break;
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
    std::cout <<"\n\nParser \n";
    for(int i=0; i<_parse_tree->nodes.size(); i++)
    {
        printNode( 0, _parse_tree->nodes[ i ] );
        std::cout <<"\n";
    }
}

Tree& Parser::getTree()
{
    return _parse_tree;
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
    