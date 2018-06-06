
// TetrisDlg.h : header file
//

#pragma once
#include "Tetris.h"


// CTetrisDlg dialog
class CTetrisDlg : public CDialogEx
{
// Construction
public:
	CTetrisDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TETRISV1024_DIALOG };
	enum { RUNNING_TIMER = IDD };
	static const int MAX_COLOR_NUM = MAX_SHAPECOLOR_NUM;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	void InitDrawingRects();
	void InitDrawingBrushs();
	int  GetRandomColor();
	int  GetCurrentColor() const;

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	CTetris		*m_pTetris;
	GAMEBOARD	m_gameBoard;
	NEXTBOARD	m_nextBoard;
	int m_gmBdWd, m_gmBdHt;
	int m_nxBdWd, m_nxBdHt;

	// painting tools
	CClientDC	*m_pGameBoardDC, *m_pNextBoardDC, *m_pScoreBoardDC;
	CDC		*m_pMemGBDC, *m_pMemNBDC, *m_pMemSBDC;
	CBrush	m_Brushs[MAX_COLOR_NUM];
	int m_shapeColor;
	CRect m_gBdRect, m_nBdRect, m_sBdRect;

	virtual void OnCancel();
public:
	afx_msg void OnBnClickedStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStop();

	void UpdateGamePanel();
	void PaintingInMemoryDC(CDC *pGameBoardDC, CDC *pNextBoardDC, CDC *pScoreBoardDC);

	void DrawFullingShape();
	void DrawGameBoard();
	void DrawNextBoard();
	void DrawScoreBoard();

protected:
	void DrawGmBdCell(int x, int y, CBrush *pBrush);
	void DrawNxBdCell(int x, int y, CBrush *pBrush);
};
