#ifndef semantic_hpp
#define semantic_hpp

#include "syntax.hpp"

// class Symbols
// {
// public:

//     struct Info 
//     {
//         std::string id;
//         int scope;
//         int index;
//     };

// public:

//     bool insert( int scope, const std::string& id, Tree& node );
//     Tree find( int scope, const std::string& id );
//     Tree find( const std::string& id );
//     Symbols();

// private:

//     std::map< std::string, std::vector< Info > > table;
//     std::vector< Tree > stmts;
//     int refcount;
// };


class Semantic{
public:

    // bool validate( int scope, Symbols& symbols, Syntax::Ref& n );
    // bool analyzeContinue( int scope, Syntax::Ref& node );
    // bool analyzeReturn( int scope, Syntax::Ref& node );
    // bool analyzeSwitch( int scope, Syntax::Ref& node );
    // bool analyzeBreak( int scope, Syntax::Ref& node );
    // bool analyzeWhile( int scope, Syntax::Ref& node );
    // bool analyzeFor( int scope, Syntax::Ref& node );
    // bool analyzeIf( int scope, Syntax::Ref& node );
    // bool analyzeScope( int scope, Syntax::Ref& n );
    // bool analyzeAddEqual( int scope, Syntax::Ref& n );
    // bool analyzeSubEqual( int scope, Syntax::Ref& n );
    // bool analyzeDivEqual( int scope, Syntax::Ref& n );
    // bool analyzeMulEqual( int scope, Syntax::Ref& n );
    // bool analyzeModEqual( int scope, Syntax::Ref& n );
    // bool analyzeExpEqual( int scope, Syntax::Ref& n );
    // bool analyzeAdd( int scope, Syntax::Ref& n );
    // bool analyzeSub( int scope, Syntax::Ref& n );
    // bool analyzeDiv( int scope, Syntax::Ref& n );
    // bool analyzeMul( int scope, Syntax::Ref& n );
    // bool analyzeMod( int scope, Syntax::Ref& n );
    // bool analyzeExp( int scope, Syntax::Ref& n );
    // bool analyzeFuncParams( int scope, Syntax::Ref& n );
    // bool analyzeFunctDefn( int scope, Syntax::Ref& node );
    // bool analyzeFunctCall( int scope, Syntax::Ref& node );
    // bool analyzeDeclaration( int scope, Syntax::Ref& node );
    // bool analyzeAssign( int scope, Syntax::Ref& node );
    //bool analyzeNode( int scope, Syntax::Ref& node );
    //bool analyze( Tree* syntax_tree );
    //int newScope();
    Semantic();

    //Symbols functions;
    //Symbols symbols;
    Syntax syntax;

    std::string errmsg;
    int _scope_counter;
};

enum
{
    GLOBAL_SCOPE = 1,
    SYNTAX_ERROR,
    SYNTAX_OK,
};

enum
{
    NONE = 0,
    CONST,
    ADDRESS,
};

#endif