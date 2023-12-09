//OnImeCompositionCommand.cpp
#include "OnImeCompositionCommand.h"
#include "TextEditor.h"
#include "Stack.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "ScrollController.h"
#include "OnImeCharCommand.h"
#include "Document.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

OnImeCompositionCommand::OnImeCompositionCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
	this->isNew = true;
	this->isMacro = false;
}

OnImeCompositionCommand::OnImeCompositionCommand(TextEditor* textEditor, bool isNew, bool macro) {
	this->textEditor = textEditor;
	this->isNew = isNew;
	this->isMacro = macro;
}

OnImeCompositionCommand::~OnImeCompositionCommand() {

}

void OnImeCompositionCommand::Execute() {
	Glyph* glyph;
	GlyphFactory glyphFactory;

	if (this->isNew) {
		if (this->textEditor->isScrolling == true) {
			this->textEditor->GetBack();
		}
		if (this->textEditor->isWrapped == true) {
			this->textEditor->note->RowUnWrap(this->textEditor->characterMetrics);
		}
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		if (this->textEditor->isComposing == true) {
			this->textEditor->current->Remove(this->textEditor->current->GetCurrent() - 1, this->textEditor->characterMetrics);
		}
		if (strcmp(this->textEditor->character, "") != 0) {
			glyph = glyphFactory.Create(this->textEditor->character);
			if (this->textEditor->current->GetCurrent() == this->textEditor->current->GetLength()) {
				this->textEditor->current->Add(glyph, this->textEditor->characterMetrics);
			}
			else {
				this->textEditor->current->Add(this->textEditor->current->GetCurrent(), glyph, this->textEditor->characterMetrics);
			}
			this->textEditor->isComposing = true;
		}
		else {
			this->textEditor->isComposing = false;
		}
		if (this->textEditor->isWrapped == true) {
			this->textEditor->note->RowWrap(this->textEditor->rect.right, this->textEditor->characterMetrics);
			this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		}
		if (this->textEditor->isModified == false) {
			this->textEditor->isModified = true;
			this->textEditor->GetParent()->SetWindowText(("*" + this->textEditor->fileName + " - ¸Þ¸ðÀå").c_str());
		}
		if (this->textEditor->current->GetCurrent() > 0 && strcmp(this->textEditor->character, "") != 0) {
			this->textEditor->x = this->textEditor->GetX(this->textEditor->note->GetCurrent(), this->textEditor->current->GetCurrent() - 1);
		}
		this->textEditor->y = this->textEditor->GetY();
		this->textEditor->isUpdated = false;

		if (this->textEditor->isWrapped == true) {
			this->textEditor->scrollFlags = SF_WRAP;
		}
		else {
			this->textEditor->scrollFlags = SF_COMPARE;
		}
		this->textEditor->scrollController->UpdateMaximum();
		this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
		this->textEditor->Notify();
		this->textEditor->Invalidate(FALSE);
	}
	
 	if (this->isMacro == true) {
		this->textEditor->unDoCommands->Push(new OnImeCompositionCommand(this->textEditor, false, true));
	}
}

void OnImeCompositionCommand::Unexecute() {

	if (this->isMacro == true) {
		this->textEditor->reDoCommands->Push(new OnImeCompositionCommand(this->textEditor, false, true));
	}
}