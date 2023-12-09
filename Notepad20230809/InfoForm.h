//InfoForm.h
#ifndef _INFOFORM_H
#define _INFOFORM_H

#include <afxwin.h>
#include "resource.h"

class InfoForm : public CDialog {

	public:
		enum {IDD = IDD_INFOMATION};

	public:
		InfoForm(CWnd* parent = NULL);
		virtual BOOL OnInitDialog();

	protected:
		afx_msg void OnCheckButtonClicked();

	DECLARE_MESSAGE_MAP()
};

#endif //_INFOFORM_H