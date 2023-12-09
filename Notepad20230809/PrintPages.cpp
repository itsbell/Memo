//PrintPages.cpp
#include "PrintPages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PrintPages::PrintPages(TextEditor* textEditor, Long capacity)
	:Preview(textEditor, capacity) {

}

PrintPages::~PrintPages() {

}