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
#include <afxdisp.h> //MFC �ڵ�ȭ Ŭ����

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

    //1�� ��� CPrintDialog : ������ StartDC���� hDC�� �޸𸮰� ���� �������� ������ ������ �����ϸ� ��� ������ ��.

    Long i;
    Long index;
    LONG startX;
    LONG startY;
    PrintPages* printPages;
    GlyphFactory glyphFactory;
    GlyphVisitor* textOutVisitor = 0;

    //����: ���� CPrintDialog���� DoModal�� �� ���� MFC���� �˾Ƽ� ��� ���¸� ���� ������ ����.
    AFX_MANAGE_STATE(AfxGetStaticModuleState()); // MFC ��� ���¸� �����ϴ� �ڵ�
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
        docInfo.lpszDocName = _T("Print Notepad"); //������Ī
        docInfo.lpszOutput = NULL;
        docInfo.lpszDatatype = NULL;
        docInfo.fwType = 0;

        //DEVMODE* pDevMode = (DEVMODE*)this->textEditor->pd->hDevMode; //������ ��� ���
        //pDevMode->dmOrientation = DMORIENT_PORTRAIT; // ����&���� ��� ����
        int iPrintWidth = ::GetDeviceCaps(pd.m_pd.hDC, HORZRES); // ����
        int iPrintHeight = ::GetDeviceCaps(pd.m_pd.hDC, VERTRES); // ����
        if (::StartDoc(pd.m_pd.hDC, &docInfo)) {
            //if (::StartDoc(pd->m_pd.hDC, &docInfo)) {
            TRACE(_T("StartDOC\n"));

            printPages = new PrintPages(this->textEditor);
            printPages->zoomController = new ZoomController;
            printPages->characterMetrics = new CharacterMetrics(this->textEditor->GetDC(), this->textEditor->font, printPages->zoomController);
            printPages->paperX = this->textEditor->psd.ptPaperSize.x / 100 * (72 / 25.4F); //DPI:72 ���� pixels ����
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

            HFONT hFont = CreateFont(this->textEditor->font->GetLogfont().lfHeight * (8.3F),			    //���ڳ��� (600/72)
                this->textEditor->font->GetLogfont().lfWidth,				//���ڳʺ�
                this->textEditor->font->GetLogfont().lfEscapement,	    	//��°���
                this->textEditor->font->GetLogfont().lfOrientation,		    //���� �������� ����
                this->textEditor->font->GetLogfont().lfWeight,			    //���ڱ���
                this->textEditor->font->GetLogfont().lfItalic,			    //Italic ���뿩��
                this->textEditor->font->GetLogfont().lfUnderline,			//�������뿩��
                this->textEditor->font->GetLogfont().lfStrikeOut,			//��Ҽ����뿩��
                this->textEditor->font->GetLogfont().lfCharSet,			    //���ڼ�����
                this->textEditor->font->GetLogfont().lfOutPrecision,		//������е�
                this->textEditor->font->GetLogfont().lfClipPrecision,		//Ŭ�������е�
                this->textEditor->font->GetLogfont().lfQuality,			    //��¹���ǰ��
                this->textEditor->font->GetLogfont().lfPitchAndFamily,	    //�۲�Pitch
                this->textEditor->font->GetLogfont().lfFaceName			    //�۲�
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
            //������ ������
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