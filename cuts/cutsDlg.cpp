
// cutsDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "cuts.h"
#include "cutsDlg.h"
#include "afxdialogex.h"

#include "my_header.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CcutsDlg dialog



CcutsDlg::CcutsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CUTS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcutsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, optimalPosValue);
}

BEGIN_MESSAGE_MAP(CcutsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CcutsDlg::button_find_area)
	ON_BN_CLICKED(IDC_BUTTON2, &CcutsDlg::button_find_uptimum)
	ON_BN_CLICKED(IDC_BUTTON3, &CcutsDlg::button_draw)
	ON_BN_CLICKED(IDC_BUTTON4, &CcutsDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CcutsDlg message handlers

BOOL CcutsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	init_cuts();
	init_poligons();
	init_draw_dialog();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CcutsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CcutsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

		mydc = new CPaintDC(drawDialog);

		mydc->SelectObject(&bluepen);
		draw_base();

		mydc->SelectObject(&blackpen);
		draw_polygons();

		mydc->SelectObject(&greenpen);
		draw_axis();

		mydc->SelectObject(&redpen);	
		draw_optimal_position();
		draw_x0();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CcutsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CcutsDlg::button_find_area()
{
	find_area();
	// TODO: Add your control notification handler code here
}


void CcutsDlg::button_find_uptimum()
{
	CString string;

	find_optimum();

	string.Format(_T("(%.2f, %.2f)"), optimalPosition.point.x, optimalPosition.point.y);
	optimalPosValue.SetWindowTextW(string);
	// TODO: Add your control notification handler code here
}


void CcutsDlg::button_draw()
{
	drawDialog->InvalidateRect(drawArea);
	OnPaint();
	// TODO: Add your control notification handler code here
}


void CcutsDlg::OnBnClickedButton4()
{
	int myRandX = 0, myRandY = 0;

	//for (size_t i = 0; i < numOfPolys; i++)
	//{
	//	myRandX = (rand() % 20) - 10;
	//	myRandY = (rand() % 20) - 10;
	//	for (size_t j = 0; j < numOfCuts; j++)
	//	{
	//		for (int k = 0; k < 2; k++)
	//		{
	//			polygons[i].edge[j].p[k].x = basePolygons[i].edge[j].p[k].x + myRandX;
	//			polygons[i].edge[j].p[k].y = basePolygons[i].edge[j].p[k].y + myRandY;
	//		}
	//	}
	//}

	find_area();
	find_optimum();
	button_draw();
	// TODO: Add your control notification handler code here
}
