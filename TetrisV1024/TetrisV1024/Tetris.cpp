//
// author: qinglanyu
// email: qinglanyu_jun@foxmail.com
// date: 2018-6-6

#include "stdafx.h"
#include "Tetris.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CShape::CShape(GAMEBOARD gameBoard, const int stateCount) 
	: g_gameBoard(gameBoard), 
	m_MAX_STATE_COUNT(stateCount)
{}

CShape::~CShape()
{
	map<int, CShapeState* >::iterator itor = m_allStates.begin();
	while (m_allStates.size() > 0)
	{
		itor = m_allStates.begin();
		CShapeState *pState = itor->second;
		if (pState)
		{
			delete pState;
			m_allStates.erase(itor);
		}
	}
}

bool CShape::InitShapeData()
{
	ResetShape();

	return true;
}

bool CShape::MoveLeft()
{
	return MoveOneCell(MD_LEFT);
}

bool CShape::MoveRight()
{
	return MoveOneCell(MD_RIGHT);
}

bool CShape::FallDown()
{
	return MoveOneCell(MD_DOWN);
}

bool CShape::Translate()
{
	return ChangeState();
}

CShapeState *CShape::getCurrentState()
{
	return getShapeState(m_currentStateKey);
}

CShapeState *CShape::getRandomState()
{
	ResetShape();

	return getCurrentState();
}


CPoint	*CShape::getShapePosition()
{
	return &m_shapePosition;
}


int  CShape::GetShapeColor() const
{
	return m_shapeColor;
}

CShapeState	* CShape::getShapeState(const int stateKey)
{
	CShapeState *pShapeState = NULL;
	map<int, CShapeState* >::iterator itor = m_allStates.begin();
	for (; itor != m_allStates.end(); itor++)
	{
		if (stateKey == itor->first)
		{
			pShapeState = itor->second;
			break;
		}
	}	

	/*pShapeState = m_allStates[stateKey];*/

	ASSERT(pShapeState);
	return pShapeState;
}

bool CShape::MoveOneCell(MoveDirection direction)
{
	bool bMoved = true;

	CPoint offset;
	switch (direction)
	{
	case CShape::MD_LEFT: offset = CPoint(0, -1); break;
	case CShape::MD_RIGHT:offset = CPoint(0, 1); break;
	case CShape::MD_DOWN: offset = CPoint(1, 0); break;
	}

	CShapeState *pCurntState = getCurrentState();
	for (int ii = 0; ii < 4; ii++)
	{
		CPoint movedPoint = pCurntState->m_cellPoint[ii] + m_shapePosition + offset;
		int xpos = movedPoint.x;
		int ypos = movedPoint.y;

		if (!((xpos >= 0) && (xpos < MAX_GAMEBOARD_ROW) &&\
			(ypos>=0) && (ypos<MAX_GAMEBOARD_COLUMN) && \
			(1 != g_gameBoard[xpos][ypos].m_isFilled)))
		{
			bMoved = false;
			break;
		}
	}

	if (bMoved)
	{
		m_shapePosition += offset;
	}

	return bMoved;
}

bool CShape::ChangeState()
{
	bool bChanged = true;
	int maxStateCount = m_allStates.size();
	int nextStateKey = m_currentStateKey + 1;
	nextStateKey %= maxStateCount;

	CShapeState *pNextState = getShapeState(nextStateKey);
	for (int ii = 0; ii < 4; ii++)
	{
		CPoint changedPoint = pNextState->m_cellPoint[ii] + m_shapePosition;
		int xpos = changedPoint.x;
		int ypos = changedPoint.y;

		if (!((xpos >= 0) && (xpos < MAX_GAMEBOARD_ROW) && (ypos < MAX_GAMEBOARD_COLUMN) && \
			(ypos >= 0) && (1 != g_gameBoard[xpos][ypos].m_isFilled)))
		{
			bChanged = false;
			break;
		}
	}

	if (bChanged)
	{
		m_currentStateKey = nextStateKey;
	}

	return bChanged;
}

void CShape::ResetShape()
{
	m_shapePosition = CPoint(0, MAX_GAMEBOARD_COLUMN / 2 - 1);
	srand((unsigned int)time(NULL));
	m_currentStateKey = rand() % m_MAX_STATE_COUNT;

	do 
	{
		m_shapeColor = rand() % MAX_SHAPECOLOR_NUM;
	} while (m_shapeColor==0);
}

CLineShape::CLineShape(GAMEBOARD gameBoard, const int stateCount)
	: CShape(gameBoard, stateCount)
{}

bool CLineShape::InitShapeData()
{
	int stateCount = 0;
	CShapeState *pState = new CShapeState;
	pState->m_cellPoint[0].y = -1;    // relative to Shape Position
	pState->m_cellPoint[1].y = 0;
	pState->m_cellPoint[2].y = 1;
	pState->m_cellPoint[3].y = 2;
	m_allStates[stateCount] = pState;
	stateCount++;

	pState = new CShapeState;
	pState->m_cellPoint[0] = CPoint(-1, 1);
	pState->m_cellPoint[1] = CPoint(0, 1);
	pState->m_cellPoint[2] = CPoint(1, 1);
	pState->m_cellPoint[3] = CPoint(2, 1);
	m_allStates[stateCount] = pState;
	stateCount++;

	ASSERT(stateCount == m_MAX_STATE_COUNT);
	return CShape::InitShapeData();
}

CSquareShape::CSquareShape(GAMEBOARD gameBoard, const int stateCount)
	: CShape(gameBoard, stateCount)
{}

bool CSquareShape::InitShapeData()
{
	int stateCount = 0;
	CShapeState *pState = new CShapeState;
	pState->m_cellPoint[1] = CPoint(1, 0);
	pState->m_cellPoint[2] = CPoint(0, 1);
	pState->m_cellPoint[3] = CPoint(1, 1);
	m_allStates[stateCount] = pState;
	stateCount++;

	ASSERT(stateCount == m_MAX_STATE_COUNT);
	return CShape::InitShapeData();
}

CZ1Shape::CZ1Shape(GAMEBOARD gameBoard, const int stateCount)
	:CShape(gameBoard, stateCount)
{}

bool CZ1Shape::InitShapeData()
{
	int stateCount = 0;
	CShapeState *pState = new CShapeState;
	pState->m_cellPoint[1] = CPoint(0, 1);
	pState->m_cellPoint[2] = CPoint(1, 1);
	pState->m_cellPoint[3] = CPoint(1, 2);
	m_allStates[stateCount] = pState;
	stateCount++;

	pState = new CShapeState;
	pState->m_cellPoint[0] = CPoint(0, 1);
	pState->m_cellPoint[1] = CPoint(1, 1);
	pState->m_cellPoint[2] = CPoint(0, 2);
	pState->m_cellPoint[3] = CPoint(-1, 2);
	m_allStates[stateCount] = pState;
	stateCount++;

	ASSERT(stateCount == m_MAX_STATE_COUNT);
	return CShape::InitShapeData();
}

CLShape::CLShape(GAMEBOARD gameBoard, const int stateCount)
	:CShape(gameBoard, stateCount)
{}

bool CLShape::InitShapeData()
{
	int stateCount = 0;
	CShapeState *pState = new CShapeState;
	pState->m_cellPoint[1] = CPoint(0, 1);
	pState->m_cellPoint[2] = CPoint(0, 2);
	pState->m_cellPoint[3] = CPoint(1, 2);
	m_allStates[stateCount] = pState;
	stateCount++;

	pState = new CShapeState;
	pState->m_cellPoint[0] = CPoint(0, 1);
	pState->m_cellPoint[1] = CPoint(1, 1);
	pState->m_cellPoint[2] = CPoint(-1, 1);
	pState->m_cellPoint[3] = CPoint(-1, 2);
	m_allStates[stateCount] = pState;
	stateCount++;

	pState = new CShapeState;
	pState->m_cellPoint[1] = CPoint(-1, 0);
	pState->m_cellPoint[2] = CPoint(0, 1);
	pState->m_cellPoint[3] = CPoint(0, 2);
	m_allStates[stateCount] = pState;
	stateCount++;

	pState = new CShapeState;
	pState->m_cellPoint[0] = CPoint(0, 1);
	pState->m_cellPoint[1] = CPoint(-1, 1);
	pState->m_cellPoint[2] = CPoint(1, 1);
	pState->m_cellPoint[3] = CPoint(1, 0);
	m_allStates[stateCount] = pState;
	stateCount++;

	ASSERT(stateCount == m_MAX_STATE_COUNT);
	return CShape::InitShapeData();
}

CWShape::CWShape(GAMEBOARD gameBoard, const int stateCount)
	:CShape(gameBoard, stateCount)
{}

bool CWShape::InitShapeData()
{
	int stateCount = 0;
	CShapeState *pState = new CShapeState;
	pState->m_cellPoint[1] = CPoint(-1, 1);
	pState->m_cellPoint[2] = CPoint(0, 1);
	pState->m_cellPoint[3] = CPoint(1, 1);
	m_allStates[stateCount] = pState;
	stateCount++;

	pState = new CShapeState;
	pState->m_cellPoint[1] = CPoint(0, 1);
	pState->m_cellPoint[2] = CPoint(0, 2);
	pState->m_cellPoint[3] = CPoint(1, 1);
	m_allStates[stateCount] = pState;
	stateCount++;

	pState = new CShapeState;
	pState->m_cellPoint[0] = CPoint(-1, 1);
	pState->m_cellPoint[1] = CPoint(0, 1);
	pState->m_cellPoint[2] = CPoint(0, 2);
	pState->m_cellPoint[3] = CPoint(1, 1);
	m_allStates[stateCount] = pState;
	stateCount++;

	pState = new CShapeState;
	pState->m_cellPoint[1] = CPoint(0, 1);
	pState->m_cellPoint[2] = CPoint(0, 2);
	pState->m_cellPoint[3] = CPoint(-1, 1);
	m_allStates[stateCount] = pState;
	stateCount++;

	ASSERT(stateCount == m_MAX_STATE_COUNT);
	return CShape::InitShapeData();
}

CGame::CGame()
{
}

CGame::~CGame()
{
}

CTetrisData::CTetrisData(const int shapeCount)
	:m_GameBoard(NULL),
	m_NextBoard(NULL),
	m_MAX_SHAPE_COUNT(shapeCount)
{
	
}

CTetrisData::~CTetrisData()
{
	map<int, CShape* >::iterator itor = m_AllShapes.begin();
	while (m_AllShapes.size() > 0)
	{
		itor = m_AllShapes.begin();
		CShape *pShape = itor->second;
		if (pShape)
		{
			delete pShape;
			m_AllShapes.erase(itor);
		}
	}

	if (m_GameBoard)
	{
		for (int ii = 0; ii < MAX_GAMEBOARD_ROW; ii++)
		{
			delete m_GameBoard[ii];
			m_GameBoard[ii] = NULL;
		}

		delete[] m_GameBoard;
		m_GameBoard = 0;
	}

	if (m_NextBoard)
	{
		for (int ii = 0; ii < MAX_NEXTBOARD_ROW; ii++)
		{
			delete m_NextBoard[ii];
			m_NextBoard[ii] = NULL;
		}

		delete[] m_NextBoard;
		m_NextBoard = 0;
	}
}

bool CTetrisData::InitGameData()
{
	static bool bInitialized = false;

	InitGameBoard();
	InitAllShapes();
	InitCurrentNextShape();

	bInitialized = true;
	return bInitialized;
}

bool CTetrisData::ResetGameData()
{
	delete m_GameBoard;
	delete m_NextBoard;
	m_GameBoard = NULL, m_NextBoard = NULL;
	InitGameBoard();
	InitCurrentNextShape();

	return true;
}

void CTetrisData::InitGameBoard()
{
	ASSERT(NULL == m_GameBoard);
	ASSERT(NULL == m_NextBoard);

	m_GameBoard = (CCell**) new CCell*[MAX_GAMEBOARD_ROW];
	for (int ii = 0; ii < MAX_GAMEBOARD_ROW; ii++)
	{
		m_GameBoard[ii] = (CCell*) new CCell[MAX_GAMEBOARD_COLUMN];
		for (int j = 0; j < MAX_GAMEBOARD_COLUMN; j++)
		{
			CCell	cell(j*CELLCX, ii*CELLCY, (j + 1)*CELLCX - 1, (ii + 1)*CELLCY - 1);
			m_GameBoard[ii][j] = cell;
		}
	}

	m_NextBoard = (CCell**) new CCell*[MAX_NEXTBOARD_ROW];
	for (int ii = 0; ii < MAX_NEXTBOARD_ROW; ii++)
	{
		m_NextBoard[ii] = (CCell*) new CCell[MAX_NEXTBOARD_COLUMN];
		for (int j = 0; j < MAX_NEXTBOARD_COLUMN; j++)
		{
			CCell ncell(j*CELLCX, ii*CELLCY, (j + 1)*CELLCX - 1, (ii + 1)*CELLCY - 1);
			//m_NextBoard[ii][j] = ncell;
			memcpy_s(&m_NextBoard[ii][j], sizeof(CCell), &ncell, sizeof(CCell));
		}
	}
}

void CTetrisData::InitAllShapes()
{
	int shapeCount = 0;
	CShape *pShape = new CLineShape(m_GameBoard);
	pShape->InitShapeData();
	m_AllShapes[shapeCount] = pShape;
	shapeCount++;

	pShape = new CSquareShape(m_GameBoard);
	pShape->InitShapeData();
	m_AllShapes[shapeCount] = pShape;
	shapeCount++;

	pShape = new CZ1Shape(m_GameBoard);
	pShape->InitShapeData();
	m_AllShapes[shapeCount] = pShape;
	shapeCount++;

	pShape = new CLShape(m_GameBoard);
	pShape->InitShapeData();
	m_AllShapes[shapeCount] = pShape;
	shapeCount++;

	pShape = new CWShape(m_GameBoard);
	pShape->InitShapeData();
	m_AllShapes[shapeCount] = pShape;
	shapeCount++;

	ASSERT(shapeCount == m_MAX_SHAPE_COUNT);
}

void CTetrisData::InitCurrentNextShape()
{
	srand((unsigned)time(NULL));
	m_currentShapeKey = rand() % m_MAX_SHAPE_COUNT;
	m_nextShapeKey = rand() % m_MAX_SHAPE_COUNT;
}

void CTetrisData::ChangeShape()
{
	// reset current shape first and then switch the current shape to the next one
	CShape *pShape = GetCurrentShape();
	if (pShape)
	{
		pShape->ResetShape();
	}

	// change to the next one
	m_currentShapeKey = m_nextShapeKey;
	srand((unsigned)time(NULL));
	m_nextShapeKey = rand() % m_MAX_SHAPE_COUNT;
}

CShape* CTetrisData::GetCurrentShape()
{
	return m_AllShapes.find(m_currentShapeKey)->second;
}

CShape* CTetrisData::GetNextShape()
{
	return m_AllShapes.find(m_nextShapeKey)->second;
}

int CTetrisData::ClearFullLines()
{
	int nClearLines = 0;

	for (int i = MAX_GAMEBOARD_ROW-1; i >= 0; --i)	// from bottom to top lines
	{
		int cells = 0;

		for (int j = 0; j < MAX_GAMEBOARD_COLUMN; ++j)
		{
			cells += m_GameBoard[i][j].m_isFilled;
		}

		if (cells == MAX_GAMEBOARD_COLUMN)
		{
			++nClearLines;

			for (int k = 0; k < MAX_GAMEBOARD_COLUMN; ++k)
			{
				m_GameBoard[i][k].m_isFilled = 0;	// clear the i^th line
			}
			for (int m = i; m >= 1; --m)
			{
				for (int jj = 0; jj < MAX_GAMEBOARD_COLUMN; ++jj)
				{
					m_GameBoard[m][jj].m_isFilled = m_GameBoard[m - 1][jj].m_isFilled;
					m_GameBoard[m][jj].m_cellColor = m_GameBoard[m - 1][jj].m_cellColor;
				}
			}
		}
	}

	return nClearLines;
}

void CTetrisData::MergeCurrentShapeToBoard()
{
	CShape *pCurrentShape = GetCurrentShape();
	if (!pCurrentShape)
	{
		return;
	}
	CShapeState *pCurrentState = pCurrentShape->getCurrentState();
	if (!pCurrentState)
	{
		return;
	}

	for (int i = 0; i < 4; ++i)
	{
		int curX = (int)pCurrentState->m_cellPoint[i].x + pCurrentShape->getShapePosition()->x;
		int curY = (int)pCurrentState->m_cellPoint[i].y + pCurrentShape->getShapePosition()->y;

		if (!(curX >= 0 && curX < MAX_GAMEBOARD_ROW) && (curY >= 0 && curY < MAX_GAMEBOARD_COLUMN))
		{
			continue;
		}
		m_GameBoard[curX][curY].m_isFilled = 1;
		m_GameBoard[curX][curY].m_cellColor = pCurrentShape->GetShapeColor();
	}
}

bool CTetrisData::IsTopLineFilled()
{
	bool bFilled = false;

	for (int j = 0; j < MAX_GAMEBOARD_COLUMN; ++j)
	{
		if (m_GameBoard[0][j].m_isFilled == 1)
		{
			bFilled = true;
			break;
		}
	}

	return bFilled;
}

void CTetrisData::MergeNextShapeToBoard()
{
	CShape *pNextShape = GetNextShape();
	if (!pNextShape)
	{
		return;
	}
	CShapeState *pNextState = pNextShape->getCurrentState();
	if (!pNextState)
	{
		return;
	}

	ClearNextBoard();

	for (int i = 0; i < 4; ++i)
	{
		int curX = pNextState->m_cellPoint[i].x + 1;
		int curY = pNextState->m_cellPoint[i].y + 1;

		if (!(curX >= 0 && curX < MAX_NEXTBOARD_ROW) && (curY >= 0 && curY < MAX_NEXTBOARD_COLUMN))
		{
			continue;
		}
		m_NextBoard[curX][curY].m_isFilled = 1;
		m_NextBoard[curX][curY].m_cellColor = pNextShape->GetShapeColor();
	}
}

void CTetrisData::ClearNextBoard()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m_NextBoard[i][j].m_isFilled = 0;
			m_NextBoard[i][j].m_cellColor = 0;
		}
	}
}


CTetris::CTetris()
	:m_pGameData(0),
	m_gameState(GS_READY)
{
}

CTetris::~CTetris()
{
	if (m_pGameData)
	{
		delete m_pGameData;
		m_pGameData = NULL;
	}
}

bool CTetris::InitGame()
{
	m_pGameData = new CTetrisData();
	m_pGameData->InitGameData();


	ResetFallingSpeed(DEFAULT_SPEED);
	m_nSpeedCount = 0;
	
	ResetGrades();

	m_keyState = 0;

	m_pGameData->MergeNextShapeToBoard();

	return true;
}

bool CTetris::RunGame()
{
	SetGameState(GS_RUNNING);
	return true;
}

bool CTetris::PauseGame()
{
	SetGameState(GS_PAUSE);
	return true;
}

bool CTetris::ResetGame()
{
	SetGameState(GS_READY);
	m_pGameData->ResetGameData();
	return true;
}

bool CTetris::ReleaseGame()
{
	SetGameState(GS_RUNNING);
	return true;
}

bool CTetris::ShutDownGame()
{
	return true;
}

void CTetris::HeartMsgProcess()
{
	m_nFallingSpeed = m_nLevelSpeed;

	if (m_gameState == GS_RUNNING)
	{
		ProcessKeyInput(KBV_DOWN);
		ProcessKeyInput(KBV_LEFT);
		ProcessKeyInput(KBV_RIGHT);
		ProcessKeyInput(KBV_UP);
		ProcessKeyInput(KBV_ACC);

		// normal fall down one row
		if (IsNeedFalling() && !FallingOneRow())
		{			
			IsGameOver();
		}

		// clear full lines
		int nClrLn = ClearLines();
		UpdateGrades(nClrLn);
	}
}

void CTetris::ProcessKeyInput(const int KBVALUE)
{
	if ((m_keyState & KBVALUE) == KBVALUE)
	{
		switch (KBVALUE)
		{
		case KBV_LEFT:	GetFallingShape()->MoveLeft(); break;
		case KBV_RIGHT: GetFallingShape()->MoveRight(); break;
		case KBV_UP:	GetFallingShape()->Translate(); break;
		case KBV_ACC:	FallingDownToBottom(); break;
		case KBV_DOWN:	m_nFallingSpeed = 3; return;
		}
		KEY_UP(m_keyState, KBVALUE);
	}
}

GameState CTetris::GetInputKey(int keyValue)
{
	int keyState = m_keyState;

	switch (keyValue)
	{
	case VK_UP:	KEY_DOWN(m_keyState, KBV_UP); break;
	case VK_DOWN: KEY_DOWN(m_keyState, KBV_DOWN); break;
	case VK_LEFT: KEY_DOWN(m_keyState, KBV_LEFT); break;
	case VK_RIGHT:KEY_DOWN(m_keyState, KBV_RIGHT); break;
	case VK_SPACE:KEY_DOWN(m_keyState, KBV_ACC); break;
	case 'G': break;// pause or continue
	default:
		break;
	}

	return m_gameState;
}

GameState CTetris::ReleaseKey(int keyValue)
{
	int keyState = m_keyState;

	switch (keyValue)
	{
	case VK_UP:	KEY_UP(m_keyState, KBV_UP); break;
	case VK_DOWN: KEY_UP(m_keyState, KBV_DOWN); break;
	case VK_LEFT: KEY_UP(m_keyState, KBV_LEFT); break;
	case VK_RIGHT:KEY_UP(m_keyState, KBV_RIGHT); break;
	case VK_SPACE:KEY_UP(m_keyState, KBV_ACC); break;
	case 'G': break; // pause or continue
	default:
		break;
	}

	return m_gameState;
}



GameState CTetris::GetGameState() const
{
	return m_gameState;
}

int CTetris::GetGameBoardWidth() const
{
	return MAX_GAMEBOARD_COLUMN;
}

int CTetris::GetGameBoardHeight() const
{
	return MAX_GAMEBOARD_ROW;
}

int CTetris::GetNextBoardWidth() const
{
	return MAX_NEXTBOARD_COLUMN;
}

int CTetris::GetNextBoardHeight() const
{
	return MAX_NEXTBOARD_ROW;
}

GAMEBOARD CTetris::GetGameBoard() const
{
	return m_pGameData->m_GameBoard;
}

NEXTBOARD CTetris::GetNextBoard() const
{
	return m_pGameData->m_NextBoard;
}

CShape* CTetris::GetFallingShape()
{
	return m_pGameData->GetCurrentShape();
}

bool CTetris::FallingOneRow()
{
	bool bMoved = true;
	CShape *pShape = GetFallingShape();
	if (pShape)
	{
		bMoved = pShape->FallDown();
	}

	if (!bMoved)
	{
		m_pGameData->MergeCurrentShapeToBoard();	// when falling down to bottom then merge the shape
		m_pGameData->ChangeShape();	// when the shape falling down to bottom, then change to next
		m_pGameData->MergeNextShapeToBoard();
	}

	return bMoved;
}

bool CTetris::FallingDownToBottom()
{
	bool bMoved = true;
	CShape *pShape = GetFallingShape();
	do 
	{
		bMoved = pShape->FallDown();
	} while (bMoved);

	m_pGameData->MergeCurrentShapeToBoard();	// when falling down to bottom then merge the shape
	m_pGameData->ChangeShape();					// when the shape falling down to bottom, then change to next
	m_pGameData->MergeNextShapeToBoard();

	return bMoved;
}

void CTetris::SetGameState(GameState gs)
{
	m_gameState = gs;
}

void CTetris::ResetFallingSpeed(const int nSpeed)
{
	m_nLevelSpeed = nSpeed;
}

bool CTetris::IsNeedFalling()
{
	bool bNeed = false;

	if ((++m_nSpeedCount % m_nFallingSpeed) == 0)
	{
		m_nSpeedCount = 0;
		bNeed = true;
	}

	return bNeed;
}

void CTetris::UpdateGrades(const int nCleardLines)
{
	m_nNowScore += (nCleardLines * nCleardLines * 100);

	m_nGameLevel = (m_nNowScore / 10000) % 10;

	m_nLevelSpeed = DEFAULT_SPEED - 3 * m_nGameLevel;	
}

int CTetris::ClearLines()
{
	return m_pGameData->ClearFullLines();
}

bool CTetris::IsGameOver()
{
	if (m_pGameData->IsTopLineFilled())
	{
		SetGameState(GS_OVER);
	}

	return (GS_OVER == GetGameState());
}

void CTetris::ResetGrades()
{
	m_nNowScore = 0;
	UpdateGrades(0);
}

CCell& CCell::operator=(const CCell& cell)
{
	if (this != &cell)
	{
		this->m_isFilled = cell.m_isFilled;
		this->m_cellColor = cell.m_cellColor;
		this->CopyRect(cell);
	}

	return *this;
}

CShapeState::CShapeState()
{
	memset(m_cellPoint, 0, sizeof(m_cellPoint));
}
