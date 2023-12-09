//Preview.h
#ifndef _PREVIEW_H
#define _PREVIEW_H

#define IDC_BUTTON_PREVIOUS 1100
#define IDC_BUTTON_NEXT 1101
#define IDC_BUTTON_PRINT 1102

#include <afxwin.h>
#include "Array.h"
typedef signed long int Long;

class TextEditor;
class CharacterMetrics;
class ZoomController;
class Document;
class Glyph;
class Paper;
class Preview : public CFrameWnd {

	public:
		Preview(TextEditor* textEditor, Long capacity = 256);
		virtual ~Preview();

		Long Add(Glyph* paper);
		void MakePreviewNotes();
		Glyph* GetAt(Long i);
		
		Long GetCapacity() const;
		Long GetLength() const;

	public:
		TextEditor* textEditor;
		ZoomController* zoomController;
		CharacterMetrics* characterMetrics;
		Document* document;
		Glyph* note;
		Glyph* current;
		LONG paperX;
		LONG paperY;

	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnPaint();
		afx_msg void OnPreviousButtonClicked();
		afx_msg void OnNextButtonClicked();
		afx_msg void OnPrintButtonClicked();

	protected:
		Array<Glyph*> notes;
		Long capacity;
		Long length;
		Long currentPage;
		RECT rectPaper;
		RECT rectMargin;
		Long x;
		Long y;
		
	private:
		CButton* previous;
		CButton* next;
		CButton* pageSetup;
		CButton* print;

	DECLARE_MESSAGE_MAP()
};

inline Long Preview::GetCapacity() const {
	return this->capacity;
}

inline Long Preview::GetLength() const {
	return this->length;
}

#endif //_PREVIEW_H