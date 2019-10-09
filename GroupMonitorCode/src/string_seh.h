#pragma once
#include <string>

#undef _TRY_BEGIN
#undef _CATCH_ALL
#undef _CATCH_END

#define _TRY_BEGIN __try{
#define _CATCH_ALL }__except(1){
//#define _RERAISE RaiseException()
#define _CATCH_END }