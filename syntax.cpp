#include "syntax.hpp"

// #define SYNTAX_ERR(parser, err, tok) parser->syntaxError( __FUNCTION__, __LINE__, err, parser->toString( tok ) )
// void Syntax::syntaxError( const char* func, int ln, Token token, const char* msg )
// {
//     const char* err = nullptr;
//     int line = preprocessor.getCurrentLineNumber();
//     std::string errstr = ":  @function -> " + std::string( func ) + " @line ->" + std::to_string( ln );
//     std::string errmsg = std::string( "[ SYNTAX-ERROR ]: " ) + errstr + " : [ issue ]: @ln -> [" +  std::to_string( line ) + "] "  + toString( token ) +" -> " + msg;
//     _error = errmsg;
// }


Tree analyzeOperation( Tree& operand );
Tree analyzeAssignment( uint32_t scope, Tree& assignment );
Tree analyzeDeclaration( uint32_t scope, Tree& declaration );
Tree getFunctionDeclaration( uint32_t scope, Tree& function );

Tree getIdentifier( uint32_t flags, Tree& param )
{
    Tree var = new Tree::Node();
    var->node_type = Lexer::_VAR;
    var->flags = flags;
    var->id = param->id;
    return var;
}

Tree getValue( uint32_t flags, Tree& variable )
{
    Tree var = new Tree::Node();
    var->id = variable->id;
    var->flags = flags;

    switch( variable->node_type )
    {
        case Lexer::_VAR_BOOL:
        case Lexer::_BOOL:
        { 
            var->node_type = Lexer::_BOOL;
        }
        break;

        case Lexer::_VAR_INT:
        case Lexer::_INT:
        { 
            var->node_type = Lexer::_INT;
        }
        break;

        case Lexer::_VAR_LONG:
        case Lexer::_LONG:
        { 
            var->node_type = Lexer::_LONG;
        }
        break;

        case Lexer::_VAR_FLOAT:
        case Lexer::_FLOAT:
        { 
            var->node_type = Lexer::_FLOAT;
        }
        break;

        case Lexer::_VAR_DOUBLE:
        case Lexer::_DOUBLE:
        { 
            var->node_type = Lexer::_DOUBLE;
        }
        break;

        case Lexer::_VAR_STRING:
        case Lexer::_STRING:
        { 
            var->node_type = Lexer::_STRING;
        }
        break;

        case Lexer::_ENCLOSE_SQUARE_BRACKET:
        { 
            var->node_type = Lexer::_ARRAY;
        }
        break;

        default:
        {
            throw std::runtime_error( __FUNCTION__ );
        }
        break;
    }

    if( !variable->nodes.empty() )
    {
        Tree& node = variable->nodes[ 0 ];
        switch ( node->node_type )
        {
            case Lexer::_ENCLOSE_PARENTHESIS:
            {
                switch ( node->nodes[ 0 ]->node_type )
                {
                    case Lexer::_ADD:
                    case Lexer::_SUB:
                    case Lexer::_MUL:
                    case Lexer::_DIV:
                    case Lexer::_MOD:
                    case Lexer::_FUNCTION_CALL:
                    {
                        var->push( analyzeOperation( node->nodes[ 0 ] ) );
                    }
                    break;
                
                    default:
                    {
                        var->id = node->nodes[ 0 ]->id;
                    }
                    break;
                }
            }
            break;
        
            default:
            {
                std::string msg = __FUNCTION__;
                msg += + " line#: " + std::to_string( variable->line );
                msg += " : dynamic function declaration '"+ node->id +"' can not be defined in global scope.";
                throw std::runtime_error( msg );
            }
            break;
        }
    }
    return var;
}

Tree getVariableDeclaration( uint32_t flags, Tree& declaration )
{
    Tree var = getIdentifier( flags, declaration->nodes[ 0 ] );
    var->push( getValue( flags, declaration->nodes[ 1 ] ) );
    return var;
}

Tree getParamAddress( uint32_t flags, Tree& address )
{
    Tree var;
    switch ( address->node_type )
    {
        case Lexer::_IDENTIFIER:
        {
            var = getIdentifier( flags | Syntax::ADDRESS, address );
        }
        break;

        case Lexer::_ASSIGN:
        {
            var = getVariableDeclaration( flags | Syntax::ADDRESS, address );
        }
        break;

        case Lexer::_DECLARATION:
        {
            var = getVariableDeclaration( flags | Syntax::ADDRESS | Syntax::STATIC_VARIABLE, address );
        }
        break;

        default:
        {
            throw std::runtime_error( __FUNCTION__ + std::string(" ") + Lexer::toString( address->node_type ) );
        }
        break;
    }
    return var;
}

Tree analyzeParameter( Tree& enclose_parenthesis )
{
    Tree parameters = new Tree::Node( enclose_parenthesis->line, Lexer::_ENCLOSE_PARENTHESIS );
    for( int i = 0; i < enclose_parenthesis->nodes.size(); i++ )
    {
        Tree& param = enclose_parenthesis->nodes[ i ];
        switch ( param->node_type )
        {
            case Lexer::_IDENTIFIER:
            {
                parameters->push( getIdentifier( 0, param ) );
            }
            break;

            case Lexer::_DECLARATION:
            {
                parameters->push( getVariableDeclaration( Syntax::STATIC_VARIABLE, param ) );
            }
            break;

            case Lexer::_ASSIGN:
            {
                parameters->push( getVariableDeclaration( 0, param ) );
            }
            break;

            case Lexer::_ADDRESS:
            {
                parameters->push( getParamAddress( 0, param->nodes[ 0 ] ) );
            }
            break;

            case Lexer::_CONST:
            {
                Tree& node = param->nodes[ 0 ];
                switch ( node->node_type )
                {
                    case Lexer::_IDENTIFIER:
                    {
                        parameters->push( getIdentifier( Syntax::CONST, node ) );
                    }
                    break;

                    case Lexer::_ASSIGN:
                    {
                        parameters->push( getVariableDeclaration( Syntax::CONST, node ) );
                    }
                    break;

                    case Lexer::_DECLARATION:
                    {
                        parameters->push( getVariableDeclaration( Syntax::CONST | Syntax::STATIC_VARIABLE, node ) );
                    }
                    break;

                    case Lexer::_ADDRESS:
                    {
                        parameters->push( getParamAddress( Syntax::CONST, node->nodes[ 0 ] ) );
                    }
                    break;

                    default:
                    {
                        throw std::runtime_error( __FUNCTION__ );
                    }
                    break;
                }
            }
            break;

            default:
            {
                throw std::runtime_error( __FUNCTION__ );
            }
            break;
        }
    }
    return parameters;
}

Tree analyzeScopeBlock( uint32_t scope, Tree& scope_block )
{
    Tree block = new Tree::Node(0, Lexer::_SCOPE_BLOCK);
    for( uint32_t i = 0; i < scope_block->nodes.size(); i++ )
    {
        Tree& stmt = scope_block->nodes[ i ];
        switch( stmt->node_type )
        {
            default:
            {
                throw std::runtime_error( __FUNCTION__ );
            }
            break;

            case Lexer::_IDENTIFIER:
            {   
                block->push( getIdentifier( 0, stmt ) );
            }
            break;

            case Lexer::_DECLARATION:
            {
                switch( stmt->nodes[ 1 ]->node_type )
                {
                    default:
                    {
                        throw std::runtime_error( __FUNCTION__ );
                    }
                    break;

                    case Lexer::_INT:
                    case Lexer::_LONG:
                    case Lexer::_FLOAT:
                    case Lexer::_DOUBLE:
                    case Lexer::_STRING:
                    case Lexer::_VAR_INT:
                    case Lexer::_VAR_LONG:
                    case Lexer::_VAR_FLOAT:
                    case Lexer::_VAR_DOUBLE:
                    case Lexer::_VAR_STRING:
                    {   
                        block->push( getVariableDeclaration( Syntax::STATIC_VARIABLE, stmt ) );
                    }
                    break;

                    case Lexer::_CONST:
                    {   
                        Tree& node = stmt->nodes[ 0 ];
                        switch ( node->node_type )
                        {
                            case Lexer::_INT:
                            case Lexer::_LONG:
                            case Lexer::_FLOAT:
                            case Lexer::_DOUBLE:
                            case Lexer::_STRING:
                            case Lexer::_VAR_INT:
                            case Lexer::_VAR_LONG:
                            case Lexer::_VAR_FLOAT:
                            case Lexer::_VAR_DOUBLE:
                            case Lexer::_VAR_STRING:
                            {   
                                block->push( getVariableDeclaration( Syntax::CONST | Syntax::STATIC_VARIABLE, node ) );
                            }
                            break;

                            case Lexer::_DECLARATION:
                            {   
                                block->push( getVariableDeclaration( Syntax::CONST | Syntax::STATIC_VARIABLE, node ) );
                            }
                            break;

                            default:
                            {
                                throw std::runtime_error( __FUNCTION__ );
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            break;

            case Lexer::_ASSIGN:
            {
                block->push( analyzeAssignment( scope, stmt ) );
            }
            break;

            case Lexer::_CONST:
            {
                Tree& node = stmt->nodes[ 0 ];
                switch ( node->node_type )
                {
                    case Lexer::_DECLARATION:
                    {   
                        block->push( analyzeDeclaration( Syntax::CONST | Syntax::STATIC_VARIABLE, node ) );
                    }
                    break;

                    case Lexer::_ASSIGN:
                    {   
                        block->push( analyzeAssignment( Syntax::CONST | Syntax::STATIC_VARIABLE, node ) );
                    }
                    break;

                    default:
                    {
                        throw std::runtime_error( __FUNCTION__ );
                    }
                    break;
                }
            }
            break;

            case Lexer::_ENCLOSE_CURLY_BRACKET:
            {
                block->push( analyzeScopeBlock( scope + 1, stmt ) );
            }
            break;

            case Lexer::_WHILE:
            {
            }
            break;

            case Lexer::_FOR:
            {
            }
            break;
        }
    }
    return block;
}

Tree getFunctionDeclaration( uint32_t scope, Tree& function )
{
    Tree function_definition = new Tree::Node();
    function_definition->node_type = Lexer::_FUNCTION; 
    function_definition->push( analyzeParameter( function->nodes[ 0 ] ) );
    function_definition->push( analyzeScopeBlock( scope, function->nodes[ 1 ] ) );
    return function_definition;
}

Tree analyzeOperation( Tree& operand )
{
    Tree op;
    switch( operand->node_type )
    {
        default:
        {   
            std::string msg = __FUNCTION__;
            msg += + " line#: " + std::to_string( operand->line );
            msg += " : invalid operation '"+ operand->id +"'";
            throw std::runtime_error( msg );
        }
        break;

        case Lexer::_IDENTIFIER:
        {   
            op = getIdentifier( 0, operand );
        }
        break;

        case Lexer::_ADD:
        {   
            op = new Tree::Node( Lexer::_ADD );
            op->push( analyzeOperation( operand->nodes[ 0 ] ) );
            op->push( analyzeOperation( operand->nodes[ 1 ] ) );
        }
        break;

        case Lexer::_SUB:
        {   
            op = new Tree::Node( Lexer::_SUB );
            op->push( analyzeOperation( operand->nodes[ 0 ] ) );
            op->push( analyzeOperation( operand->nodes[ 1 ] ) );
        }
        break;

        case Lexer::_DIV:
        {   
            op = new Tree::Node( Lexer::_DIV );
            op->push( analyzeOperation( operand->nodes[ 0 ] ) );
            op->push( analyzeOperation( operand->nodes[ 1 ] ) );
        }
        break;

        case Lexer::_MUL:
        {   
            op = new Tree::Node( Lexer::_MUL );
            op->push( analyzeOperation( operand->nodes[ 0 ] ) );
            op->push( analyzeOperation( operand->nodes[ 1 ] ) );
        }
        break;

        case Lexer::_MOD:
        {   
            op = new Tree::Node( Lexer::_MOD );
            op->push( analyzeOperation( operand->nodes[ 0 ] ) );
            op->push( analyzeOperation( operand->nodes[ 1 ] ) );
        }
        break;

        case Lexer::_FUNCTION_CALL:
        {   
            op = new Tree::Node( Lexer::_FUNCTION_CALL, operand->id );
            for( int i=0; i<operand->nodes.size(); i++ )
            {
                op->push( analyzeOperation( operand->nodes[ i ] ) );
            }
        }
        break;

        case Lexer::_INT:
        case Lexer::_LONG:
        case Lexer::_FLOAT:
        case Lexer::_DOUBLE:
        case Lexer::_STRING:
        case Lexer::_VAR_INT:
        case Lexer::_VAR_LONG:
        case Lexer::_VAR_FLOAT:
        case Lexer::_VAR_DOUBLE:
        case Lexer::_VAR_STRING:
        {   
            op = getValue( 0, operand );
        }
        break;
    }
    return op;
}

Tree analyzeAssignment( uint32_t scope, Tree& assignment )
{
    Tree assign = new Tree::Node( assignment->line, Lexer::_ASSIGN );
    assign->push( getIdentifier( 0, assignment->nodes[ 0 ] ) );

    switch( assignment->nodes[ 1 ]->node_type )
    {
        default:
        {
            std::string msg = __FUNCTION__;
            msg += + " line#: " + std::to_string( assignment->line );
            msg += " : syntax error '"+ assignment->nodes[ 0 ]-> id +"'";
            throw std::runtime_error( msg );
        }
        break;

        case Lexer::_FUNCTION:
        {   
            std::string msg = __FUNCTION__;
            msg += + " line#: " + std::to_string( assignment->line );
            msg += " : dynamic function declaration '"+ assignment->nodes[ 0 ]-> id +"' can not be defined in global scope.";
            throw std::runtime_error( msg );
        }
        break;

        case Lexer::_INT:
        case Lexer::_LONG:
        case Lexer::_FLOAT:
        case Lexer::_DOUBLE:
        case Lexer::_STRING:
        case Lexer::_VAR_INT:
        case Lexer::_VAR_LONG:
        case Lexer::_VAR_FLOAT:
        case Lexer::_VAR_DOUBLE:
        case Lexer::_VAR_STRING:
        {   
            assign->push( getValue( 0, assignment->nodes[ 1 ] ) );
        }
        break;

        case Lexer::_CONST:
        {   
            Tree& node = assignment->nodes[ 1 ]->nodes[ 0 ];
            switch ( node->node_type )
            {
                case Lexer::_INT:
                case Lexer::_LONG:
                case Lexer::_FLOAT:
                case Lexer::_DOUBLE:
                case Lexer::_STRING:
                case Lexer::_VAR_INT:
                case Lexer::_VAR_LONG:
                case Lexer::_VAR_FLOAT:
                case Lexer::_VAR_DOUBLE:
                case Lexer::_VAR_STRING:
                {   
                    assign->push( getValue( Syntax::CONST, node ) );
                }
                break;

                default:
                {
                    std::string msg = __FUNCTION__;
                    msg += + " line#: " + std::to_string( assignment->line );
                    msg += " : dynamic function declaration '"+ assignment->nodes[ 0 ]-> id +"' can not be defined in global scope.";
                    throw std::runtime_error( msg );
                }
                break;
            }
        }
        break;

        case Lexer::_ADD:
        case Lexer::_SUB:
        case Lexer::_MUL:
        case Lexer::_DIV:
        case Lexer::_MOD:
        {   
            assign->push( analyzeOperation( assignment->nodes[ 1 ] ) );
        }
        break;
    }
    return assign;
}

Tree analyzeDeclaration( uint32_t scope, Tree& declaration )
{
    Tree var;
    switch( declaration->nodes[ 1 ]->node_type )
    {
        default:
        {
            throw std::runtime_error( __FUNCTION__ );
        }
        break;

        case Lexer::_HOST:
        {   
            //analyzeHost( _syntax_tree, node );
        }
        break;

        case Lexer::_FUNCTION:
        {   
            var = getFunctionDeclaration( scope, declaration->nodes[ 1 ] );
            var->id = declaration->nodes[ 0 ]->id;
            var->flags = Syntax::STATIC_VARIABLE;
        }
        break;

        case Lexer::_INT:
        case Lexer::_LONG:
        case Lexer::_FLOAT:
        case Lexer::_DOUBLE:
        case Lexer::_STRING:
        case Lexer::_VAR_INT:
        case Lexer::_VAR_LONG:
        case Lexer::_VAR_FLOAT:
        case Lexer::_VAR_DOUBLE:
        case Lexer::_VAR_STRING:
        {   
            var = getVariableDeclaration( Syntax::STATIC_VARIABLE, declaration );
        }
        break;

        case Lexer::_CONST:
        {   
            switch ( declaration->nodes[ 1 ]->nodes[ 0 ]->node_type )
            {
                case Lexer::_VAR_INT:
                case Lexer::_VAR_LONG:
                case Lexer::_VAR_FLOAT:
                case Lexer::_VAR_DOUBLE:
                case Lexer::_VAR_STRING:
                {   
                    var = getVariableDeclaration( Syntax::STATIC_VARIABLE | Syntax::CONST, declaration->nodes[ 1 ]->nodes[ 0 ] );
                }
                break;

                default:
                {
                    throw std::runtime_error( __FUNCTION__ );
                }
                break;
            }
        }
        break;
    }
    return var;
}

Tree analyzeConst( uint32_t scope, Tree& cnst )
{
    Tree stmt;
    switch ( cnst->nodes[ 0 ]->node_type )
    {
        case Lexer::_DECLARATION:
        {   
            stmt = analyzeDeclaration( scope, cnst->nodes[ 0 ] );
            stmt->flags += Syntax::CONST;
        }
        break;

        case Lexer::_ASSIGN:
        {   
            stmt = analyzeAssignment( scope, cnst->nodes[ 0 ] );
            stmt->flags += Syntax::CONST;
        }
        break;

        case Lexer::_IDENTIFIER:
        {   
            std::string msg = __FUNCTION__;
            msg += + " line#: " + std::to_string( cnst->line );
            msg += " : constant declared variables require a value.";
            throw std::runtime_error( msg );
        }
        break;

        default:
        {
            throw std::runtime_error( __FUNCTION__ );
        }
        break;
    }

    return stmt;
}

bool Syntax::analyze( Parser* parser )
{
    try
    {
        uint32_t scope = 0;
        _syntax_tree = new Tree::Node();
        Tree parse_tree = parser->getTree();
        for( uint32_t i = 0; i < parse_tree->nodes.size(); i++ )
        { 
            Tree& node = parse_tree->nodes[ i ];

            switch( node->node_type )
            {
                case Lexer::_DECLARATION:
                {   
                    _syntax_tree->push( analyzeDeclaration( scope, node ) );
                }
                break;

                case Lexer::_ASSIGN:
                {   
                    _syntax_tree->push( analyzeAssignment( scope, node ) );
                }
                break;

                case Lexer::_CONST:
                {   
                    _syntax_tree->push( analyzeConst( scope, node ) );
                }
                break;

                case Lexer::_IDENTIFIER:
                {   
                    _syntax_tree->push( getIdentifier( 0, node ) );
                }
                break;

                default:
                {
                    throw std::runtime_error( __FUNCTION__ );
                }
                break;
            }
        }
    }
    catch(const std::runtime_error& e)
    {
        std::cerr <<"[ ERROR ] : "<< e.what() << '\n';
    }
    
    return true;
}

void printSyntaxTree(int tabs, Tree& ref)
{
    for(int i=0; i<tabs; i++) std::cout <<"- ";
    std::cout << Lexer::toString( ref->node_type ) <<": " << ref->id <<"\n";
    for(int i=0; i<ref->nodes.size(); i++)
    {
        printSyntaxTree( tabs + 1, ref->nodes[ i ] );
    }
}

void Syntax::printTree()
{
    if( !_syntax_tree ) return;
    for(int i=0; i<_syntax_tree->nodes.size(); i++)
    {
        printSyntaxTree( 0, _syntax_tree->nodes[ i ] );
    }
}

Tree& Syntax::getTree()
{
    return _syntax_tree;
}

void Syntax::clear()
{
    _syntax_tree.clear();
}

Syntax::~Syntax()
{
    clear();
}

Syntax::Syntax()
{}
    