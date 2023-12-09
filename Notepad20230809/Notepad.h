//Notepad.h
#ifndef _NOTEPAD_H
#define _NOTEPAD_H

#include <afxwin.h>

#define IDC_TEXTEDITOR 3000

class TextEditor;
class CStatusBar;
class Notepad : public CFrameWnd{

	public:
		Notepad();
		virtual ~Notepad();

	public:
		TextEditor* textEditor;
		CStatusBar* sb;
		bool exit;

	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSetFocus(CWnd* pOldWnd);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
		afx_msg void OnMenuClicked(UINT nID);
		afx_msg void OnTextEditorMenuClicked(UINT nID);
		afx_msg void OnUpdateMenu(CCmdUI* pCmdUI);
		afx_msg void OnClose();

	private:
		CMenu menu;

	DECLARE_MESSAGE_MAP()
};

#endif //_NOTEPAD_H