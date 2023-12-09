//PageSetupCommand.cpp
#include "PageSetupCommand.h"
#include "TextEditor.h"
#include <afxdisp.h> //MFC �ڵ�ȭ Ŭ����
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
	//�ʱ� ������������ �ϴ�.
	this->textEditor->psd = new PAGESETUPDLG;

	//���������� �ʱ⼼��
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

	//���� �� �Ǵ� �ڵ��̸�, API�Լ��� �̿��� �������¾� ���̾�α׸� ���� �ڵ�
	if (this->textEditor->pd != 0) {
		this->textEditor->psd->hDevMode = this->textEditor->pd->hDevMode; // Don't forget to free or store hDevMode.
		this->textEditor->psd->hDevNames = this->textEditor->pd->hDevNames; // Don't forget to free or store hDevNames.
	}
	
	PageSetupDlg(this->textEditor->psd);
#endif

	//��� 2. MFC Ŭ���� CPageSetupDialog�� �̿��Ͽ� ��ü�������� ���̾�α׸� ���� �ڵ�
	
	//����: ���� CPageSetupDialog���� DoModal�� �� ���� MFC���� �˾Ƽ� ��� ���¸� ���� ������ ����.

	AFX_MANAGE_STATE(AfxGetStaticModuleState()); // MFC ��� ���¸� �����ϴ� �ڵ�
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