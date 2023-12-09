//UnDoReDoCommand.h
#ifndef _UNDOREDOCOMMAND_H
#define _UNDOREDOCOMMAND_H

#include "Command.h"
#include "Position.h"
#include <afxwin.h>
#include <string>
using namespace std;

typedef signed long int Long;

class Glyph;
class UnDoReDoCommand : public Command {

	public:
		UnDoReDoCommand();
		virtual ~UnDoReDoCommand() = 0;

	public:
		Command* pCommand;

	protected:
		bool isNew;
		bool isMacro;
		TCHAR character[2];
		Long row;
		Long column;
		Long afterRow;
		Long afterColumn;
		Position startPosition;
		Position endPosition;
		string contents;
		Long longestRow;
		Long hSMax;
		CString findWhat;
		CString replaceWith;
		Long replaceWithLength;
		
};

#endif //_UNDOREDOCOMMAND_H