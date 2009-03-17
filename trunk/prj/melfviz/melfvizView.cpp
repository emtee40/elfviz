// melfvizView.cpp : implementation of the CMelfvizView class
//

#include "stdafx.h"
#include "melfviz.h"

#include "melfvizDoc.h"
#include "melfvizView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMelfvizView

IMPLEMENT_DYNCREATE(CMelfvizView, CEditView)

BEGIN_MESSAGE_MAP(CMelfvizView, CEditView)
	//{{AFX_MSG_MAP(CMelfvizView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMelfvizView construction/destruction

CMelfvizView::CMelfvizView()
{
	// TODO: add construction code here

}

CMelfvizView::~CMelfvizView()
{
}

BOOL CMelfvizView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CMelfvizView drawing

void CMelfvizView::OnDraw(CDC* pDC)
{
	CMelfvizDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMelfvizView printing

BOOL CMelfvizView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CMelfvizView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing.
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CMelfvizView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CMelfvizView diagnostics

#ifdef _DEBUG
void CMelfvizView::AssertValid() const
{
	CEditView::AssertValid();
}

void CMelfvizView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CMelfvizDoc* CMelfvizView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMelfvizDoc)));
	return (CMelfvizDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMelfvizView message handlers

#define COLUMN_SIZE 16
void CMelfvizView::Refresh(elf_section_t *elf)
{
	elf_attr_t* attr = elf->get_attr();
	if(!attr) return;
	int num = attr->get_num();
	if(!num) return;
	CString text;
	GetWindowText(text);
	text.Empty();
	CString str;
	int type = 0;
	for(int i = 0 ; i < num ; i++){
		int type = attr->get_type(i);
		if(type & ELF_TYPE_STR) {
			str.Format("%s=%s", attr->get_name(i), attr->get_str(i));
			if(type & ELF_TYPE_INT){
				CString stra;
				stra.Format(" (0x%x)", attr->get_int(i));
				str += stra;
			}
			str += "\r\n";
		} else if(type & ELF_TYPE_HEX) {
			str.Format("%s=0x%x\r\n", attr->get_name(i), attr->get_int(i));
		} else {
			str.Format("%s=%d\r\n", attr->get_name(i), attr->get_int(i));
		}
		text += str;
	}
	text += "\r\n";
	elf_buffer_t* buf = elf->get_body();
	if(buf){
		for(i = 0 ; i < buf->size ; i += COLUMN_SIZE){
			int j = 0;
			int mx = buf->size - i;
			if(mx > COLUMN_SIZE) mx = COLUMN_SIZE;
			str.Format("%08x: ", (unsigned int)i);
			text += str;
			for(j = 0 ; j < mx ; j++) {
				str.Format("%02x ", buf->buffer[i + j]);
				text += str;
				if(j == 7)	text += " ";
			}
			if(mx < COLUMN_SIZE) {
				for(j = 0 ; j < COLUMN_SIZE - mx ; j++){
					text += "   ";
				}
				if(mx < 7) text += " ";
			}
			text += "  ";
			for(j = 0 ; j < mx ; j++) {
				str.Format("%c", (isprint(buf->buffer[i + j])) ? buf->buffer[i + j] : '.');
				text += str;
			}
			text += "\r\n";
		}
	}
	SetWindowText(text);
}
