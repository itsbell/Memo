//InfoForm.cpp
#include "InfoForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(InfoForm, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CHECK, OnCheckButtonClicked)
END_MESSAGE_MAP()

InfoForm::InfoForm(CWnd* parent)
	:CDialog(InfoForm::IDD, parent) {

}

BOOL InfoForm::OnInitDialog() {
	CDialog::OnInitDialog();

	return TRUE;
}

void InfoForm::OnCheckButtonClicked() {
	this->EndDialog(0);
}