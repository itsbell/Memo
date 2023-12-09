//PrintPages.h
#ifndef _PRINTPAGES_H
#define _PRINTPAGES_H

#include "Preview.h"

class TextEditor;
class PrintPages : public Preview {

	public:
		PrintPages(TextEditor* textEditor, Long capacity = 256);
		virtual ~PrintPages();
};

#endif //_PRINTPAGES_H