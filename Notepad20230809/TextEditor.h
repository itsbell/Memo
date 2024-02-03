//TextEditor.h
#ifndef _TEXTEDITOR_H
#define _TEXTEDITOR_H

#include <afxwin.h>
#include <string>
using namespace std;
#include "Subject.h"

typedef signed long int Long;

class Glyph;
class CharacterMetrics;
class Font;
class CaretController;
class ScrollController;
class MemoryController;
class ZoomController;
class StatusBarController;
class Stack;
class Document;
class Position;
class CFindReplaceDialog;
class Time;

class TextEditor : public CWnd , public Subject{

	public: 
		Document* document;
		Glyph* note;
		Glyph* current;
		Font* font;
		CharacterMetrics* characterMetrics;
		CaretController* caretController;
		ScrollController* scrollController;
		MemoryController* memoryController;
		StatusBarController* statusBarController;
		ZoomController* zoomController;
		Stack* unDoCommands;
		Stack* reDoCommands;
		CFindReplaceDialog* findDialog;
		CFindReplaceDialog* findReplaceDialog;
		Time* doubleClickTime;
		CRect rect;

		bool isMoving;
		bool isWrapped;
		bool isModified;
		bool isComposing;
		bool isClicking;
		bool isUpdated;
		bool isScrolling;

		Long row;
		Long column;
		Long wrappedRow;
		Long wrappedColumn;
		int scrollFlags;
		Long longestRow;
		
		string fileName;
		string pathName;
		Long x;
		Long y;
		Long destination;

		TCHAR time[10];
		char encoding[16];
		TCHAR character[2];
		CString findWhat;
		CString replaceWith;
		BOOL matchCase;
		BOOL matchWholeWord;
		WPARAM wParam; //OnFindReplace
		LPARAM lParam; //OnFindReplace
		UINT nFlags; // Mouse
		CPoint point; // Mouse
		short zDelta; //OnMouseWheel

		PAGESETUPDLG psd;
		PRINTDLG pd;
		CFrameWnd* preview;

	public:
		TextEditor();
		virtual ~TextEditor();
		
		Long GetX(Long row, Long column);
		Long GetY(Long row);
		Long GetY();
		void SetCurrent();
		void GetBack();
		
	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnPaint();
		afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		afx_msg LRESULT OnImeStartComposition(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnImeComposition(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnImeChar(WPARAM wParam, LPARAM lParam);
		afx_msg void OnSetFocus(CWnd* pOldWnd);
		afx_msg void OnKillFocus(CWnd* pNewWnd);
		afx_msg void OnMenuClicked(UINT nID);
		afx_msg void OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags);
		afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg LRESULT OnFindReplace(WPARAM wParam, LPARAM lParam);
		afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
		afx_msg void OnLButtonDown(UINT nFlags,	CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
		afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		//afx_msg void OnMouseLeave();
		afx_msg void OnDestroy();
		

	private:
		HCURSOR cursor;

	DECLARE_MESSAGE_MAP()
};

#endif //_TEXTEDITOR_H