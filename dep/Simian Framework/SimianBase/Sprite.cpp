#include "Sprite.h"
#include "SimianBase.h"
#include "Image.h"
#include "Timer.h"

Sprite::Sprite(SimianBase * theApp, char * thePath) : RenderObject()
{
	Init();
	mApp = theApp;
	mImage = mApp->CreateImage(thePath);
	SyncSprite();
}

Sprite::Sprite(SimianBase * theApp, Image * theImage) : RenderObject()
{
	Init();
	mApp = theApp;
	mImage = theImage;
	SyncSprite();
}

Sprite::~Sprite()
{

}

void Sprite::Init()
{
	mImage = 0;
	mApp = 0;
	mRows = 0;
	mColumns = 0;
	mCellHeight = mCellWidth = 0;
	mSpeed = 0;
	mCurrentSequence = 0;
	curFrame = 0;
	mLoop = false;
	mCurTime = 0;
}

void Sprite::SetCellHeight(int Height)
{
	mCellHeight = Height;
	mRows = mHeight/Height;
}

void Sprite::SetCellWidth(int Width)
{
	mCellWidth = Width;
	mColumns = mWidth/Width;
}

void Sprite::SetCellSize(int Width, int Height)
{
	SetCellHeight(Height);
	SetCellWidth(Width);
}

void Sprite::SetRows(int theRows)
{
	mRows = theRows;
	mCellHeight = mActualHeight/theRows;
	if (!mHeight)
		mHeight = mCellHeight;
}

void Sprite::SetColumns(int theColumns)
{
	mColumns = theColumns;
	mCellWidth = mActualWidth/theColumns;
	if (!mWidth)
		mWidth = mCellWidth;
}

void Sprite::SetFrames(int theRows, int theColumns)
{
	SetRows(theRows);
	SetColumns(theColumns);
}

void Sprite::SyncSprite()
{
	//SetSize(mImage->GetWidth(), mImage->GetHeight());
	mActualWidth = mImage->GetActualWidth();
	mActualHeight = mImage->GetActualHeight();
	SetPosition(mImage->GetX(), mImage->GetY());
}

int Sprite::AddAnimation(int * theSequence, int Length)
{
	mSequences.resize(mCurrentSequence + 1);
	FrameSequence * a = new FrameSequence();
	a->Frames = new int[Length];
	for (int i = 0; i < Length; i ++)
		a->Frames[i] = theSequence[i];
	a->Length = Length;
	mSequences[mCurrentSequence] = a;
	mCurrentSequence ++;
	return mCurrentSequence - 1;
}

void Sprite::PlayAnimation(int theSequenceId, bool Loop)
{
	mLoop = Loop;
	mCurrentAnim = theSequenceId;
	curFrame = 0;
}

void Sprite::SetAnimationSpeed(int theMilliseconds)
{
	mSpeed = theMilliseconds;
}

void Sprite::SetAnimationSpeedf(float Seconds)
{
	mSpeed = (int)(Seconds * 1000.0f);
}

void Sprite::Render()
{
	mImage->SetPosition(GetX(), GetY());
	mImage->SetSize(GetWidth(), GetHeight());
	int FrameNumber = mSequences[mCurrentAnim]->Frames[curFrame];
	int srcx, srcy;
	if (FrameNumber < mColumns)
	{
		srcx = FrameNumber;
		srcy = 0;
	}
	else
	{
		srcx = FrameNumber%mColumns;
		srcy = FrameNumber/mColumns;
	}

	srcx = srcx * mCellWidth;
	srcy = srcy * mCellHeight;

	mImage->Render(srcx, srcy, mCellWidth, mCellHeight);
}

void Sprite::Render(float x, float y)
{
	mImage->SetPosition(GetX(), GetY());
	mImage->SetSize(GetWidth(), GetHeight());
	int FrameNumber = mSequences[mCurrentAnim]->Frames[curFrame];
	int srcx, srcy;
	if (FrameNumber < mColumns)
	{
		srcx = FrameNumber;
		srcy = 0;
	}
	else
	{
		srcx = FrameNumber%mColumns;
		srcy = FrameNumber/mColumns;
	}

	srcx = srcx * mCellWidth;
	srcy = srcy * mCellHeight;

	mImage->Render(x, y, srcx, srcy, mCellWidth, mCellHeight);
}

void Sprite::Update()
{
	int theTimeDelta;
	
	if (mApp->GetUpdateMSPF() > 0)
		theTimeDelta = mApp->mTimeDelta;
	else
		theTimeDelta = mApp->mTimer->GetDeltaTime();

	mCurTime += theTimeDelta;
	if (mCurTime > mSpeed)
	{
		curFrame ++;
		mCurTime = 0;

		if (curFrame == mSequences[mCurrentAnim]->Length)
		{
			if (mLoop)
				curFrame = 0;
			else
				curFrame --;
		}
	}
}