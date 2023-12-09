//PrintCommand.cpp
#include "PrintCommand.h"
#include "TextEditor.h"
#include <afxdlgs.h>
#include "Preview.h"
#include "Glyph.h"
#include "GlyphVisitor.h"
#include "PrintTextOutVisitor.h"
#include "CharacterMetrics.h"
#include "ZoomController.h"
#include "Paper.h"
#include "PrintPages.h"
#include "GlyphFactory.h"
#include "Font.h"
#include <afxdisp.h> //MFC 자동화 클래스

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PrintCommand::PrintCommand(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

PrintCommand::~PrintCommand() {

}

void PrintCommand::Execute() {

    //1번 방법 CPrintDialog : 오류는 StartDC에서 hDC에 메모리가 없기 때문으로 추측중 하지만 무시하면 계속 실행은 됨.

    Long i;
    Long index;
    LONG startX;
    LONG startY;
    PrintPages* printPages;
    GlyphFactory glyphFactory;
    GlyphVisitor* textOutVisitor = 0;

    //참고: 지금 CPrintDialog에서 DoModal을 할 때는 MFC에서 알아서 모듈 상태를 변경 해주지 않음.
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); // MFC 모듈 상태를 변경하는 코드
    CoInitialize(NULL);
    AfxEnableControlContainer();

    CPrintDialog pd(FALSE);
    pd.m_pd.hDevMode = this->textEditor->psd.hDevMode;
    pd.m_pd.hDevNames = this->textEditor->psd.hDevNames;
    if (pd.DoModal() == IDOK) {
        
        // GDI calls to render output. 
        DOCINFO docInfo;
        memset(&docInfo, 0, sizeof(DOCINFO));
        docInfo.cbSize = sizeof(DOCINFO);
        docInfo.lpszDocName = _T("Print Notepad"); //문서명칭
        docInfo.lpszOutput = NULL;
        docInfo.lpszDatatype = NULL;
        docInfo.fwType = 0;

        //DEVMODE* pDevMode = (DEVMODE*)this->textEditor->pd->hDevMode; //프린터 모드 얻기
        //pDevMode->dmOrientation = DMORIENT_PORTRAIT; // 가로&세로 모드 설정
        int iPrintWidth = ::GetDeviceCaps(pd.m_pd.hDC, HORZRES); // 가로
        int iPrintHeight = ::GetDeviceCaps(pd.m_pd.hDC, VERTRES); // 세로
        if (::StartDoc(pd.m_pd.hDC, &docInfo)) {
            //if (::StartDoc(pd->m_pd.hDC, &docInfo)) {
            TRACE(_T("StartDOC\n"));

            printPages = new PrintPages(this->textEditor);
            printPages->zoomController = new ZoomController;
            printPages->characterMetrics = new CharacterMetrics(this->textEditor->GetDC(), this->textEditor->font, printPages->zoomController);
            printPages->paperX = this->textEditor->psd.ptPaperSize.x / 100 * (72 / 25.4F); //DPI:72 기준 pixels 공식
            printPages->paperY = this->textEditor->psd.ptPaperSize.y / 100 * (72 / 25.4F);
            index = printPages->Add(glyphFactory.Create((char*)"\0"));
            printPages->note = printPages->GetAt(index);
            printPages->MakePreviewNotes();
            if (printPages->characterMetrics != 0) {
                delete printPages->characterMetrics;
            }
            printPages->characterMetrics = new CharacterMetrics(pd.m_pd.hDC, this->textEditor->font);
            startX = this->textEditor->psd.rtMargin.left / 100 * (600 / 25.4F);
            startY = this->textEditor->psd.rtMargin.top / 100 * (600 / 25.4F);

            HFONT hFont = CreateFont(this->textEditor->font->GetLogfont().lfHeight * (8.3F),			    //글자높이 (600/72)
                this->textEditor->font->GetLogfont().lfWidth,				//글자너비
                this->textEditor->font->GetLogfont().lfEscapement,	    	//출력각도
                this->textEditor->font->GetLogfont().lfOrientation,		    //기준 선에서의 각도
                this->textEditor->font->GetLogfont().lfWeight,			    //글자굵기
                this->textEditor->font->GetLogfont().lfItalic,			    //Italic 적용여부
                this->textEditor->font->GetLogfont().lfUnderline,			//밑줄적용여부
                this->textEditor->font->GetLogfont().lfStrikeOut,			//취소선적용여부
                this->textEditor->font->GetLogfont().lfCharSet,			    //문자셋종류
                this->textEditor->font->GetLogfont().lfOutPrecision,		//출력정밀도
                this->textEditor->font->GetLogfont().lfClipPrecision,		//클리핑정밀도
                this->textEditor->font->GetLogfont().lfQuality,			    //출력문자품질
                this->textEditor->font->GetLogfont().lfPitchAndFamily,	    //글꼴Pitch
                this->textEditor->font->GetLogfont().lfFaceName			    //글꼴
            );
            SelectObject(pd.m_pd.hDC, hFont);

            i = 0;
            while (i < printPages->GetLength()) {

                if (textOutVisitor == 0) {
                    textOutVisitor = new PrintTextOutVisitor(printPages, pd.m_pd.hDC, startX, startY);
                }
                if (::StartPage(pd.m_pd.hDC)) {
                    TRACE(_T("StartPage : %d Page\n"), i + 1);
                    printPages->note = printPages->GetAt(i);
                    printPages->note->Accept(textOutVisitor);
                    ::EndPage(pd.m_pd.hDC);
                }
                else {
                    TRACE(_T("error StartPage : %d Page\n", i + 1));
                }
                if (textOutVisitor != 0) {
                    delete textOutVisitor;
                    textOutVisitor = 0;
                }
                i++;
            }
            if (printPages != 0) {
                delete printPages;
            }
            //문서를 종료함
            ::EndDoc(pd.m_pd.hDC);
        }
        else {
            TRACE(_T("error StartDoc\n"));
        }
        // Delete DC when done.
        DeleteDC(pd.m_pd.hDC);
    }
    this->textEditor->psd.hDevMode = pd.m_pd.hDevMode;
    this->textEditor->psd.hDevNames = pd.m_pd.hDevNames;

}

void PrintCommand::Unexecute() {

}