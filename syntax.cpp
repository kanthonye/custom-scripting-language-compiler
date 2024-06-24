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
Tree getIdentifier( Tree& declaration );
Tree analyzeFunction( Tree& function );
Tree analyzeHost( Tree& function );
Tree analyzeNode( Tree& node );
Tree getValue( Tree& node );


Tree analyzeParameter( Tree& node )
{
    Tree var;
    switch ( node->node_type )
    {
        case Lexer::_IDENTIFIER:
        {
            var = getIdentifier( node );
        }
        break;

        case Lexer::_DECLARATION:
        {
            var = new Tree::Node( Lexer::_VAR_STATIC, node->nodes[ 0 ]->id );
            var->push( analyzeOperation( node->nodes[ 1 ] ) );
        }
        break;

        case Lexer::_ASSIGN:
        {
            var = analyzeOperation( node->nodes[ 1 ] );
        }
        break;

        case Lexer::_ADDRESS:
        {
            var = new Tree::Node( Lexer::_ADDRESS );
            var = analyzeParameter( node->nodes[ 0 ] );
        }
        break;

        case Lexer::_CONST:
        {
            Tree& node = node->nodes[ 0 ];
            var = analyzeParameter( node );
        }
        break;

        default:
        {
            throw std::runtime_error( __FUNCTION__ );
        }
        break;
    }
    
}
Tree analyzeParameters( Tree& node )
{
    Tree parameters = new Tree::Node( node->line, Lexer::_ENCLOSE_PARENTHESIS );
    for( int i = 0; i < node->nodes.size(); i++ )
    {
        parameters->push( node->nodes[ i ] );
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
            case Lexer::_DECLARATION:
            case Lexer::_ASSIGN:
            case Lexer::_CONST:
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

Tree analyzeFunction( Tree& function )
{
    Tree function_definition = new Tree::Node();
    function_definition->node_type = Lexer::_FUNCTION; 
    function_definition->push( analyzeParameters( function->nodes[ 0 ] ) );
    function_definition->push( analyzeScopeBlock( 0, function->nodes[ 1 ] ) );
    return function_definition;
}

Tree analyzeHost( Tree& function )
{
    Tree function_definition = new Tree::Node();
    function_definition->node_type = Lexer::_HOST; 
    function_definition->push( analyzeParameters( function->nodes[ 0 ] ) );
    return function_definition;
}

Tree getIdentifier( uint32_t flags, Tree& node )
{
    Tree var = new Tree::Node();
    if ( node->nodes.empty() )
    {
        var->node_type = Lexer::_VAR;
        var->flags = flags;
        var->id = node->id;
    }
    else
    {
        var->node_type = node->nodes[ 0 ]->node_type;
        var->id = node->id;
    }
    return var;
}

/** 
 * @fn getValue 
 * @param flags are bit flags to set constrains on the value {CONST, ADDRESS, STATIC_VARIABLE}.
 * @param node The varible node to decode. 7, int(7), float( 3 + n ), etc
 * @return Tree node containing the node data and type
 */
Tree getValue( Tree& node )
{
    Tree var = new Tree::Node();
    var->id = node->id;
    
    switch( node->node_type )
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

        case Lexer::_ARRAY:
        { 
            var->node_type = Lexer::_ARRAY;
        }
        break;

        case Lexer::_FUNCTION:
        {   
            var = analyzeFunction( node );
        }
        break;

        case Lexer::_HOST:
        {   
            var = analyzeHost( node );
        }
        break;

        case Lexer::_CONST:
        { 
            var = getValue( node->nodes[ 0 ] );
            switch ( var->node_type )
            {
                case Lexer::_INT:
                {
                    var->node_type = Lexer::_CONST_INT;
                }
                break;

                case Lexer::_LONG:
                {   
                    var->node_type = Lexer::_CONST_LONG;
                }
                break;

                case Lexer::_FLOAT:
                {   
                    var->node_type = Lexer::_CONST_FLOAT;
                }
                break;

                case Lexer::_DOUBLE:
                {   
                    var->node_type = Lexer::_CONST_DOUBLE;
                }
                break;

                case Lexer::_STRING:
                {   
                    var->node_type = Lexer::_CONST_STRING;
                }
                break;

                case Lexer::_FUNCTION:
                {   
                    var->node_type = Lexer::_CONST_FUNCTION;
                }
                break;

                case Lexer::_ARRAY:
                {   
                    var->node_type = Lexer::_CONST_STRING;
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

    if( !node->nodes.empty() && node->node_type != Lexer::_CONST )
    {
        var->push( analyzeOperation( node->nodes[ 0 ] ) );
    }
    return var;
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

        case Lexer::_AND:
        {   
            op = new Tree::Node( Lexer::_AND );
            op->push( analyzeOperation( operand->nodes[ 0 ] ) );
            op->push( analyzeOperation( operand->nodes[ 1 ] ) );
        }
        break;

        case Lexer::_OR:
        {   
            op = new Tree::Node( Lexer::_AND );
            op->push( analyzeOperation( operand->nodes[ 0 ] ) );
            op->push( analyzeOperation( operand->nodes[ 1 ] ) );
        }
        break;

        case Lexer::_INCREMENT:
        {   
            op = new Tree::Node( Lexer::_INCREMENT );
            op->id = operand->nodes[ 0 ]->id;
        }
        break;

        case Lexer::_DECREMENT:
        {   
            op = new Tree::Node( Lexer::_DECREMENT );
            op->id = operand->nodes[ 0 ]->id;
        }
        break;

        case Lexer::_NOT:
        {   
            op = new Tree::Node( Lexer::_NOT );
            op->id = operand->nodes[ 0 ]->id;
        }
        break;

        case Lexer::_INVERT:
        {   
            op = new Tree::Node( Lexer::_INVERT );
            op->id = operand->nodes[ 0 ]->id;
        }
        break;

        case Lexer::_ADD_EQUAL:
        {   
            Tree n = new Tree::Node( Lexer::_ADD );
            n->push( getIdentifier( 0, operand->nodes[ 0 ] ) );
            n->push( analyzeOperation( operand->nodes[ 1 ] ) );

            op = new Tree::Node( Lexer::_ASSIGN );
            op->push( n->nodes[ 0 ] );
            op->push( n );
        }
        break;

        case Lexer::_SUB_EQUAL:
        {   
            Tree n = new Tree::Node( Lexer::_SUB );
            n->push( getIdentifier( 0, operand->nodes[ 0 ] ) );
            n->push( analyzeOperation( operand->nodes[ 1 ] ) );

            op = new Tree::Node( Lexer::_ASSIGN );
            op->push( n->nodes[ 0 ] );
            op->push( n );
        }
        break;

        case Lexer::_MUL_EQUAL:
        {   
            Tree n = new Tree::Node( Lexer::_MUL );
            n->push( getIdentifier( 0, operand->nodes[ 0 ] ) );
            n->push( analyzeOperation( operand->nodes[ 1 ] ) );

            op = new Tree::Node( Lexer::_ASSIGN );
            op->push( n->nodes[ 0 ] );
            op->push( n );
        }
        break;

        case Lexer::_DIV_EQUAL:
        {   
            Tree n = new Tree::Node( Lexer::_DIV );
            n->push( getIdentifier( 0, operand->nodes[ 0 ] ) );
            n->push( analyzeOperation( operand->nodes[ 1 ] ) );

            op = new Tree::Node( Lexer::_ASSIGN );
            op->push( n->nodes[ 0 ] );
            op->push( n );
        }
        break;

        case Lexer::_MOD_EQUAL:
        {   
            Tree n = new Tree::Node( Lexer::_MOD );
            n->push( getIdentifier( 0, operand->nodes[ 0 ] ) );
            n->push( analyzeOperation( operand->nodes[ 1 ] ) );

            op = new Tree::Node( Lexer::_ASSIGN );
            op->push( n->nodes[ 0 ] );
            op->push( n );
        }
        break;

        case Lexer::_INVERT_EQUAL:
        {   
            Tree n = new Tree::Node( Lexer::_INVERT );
            n->push( getIdentifier( 0, operand->nodes[ 0 ] ) );
            n->push( analyzeOperation( operand->nodes[ 1 ] ) );

            op = new Tree::Node( Lexer::_ASSIGN );
            op->push( n->nodes[ 0 ] );
            op->push( n );
        }
        break;

        case Lexer::_EXP_EQUAL:
        {   
            Tree n = new Tree::Node( Lexer::_EXP );
            n->push( getIdentifier( 0, operand->nodes[ 0 ] ) );
            n->push( analyzeOperation( operand->nodes[ 1 ] ) );

            op = new Tree::Node( Lexer::_ASSIGN );
            op->push( n->nodes[ 0 ] );
            op->push( n );
        }
        break;

        case Lexer::_ENCLOSE_PARENTHESIS:
        {   
            op = new Tree::Node( Lexer::_ENCLOSE_PARENTHESIS );
            op->push( analyzeOperation( operand->nodes[ 0 ] ) );
        }
        break;

        case Lexer::_INT:
        case Lexer::_BOOL:
        case Lexer::_LONG:
        case Lexer::_CONST:
        case Lexer::_FLOAT:
        case Lexer::_DOUBLE:
        case Lexer::_STRING:
        case Lexer::_VAR_INT:
        case Lexer::_VAR_BOOL:
        case Lexer::_VAR_LONG:
        case Lexer::_VAR_FLOAT:
        case Lexer::_VAR_DOUBLE:
        case Lexer::_VAR_STRING:
        case Lexer::_FUNCTION_CALL:
        {   
            op = getValue( operand );
        }
        break;
    }
    return op;
}

Tree analyzeNode( Tree& node )
{
    Tree var;
    switch( node->node_type )
    {
        case Lexer::_IDENTIFIER:
        {   
            var = getIdentifier( 0, node );
        }
        break;

        case Lexer::_DECLARATION:
        {   
            var = new Tree::Node( Lexer::_VAR_STATIC, node->nodes[ 0 ]->id );
            var->push( analyzeOperation( node->nodes[ 1 ] ) );
        }
        break;

        case Lexer::_ASSIGN:
        {   
            var = new Tree::Node( Lexer::_VAR, node->nodes[ 0 ]->id );
            var->push( analyzeOperation( node->nodes[ 1 ] ) );
        }
        break;

        case Lexer::_CONST:
        {   
            var = analyzeNode( node->nodes[ 0 ] );
            switch( var->node_type )
            {
                case Lexer::_IDENTIFIER:
                {   
                    var->node_type = Lexer::_CONST_VAR;
                }
                break;

                case Lexer::_VAR:
                {   
                    var->node_type = Lexer::_CONST_VAR;
                }
                break;

                case Lexer::_VAR_STATIC:
                {   
                    var->node_type = Lexer::_CONST_VAR_STATIC;
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
    return var;
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
            _syntax_tree->push( analyzeNode( node ) );
        }
    }
    catch(const std::runtime_error& e)
    {
        std::cerr <<"[ ERROR ] : "<< e.what() << '\n';
        return false;
    }
    
    return true;
}

void printSyntaxTree(int tabs, Tree& n)
{
    for(int i=0; i<tabs; i++) std::cout <<"- ";

    std::cout << Lexer::toString( n->node_type ) <<": " << n->id <<"\n";
    
    for(int i=0; i<n->nodes.size(); i++)
    {
        printSyntaxTree( tabs + 1, n->nodes[ i ] );
    }
}

void Syntax::printTree()
{
    if( !_syntax_tree ) return;
    std::cout <<"\n\n--- --- Syntax Analyzer --- ---\n";
    for(int i=0; i<_syntax_tree->nodes.size(); i++)
    {
        printSyntaxTree( 0, _syntax_tree->nodes[ i ] );
        std::cout <<"\n";
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
    