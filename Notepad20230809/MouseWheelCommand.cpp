//MouseWheelCommand.cpp
#include "MouseWheelCommand.h"
#include "TextEditor.h"
#include "CharacterMetrics.h"
#include "ScrollController.h"
#include "Glyph.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#endif
MouseWheelCommand::MouseWheelCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

MouseWheelCommand::~MouseWheelCommand() {

}

void MouseWheelCommand::Execute() {
	Long position;

	switch (this->textEditor->nFlags) {

	case 0:
		if (this->textEditor->zDelta < 0) {
			position = this->textEditor->scrollController->LineDown(this->textEditor->characterMetrics->GetHeight() * 3);
		}
		else {
			position = this->textEditor->scrollController->LineUp(this->textEditor->characterMetrics->GetHeight() * 3);
		}
		if (this->textEditor->isScrolling == false) {
			Long vaildMin;
			Long vaildMax;

			vaildMin = this->textEditor->scrollController->GetVaildMin();
			vaildMax = vaildMin + this->textEditor->note->GetLength() * this->textEditor->characterMetrics->GetHeight() - this->textEditor->rect.bottom;
			if (vaildMax < 0) {
				vaildMax = 0;
			}
			if (position < vaildMin || position > vaildMax) {
				this->textEditor->SetCurrent();
				this->textEditor->isScrolling = true;
			}
		}
		this->textEditor->Notify();
		this->textEditor->Invalidate(FALSE);
		break;

	case MK_CONTROL:
		if (this->textEditor->zDelta < 0) {
			this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_VIEW_ZOOMOUT, NULL));
		}
		else {
			this->textEditor->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_VIEW_ZOOMIN, NULL));
		}
		break;

	default: break;
	}

}

void MouseWheelCommand::Unexecute() {

}
