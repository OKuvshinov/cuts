
// cutsDlg.h : header file
//

#pragma once


// CcutsDlg dialog
class CcutsDlg : public CDialogEx
{
// Construction
public:
	CcutsDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUTS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void button_find_area();
	afx_msg void button_find_uptimum();
	afx_msg void button_draw();
	afx_msg void OnBnClickedButton4();
	CEdit optimalPosValue;
};
