
#ifndef tree_hpp
#define tree_hpp

#include "lexer.hpp"

class Tree{
public:

    class Node{
    public:
        bool empty()const{ return nodes.empty(); }
        void push( Tree n )
        {
            nodes.push_back( n );
        }

        void clear()
        {
            for( int i=0; i<nodes.size(); i++) nodes[i].clear();
            nodes.clear();
        }

        Node( int ln, Lexer::Token type, const std::string& id )
        :   id( id )
        ,   node_type( type )
        ,   flags( 0 )
        ,   refcount( 0 )
        ,   line( ln )
        {}

        Node( int ln, Lexer::Token type )
        :   node_type( type )
        ,   flags( 0 )
        ,   refcount( 0 )
        ,   line( ln )
        {}

        Node( Lexer::Token type, const std::string& id )
        :   node_type( type )
        ,   flags( 0 )
        ,   refcount( 0 )
        ,   line( 0 )
        ,   id( id )
        {}

        Node( Lexer::Token type )
        :   node_type( type )
        ,   flags( 0 )
        ,   refcount( 0 )
        ,   line( 0 )
        {}

        Node(){}

        std::vector< Tree > nodes;
        std::string  id;
        Lexer::Token node_type;


    // protected:

        //int data_type; // this property is set by the syntax analyzer
        int flags;    // this property is set by the syntax analyzer
        int line;     // this property is set by the lexical analyzer, it holds the code line number

        int refcount;
        //friend Tree;
    };

public:

    Tree& operator =( const Tree& n );
    Node* operator ->();
    operator bool();
    
    void clear();

    Tree(const Tree& n);
    Tree(Node* n);
    ~Tree();
    Tree();

    Node* node;
};

#endif // tree_hpp