#include "Utility.h"

Utility::Utility() {}

template <typename AssertA,typename AssertB>
bool Utility::Assert(AssertA ExpressionA,AssertB ExpressionB) {return (ExpressionA == ExpressionB) ? false : true;}
