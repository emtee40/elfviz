// melfvizDoc.h : interface of the CMelfvizDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MELFVIZDOC_H__802D20D7_3397_4E8A_A96E_A7B5BE007F5A__INCLUDED_)
#define AFX_MELFVIZDOC_H__802D20D7_3397_4E8A_A96E_A7B5BE007F5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMelfvizDoc : public CDocument
{
protected: // create from serialization only
	CMelfvizDoc();
	DECLARE_DYNCREATE(CMelfvizDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMelfvizDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Export(LPCTSTR format, LPCTSTR path);
	virtual ~CMelfvizDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMelfvizDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	elfSection* pElf;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MELFVIZDOC_H__802D20D7_3397_4E8A_A96E_A7B5BE007F5A__INCLUDED_)
