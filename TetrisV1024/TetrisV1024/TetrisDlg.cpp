
//TetrisDlg.cpp : implementation file
//
//
// author: qinglanyu
// email: qinglanyu_jun@foxmail.com
// date: 2018-6-6

#include "stdafx.h"
#include "TetrisV1024.h"
#include "TetrisDlg.h"
#include "afxdialogex.h"
#include "Tetris.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTetrisDlg dialog



CTetrisDlg::CTetrisDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTetrisDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTetrisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTetrisDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDB_START, &CTetrisDlg::OnBnClickedStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDB_STOP, &CTetrisDlg::OnBnClickedStop)
END_MESSAGE_MAP()


// CTetrisDlg message handlers

BOOL CTetrisDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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
	// initialize game data
	if (!m_pTetris)
	{
		m_pTetris = new CTetris;
		m_gmBdWd = m_pTetris->GetGameBoardWidth();
		m_gmBdHt = m_pTetris->GetGameBoardHeight();
		m_nxBdWd = m_pTetris->GetNextBoardWidth();
		m_nxBdHt = m_pTetris->GetNextBoardHeight();
		m_pTetris->InitGame();
		m_gameBoard = m_pTetris->GetGameBoard();
		m_nextBoard = m_pTetris->GetNextBoard();
	}

	// game graphing 
	m_pGameBoardDC = new CClientDC(this);
	m_pNextBoardDC = new CClientDC(this);
	m_pScoreBoardDC = new CClientDC(this);

	GetDlgItem(IDC_INFORECT)->ShowWindow(SW_HIDE);
	InitDrawingRects();
	InitDrawingBrushs();

	// start timer for graphing 
	SetTimer(RUNNING_TIMER, 10, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CTetrisDlg::InitDrawingRects()
{
	GetDlgItem(IDC_GAMEBOARD)->GetWindowRect(&m_gBdRect);
	GetDlgItem(IDC_NEXTBOARD)->GetWindowRect(&m_nBdRect);
	GetDlgItem(IDC_INFORECT)->GetWindowRect(&m_sBdRect);
	ScreenToClient(m_gBdRect);
	ScreenToClient(m_nBdRect);
	ScreenToClient(m_sBdRect);
}


void CTetrisDlg::InitDrawingBrushs()
{
	int nRed[] = { 0, 255, 255, 0, 0, 0, 128, 255, 0, 0, 255, 128, 0, 128, 0, 255, 255, 128, 0, 255, 255 };
	int nGrn[] = { 0, 128, 255, 255, 255, 128, 128, 0, 0, 255, 255, 255, 255, 255, 128, 128, 255, 255, 128, 0, 128 };
	int nBlu[] = { 0, 128, 128, 64, 255, 255, 192, 0, 255, 0, 255, 128, 128, 255, 255, 192, 0, 0, 192, 255, 64 };

	const int maxRGBelem = sizeof(nRed) / sizeof(nRed[0]);

	for (int ii = 0; ii < MAX_COLOR_NUM; ii++)
	{
		COLORREF	rndColor = RGB(nRed[ii], nGrn[ii], nBlu[ii]);

		if (m_Brushs[ii].GetSafeHandle())
		{
			m_Brushs[ii].DeleteObject();
		}
		m_Brushs[ii].CreateSolidBrush(rndColor);
	}
	
	GetRandomColor();
}

int CTetrisDlg::GetRandomColor()
{
	srand((unsigned)time(NULL));
	m_shapeColor = rand() % MAX_COLOR_NUM;
	return m_shapeColor;
}

int CTetrisDlg::GetCurrentColor() const
{
	return m_shapeColor;
}


void CTetrisDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTetrisDlg::OnPaint()
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
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTetrisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CTetrisDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (!(pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP))
	{
		return CDialogEx::PreTranslateMessage(pMsg);
	}

#define ProcessKeyMsg(wp)	(pMsg->message==WM_KEYDOWN? m_pTetris->GetInputKey(wp):true,\
							 pMsg->message==WM_KEYUP?m_pTetris->ReleaseKey(wp):true)
	int nRet = 0;

	switch (pMsg->wParam)
	{
	case 'a':
	case 'A':
	case VK_LEFT:
		ProcessKeyMsg(VK_LEFT);
		return TRUE;
	case 's':
	case 'S':
	case VK_DOWN:
		ProcessKeyMsg(VK_DOWN);
		return TRUE;
	case 'w':
	case 'W':
	case VK_UP:
		ProcessKeyMsg(VK_UP);
		return TRUE;
	case 'd':
	case 'D':
	case VK_RIGHT:
		ProcessKeyMsg(VK_RIGHT);
		return TRUE;
	case VK_ESCAPE:
		return TRUE;
	case 'g': //pause or continue
	case 'G':
		nRet = ProcessKeyMsg('G');
		SetDlgItemText(IDB_START, _T("Start"));
		return TRUE;
	case VK_SPACE:
		//ProcessKeyMsg(VK_SPACE);
		(pMsg->message == WM_KEYUP ? m_pTetris->GetInputKey(VK_SPACE) : true);
		return TRUE;
	default:
		break;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CTetrisDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pTetris)
	{
		m_pTetris->ShutDownGame();
		delete m_pTetris;
		m_pTetris = 0;
	}

	if (m_pGameBoardDC)
	{
		delete m_pGameBoardDC;
		m_pGameBoardDC = 0;
	}

	if (m_pNextBoardDC)
	{
		delete m_pNextBoardDC;
		m_pNextBoardDC = 0;
	}

	if (m_pScoreBoardDC)
	{
		delete m_pScoreBoardDC;
		m_pScoreBoardDC = 0;
	}

	CDialogEx::OnCancel();
}


void CTetrisDlg::OnBnClickedStart()
{
	// TODO: Add your control notification handler code here
	GameState gs = m_pTetris->GetGameState();
	switch (gs)
	{
	case GS_READY:		// ready -> falling
		m_pTetris->RunGame();
		SetDlgItemText(IDB_START, _T("Pause"));
		break;
	case GS_RUNNING:	// running -> pause
		m_pTetris->PauseGame();
		SetDlgItemText(IDB_START, _T("Continue"));
		break;
	case GS_PAUSE:		// pause -> continue
		m_pTetris->ReleaseGame();
		SetDlgItemText(IDB_START, _T("Pause"));
		break;
	}
}

void CTetrisDlg::OnBnClickedStop()
{
	// TODO: Add your control notification handler code here
	//KillTimer(RUNNING_TIMER);
	GameState gs = m_pTetris->GetGameState();
	switch (gs)
	{
	case GS_READY:
	case GS_RUNNING:
	case GS_PAUSE:
		m_pTetris->ResetGame();
		m_gameBoard = m_pTetris->GetGameBoard();
		SetDlgItemText(IDB_START, _T("Start"));
		break;
	default:
		break;
	}
}


void CTetrisDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case RUNNING_TIMER:
	{
		if (m_pTetris->GetGameState() != GS_OVER)
		{
			m_pTetris->HeartMsgProcess();
		}
		
		UpdateGamePanel();
		break;
	}
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CTetrisDlg::UpdateGamePanel()
{
	// temp bit map
	CBitmap	gameBoardBmp, nextBoardBmp, scoreBoardBmp;
	gameBoardBmp.CreateCompatibleBitmap(m_pGameBoardDC, m_gBdRect.Width(), m_gBdRect.Height());
	nextBoardBmp.CreateCompatibleBitmap(m_pNextBoardDC, m_nBdRect.Width(), m_nBdRect.Height());
	//scoreBoardBmp.CreateCompatibleBitmap(m_pScoreBoardDC, scoreBoardRect.Width(), scoreBoardRect.Height());

	// temp memory dc
	CDC	memGameBoardDC, memNextBoardDC, memScoreBoardDC;
	memGameBoardDC.CreateCompatibleDC(m_pGameBoardDC);
	memNextBoardDC.CreateCompatibleDC(m_pNextBoardDC);
	memScoreBoardDC.CreateCompatibleDC(m_pScoreBoardDC);

	memGameBoardDC.SelectObject(&gameBoardBmp);
	memNextBoardDC.SelectObject(&nextBoardBmp);
	memScoreBoardDC.SelectObject(&scoreBoardBmp);

	/* begin painting in memory dc */
	PaintingInMemoryDC(&memGameBoardDC, &memNextBoardDC, &memScoreBoardDC);
	/* end painting in memory dc */

	// copy memory dc image into CClientDC
	int xPos = m_gBdRect.left, yPos = m_gBdRect.top, bdWidth = m_gBdRect.Width(), bdHight = m_gBdRect.Height();
	m_pGameBoardDC->BitBlt(xPos, yPos, bdWidth, bdHight, &memGameBoardDC, 0, 0, SRCCOPY);
	xPos = m_nBdRect.left, yPos = m_nBdRect.top, bdWidth = m_nBdRect.Width(), bdHight = m_nBdRect.Height();
	m_pNextBoardDC->BitBlt(xPos, yPos, bdWidth, bdHight, &memNextBoardDC, 0, 0, SRCCOPY);
	xPos = m_sBdRect.left, yPos = m_sBdRect.top, bdWidth = m_sBdRect.Width(), bdHight = m_sBdRect.Height();
	m_pScoreBoardDC->BitBlt(xPos, yPos, bdWidth, bdHight, &memScoreBoardDC, 0, 0, SRCCOPY);
}


void CTetrisDlg::PaintingInMemoryDC(CDC *pGameBoardDC, CDC *pNextBoardDC, CDC *pScoreBoardDC)
{
	m_pMemGBDC = pGameBoardDC, m_pMemNBDC = pNextBoardDC, m_pMemSBDC = pScoreBoardDC;

	DrawGameBoard();
	DrawFullingShape();	
	DrawNextBoard();
	DrawScoreBoard();
}

void CTetrisDlg::DrawFullingShape()
{
	CShape *pShape = m_pTetris->GetFallingShape();
	if (!pShape)
	{
		return;
	}

	CShapeState *pState = pShape->getCurrentState();
	if (!pState)
	{
		return;
	}

	for (int ii = 0; ii < 4; ii++)
	{
		int xpos = (int)pState->m_cellPoint[ii].x + pShape->getShapePosition()->x;
		int ypos = (int)pState->m_cellPoint[ii].y + pShape->getShapePosition()->y;
		DrawGmBdCell(xpos, ypos, &m_Brushs[pShape->GetShapeColor()]);
	}
}

void CTetrisDlg::DrawGameBoard()
{
	for (int iRow = 0; iRow < m_gmBdHt; iRow++)
	{
		for (int jCol = 0; jCol < m_gmBdWd; jCol++)
		{
			if (m_gameBoard != NULL)
			{
				int brushColor = m_gameBoard[iRow][jCol].m_cellColor;
				DrawGmBdCell(iRow, jCol, &m_Brushs[brushColor]);
			}			
		}
	}
}

void CTetrisDlg::DrawNextBoard()
{
	for (int iRow = 0; iRow < m_nxBdHt; iRow++)
	{
		for (int jCol = 0; jCol < m_nxBdWd; jCol++)
		{
			int brushColor = m_nextBoard[iRow][jCol].m_cellColor;
			DrawNxBdCell(iRow, jCol, &m_Brushs[brushColor]);
		}
	}
}

void CTetrisDlg::DrawScoreBoard()
{

}

void CTetrisDlg::DrawGmBdCell(int x, int y, CBrush *pBrush)
{
	if (x < 0 || x >= m_gmBdHt || y<0 || y>m_gmBdWd)
		return;

	m_pMemGBDC->FillRect(m_gameBoard[x][y], pBrush);
}

void CTetrisDlg::DrawNxBdCell(int x, int y, CBrush *pBrush)
{
	if (x < 0 || x >= m_nxBdHt || y < 0 || y >= m_nxBdWd)
		return;

	m_pMemNBDC->FillRect(m_nextBoard[x][y], pBrush);
}



