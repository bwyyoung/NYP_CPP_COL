#pragma once
#include "Entity.h"
#include <string>
#include <vector>

using namespace std;

struct Message
{
	Message()
	{
		Poster = 0;
		TargetType = ET_NONE;
		mMessage = "null";
		Target = 0;
	}

	Message(Entity * thePoster, Entity_Type a, Entity * b)
	{
		Poster = thePoster;
		TargetType = a;
		Target = b;
	}

	Entity_Type TargetType;
	string mMessage;
	Entity * Poster;
	Entity * Target;
};

typedef vector<Message> MessageBoardMessages;

class MessageBoard
{
public:
	MessageBoard();
	~MessageBoard();

	static MessageBoard * GetBoard();

	void PostMMessage(Message theMessage);
	Message CollectMessage(MessageBoardMessages::iterator &CurPos);
	void DismissMessage(MessageBoardMessages::iterator Pos);
protected:
	MessageBoardMessages mMessages;
};