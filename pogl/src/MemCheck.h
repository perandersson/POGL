#pragma once

#ifdef _DEBUG

#ifdef WIN32
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define _NEW new( _NORMAL_BLOCK, __FILE__, __LINE__)
#define new _NEW
#elif MACOSX
#error IMPLEMENT THIS
#endif

#endif
