#include "syntax.hpp"


Tree analyzeExpression( Lexer* lexer, Lexer::Token& token );
Tree analyzeIfStatement( Lexer* lexer, Lexer::Token& token );
Tree analyzeForLoop( Lexer* lexer, Lexer::Token& token );
Tree analyzeScopeBlock( Lexer* lexer, Lexer::Token& token );
Tree analyzeFunction( Lexer* lexer, Lexer::Token& token );
Tree analyzeDataType( Lexer* lexer, Lexer::Token& token );
Tree analyzeIdentifier( Lexer* lexer, Lexer::Token& token );
Tree analyzeConstant( Lexer* lexer, Lexer::Token& token );

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

void setValue( Lexer* lexer, Lexer::Token& token, Tree& var )
{
    token = lexer->getNextToken();
    if( token == Lexer::_OPEN_PARENTHESIS )
    {
        if( token != Lexer::_CLOSE_PARENTHESIS )
        {
            token = lexer->getNextToken();
            var->push( analyzeExpression( lexer, token ) );
            if( token != Lexer::_CLOSE_PARENTHESIS )
            {
                THROW_PARSING_ERROR( EXPECTED_TOKEN, lexer->getCurrLine(), Lexer::_CLOSE_PARENTHESIS );
            }
            token = lexer->getNextToken();
        }
        token = lexer->getNextToken();
    }
}

Tree analyzeDataType( Lexer* lexer, Lexer::Token& token )
{
    int ln = lexer->getCurrLine();
    Tree var;

    switch( token )
    {
        case Lexer::_BOOL:
        {
            var = new Tree::Node( ln, Lexer::_BOOL, lexer->getLexeme() );
            token = lexer->getNextToken();
        }
        break;

        case Lexer::_INT:
        {
            var = new Tree::Node( ln, Lexer::_INT, lexer->getLexeme() );
            token = lexer->getNextToken();
        }
        break;

        case Lexer::_LONG:
        {
            var = new Tree::Node( ln, Lexer::_LONG, lexer->getLexeme());
            token = lexer->getNextToken();
        }
        break;

        case Lexer::_FLOAT:
        {
            var = new Tree::Node( ln, Lexer::_FLOAT, lexer->getLexeme() );
            token = lexer->getNextToken();
        }
        break;

        case Lexer::_DOUBLE:
        {
            var = new Tree::Node( ln, Lexer::_DOUBLE, lexer->getLexeme() );
            token = lexer->getNextToken();
        }
        break;

        case Lexer::_STRING:
        {
            var = new Tree::Node( ln, Lexer::_STRING, lexer->getLexeme() );
            token = lexer->getNextToken();
        }
        break;

        case Lexer::_VAR_BOOL:
        {
            var = new Tree::Node( ln, Lexer::_BOOL );
            setValue( lexer, token, var );
        }
        break;

        case Lexer::_VAR_INT:
        {
            var = new Tree::Node( ln, Lexer::_INT );
            setValue( lexer, token, var );
        }
        break;

        case Lexer::_VAR_LONG:
        {
            var = new Tree::Node( ln, Lexer::_LONG );
            setValue( lexer, token, var );
        }
        break;

        case Lexer::_VAR_FLOAT:
        {
            var = new Tree::Node( ln, Lexer::_FLOAT );
            setValue( lexer, token, var );
        }
        break;

        case Lexer::_VAR_DOUBLE:
        {
            var = new Tree::Node( ln, Lexer::_DOUBLE );
            setValue( lexer, token, var );
        }
        break;

        case Lexer::_VAR_STRING:
        {
            var = new Tree::Node( ln, Lexer::_STRING );
            setValue( lexer, token, var );
        }
        break;

        default:
        {
            THROW_PARSING_ERROR( UNSUPPORTED_TYPE, ln, token );
        }
        break;
    }
    return var;
}

Tree analyzeArrayObject( Lexer* lexer, Lexer::Token& token )
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
            size->push( analyzeExpression( lexer, token ) );
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

    stmt->push( analyzeExpression( lexer, token ) );
    while ( token == Lexer::_COMMA )
    {
        token = lexer->getNextToken();
        stmt->push( analyzeExpression( lexer, token ) );
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
        return analyzeScopeBlock( lexer, token );  
    } 
    else 
    {
        return analyzeIdentifier( lexer, token );  
    } 
}

Tree analyzeForLoopCondition( Lexer* lexer, Lexer::Token& token )
{
    Tree condition_list = new Tree::Node( lexer->getCurrLine(), Lexer::_CONDITIONS );
    token = lexer->getNextToken();

    Tree conditions[ 3 ];
    for (size_t i = 0; i < 3; i++)
    {
        conditions[ i ] = new Tree::Node( lexer->getCurrLine(), Lexer::_PARAMETERS );
        conditions[ i ]->push( analyzeExpression( lexer, token ) );
        while ( token != Lexer::_SEMICOLON && token == Lexer::_COMMA )
        {
            token = lexer->getNextToken();
            conditions[ i ]->push( analyzeExpression( lexer, token ) );
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

Tree analyzeForLoop( Lexer* lexer, Lexer::Token& token )
{
    Tree stmt;
    if ( token == Lexer::_OPEN_PARENTHESIS )
    {
        stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_FOR );
        stmt->push( analyzeForLoopCondition( lexer, token ) );
        stmt->push( getBodyStmt( lexer, token ) );
    }
    return stmt;
}

Tree analyzeWhileLoop( Lexer* lexer, Lexer::Token& token )
{
    Tree stmt;
    if ( token == Lexer::_OPEN_PARENTHESIS )
    {
        Tree conditions = new Tree::Node( lexer->getCurrLine(), Lexer::_CONDITIONS );
        token = lexer->getNextToken();

        conditions->push( analyzeExpression( lexer, token ) );
        if ( token != Lexer::_CLOSE_PARENTHESIS )
        {
            THROW_PARSING_ERROR( UNEXPECTED_TOKEN, lexer->getCurrLine(), token );
        }
        token = lexer->getNextToken();

        stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_WHILE );
        stmt->push( conditions );

        stmt->push( getBodyStmt( lexer, token ) );
    }
    return stmt;
}

Tree analyzeIfStatement( Lexer* lexer, Lexer::Token& token )
{
    Tree stmt;
    if ( token == Lexer::_OPEN_PARENTHESIS )
    {
        stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_IF );
        Tree conditions = new Tree::Node( lexer->getCurrLine(), Lexer::_CONDITIONS );
        token = lexer->getNextToken();

        conditions->push( analyzeExpression( lexer, token ) );
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

Tree analyzeElseStatement( Lexer* lexer, Lexer::Token& token )
{
    Tree els = new Tree::Node( lexer->getCurrLine(), Lexer::_ELSE );
    token = lexer->getNextToken();

    if ( token == Lexer::_IF )
    {
        token = lexer->getNextToken();
        els->push( analyzeIfStatement( lexer,  token ) );
    }
    else 
    {
        els->push( getBodyStmt( lexer, token ) );
    }
    return els;
}

Tree analyzeScopeBlock( Lexer* lexer, Lexer::Token& token )
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
                stmts->push( analyzeIdentifier( lexer, token ) );
            }
            break;

            case Lexer::_CONST:
            {   
                token = lexer->getNextToken();
                stmts->push( analyzeConstant( lexer, token ) );
            }
            break;

            case Lexer::_IF:
            {   
                token = lexer->getNextToken();
                stmts->push( analyzeIfStatement( lexer, token ) );
                while ( token == Lexer::_ELSE )
                {
                    stmts->push( analyzeElseStatement( lexer, token ) );
                }
            }
            break;

            case Lexer::_FOR:
            {
                token = lexer->getNextToken();
                stmts->push( analyzeForLoop( lexer, token ) );          
            }
            break;

            case Lexer::_WHILE:
            {  
                token = lexer->getNextToken();
                stmts->push( analyzeWhileLoop( lexer, token ) );
            }
            break;

            case Lexer::_OPEN_CURLY_BRACKET:
            {
                stmts->push( analyzeScopeBlock( lexer, token ) ); 
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
        case Lexer::_FUNCTION:
        {
            stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_FUNCTION );
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
            stmt = analyzeDataType( lexer, token );
            if ( token != Lexer::_COMMA && token != Lexer::_CLOSE_PARENTHESIS )
            {
                THROW_PARSING_ERROR( UNEXPECTED_TOKEN, lexer->getCurrLine(), token );
            }
        }
        break;
    }
    return stmt;
}

Tree analyzeFunctParameter( Lexer* lexer, Lexer::Token& token )
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
                case Lexer::_COLON:
                {
                    token = lexer->getNextToken();
                    identifier->push( getFunctParam( lexer, token ) );
                    identifier->type = Lexer::_STATIC_IDENTIFIER;
                    expr = identifier;
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

        case Lexer::_CONST:
        {
            token = lexer->getNextToken();
            expr = analyzeFunctParameter( lexer, token );
            switch( expr->type )
            {
                case Lexer::_IDENTIFIER:
                {   
                    expr->type = Lexer::_CONST_IDENTIFIER;
                }
                break;

                case Lexer::_STATIC_IDENTIFIER:
                {   
                    expr->type = Lexer::_CONST_STATIC_IDENTIFIER;
                }
                break;

                default:
                {
                    THROW_PARSING_ERROR( UNEXPECTED_TOKEN, lexer->getCurrLine(),  token );
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

Tree analyzeFunctionParameters( Lexer* lexer, Lexer::Token& token )
{
    Tree parameters = new Tree::Node( lexer->getCurrLine(), Lexer::_PARAMETERS );
    token = lexer->getNextToken();

    if( token == Lexer::_CLOSE_PARENTHESIS )
    {
        token = lexer->getNextToken();
        return parameters;
    }

    parameters->push( analyzeFunctParameter( lexer, token ) );
    while ( token == Lexer::_COMMA )
    {
        token = lexer->getNextToken();
        parameters->push( analyzeFunctParameter( lexer, token ) );
    }

    if( token != Lexer::_CLOSE_PARENTHESIS )
    {
        THROW_PARSING_ERROR( MISSING_TOKEN, lexer->getCurrLine(), Lexer::_CLOSE_PARENTHESIS );
    }

    token = lexer->getNextToken();
    return parameters;
}

Tree analyzeFunction( Lexer* lexer, Lexer::Token& token )
{
    Tree function = new Tree::Node( lexer->getCurrLine(), Lexer::_FUNCTION );
    token = lexer->getNextToken();
    function->push( analyzeFunctionParameters( lexer, token ) );
    function->push( analyzeScopeBlock( lexer, token ) );
    return function;
}

Tree analyzeHost( Lexer* lexer, Lexer::Token& token )
{
    Tree function = new Tree::Node( lexer->getCurrLine(), Lexer::_HOST );
    token = lexer->getNextToken();

    Tree params = analyzeFunctionParameters( lexer, token );
    for(int i=0; i<params->nodes.size(); i++)
    {
        function->push( params->nodes[ i ] );
    }
    return function;
}

Tree analyzeExpressionIdentifier( Lexer* lexer, Lexer::Token& token )
{   
    Tree identifier = new Tree::Node( Lexer::_IDENTIFIER, lexer->getLexeme() );
    token = lexer->getNextToken();
    Tree stmt;

    switch ( token )
    {
        case Lexer::_OPEN_PARENTHESIS: // function call
        {
            stmt = identifier;
            stmt->type = Lexer::_FUNCTION_CALL;
            stmt->line = lexer->getCurrLine();
            token = lexer->getNextToken();

            if ( token != Lexer::_CLOSE_PARENTHESIS )
            {
                stmt->push( analyzeExpression( lexer, token ) );
                while( token == Lexer::_COMMA )
                {
                    token = lexer->getNextToken();
                    stmt->push( analyzeExpression( lexer, token ) );
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
            stmt->type = Lexer::_ARRAY_INDEXED;
            stmt->line = lexer->getCurrLine();
            token = lexer->getNextToken();

            stmt->push( analyzeExpression( lexer, token ) );
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
            stmt->push( analyzeExpression( lexer, token ) );
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
            stmt->push( analyzeExpression( lexer, token ) ); 
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

bool tokenIsDelimiter( Lexer::Token& token )
{
    switch ( token )
    {
        case Lexer::_COMMA:
        case Lexer::_CLOSE_SQUARE_BRACKET:
        case Lexer::_CLOSE_CURLY_BRACKET: 
        case Lexer::_CLOSE_PARENTHESIS:
        case Lexer::_OPEN_CURLY_BRACKET: 
        case Lexer::_SEMICOLON: 
        case Lexer::_EQUAL: 
        case Lexer::_END_STREAM: return true;

        default: return false;
    }
}

Tree analyzeExpression( Lexer* lexer, Lexer::Token& token )
{
    std::stack< Tree > operators; // Stack to hold operators
    std::stack< Tree > operands;  // Stack to hold operands
    bool op = false;
    Tree e;

    while( !tokenIsDelimiter( token ) )
    {
        switch ( token )
        {
            default:
            {
                token = lexer->getNextToken();
            }
            break;

            case Lexer::_IDENTIFIER:
            {
                operands.push( analyzeIdentifier( lexer, token ) );
                op = false;
            }
            break;

            case Lexer::_OPEN_SQUARE_BRACKET:
            {
                operands.push( analyzeArrayObject( lexer, token ) );
                op = false;
            }
            break;

            case Lexer::_OPEN_PARENTHESIS:
            {
                e = new Tree::Node( lexer->getCurrLine(), Lexer::_ENCLOSE_PARENTHESIS );
                token = lexer->getNextToken();

                if ( token == Lexer::_CLOSE_PARENTHESIS )
                {
                    THROW_PARSING_ERROR( EXPECTED_TOKEN, lexer->getCurrLine(), Lexer::_SEMICOLON );
                }

                e->push( analyzeExpression( lexer, token ) );
                if ( token != Lexer::_CLOSE_PARENTHESIS )
                {
                    THROW_PARSING_ERROR( EXPECTED_TOKEN, lexer->getCurrLine(), Lexer::_SEMICOLON );
                }

                token = lexer->getNextToken();
                operands.push( e );
                op = false;
            }
            break;

            case Lexer::_NULL:
            {
                operands.push( new Tree::Node( lexer->getCurrLine(), Lexer::_NULL ) );
                token = lexer->getNextToken();
            }
            break;

            case Lexer::_VAR_STRING:
            case Lexer::_VAR_DOUBLE:
            case Lexer::_VAR_FLOAT:
            case Lexer::_VAR_LONG:
            case Lexer::_VAR_INT:
            case Lexer::_STRING:
            case Lexer::_DOUBLE:
            case Lexer::_FLOAT:
            case Lexer::_LONG:
            case Lexer::_INT:
            case Lexer::_BOOL:
            {
                operands.push( analyzeDataType( lexer, token ) );
                op = false;
            }
            break;

            case Lexer::_INVERT:
            case Lexer::_NOT:
            {
                Tree n = new Tree::Node( lexer->getCurrLine(), token );
                token = lexer->getNextToken();

                n->push( analyzeExpression( lexer, token ) );
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
                    while (!operators.empty() && precedence( operators.top()->type ) >= precedence( token ) )
                    {
                        resolve( operators, operands );
                    }
                }
                
                operators.push( new Tree::Node( lexer->getCurrLine(), token ) );
                token = lexer->getNextToken();
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

Tree analyzeRightTerm( Lexer* lexer, Lexer::Token& token )
{
    Tree stmt;
    switch ( token )
    {
        case Lexer::_FUNCTION:
        {
            stmt = analyzeFunction( lexer, token );
        }
        break;

        case Lexer::_HOST:
        {
            stmt = analyzeHost( lexer, token );
        }
        break;

        // case Lexer::_STRUCT: TODO: for the future
        // {}
        // break;
        
        default:
        {
            stmt = analyzeExpression( lexer, token );
        }
        break;
    }
    
    return stmt;
}

Tree analyzeIdentifier( Lexer* lexer, Lexer::Token& token )
{   
    Tree identifier = new Tree::Node( lexer->getCurrLine(), Lexer::_IDENTIFIER, lexer->getLexeme() );
    token = lexer->getNextToken();
    Tree stmt;

    switch ( token )
    {
        case Lexer::_EQUAL:
        {
            stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_ASSIGN );
            token = lexer->getNextToken();
            stmt->push( identifier );
            stmt->push( analyzeRightTerm( lexer, token ) );
        }
        break;

        case Lexer::_COLON:
        {
            stmt = new Tree::Node( lexer->getCurrLine(), Lexer::_DECLARATION );
            stmt->push( identifier );

            token = lexer->getNextToken();

            identifier->type = Lexer::_STATIC_IDENTIFIER;
            identifier->push( analyzeRightTerm( lexer, token ) );
        }
        break;

        case Lexer::_OPEN_PARENTHESIS: // function call
        {
            stmt = identifier;
            stmt->type = Lexer::_FUNCTION_CALL;
            token = lexer->getNextToken();

            if ( token != Lexer::_CLOSE_PARENTHESIS )
            {
                stmt->push( analyzeExpression( lexer, token ) );
                while( token == Lexer::_COMMA )
                {
                    token = lexer->getNextToken();
                    stmt->push( analyzeExpression( lexer, token ) );
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
            stmt->type = Lexer::_ARRAY_INDEXED;
            stmt->line = lexer->getCurrLine();
            token = lexer->getNextToken();

            stmt->push( analyzeExpression( lexer, token ) );

            if ( token != Lexer::_CLOSE_SQUARE_BRACKET )
            {
                THROW_PARSING_ERROR( UNEXPECTED_TOKEN, lexer->getCurrLine(), token );
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
            stmt->push( analyzeExpression( lexer, token ) ); 
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

Tree analyzeConstant( Lexer* lexer, Lexer::Token& token )
{
    token = lexer->getNextToken();
    Tree stmt = analyzeIdentifier( lexer, token );
    
    switch( stmt->type )
    {
        case Lexer::_IDENTIFIER:
        {   
            stmt->type = Lexer::_CONST_IDENTIFIER;
        }
        break;

        case Lexer::_ASSIGN:
        {   
            stmt[ 0 ]->type = Lexer::_CONST_IDENTIFIER;
        }
        break;

        case Lexer::_DECLARATION:
        {   
            stmt[ 0 ]->type = Lexer::_CONST_STATIC_IDENTIFIER;
        }
        break;

        case Lexer::_STATIC_IDENTIFIER:
        {   
            stmt->type = Lexer::_CONST_STATIC_IDENTIFIER;
        }
        break;

        default:
        {
            THROW_PARSING_ERROR( UNEXPECTED_TOKEN, lexer->getCurrLine(),  token );
        }
        break;
    }

    return stmt;
}

void evaluateSemicolon( Lexer* lexer, Lexer::Token& token, Tree& node )
{

    if ( token != Lexer::_SEMICOLON )
    {
        switch ( node->type )
        {
            case Lexer::_DECLARATION:
            {
                if ( node[ 0 ][ 0 ]->type != Lexer::_FUNCTION )
                {
                    THROW_PARSING_ERROR( EXPECTED_TOKEN, lexer->getCurrLine(),  Lexer::_SEMICOLON );
                }
            }
            break;

            case Lexer::_CONST:
            {
                evaluateSemicolon( lexer, token, node[ 0 ] );
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

bool Syntax::analyze( Lexer* lexer )
{
    _tree = new Tree::Node();

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
                    stmt = analyzeIdentifier( lexer, token );
                    _tree->push( stmt );
                    evaluateSemicolon( lexer, token, stmt );
                }
                break;

                case Lexer::_CONST:
                {   
                    stmt = analyzeConstant( lexer, token );
                    _tree->push( stmt );
                    evaluateSemicolon( lexer, token, stmt );
                }
                break;

                case Lexer::_SEMICOLON:
                {   
                    token = lexer->getNextToken();
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
    std::cout <<Lexer::toString( ref->type ) <<": " << ref->id <<"\n";
    for(int i=0; i<ref->nodes.size(); i++)
    {
        printNode( tabs + 1, ref->nodes[ i ] );
    }
}

void Syntax::printTree()
{
    std::cout <<"\n\nParser \n";
    for(int i=0; i<_tree->nodes.size(); i++)
    {
        printNode( 0, _tree->nodes[ i ] );
        std::cout <<"\n";
    }
}

Tree& Syntax::getTree()
{
    return _tree;
}

void Syntax::clear()
{
    _tree.clear();
}

Syntax::~Syntax()
{
    clear();
}

Syntax::Syntax()
{}
    