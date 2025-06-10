#ifndef __UTILITY_HEADER__
#define __UTILITY_HEADER__

#include <ctype.h>

class Utility
{
    public:
        Utility();
        template <typename AssertA,typename AssertB> bool Assert(AssertA ExpressionA,AssertB ExpressionB);
    private:
        
};

#endif