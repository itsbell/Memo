//OnCreateMessage.cpp
#include "OnCreateMessage.h"

#include "ANSIConverter.h"
#include "CharacterMetrics.h"
#include "Document.h"
#include "FileMaker.h"
#include "Font.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "MemoryController.h"
#include "Registry.h"
#include "resource.h"
#include "ScrollController.h"
#include "StatusBarController.h"
#include "TextConverter.h"
#include "TextEditor.h"
#include "TextConverterFactory.h"
#include "Time.h"
#include "UTF16BEConverter.h"
#include "UTF16LEConverter.h"
#include "UTF8Converter.h"
#include "UTF8BOMConverter.h"
#include "ZoomController.h"
#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

OnCreateMessage::OnCreateMessage(TextEditor* textEditor) {
	this->textEditor = textEditor;
}

OnCreateMessage::~OnCreateMessage() {

}

void OnCreateMessage::Execute() {
	bool ret;
	Long i;
	LONG style;
	LOGFONT lf;
	Glyph* glyph;
	GlyphFactory glyphFactory;
	Registry* registry;
	
	Time time = time.GetCurrent();
	sprintf(this->textEditor->time, "%02d%02d%02d%03d", time.GetHour(), time.GetMin(), time.GetSec(), time.GetMillisec());
	this->textEditor->doubleClickTime = new Time(time); // doubleClickTime 초기화

	FileMaker fileMaker(this->textEditor);

	//레지스트리에서 기존 설정을 읽다.
	registry = new Registry;
	ret = registry->GetFont(&lf);
	if (ret == true) {
		this->textEditor->font = new Font(lf);
	}
	else {
		this->textEditor->font = new Font;
	}
	this->textEditor->isWrapped = registry->GetOnWordWrap();

	// 기존 윈도우스타일에 수평,수직 스크롤 스타일을 추가하다.
 	style = ::GetWindowLong(this->textEditor->m_hWnd, GWL_STYLE);
	style |= WS_VSCROLL | WS_HSCROLL;
	::SetWindowLong(this->textEditor->m_hWnd, GWL_STYLE, style);

	// 컨트롤러들을 만들다.
	this->textEditor->memoryController = new MemoryController(this->textEditor);
	this->textEditor->scrollController = new ScrollController(this->textEditor);
	this->textEditor->statusBarController = new StatusBarController(this->textEditor);
	this->textEditor->zoomController = new ZoomController;

	// 현재 DC와 폰트에 맞는 CharacterMetrics를 생성하다.
	this->textEditor->characterMetrics = new CharacterMetrics(this->textEditor->GetDC(), this->textEditor->font, this->textEditor->zoomController);

	// 새 문서를 만들다.
	this->textEditor->document = new Document;
	glyph = glyphFactory.Create((char*)"\0");
	this->textEditor->note = glyph;
	this->textEditor->document->paper = glyph;
	glyph = glyphFactory.Create((char*)"\r\n");
	i = this->textEditor->note->Add(glyph);
	this->textEditor->current = this->textEditor->note->GetAt(i);

	//메모리 컨트롤을 위한 파일을 만들다.
	fileMaker.New();

	this->textEditor->longestRow = 1;
	this->textEditor->isUpdated = true;
	this->textEditor->document->SetStart(1);
	this->textEditor->document->SetEnd(1);
	this->textEditor->document->SetLength(1);

	strcpy(this->textEditor->encoding, "UTF-8");

	//txt파일 열기 시 불러오기가 되는 임시코드
	LPSTR commandLine;
	commandLine = ::GetCommandLine();
	bool open = false;
	Long start;
	Long end;
	CString pathName;
	CString fileName;
	i = 0;

	while (commandLine[i] != '\0' && open == false) {
		if (commandLine[i] == '.') {
			if (commandLine[i + 1] == 't' && commandLine[i + 2] == 'x' && commandLine[i + 3] == 't') {
				end = i + 3;
				i--;
				while (commandLine[i] != '\\') {
					i--;
				}
				i++;
				while (commandLine[i] != '.') {
					fileName += commandLine[i];
					i++;
				}
				while (commandLine[i] != '\"') {
					i--;
				}
				start = i + 1;
				open = true;
			}
		}
		i++;
	}
	if (open == true) {
		i = start;
		while (i <= end) {
			pathName += commandLine[i];
			i++;
		}
		if (this->textEditor->note != 0) {
			delete this->textEditor->note;
		}
		this->textEditor->note = glyphFactory.Create((char*)"\0");
		this->textEditor->document->paper = this->textEditor->note;
		memset(this->textEditor->encoding, 0, 16);
		strcpy(this->textEditor->encoding, "자동 검색");

		CFile file;
		if (file.Open(pathName, CFile::modeRead)) {
			Long fileLength = (Long)file.GetLength();
			TCHAR* pData = new TCHAR[fileLength + 2];
			memset(pData, 0, fileLength + 2);
			LONG readSize = file.Read(pData, fileLength);
			if (readSize == fileLength) {
				TextConverterFactory textConverterFactory(this->textEditor->document);
				TextConverter* textConverter;
				textConverter = textConverterFactory.Create(pData, fileLength);
				char* buffer = textConverter->Decode();
				fileMaker.Remove();
				Long count = fileMaker.Make(buffer);
				this->textEditor->document->SetLength(count);
				if (dynamic_cast<ANSIConverter*>(textConverter)) {
					strcpy(this->textEditor->encoding, "ANSI");
				}
				else if (dynamic_cast<UTF8Converter*>(textConverter)) {
					strcpy(this->textEditor->encoding, "UTF-8");
				}
				else if (dynamic_cast<UTF8BOMConverter*>(textConverter)) {
					strcpy(this->textEditor->encoding, "UTF-8(BOM)");
				}
				else if (dynamic_cast<UTF16LEConverter*>(textConverter)) {
					strcpy(this->textEditor->encoding, "UTF-16 LE");
				}
				else if (dynamic_cast<UTF16BEConverter*>(textConverter)) {
					strcpy(this->textEditor->encoding, "UTF-16 BE");
				}
				if (textConverter != 0) {
					delete textConverter;
				}
				if (pData != 0) {
					delete[] pData;
				}
				if (buffer != 0) {
					delete[] buffer;
				}
			}
		}
		file.Close();
		if (this->textEditor->isWrapped) {
			this->textEditor->document->LoadToMemory(1, OPTIMAL, this->textEditor->characterMetrics, this->textEditor->rect.right, &fileMaker);
		}
		else {
			this->textEditor->document->LoadToMemory(1, OPTIMAL, this->textEditor->characterMetrics, &fileMaker);
		}
		this->textEditor->note->First();
		this->textEditor->current = this->textEditor->note->GetAt(0);
		this->textEditor->current->First();
		this->textEditor->document->SetStart(1);
		this->textEditor->document->SetEnd(this->textEditor->note->GetRowCount());
		this->textEditor->isUpdated = true;
		this->textEditor->isScrolling = false;
		this->textEditor->document->isSelecting = false;
		this->textEditor->GetParent()->SetWindowText(fileName + " - 메모장");
		this->textEditor->fileName = fileName;
		this->textEditor->pathName = pathName;
		this->textEditor->isModified = false;
		this->textEditor->x = 0;
		this->textEditor->y = 0;
		this->textEditor->isMoving = false;
		this->textEditor->destination = 0;
		this->textEditor->scrollController->UpdateFileVSInfo(true);
		this->textEditor->scrollFlags = SF_ALL;
		this->textEditor->scrollController->UpdateMaximum();
		this->textEditor->scrollController->UpdatePosition(this->textEditor->characterMetrics);
		this->textEditor->Notify();
		this->textEditor->Invalidate(FALSE);
	}

	//페이지설정 다이얼로그 초기설정
	this->textEditor->psd.ptPaperSize.x = 21000;
	this->textEditor->psd.ptPaperSize.y = 29700;
	this->textEditor->psd.rtMargin.top = 1000;
	this->textEditor->psd.rtMargin.left = 1000;
	this->textEditor->psd.rtMargin.right = 1000;
	this->textEditor->psd.rtMargin.bottom = 1000;
	this->textEditor->psd.lpfnPagePaintHook = NULL;

	if (registry != 0) {
		delete registry;
	}
}

void OnCreateMessage::Unexecute() {

}
