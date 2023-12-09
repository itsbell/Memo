//TextEditorCommandFactory.h
#ifndef _TEXTEDITORCOMMANDFACTORY_H
#define _TEXTEDITORCOMMANDFACTORY_H

typedef unsigned int UINT;

class TextEditor;
class Command;
class TextEditorCommandFactory {
	
	public:
		TextEditorCommandFactory(TextEditor* textEditor);
		~TextEditorCommandFactory();

		Command* Create(UINT nID);

	private:
		TextEditor* textEditor;
};

#endif //_TEXTEDITORCOMMANDFACTORY_H