//CFileDialogEx.h

#ifndef _CFILEDIALOG_H
#define _CFILEDIALOG_H

#include <afxdlgs.h>
#include <dlgs.h> //cmb1

class CFileDialogEx : public CFileDialog {
//	DECLARE_DYNAMIC(CFileDialogEx)

	public:
		CStatic m_static;
		CComboBox m_combo;
		char encoding[16];

	public:
		CFileDialogEx(BOOL bOpenFileDialog, LPCTSTR lpszDefExt = NULL, LPCTSTR lpszFileName = NULL, DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, LPCTSTR lpszFilter = NULL, CWnd* pParentWnd = NULL, DWORD dwSize = 0, BOOL bVistaStyle = FALSE);

	protected:
		virtual BOOL OnInitDialog();
		afx_msg void OnDestroy();

		DECLARE_MESSAGE_MAP()
};

#endif //_CFILEDIALOG_H