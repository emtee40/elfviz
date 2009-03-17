// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "melfviz.h"

#include "melfvizDoc.h"
#include "LeftView.h"
#include "melfvizview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	//{{AFX_MSG_MAP(CLeftView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTreeView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftView construction/destruction

CLeftView::CLeftView()
{
	// TODO: add construction code here

}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView drawing

void CLeftView::OnDraw(CDC* pDC)
{
	CMelfvizDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


/////////////////////////////////////////////////////////////////////////////
// CLeftView printing

BOOL CLeftView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLeftView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLeftView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CMelfvizDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMelfvizDoc)));
	return (CMelfvizDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeftView message handlers

void CLeftView::ElfSectionToTreeCtrl(CTreeCtrl &tree, HTREEITEM hParent, elf_section_t *elfio)
{
	char* name = (char*)elfio->name();

	TVINSERTSTRUCT ti;
	memset(&ti, 0, sizeof(ti));
	ti.hParent = hParent;
	ti.hInsertAfter = TVI_LAST;
	ti.item.mask = TVIF_TEXT;
	ti.item.pszText = _T(name);
	ti.item.lParam = (LPARAM)elfio;

	HTREEITEM hNew = tree.InsertItem(&ti);
	tree.SetItemData(hNew, (DWORD)elfio);
	int num = elfio->get_child_num();
	for(int i = 0 ; i < num ; i++)
		ElfSectionToTreeCtrl(tree, hNew, elfio->get_child(i));
}

void CLeftView::Refresh(elf_section_t *elfio)
{
	CTreeCtrl& tree = GetTreeCtrl();
	tree.DeleteAllItems();
	ElfSectionToTreeCtrl(tree, 0, elfio);
}

int CLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	long style = TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT;
	long oldstyle = GetWindowLong(GetTreeCtrl().m_hWnd, GWL_STYLE);
	SetWindowLong(GetTreeCtrl().m_hWnd, GWL_STYLE, oldstyle | style);
	return 0;
}

void CLeftView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	CMelfvizDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CTreeCtrl& tree = GetTreeCtrl();
	elf_section_t* elf = (elf_section_t*)tree.GetItemData(pNMTreeView->itemNew.hItem);
	if(elf)
		m_wndEdit->Refresh(elf);
	*pResult = 0;
}

void CLeftView::SetEditView(CMelfvizView *view)
{
	m_wndEdit = view;
}
