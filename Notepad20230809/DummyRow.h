//DummyRow.h
#ifndef _DUMMYROW_H
#define _DUMMYROW_H

#include "Row.h"

class DummyRow : public Row {

	public:
		virtual Glyph* Clone();
};

#endif //_DUMMYROW_H