#ifndef __TETRIS_H
#define __TETRIS_H

#include <map>
#include <iostream>
using namespace std;


#define	MAX_GAMEBOARD_COLUMN	11
#define MAX_GAMEBOARD_ROW		21
#define	MAX_NEXTBOARD_COLUMN	4
#define MAX_NEXTBOARD_ROW		MAX_NEXTBOARD_COLUMN
#define MAX_SHAPECOLOR_NUM		10
#define CELLCX	16
#define CELLCY	CELLCX
#define KEY_UP(keyState, keyBoardValue)			(keyState &= ~keyBoardValue)
#define KEY_DOWN(keyState, keyBoardValue)		(keyState |= keyBoardValue)

static const int KBV_LEFT = 0x00000001;
static const int KBV_RIGHT = 0x00000002;
static const int KBV_UP = 0x00000004;
static const int KBV_DOWN = 0x00000008;
static const int KBV_ACC = 0x00000010;

static enum GameState { GS_READY, GS_RUNNING, GS_PAUSE, GS_OVER };

class CCell : public CRect
{
public:
	CCell()
		: CRect(0, 0, 0, 0),
		m_isFilled(0),
		m_cellColor(0)
	{}
	CCell(LONG l, LONG t, LONG r, LONG b, int filled = 0, int color = 0)
		:CRect(l, t, r, b),
		m_isFilled(filled),
		m_cellColor(color)
	{}

	CCell& operator=(const CCell& cell);
public:
	int m_isFilled;		// 0-unFilled, 1-Filled, [0,1]
	int m_cellColor;	// color index used for filling, [0, 255]
};

typedef CCell **GAMEBOARD, **NEXTBOARD;

class CShapeState
{
public:
	CShapeState();
public:
	CPoint m_cellPoint[4];	// each shape state has 4 cells. x, y of each cellPoint is the offset to shape position
};

class CShape
{
public:
	CShape(GAMEBOARD gameBoard, const int stateCount);
	virtual ~CShape();

	enum MoveDirection {MD_LEFT, MD_RIGHT, MD_DOWN};

public:
	virtual bool InitShapeData();
	bool MoveLeft();
	bool MoveRight();
	bool FallDown();
	bool Translate();
	CShapeState *getCurrentState();
	CShapeState *getRandomState();
	CPoint	*getShapePosition();
	int  GetShapeColor() const;

protected:
	CShapeState	*getShapeState(const int stateKey);
	bool MoveOneCell(MoveDirection direction);
	bool ChangeState();

public:
	void ResetShape();
	
protected:
	map<int, CShapeState* >m_allStates;
	int			m_currentStateKey;
	CPoint		m_shapePosition;
	GAMEBOARD	g_gameBoard;
	const int	m_MAX_STATE_COUNT;
	int			m_shapeColor;			// shape color index [0,MAX_COLOR_NUM]
};

class CLineShape : public CShape
{
public:
	CLineShape(GAMEBOARD gameBoard, const int stateCount = 2);

public:
	bool InitShapeData();
};

class CSquareShape : public CShape
{
public:
	CSquareShape(GAMEBOARD gameBoard, const int stateCount = 1);
public:
	bool InitShapeData();
};

class CZ1Shape : public CShape
{
public:
	CZ1Shape(GAMEBOARD gameBoard, const int stateCount = 2);
public:
	bool InitShapeData();
};

class CLShape : public CShape
{
public:
	CLShape(GAMEBOARD gameBoard, const int stateCount = 4);
public:
	bool InitShapeData();
};

class CWShape : public CShape
{
public:
	CWShape(GAMEBOARD gameBoard, const int stateCount = 4);
public:
	bool InitShapeData();
};

class CGame
{
public:
	CGame();
	virtual ~CGame();

public:
	virtual bool InitGame() = 0;
	virtual bool RunGame() = 0;
	virtual bool ShutDownGame() = 0;
};

class CGameData
{
public:
	virtual bool InitGameData() = 0;
};

class CTetrisData : public CGameData
{
public:
	CTetrisData(const int shapeCount = 5);
	~CTetrisData();
public:
	bool InitGameData();
	bool ResetGameData();
	bool IsTopLineFilled();

protected:
	void InitGameBoard();
	void InitAllShapes();
	void InitCurrentNextShape();
	void ClearNextBoard();

public:
	void MergeCurrentShapeToBoard();
	void MergeNextShapeToBoard();
	void ChangeShape();
	CShape* GetCurrentShape();
	CShape* GetNextShape();
	int ClearFullLines();

public:
	GAMEBOARD   m_GameBoard;
	NEXTBOARD   m_NextBoard;
	map<int, CShape*>   m_AllShapes;
	const int   m_MAX_SHAPE_COUNT;
	int         m_currentShapeKey;
	int         m_nextShapeKey;
};

class CTetris : public CGame
{
public:
	CTetris();
	~CTetris();

	enum { DEFAULT_SPEED = 30 };

public:
	bool InitGame();	// initialize game first time
	bool RunGame();
	bool PauseGame();
	bool ResetGame();	// reset game before start
	bool ReleaseGame();	// continue game after paused
	bool ShutDownGame();

	void HeartMsgProcess();
	void ProcessKeyInput(const int KBVALUE);

	GameState GetInputKey(int keyValue);
	GameState ReleaseKey(int keyValue);

	GameState GetGameState() const;

	int GetGameBoardWidth() const;
	int GetGameBoardHeight() const;
	int GetNextBoardWidth() const;
	int GetNextBoardHeight() const;

	GAMEBOARD GetGameBoard() const;
	NEXTBOARD GetNextBoard() const;

public:
	CShape* GetFallingShape();
	bool FallingOneRow();
	bool FallingDownToBottom();
	int ClearLines();

protected:
	void SetGameState(GameState gs);
	bool IsGameOver();
	void ResetFallingSpeed(const int nSpeed);
	bool IsNeedFalling();
	void UpdateGrades(const int nCleardLines);	// update scores, level and speed
	void ResetGrades();

private:
	CTetrisData *m_pGameData;
	GameState	m_gameState;
	int		m_keyState;

	
	/* 
	 * m_nLevelSpeed: shape falling speed - count for timer interrupt times
	 * default = 30 * 10ms = 300ms, decrease with level increasing 
	*/
	int		m_nLevelSpeed;
	int		m_nFallingSpeed;
	int		m_nSpeedCount;	/*m_nSpeedCount: count for timer interrupt times*/
	int		m_nGameLevel;
	int		m_nNowScore;
};

#endif
