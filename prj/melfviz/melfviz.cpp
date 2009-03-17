// melfviz.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "melfviz.h"

#include "MainFrm.h"
#include "melfvizDoc.h"
#include "LeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMelfvizApp

BEGIN_MESSAGE_MAP(CMelfvizApp, CWinApp)
	//{{AFX_MSG_MAP(CMelfvizApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_EXPORT_TXT, OnFileExportTxt)
	ON_COMMAND(ID_FILE_EXPORT_XML, OnFileExportXml)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMelfvizApp construction

CMelfvizApp::CMelfvizApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMelfvizApp object

CMelfvizApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMelfvizApp initialization

BOOL CMelfvizApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMelfvizDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CLeftView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CMelfvizApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMelfvizApp message handlers


void CMelfvizApp::OnFileExportTxt() 
{
	// TODO: Add your command handler code here
	DoExportFilePrompt("txt", "Text(*.txt)|*.txt||");
}

void CMelfvizApp::OnFileExportXml() 
{
	// TODO: Add your command handler code here
	DoExportFilePrompt("xml", "XML(*.xml)|*.xml||");
}

BOOL CMelfvizApp::DoExportFilePrompt(LPCTSTR lpszExt, LPCTSTR lpszFilter)
{
	MessageBox(NULL, "I'm on debuging,\nUse elf2txt.exe instead, please.", "Sorry", MB_OK);
	return false;

	POSITION pos = GetFirstDocTemplatePosition();
	CSingleDocTemplate* pDocTemplate = (CSingleDocTemplate*)GetNextDocTemplate(pos);
	if(!pDocTemplate) return false;
	pos = pDocTemplate->GetFirstDocPosition();
	CMelfvizDoc* pDoc = (CMelfvizDoc*)pDocTemplate->GetNextDoc(pos);
	if(!pDoc) return false;
	CFileDialog dlg(false, lpszExt, pDoc->GetPathName(), OFN_OVERWRITEPROMPT, lpszFilter);
	if(dlg.DoModal() == IDOK)	pDoc->Export(lpszExt, dlg.GetPathName());
	return true;
}
