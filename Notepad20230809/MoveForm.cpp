//MoveForm.cpp
#include "MoveForm.h"
#include "TextEditor.h"
#include "MemoryController.h"
#include "ScrollController.h"
#include "CharacterMetrics.h"
#include "Glyph.h"
#include "Document.h"
#include "FileMaker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(MoveForm, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_MOVE, OnMoveButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnCancelButtonClicked)
END_MESSAGE_MAP()

MoveForm::MoveForm(TextEditor* textEditor, CWnd* parent)
	:CDialog(MoveForm::IDD, parent) {
	this->textEditor = textEditor;
}

BOOL MoveForm::OnInitDialog() {
	CDialog::OnInitDialog();

	return TRUE;
}

void MoveForm::OnMoveButtonClicked() {
	CString number;
	GetDlgItem(IDC_EDIT_NUMBER)->GetWindowText(number);
	Long i = atoi(number);
	Long row = i;
	Long count = 0;
	Long index = 1;
	Long max = this->textEditor->scrollController->vSMax / this->textEditor->characterMetrics->GetHeight();
	Long below = this->textEditor->scrollController->below;
	FileMaker fm(this->textEditor);

	if (this->textEditor->isUpdated == false) {
		this->textEditor->memoryController->Save();
		this->textEditor->scrollController->UpdateFileVSInfo(true);
	}
	if(i >= 1 && i <= max){
		if (this->textEditor->document->isSelecting) {
			this->textEditor->note->UnSelect();
			this->textEditor->document->isSelecting = false;
		}
		if (this->textEditor->isWrapped) {
			if (i > below + this->textEditor->note->GetLength()) {
				this->textEditor->document->CountRows(this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount(), i,
					below + this->textEditor->note->GetLength(), this->textEditor->rect.right, this->textEditor->characterMetrics, &row, &count, &fm);
				this->textEditor->memoryController->MoveDown(row - this->textEditor->note->GetRowCount() + 1);
				this->textEditor->document->Move(row, 0);
				while (index <= count) {
					this->textEditor->note->Next();
					index++;
				}
			}
			else if (i <= below) {
				this->textEditor->document->CountRows(1, i, 0, this->textEditor->rect.right, this->textEditor->characterMetrics, &row, &count, &fm);
				this->textEditor->memoryController->MoveUp(row);
				this->textEditor->document->Move(row, 0);
				while (index <= count) {
					this->textEditor->note->Next();
					index++;
				}
			}
			else if (i > below && i < below + this->textEditor->note->GetLength() + 1) {
				this->textEditor->document->SetEnd(this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1);
				this->textEditor->note->Move(i - below);
			}
		}
		else {
			if (i < this->textEditor->document->GetStart()) {
				this->textEditor->memoryController->MoveUp(row);
			}
			else if (i > this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1) {
				this->textEditor->memoryController->MoveDown(row - this->textEditor->note->GetRowCount() + 1);
			}
			else {
				this->textEditor->document->SetEnd(this->textEditor->document->GetStart() + this->textEditor->note->GetRowCount() - 1);
			}
			this->textEditor->document->Move(row, 0);
			while (index <= count) {
				this->textEditor->note->Next();
				index++;
			}
		}
		this->textEditor->current = this->textEditor->note->GetAt(this->textEditor->note->GetCurrent() - 1);
		this->textEditor->current->First();
		this->textEditor->isMoving = false;
		this->textEditor->isScrolling = false;
		this->textEditor->x = 0;
		this->textEditor->y = this->textEditor->GetY();
		this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
		this->textEditor->Notify();
		this->textEditor->Invalidate(FALSE);
		this->EndDialog(0);
	}
	else{
		this->MessageBox("줄 번호가 전체 줄 수를 넘습니다.", "메모장 - 줄로 이동", MB_OK);
	}
}

void MoveForm::OnCancelButtonClicked() {
	this->EndDialog(0);
}


BOOL MoveForm::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN) {
			this->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_BUTTON_MOVE, NULL));
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
