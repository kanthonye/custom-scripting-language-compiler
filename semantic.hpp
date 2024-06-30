#ifndef semantic_hpp
#define semantic_hpp

#include "syntax.hpp"

class Semantic{
public:

    bool analyze( Syntax* syntax );
    Semantic();
};

#endif