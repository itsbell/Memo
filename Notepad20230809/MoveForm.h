//MoveForm.h
#ifndef _MOVEFORM_H
#define _MOVEFORM_H

#include <afxwin.h>
#include "resource.h"

class TextEditor;
class MoveForm : public CDialog {
	
	public:
		enum { IDD = IDD_MOVEFORM };

	public:
		MoveForm(TextEditor* textEditor, CWnd* parent = NULL);
		virtual BOOL OnInitDialog();
		virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
		afx_msg void OnMoveButtonClicked();
		afx_msg void OnCancelButtonClicked();

	private:
		TextEditor* textEditor;

	DECLARE_MESSAGE_MAP()
	
};

#endif //_MOVEFORM_H