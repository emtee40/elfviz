// melfvizDoc.cpp : implementation of the CMelfvizDoc class
//

#include "stdafx.h"
#include "melfviz.h"

#include "melfvizDoc.h"

#include "LeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMelfvizDoc

IMPLEMENT_DYNCREATE(CMelfvizDoc, CDocument)

BEGIN_MESSAGE_MAP(CMelfvizDoc, CDocument)
	//{{AFX_MSG_MAP(CMelfvizDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMelfvizDoc construction/destruction

CMelfvizDoc::CMelfvizDoc()
{
	// TODO: add one-time construction code here
	pElf = NULL;
}

CMelfvizDoc::~CMelfvizDoc()
{
	if(pElf) delete pElf;
}

BOOL CMelfvizDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

//	((CEditView*)m_viewList.GetHead())->SetWindowText(NULL);

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMelfvizDoc serialization

void CMelfvizDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
//	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CMelfvizDoc diagnostics

#ifdef _DEBUG
void CMelfvizDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMelfvizDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMelfvizDoc commands

BOOL CMelfvizDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	try{
		pElf = elfio_new((char*)lpszPathName);
	} catch(char* e){
		MessageBox(NULL, e, "error", MB_OK);
		return FALSE;
	}
	((CLeftView*)m_viewList.GetHead())->Refresh(pElf);
	return TRUE;
}


void CMelfvizDoc::Export(LPCTSTR format, LPCTSTR path)
{
	char szPath[MAX_PATH];
	GetModuleFileName(0, szPath, MAX_PATH);
	*(strrchr(szPath, '\\') + 1) = 0;
	CString str = "\"";
	str += szPath;
	str += "elf2txt.exe\" \"";
	str += GetPathName();
	str += "\" -o \"";
	str += path;
	str += "\" -f ";
	str += format;
	str.Replace("\\", "\\\\");

	TRACE("%s\n", str);

	//TODO:how to execute elf2txt
	HINSTANCE ret = ShellExecute(NULL, "open", str, NULL, NULL, SW_SHOW);
	return;
}
