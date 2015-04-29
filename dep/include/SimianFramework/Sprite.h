#pragma once
#pragma warning(disable : 4251)

#include "Macros.h"
#include "renderobject.h"
#include <Vector>

class Image;
class SimianBase;

using namespace std;

class Timer;

struct SIM_EXP FrameSequence
{
	int * Frames;
	int Length;
};

class SIM_EXP Sprite : public RenderObject
{
public:
	Sprite(SimianBase * theApp, Image * theImage);
	Sprite(SimianBase * theApp, char * thePath);
	~Sprite();

	void SetRows(int theRows);
	void SetColumns(int theColumns);
	void SetFrames(int theRows, int theColumns);
	void SetCellWidth(int Width);
	void SetCellHeight(int Height);
	void SetCellSize(int Width, int Height);

	int GetRows();
	int GetColumns();
	int GetCellWidth();
	int GetCellHeight();

	int AddAnimation(int * theSequence, int Length);
	void PlayAnimation(int theSequenceId, bool Loop = true);
	void SetAnimationSpeed(int theMilliseconds);
	void SetAnimationSpeedf(float Seconds);

	virtual void Render();
	virtual void Render(float x, float y);
	virtual void Update();

	friend class BitmappedFont;
protected:
	void Init();
	void SyncSprite();

	Image * mImage;
	SimianBase * mApp;
	int mRows;
	int mColumns;
	int mCellWidth, mCellHeight;
	int mActualWidth, mActualHeight;

	int mSpeed;
	vector<FrameSequence *> mSequences;
	int mCurrentSequence;
	bool mLoop;

	int mCurrentAnim;
	int curFrame;
	int mCurTime;
};