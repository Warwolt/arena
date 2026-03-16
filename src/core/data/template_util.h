#pragma once

#define JOIN2(a, b) a##b
#define JOIN(a, b) JOIN2(a, b)
#define TO_STRING2(macro) #macro
#define TO_STRING(macro) TO_STRING2(macro)
