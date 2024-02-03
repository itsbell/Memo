//TextEditorCommandFactory.cpp

#include "TextEditorCommandFactory.h"
#include "TextEditor.h"
#include "Document.h"
#include "Glyph.h"

#include "OnCharCommand.h"
#include "OnCharEnterCommand.h"
#include "OnCharSelectingCommand.h"
#include "OnCharEnterSelectingCommand.h"

#include "OnImeCompositionCommand.h"
#include "OnImeCompositionSelectingCommand.h"
#include "OnImeCharCommand.h"

#include "BackSpaceKeyCommand.h"
#include "CharacterBackSpaceKeyCommand.h"
#include "RowBackSpaceKeyCommand.h"
#include "SelectingBackSpaceKeyCommand.h"
#include "DeleteKeyCommand.h"
#include "CharacterDeleteKeyCommand.h"
#include "RowDeleteKeyCommand.h"

#include "FindReplaceCommand.h"
#include "ReplaceFindCommand.h"
#include "ReplaceAllCommand.h"

#include "UnDoCommand.h"
#include "ReDoCommand.h"
#include "CutCommand.h"
#include "CopyCommand.h"
#include "PasteCommand.h"
#include "PasteSelectingCommand.h"
#include "DeleteCommand.h"
#include "FindCommand.h"
#include "FindNextCommand.h"
#include "FindPreviousCommand.h"
#include "ReplaceCommand.h"
#include "MoveCommand.h"
#include "SelectAllCommand.h"
#include "DateTimeCommand.h"
#include "SelectingDateTimeCommand.h"
#include "FontCommand.h"

#include "WrapCommand.h"
#include "ZoomInCommand.h"
#include "ZoomOutCommand.h"
#include "ZoomResetCommand.h"

#include "LButtonDownCommand.h"
#include "LButtonUpCommand.h"
#include "LButtonDblClkCommand.h"
#include "LButtonTripleClickedCommand.h"
#include "RButtonUpCommand.h"
#include "MouseMoveCommand.h"
#include "MouseWheelCommand.h"

#include "OnCreateMessage.h"

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TextEditorCommandFactory::TextEditorCommandFactory(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

TextEditorCommandFactory::~TextEditorCommandFactory() {

}

Command* TextEditorCommandFactory::Create(UINT nID) {
	Command* command = 0;

	if (nID == IDM_CHAR) {
		command = new OnCharCommand(this->textEditor);
	}

	else if (nID == IDM_CHARSELECTING) {
		command = new OnCharSelectingCommand(this->textEditor);
	}

	else if (nID == IDM_CHARENTER) {
		command = new OnCharEnterCommand(this->textEditor);
	}

	else if (nID == IDM_CHARENTERSELECTING) {
		command = new OnCharEnterSelectingCommand(this->textEditor);
	}

	else if (nID == IDM_IMECOMPOSITION) {
		command = new OnImeCompositionCommand(this->textEditor);
	}

	else if (nID == IDM_IMECOMPOSITIONSELECTING) {
		command = new OnImeCompositionSelectingCommand(this->textEditor);
	}

	else if (nID == IDM_IMECHAR) {
		command = new OnImeCharCommand(this->textEditor);
	}

	else if (nID == IDM_BACKSPACEKEY) {
		command = new BackSpaceKeyCommand(this->textEditor);
	}

	else if (nID == IDM_CHARACTERBACKSPACEKEY) {
		command = new CharacterBackSpaceKeyCommand(this->textEditor);
	}

	else if (nID == IDM_ROWBACKSPACEKEY) {
		command = new RowBackSpaceKeyCommand(this->textEditor);
	}

	else if (nID == IDM_SELECTINGBACKSPACEKEY) {
		command = new SelectingBackSpaceKeyCommand(this->textEditor);
	}

	else if (nID == IDM_DELETEKEY) {
		command = new DeleteKeyCommand(this->textEditor);
	}

	else if (nID == IDM_CHARACTERDELETEKEY) {
		command = new CharacterDeleteKeyCommand(this->textEditor);
	}

	else if (nID == IDM_ROWDELETEKEY) {
		command = new RowDeleteKeyCommand(this->textEditor);
	}

	else if (nID == IDM_FINDREPLACE) {
		command = new FindReplaceCommand(this->textEditor);
	}

	else if (nID == IDM_REPLACEFIND) {
		command = new ReplaceFindCommand(this->textEditor);
	}

	else if (nID == IDM_REPLACEALL) {
		command = new ReplaceAllCommand(this->textEditor);
	}

	else if (nID == IDM_EDIT_UNDO) {
		command = new UnDoCommand(this->textEditor);
	}

	else if (nID == IDM_EDIT_REDO) {
		command = new ReDoCommand(this->textEditor);
	}

	else if (nID == IDM_EDIT_CUT) {
		if (this->textEditor->document->isSelecting == true) {
			command = new CutCommand(this->textEditor);
		}
	}

	else if (nID == IDM_EDIT_COPY) {
		command = new CopyCommand(this->textEditor);
	}

	else if (nID == IDM_EDIT_PASTE) {
		if (this->textEditor->document->isSelecting == false) {
			command = new PasteCommand(this->textEditor);
		}
		else {
			command = new PasteSelectingCommand(this->textEditor);
		}
	}

	else if (nID == IDM_EDIT_DELETE) {
		command = new DeleteCommand(this->textEditor);
	}

	else if (nID == IDM_EDIT_FIND) {
		command = new FindCommand(this->textEditor);
	}

	else if (nID == IDM_EDIT_FINDNEXT) {
		command = new FindNextCommand(this->textEditor);
	}

	else if (nID == IDM_EDIT_FINDPREVIOUS) {
		command = new FindPreviousCommand(this->textEditor);
	}

	else if (nID == IDM_EDIT_REPLACE) {
		command = new ReplaceCommand(this->textEditor);
	}

	else if (nID == IDM_EDIT_MOVE) {
		command = new MoveCommand(this->textEditor);
	}

	else if (nID == IDM_EDIT_SELECTALL) {
		command = new SelectAllCommand(this->textEditor);
	}

	else if (nID == IDM_EDIT_DATETIME) {
		if (this->textEditor->document->isSelecting == false) {
			command = new DateTimeCommand(this->textEditor);
		}
		else {
			command = new SelectingDateTimeCommand(this->textEditor);
		}
	}

	else if (nID == IDM_EDIT_FONT) {
		command = new FontCommand(this->textEditor);
	}

	else if (nID == IDM_VIEW_WRAP) {
		command = new WrapCommand(this->textEditor);
	}

	else if (nID == IDM_VIEW_ZOOMIN) {
		command = new ZoomInCommand(this->textEditor);
	}

	else if (nID == IDM_VIEW_ZOOMOUT) {
		command = new ZoomOutCommand(this->textEditor);
	}

	else if (nID == IDM_VIEW_RESET) {
		command = new ZoomResetCommand(this->textEditor);
	}

	else if (nID == IDM_LBUTTONDOWN) {
		command = new LButtonDownCommand(this->textEditor);
	}

	else if (nID == IDM_LBUTTONUP) {
		command = new LButtonUpCommand(this->textEditor);
	}

	else if (nID == IDM_LBUTTONDBLCLK) {
		command = new LButtonDblClkCommand(this->textEditor);
	}

	else if (nID == IDM_LBUTTONTRIPLECLK) {
		command = new LButtonTripleClickedCommand(this->textEditor);
	}

	else if (nID == IDM_RBUTTONUP) {
		command = new RButtonUpCommand(this->textEditor);
	}

	else if (nID == IDM_MOUSEMOVE) {
		command = new MouseMoveCommand(this->textEditor);
	}

	else if (nID == IDM_MOUSEWHEEL) {
		command = new MouseWheelCommand(this->textEditor);
	}

	else if (nID == IDM_ONCREATE) {
		command = new OnCreateMessage(this->textEditor);
	}

	return command;
}