#include "tree.hpp"

Tree& Tree::operator =( const Tree& n )
{ 
    node = n.node;
    if( n.node )node->refcount++;
    return *this; 
}

Tree::Node* Tree::operator ->(){ return node; }

Tree::operator bool(){ return node != nullptr; }

void Tree::clear()
{  
    if ( node )
    {
        node->refcount--;
        if( node->refcount == 0 )
        {
            delete node;
            node = nullptr;
        }
    }
}

Tree::Tree(const Tree& n)
{
    node = n.node;
    if( n.node ) node->refcount++;
}

Tree::Tree(Node* n)
{
    node = n;
    if( node ) node->refcount++;
}

Tree::~Tree()
{
    clear();
}

Tree::Tree(): node( nullptr ) {}
