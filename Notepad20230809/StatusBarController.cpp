//StatusBarController.cpp
#include "StatusBarController.h"
#include "TextEditor.h"
#include "Notepad.h"
#include "Glyph.h"
#include "ZoomController.h"
#include "ScrollController.h"
#include "Document.h"

#include <afxext.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

StatusBarController::StatusBarController(TextEditor* textEditor) {
	this->textEditor = textEditor;
	dynamic_cast<Subject*>(this->textEditor)->Attach(this);
}

StatusBarController::~StatusBarController() {
	dynamic_cast<Subject*>(this->textEditor)->Detach(this);
}

void StatusBarController::Update(Subject* subject) {
	
	if (subject == this->textEditor) {
		Notepad* notepad = (Notepad*)this->textEditor->GetParent();
		if (notepad->sb != 0) {
			Long currentRow;
			Long currentColumn;

			if (this->textEditor->isScrolling == false) {
				currentRow = this->textEditor->scrollController->GetBelow() + this->textEditor->note->GetCurrent();
				currentColumn = this->textEditor->current->GetCurrent() + 1;
			}
			else {
				if(this->textEditor->isWrapped){
					currentRow = this->textEditor->wrappedRow;
					currentColumn = this->textEditor->wrappedColumn + 1;
				}
				else {
					currentRow = this->textEditor->row;
					currentColumn = this->textEditor->column + 1;
				}
			}

			CString position;
			position.Format("ÁÙ %d, ¿­ %d", currentRow, currentColumn);

			Long zoom = this->textEditor->zoomController->GetMagnification() * 100;
			if (zoom % 10 > 0) {
				zoom += 1;
			}
			
			CString magnification;
			magnification.Format("%d%%", zoom);

			notepad->sb->SetPaneText(0, position, TRUE);
			notepad->sb->SetPaneText(1, magnification, TRUE);
			notepad->sb->SetPaneText(3, this->textEditor->encoding, TRUE);
		}
	}
}