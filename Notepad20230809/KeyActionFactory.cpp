//KeyActionFactory.cpp
#include "KeyActionFactory.h"
#include "TextEditor.h"
#include "Glyph.h"
#include "LeftKeyAction.h"
#include "RightKeyAction.h"
#include "HomeKeyAction.h"
#include "EndKeyAction.h"
#include "DownKeyAction.h"
#include "UpKeyAction.h"
#include "CtrlUpKeyAction.h"
#include "CtrlDownKeyAction.h"
#include "CtrlHomeKeyAction.h"
#include "CtrlEndKeyAction.h"
#include "CtrlLeftKeyAction.h"
#include "CtrlRightKeyAction.h"
#include "BackSpaceKeyAction.h"
#include "DeleteKeyAction.h"
#include "PageUpKeyAction.h"
#include "PageDownKeyAction.h"
#include "ShiftLeftKeyAction.h"
#include "ShiftRightKeyAction.h"
#include "ShiftDownKeyAction.h"
#include "ShiftUpKeyAction.h"
#include "ShiftHomeKeyAction.h"
#include "ShiftEndKeyAction.h"
#include "ShiftPageUpKeyAction.h"
#include "ShiftPageDownKeyAction.h"
#include "CtrlShiftLeftKeyAction.h"
#include "CtrlShiftRightKeyAction.h"
#include "CtrlShiftUpKeyAction.h"
#include "CtrlShiftDownKeyAction.h"
#include "CtrlShiftHomeKeyAction.h"
#include "CtrlShiftEndKeyAction.h"
#include "SelectingLeftKeyAction.h"
#include "SelectingRightKeyAction.h"
#include "SelectingBackSpaceKeyAction.h"
#include "SelectingDeleteKeyAction.h"
#include "CtrlOKeyAction.h"
#include "CtrlSKeyAction.h"
#include "CtrlShiftSKeyAction.h"
#include "CtrlZKeyAction.h"
#include "CtrlYKeyAction.h"
#include "CtrlCKeyAction.h"
#include "CtrlVKeyAction.h"
#include "CtrlXKeyAction.h"
#include "CtrlFKeyAction.h"
#include "CtrlHKeyAction.h"
#include "CtrlAKeyAction.h"
#include "F3KeyAction.h"
#include "ShiftF3KeyAction.h"
#include "CtrlPlusKeyAction.h"
#include "CtrlMinusKeyAction.h"
#include "Ctrl0KeyAction.h"
#include "CtrlPKeyAction.h"
#include "F11KeyAction.h"
#include "CtrlGKeyAction.h"
#include "CtrlNKeyAction.h"
#include "Stack.h"
#include "Document.h"
#include "CtrlDKeyAction.h"
#include "F5KeyAction.h"
#include "CtrlWKeyAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KeyActionFactory::KeyActionFactory() {

}

KeyActionFactory::KeyActionFactory(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

KeyActionFactory::~KeyActionFactory() {

}

KeyAction* KeyActionFactory::Create(UINT nChar) {
	bool isSelecting = this->textEditor->document->isSelecting;
	KeyAction* keyAction = 0;
	Glyph* firstRow = this->textEditor->note->GetAt(0);

	//1. ¹æÇâÅ° '¡ç'Å°¸¦ ´­·¶À» ¶§
	if (nChar == VK_LEFT) {

		if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState(VK_SHIFT) & 0x8000) {
			keyAction = new CtrlShiftLeftKeyAction(this->textEditor);
		}

		else if (GetKeyState(VK_CONTROL) & 0x8000) {
			keyAction = new CtrlLeftKeyAction(this->textEditor);
		}

		else if (GetKeyState(VK_SHIFT) & 0x8000) {
			keyAction = new ShiftLeftKeyAction(this->textEditor);
		}

		else if (isSelecting == true) {
			keyAction = new SelectingLeftKeyAction(this->textEditor);
		}

		else {
			keyAction = new LeftKeyAction(this->textEditor);
		}
	}

	//2. ¹æÇâÅ° '¡æ'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == VK_RIGHT) {

		if (GetKeyState(VK_CONTROL) & 0x8000 && (GetKeyState(VK_SHIFT) & 0x8000)) {
			keyAction = new CtrlShiftRightKeyAction(this->textEditor);
		}
		else if (GetKeyState(VK_CONTROL) & 0x8000) {
			keyAction = new CtrlRightKeyAction(this->textEditor);
		}
		else if (GetKeyState(VK_SHIFT) & 0x8000) {
			keyAction = new ShiftRightKeyAction(this->textEditor);
		}
		else if (isSelecting == true) {
			keyAction = new SelectingRightKeyAction(this->textEditor);
		}
		else {
			keyAction = new RightKeyAction(this->textEditor);
		}
	}

	//3. ¹æÇâÅ° '¡é'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == VK_DOWN) {


		if ((GetKeyState(VK_CONTROL) & 0x8000) && (GetKeyState(VK_SHIFT) & 0x8000)) {
			keyAction = new CtrlShiftDownKeyAction(this->textEditor);
		}

		else if (GetKeyState(VK_CONTROL) & 0x8000) {
			keyAction = new CtrlDownKeyAction(this->textEditor);
		}

		else if (GetKeyState(VK_SHIFT) & 0x8000) {
			keyAction = new ShiftDownKeyAction(this->textEditor);
		}

		else {
			keyAction = new DownKeyAction(this->textEditor);
		}
	}

	//4. ¹æÇâÅ° '¡è'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == VK_UP) {

		if ((GetKeyState(VK_CONTROL) & 0x8000) && (GetKeyState(VK_SHIFT) & 0x8000)) {
			keyAction = new CtrlShiftUpKeyAction(this->textEditor);
		}

		else if (GetKeyState(VK_CONTROL) & 0x8000) {
			keyAction = new CtrlUpKeyAction(this->textEditor);
		}

		else if (GetKeyState(VK_SHIFT) & 0x8000) {
			keyAction = new ShiftUpKeyAction(this->textEditor);
		}

		else {
			keyAction = new UpKeyAction(this->textEditor);
		}
	}

	//5. 'HOME' Å°¸¦ ´­·¶À» ¶§
	else if (nChar == VK_HOME) {

		if ((GetKeyState(VK_CONTROL) & 0x8000) && (GetKeyState(VK_SHIFT) & 0x8000)) {
			keyAction = new CtrlShiftHomeKeyAction(this->textEditor);
		}

		else if (GetKeyState(VK_CONTROL) & 0x8000) {
			keyAction = new CtrlHomeKeyAction(this->textEditor);
		}

		else if (GetKeyState(VK_SHIFT) & 0x8000) {
			keyAction = new ShiftHomeKeyAction(this->textEditor);
		}

		else {
			keyAction = new HomeKeyAction(this->textEditor);
		}

	}

	//6. 'END' Å°¸¦ ´­·¶À» ¶§
	else if (nChar == VK_END) {

		if ((GetKeyState(VK_CONTROL) & 0x8000) && (GetKeyState(VK_SHIFT) & 0x8000)) {
			keyAction = new CtrlShiftEndKeyAction(this->textEditor);
		}

		else if (GetKeyState(VK_CONTROL) & 0x8000) {
			keyAction = new CtrlEndKeyAction(this->textEditor);
		}

		else if (GetKeyState(VK_SHIFT) & 0x8000) {
			keyAction = new ShiftEndKeyAction(this->textEditor);
		}

		else {
			keyAction = new EndKeyAction(this->textEditor);
		}
	}

	//7. 'PAGEUP'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == VK_PRIOR) {

		if (GetKeyState(VK_SHIFT) & 0x8000) {
			keyAction = new ShiftPageUpKeyAction(this->textEditor);
		}

		else {
			keyAction = new PageUpKeyAction(this->textEditor);
		}
	}

	//8. 'PAGEDOWN'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == VK_NEXT) {

		if (GetKeyState(VK_SHIFT) & 0x8000) {
			keyAction = new ShiftPageDownKeyAction(this->textEditor);
		}

		else {
			keyAction = new PageDownKeyAction(this->textEditor);
		}
	}

	//9. 'BACKSPACE'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == VK_BACK) {
		if (isSelecting == true) {
			keyAction = new SelectingBackSpaceKeyAction(this->textEditor);
		}
		else {
			keyAction = new BackSpaceKeyAction(this->textEditor);
		}
	}

	//10. 'DELETE'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == VK_DELETE) {
		if (isSelecting == true) {
			keyAction = new SelectingDeleteKeyAction(this->textEditor);
		}
		else {
			keyAction = new DeleteKeyAction(this->textEditor);
		}
	}

	//11. 'CTRL + O'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == 0x4F && GetKeyState(VK_CONTROL) & 0x8000) {
		keyAction = new CtrlOKeyAction(this->textEditor);
	}

	//12. 'CTRL + S'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == 0x53 && GetKeyState(VK_CONTROL) & 0x8000) {
		if (GetKeyState(VK_SHIFT) & 0x8000) {
			keyAction = new CtrlShiftSKeyAction(this->textEditor);
		}

		else {
			keyAction = new CtrlSKeyAction(this->textEditor);
		}
	}

	//13. 'CTRL + Z'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == 0x5A && GetKeyState(VK_CONTROL) & 0x8000) {
		if (this->textEditor->unDoCommands != 0 && this->textEditor->unDoCommands->GetLength() > 0){
			keyAction = new CtrlZKeyAction(this->textEditor);
		}
	}

	//14. 'CTRL + Y'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == 0x59 && GetKeyState(VK_CONTROL) & 0x8000) {
		if (this->textEditor->reDoCommands != 0 && this->textEditor->reDoCommands->GetLength() > 0) {
			keyAction = new CtrlYKeyAction(this->textEditor);
		}	
	}

	//14. 'CTRL + C'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == 0x43 && GetKeyState(VK_CONTROL) & 0x8000) {
		if (isSelecting == true) {
			keyAction = new CtrlCKeyAction(this->textEditor);
		}
	}

	//15. 'CTRL + V'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == 0x56 && GetKeyState(VK_CONTROL) & 0x8000) {
		unsigned int priority_list = CF_TEXT;
		if (::GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT) {
			keyAction = new CtrlVKeyAction(this->textEditor);
		}
	}

	//13. 'CTRL + X'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == 0x58 && GetKeyState(VK_CONTROL) & 0x8000) {
		if (isSelecting == true) {
			keyAction = new CtrlXKeyAction(this->textEditor);
		}
	}

	//14. 'CTRL + F'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == 0x46 && GetKeyState(VK_CONTROL) & 0x8000) {
		if (this->textEditor->note->GetLength() >= 2 || firstRow->GetLength() >= 1) {
			keyAction = new CtrlFKeyAction(this->textEditor);
		}
	}

	//15. 'CTRL + H'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == 0x48 && GetKeyState(VK_CONTROL) & 0x8000) {
		if (this->textEditor->note->GetLength() >= 2 || firstRow->GetLength() >= 1) {
			keyAction = new CtrlHKeyAction(this->textEditor);
		}
	}

	//16. 'CTRL + A'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == 0x41 && GetKeyState(VK_CONTROL) & 0x8000) {
		keyAction = new CtrlAKeyAction(this->textEditor);
	}

	//17. 'F3'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == VK_F3) {
		if (this->textEditor->note->GetLength() >= 2 || firstRow->GetLength() >= 1) {
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				keyAction = new ShiftF3KeyAction(this->textEditor);
			}

			else {
				keyAction = new F3KeyAction(this->textEditor);
			}
		}
	}

	//18. 'Ctrl + +'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == VK_OEM_PLUS && GetKeyState(VK_CONTROL) & 0x8000) {
		keyAction = new CtrlPlusKeyAction(this->textEditor);
	}

	//19. 'Ctrl + -'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == VK_OEM_MINUS && GetKeyState(VK_CONTROL) & 0x8000) {
		keyAction = new CtrlMinusKeyAction(this->textEditor);
	}
	
	//20. 'Ctrl + 0'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == 0x30 && GetKeyState(VK_CONTROL) & 0x8000) {
		keyAction = new Ctrl0KeyAction(this->textEditor);
	}

	//21. 'Ctrl + P'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == 0x50 && GetKeyState(VK_CONTROL) & 0x8000) {
		keyAction = new CtrlPKeyAction(this->textEditor);
	}

	//22. 'F11'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == VK_F11) {
		keyAction = new F11KeyAction(this->textEditor);
	}

	//23. 'Ctrl + G'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == 0x47 && GetKeyState(VK_CONTROL) & 0x8000) {
		keyAction = new CtrlGKeyAction(this->textEditor);
	}

	//24. 'Ctrl + N'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == 0x4E && GetKeyState(VK_CONTROL) & 0x8000) {
		keyAction = new CtrlNKeyAction(this->textEditor);
	}

	//25. 'F12'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == 0x44 && GetKeyState(VK_CONTROL) & 0x8000) {
		keyAction = new CtrlDKeyAction(this->textEditor);
	}

	//26. 'F5' Å°¸¦ ´­·¶À» ¶§
	else if (nChar == VK_F5) {
		keyAction = new F5KeyAction(this->textEditor);
	}

	//27. 'Ctrl + W'Å°¸¦ ´­·¶À» ¶§
	else if (nChar == 0x57 && GetKeyState(VK_CONTROL) & 0x8000) {
		keyAction = new CtrlWKeyAction(this->textEditor);
	}
	
	return keyAction;
}