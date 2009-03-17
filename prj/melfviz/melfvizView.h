// melfvizView.h : interface of the CMelfvizView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MELFVIZVIEW_H__0531AA97_C794_4763_BD3E_C4E7C0EB6104__INCLUDED_)
#define AFX_MELFVIZVIEW_H__0531AA97_C794_4763_BD3E_C4E7C0EB6104__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "elfio/elfio.h"

class CMelfvizView : public CEditView
{
protected: // create from serialization only
	CMelfvizView();
	DECLARE_DYNCREATE(CMelfvizView)

// Attributes
public:
	CMelfvizDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMelfvizView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Refresh(elf_section_t* elf);
	virtual ~CMelfvizView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMelfvizView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in melfvizView.cpp
inline CMelfvizDoc* CMelfvizView::GetDocument()
   { return (CMelfvizDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MELFVIZVIEW_H__0531AA97_C794_4763_BD3E_C4E7C0EB6104__INCLUDED_)
