#include "MessageBoard.h"

MessageBoard * mBoard = 0;

MessageBoard::MessageBoard()
{

}

MessageBoard::~MessageBoard()
{

}

MessageBoard * MessageBoard::GetBoard()
{
	if (!mBoard)
		mBoard = new MessageBoard();

	return mBoard;
}

void MessageBoard::PostMMessage(Message theMessage)
{
	mMessages.push_back(theMessage);
}

Message MessageBoard::CollectMessage(MessageBoardMessages::iterator &CurPos)
{
	CurPos = mMessages.begin();

	if (CurPos == mMessages.end())
	{
		return Message(0, ET_NONE, 0);
	}

	return *mMessages.begin();
}

void MessageBoard::DismissMessage(MessageBoardMessages::iterator Pos)
{
	mMessages.erase(Pos);
}