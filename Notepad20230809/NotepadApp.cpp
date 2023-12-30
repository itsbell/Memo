//NotepadApp.cpp
#include "NotepadApp.h"
#include "Notepad.h"
#include "Registry.h"

#if 0
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL NotepadApp::InitInstance() {
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	CWinApp::InitInstance();

	//1. CFrameWnd ��ü �Ҵ�
	Notepad* pFrame = new Notepad;
	
	//������Ʈ�� Ű ����
	this->SetRegistryKey("Memo");

	//2. ������������ ����	
	pFrame->Create(0, "������� - �޸���");
	//3. ȭ�鿡 ������ ���
	pFrame->ShowWindow(SW_SHOW);
	//4. ������ ����
	pFrame->UpdateWindow();
	m_pMainWnd = pFrame;
	// ������ �ʱ� ��ġ �� ũ�� ����
	Registry registry;
	RECT rect;
	if (registry.GetClientPosition(&rect) == true) {
		m_pMainWnd->SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL);
	}
	else {
		m_pMainWnd->SetWindowPos(NULL, 600, 200, 850, 650, NULL);
	}
	
	return TRUE;
}

NotepadApp notepadApp;