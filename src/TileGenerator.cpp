#include "TileGenerator.h"
#include "BaseApp.h"
#include "Image.h"
#include "Macs.h"

TileGenerator::TileGenerator(BaseApp * theApp) : RenderObject()
{
	mTileSheet = 0;
	mTiles = 0;
	mApp = theApp;
}

TileGenerator::~TileGenerator()
{
	if (mTiles)
	{
		delete mTiles;
		mTiles = 0;
	}

	SAFE_DELETE(mTileSheet);
}

void TileGenerator::LoadTiles(char *ImagePath)
{
	//mTileSheet = mApp->CreateImage(ImagePath, "Tiles");
	mTileSheet = mApp->CreateImage(ImagePath);
	if (!mTileSheet)
		mApp->ShowFatalError("Tile Sheet loading failed.");
}

void TileGenerator::Render(float x, float y)
{

}

void TileGenerator::Render()
{
	for (int i = 0; i < TileHeight; i ++)
		for (int j = 0; j < TileWidth; j ++)
		{
			int mTileNumber = mTiles[i][j];
			int srcx, srcy;
			if (mTileNumber < 6)
			{
				srcx = mTileNumber;
				srcy = 0;
			}
			else
			{
				srcx = mTileNumber%6;
				srcy = mTileNumber/6;
			}
			mTileSheet->SetPosition((float)j * 32 + mX, (float)i * 32 + mY);
			mTileSheet->SetSize(32, 32);
			mTileSheet->Render(srcx * 16, srcy * 16, 16, 16);
		}
}

void TileGenerator::Update()
{

}

#define _TC(a,b) mTiles[i b][j a]

void TileGenerator::GenerateTiles(int SizeX, int SizeY)
{
	mWidth = SizeX * 32;
	mHeight = SizeY * 32;
	TileWidth = SizeX;
	TileHeight = SizeY;
	mTiles = new int *[SizeY];

	for (int i = 0; i < SizeY; i ++)
	{
		mTiles[i] = new int[SizeX];
		for (int j = 0; j < SizeX; j ++)
			mTiles[i][j] = rand()%2;
	}

	//blend the tiles..
	for (int i = 0; i < SizeY; i ++)
		for (int j = 0; j < SizeX; j ++)
		{
			if (mTiles[i][j] == 1)
			{
				mTiles[i][j] = 16;
				bool up, down, left, right;

				if (i == 0)
					up = true;
				else
				{
					if (mTiles[i - 1][j] > 0)
						up = true;
					else
						up = false;
				}

				if (i == SizeY - 1)
					down = true;
				else
				{
					if (mTiles[i + 1][j] > 0)
						down = true;
					else
						down = false;
				}

				if (j == 0)
					left = true;
				else
				{
					if (mTiles[i][j - 1] > 0)
						left = true;
					else
						left = false;
				}

				if (j == SizeX - 1)
					right = true;
				else
				{
					if (mTiles[i][j + 1] > 0)
						right = true;
					else
						right = false;
				}

				if (down && right && !up && !left)
					mTiles[i][j] = 9;
				if (down && left && !up && !right)
					mTiles[i][j] = 11;
				if (up && right && !down && !left)
					mTiles[i][j] = 21;
				if (up && left && !down && !right)
					mTiles[i][j] = 23;
				if (up && down && left && !right)
					mTiles[i][j] = 17;
				if (up && down && !left && right)
					mTiles[i][j] = 15;
				if (!up && down && left && right)
					mTiles[i][j] = 10;
				if (up && !down && left && right)
					mTiles[i][j] = 22;
				if (up && down && !left && !right)
					mTiles[i][j] = 5;
				if (!up && !down && left && right)
					mTiles[i][j] = 5;
				if (!up && !down && !left && right)
					mTiles[i][j] = 5;
				if (!up && !down && left && !right)
					mTiles[i][j] = 5;
				if (!up && down && !left && !right)
					mTiles[i][j] = 5;
				if (up && !down && !left && !right)
					mTiles[i][j] = 5;
				if (!up && !down && !left && !right)
					mTiles[i][j] = 5;
			}
		}
}