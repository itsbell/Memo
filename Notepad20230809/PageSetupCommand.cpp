//PageSetupCommand.cpp
#include "PageSetupCommand.h"
#include "TextEditor.h"
#include <afxdisp.h> //MFC 자동화 클래스
#include <afxdlgs.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PageSetupCommand::PageSetupCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

PageSetupCommand::~PageSetupCommand() {

}

void PageSetupCommand::Execute() {

#if 0
	//초기 페이지설정을 하다.
	this->textEditor->psd = new PAGESETUPDLG;

	//페이지설정 초기세팅
	ZeroMemory(this->textEditor->psd, sizeof((*this->textEditor->psd)));
	this->textEditor->psd->lStructSize = sizeof(*(this->textEditor->psd));
	this->textEditor->psd->hwndOwner = this->textEditor->m_hWnd;
	this->textEditor->psd->hDevMode = NULL; // Don't forget to free or store hDevMode.
	this->textEditor->psd->hDevNames = NULL; // Don't forget to free or store hDevNames.

	this->textEditor->psd->Flags = PSD_MARGINS;
	this->textEditor->psd->rtMargin.top = 1000;
	this->textEditor->psd->rtMargin.left = 1000;
	this->textEditor->psd->rtMargin.right = 1000;
	this->textEditor->psd->rtMargin.bottom = 1000;
	this->textEditor->psd->lpfnPagePaintHook = NULL;
#endif

#if 0

	//기존 잘 되는 코드이며, API함수를 이용해 페이지셋업 다이얼로그를 띄우는 코드
	if (this->textEditor->pd != 0) {
		this->textEditor->psd->hDevMode = this->textEditor->pd->hDevMode; // Don't forget to free or store hDevMode.
		this->textEditor->psd->hDevNames = this->textEditor->pd->hDevNames; // Don't forget to free or store hDevNames.
	}
	
	PageSetupDlg(this->textEditor->psd);
#endif

	//방법 2. MFC 클래스 CPageSetupDialog를 이용하여 객체지향으로 다이얼로그를 띄우는 코드
	
	//참고: 지금 CPageSetupDialog에서 DoModal을 할 때는 MFC에서 알아서 모듈 상태를 변경 해주지 않음.

	AFX_MANAGE_STATE(AfxGetStaticModuleState()); // MFC 모듈 상태를 변경하는 코드
	CoInitialize(NULL);
	AfxEnableControlContainer();

	CPageSetupDialog psd(PSD_MARGINS, NULL);

	// Initialize psd
	psd.m_psd.hDevMode = this->textEditor->psd.hDevMode;
	psd.m_psd.hDevNames = this->textEditor->psd.hDevNames;
	psd.m_psd.ptPaperSize = this->textEditor->psd.ptPaperSize;
	psd.m_psd.rtMargin.top = this->textEditor->psd.rtMargin.top;
	psd.m_psd.rtMargin.left = this->textEditor->psd.rtMargin.left;
	psd.m_psd.rtMargin.right = this->textEditor->psd.rtMargin.right;
	psd.m_psd.rtMargin.bottom = this->textEditor->psd.rtMargin.bottom;

	if (psd.DoModal()) {
		this->textEditor->psd = psd.m_psd;
	}

	else {
		TRACE(_T("CommDlgExtendedError returned error %d from ")
			_T("CPageSetupDialog::DoModal().\n"),
			(int)CommDlgExtendedError());
	}

}

void PageSetupCommand::Unexecute() {

}