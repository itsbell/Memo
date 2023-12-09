//PreviewCommand.cpp
#include "PreviewCommand.h"
#include "Preview.h"
#include "TextEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PreviewCommand::PreviewCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

PreviewCommand::~PreviewCommand() {

}

void PreviewCommand::Execute() {
	
	this->textEditor->preview = new Preview(this->textEditor);
	//2. 프레임윈도우 생성
	this->textEditor->preview->Create(0, "미리보기");
	//3. 화면에 윈도우 출력
	this->textEditor->preview->ShowWindow(SW_SHOW);
	//4. 윈도우 갱신
	this->textEditor->preview->UpdateWindow();
	// 윈도우 초기 위치 및 크기 설정
	RECT rect;
	this->textEditor->GetParent()->GetWindowRect(&rect);
	this->textEditor->preview->SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL);
	this->textEditor->GetParent()->ShowWindow(SW_HIDE);
}

void PreviewCommand::Unexecute() {

}