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
클래스명: CaretController
기    능: CaretController -▷ Observer / 캐럿 위치를 업데이트 하는 클래스 / 관련 디자인패턴(Observer 디자인패턴)
*/

/*
함수이름 : CaretController
기    능 : CaretController생성자 / TextEditor의 주소를 저장하고 TextEditor의 SuperClass인 Subject클래스에 추가하다.
입    력 : TextEditor Link
출    력 : 없음
*/
CaretController::CaretController(TextEditor* textEditor) {
	this->textEditor = textEditor;
	dynamic_cast<Subject*>(this->textEditor)->Attach(this);
}

/*
함수이름 : ~CaretController
기    능 : CaretController소멸자 / 캐럿을 지우고 TextEditor의 SuperClass인 Subject클래스에서 지우다.
입    력 : 없음
출    력 : 없음
*/
CaretController::~CaretController() {
	::DestroyCaret();
	dynamic_cast<Subject*>(this->textEditor)->Detach(this);
}

/*
함수이름 : Update
기    능 : 캐럿의 위치와 크기를 변경하여 화면에 출력하다.
입    력 : 없음
출    력 : 없음
*/
void CaretController::Update(Subject* subject) {
	Long width = 0;

	//1. 한글을 조립중이면
	if (this->textEditor->isComposing == true) {

		//1.1. MultiByteCharacter의 글자 너비를 구하다.
		width = this->textEditor->characterMetrics->GetWidth(128); // 128번째에 멀티바이트 문자의 넓이가 저장되어 있습니다.
	}
	
	//2. subject가 TextEditor랑 같으면
	if (subject == this->textEditor) {
		
		//2.1. 캐럿을 만들다.
		::CreateCaret(this->textEditor->m_hWnd, NULL, width, this->textEditor->characterMetrics->GetHeight());

		//2.2. 캐럿의 위치를 지정하다.
		::SetCaretPos(this->textEditor->x - this->textEditor->scrollController->horizontalScroll->GetPosition(),
			this->textEditor->y - this->textEditor->scrollController->verticalScroll->GetPosition());

		//2.3. 캐럿을 화면에 출력하다.
		::ShowCaret(this->textEditor->m_hWnd);
	}
}
