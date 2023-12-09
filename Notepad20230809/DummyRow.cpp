//DummyRow.cpp
#include "DummyRow.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <new>
#include <memory>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

Glyph* DummyRow::Clone() {
	return new DummyRow(*this);

}