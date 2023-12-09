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
	//2. ������������ ����
	this->textEditor->preview->Create(0, "�̸�����");
	//3. ȭ�鿡 ������ ���
	this->textEditor->preview->ShowWindow(SW_SHOW);
	//4. ������ ����
	this->textEditor->preview->UpdateWindow();
	// ������ �ʱ� ��ġ �� ũ�� ����
	RECT rect;
	this->textEditor->GetParent()->GetWindowRect(&rect);
	this->textEditor->preview->SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL);
	this->textEditor->GetParent()->ShowWindow(SW_HIDE);
}

void PreviewCommand::Unexecute() {

}