//CFileDialogEx.cpp

#include "CFileDialogEx.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CFileDialogEx, CFileDialog)

	ON_WM_DESTROY()
END_MESSAGE_MAP()

CFileDialogEx::CFileDialogEx(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName, DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd, DWORD dwSize, BOOL bVistaStyle)
	:CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd, dwSize, bVistaStyle){

}


BOOL CFileDialogEx::OnInitDialog() {
	CFileDialog::OnInitDialog();
	
	CWnd* fileDialog = GetParent();
	CWnd* fDComboCtrl = fileDialog->GetDlgItem(cmb1);
	CWnd* fDStaticCtrl = fileDialog->GetDlgItem(stc2);
	RECT rect;
	UINT extraSize = 25;

	fileDialog->GetWindowRect(&rect);
	fileDialog->SetWindowPos(NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top + extraSize, SWP_NOMOVE);

	fDComboCtrl->GetWindowRect(&rect);
	fileDialog->ScreenToClient(&rect); // Remember it is child controls

	// Put our control(s) somewhere below HIDDEN checkbox
	// Make space for 3 different subtypes
	rect.top += 30;
	rect.bottom += 90;
	rect.left += 150;

	this->m_combo.Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_SORT | WS_VSCROLL | WS_TABSTOP,	rect, fileDialog, IDC_COMBO_ENCODING);
	this->m_combo.SetFont(fDComboCtrl->GetFont(), TRUE);
	//this->m_combo.SetFocus();

	fDStaticCtrl->GetWindowRect(&rect);
	fileDialog->ScreenToClient(&rect);
	rect.top += 30;
	rect.bottom += 50;
	rect.left += 200;
	rect.right += 150;

	this->m_static.Create("인코딩 :", WS_CHILD | WS_VISIBLE, rect, fileDialog, IDC_STATIC_ENCODING);
	this->m_static.SetFont(fDComboCtrl->GetFont(), TRUE);

	if(this->m_bOpenFileDialog){
		fileDialog->SendDlgItemMessage(IDC_COMBO_ENCODING, CB_INSERTSTRING, (WPARAM)(-1), (LPARAM)"자동 검색");
		fileDialog->SetDlgItemText(IDC_COMBO_ENCODING, "자동 검색");
	}
	else {
		fileDialog->SetDlgItemText(IDC_COMBO_ENCODING, "UTF-8");
	}
	fileDialog->SendDlgItemMessage(IDC_COMBO_ENCODING, CB_INSERTSTRING, (WPARAM)(-1), (LPARAM)"ANSI");
	fileDialog->SendDlgItemMessage(IDC_COMBO_ENCODING, CB_INSERTSTRING, (WPARAM)(-1), (LPARAM)"UTF-16 LE");
	fileDialog->SendDlgItemMessage(IDC_COMBO_ENCODING, CB_INSERTSTRING, (WPARAM)(-1), (LPARAM)"UTF-16 BE");
	fileDialog->SendDlgItemMessage(IDC_COMBO_ENCODING, CB_INSERTSTRING, (WPARAM)(-1), (LPARAM)"UTF-8");
	fileDialog->SendDlgItemMessage(IDC_COMBO_ENCODING, CB_INSERTSTRING, (WPARAM)(-1), (LPARAM)"UTF-8(BOM)");
	fileDialog->SetDlgItemText(IDC_STATIC_ENCODING, "인코딩 :");

	return TRUE;
}

void CFileDialogEx::OnDestroy() {
	CFileDialog::OnDestroy();

	GetParent()->GetDlgItemText(IDC_COMBO_ENCODING, this->encoding, 16);
}