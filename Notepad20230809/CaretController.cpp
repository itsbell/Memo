//CaretController.cpp

#include "CaretController.h"
#include "TextEditor.h"
#include "CharacterMetrics.h"
#include "ScrollController.h"
#include "Scroll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
Ŭ������: CaretController
��    ��: CaretController -�� Observer / ĳ�� ��ġ�� ������Ʈ �ϴ� Ŭ���� / ���� ����������(Observer ����������)
*/

/*
�Լ��̸� : CaretController
��    �� : CaretController������ / TextEditor�� �ּҸ� �����ϰ� TextEditor�� SuperClass�� SubjectŬ������ �߰��ϴ�.
��    �� : TextEditor Link
��    �� : ����
*/
CaretController::CaretController(TextEditor* textEditor) {
	this->textEditor = textEditor;
	dynamic_cast<Subject*>(this->textEditor)->Attach(this);
}

/*
�Լ��̸� : ~CaretController
��    �� : CaretController�Ҹ��� / ĳ���� ����� TextEditor�� SuperClass�� SubjectŬ�������� �����.
��    �� : ����
��    �� : ����
*/
CaretController::~CaretController() {
	::DestroyCaret();
	dynamic_cast<Subject*>(this->textEditor)->Detach(this);
}

/*
�Լ��̸� : Update
��    �� : ĳ���� ��ġ�� ũ�⸦ �����Ͽ� ȭ�鿡 ����ϴ�.
��    �� : ����
��    �� : ����
*/
void CaretController::Update(Subject* subject) {
	Long width = 0;

	//1. �ѱ��� �������̸�
	if (this->textEditor->isComposing == true) {

		//1.1. MultiByteCharacter�� ���� �ʺ� ���ϴ�.
		width = this->textEditor->characterMetrics->GetWidth(128); // 128��°�� ��Ƽ����Ʈ ������ ���̰� ����Ǿ� �ֽ��ϴ�.
	}
	
	//2. subject�� TextEditor�� ������
	if (subject == this->textEditor) {
		
		//2.1. ĳ���� �����.
		::CreateCaret(this->textEditor->m_hWnd, NULL, width, this->textEditor->characterMetrics->GetHeight());

		//2.2. ĳ���� ��ġ�� �����ϴ�.
		::SetCaretPos(this->textEditor->x - this->textEditor->scrollController->horizontalScroll->GetPosition(),
			this->textEditor->y - this->textEditor->scrollController->verticalScroll->GetPosition());

		//2.3. ĳ���� ȭ�鿡 ����ϴ�.
		::ShowCaret(this->textEditor->m_hWnd);
	}
}
