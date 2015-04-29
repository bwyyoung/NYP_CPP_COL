#pragma once
#include "ImageFormat.h"

class SIM_EXP GIFFormat : public ImageFormat
{
public:
	GIFFormat();
	~GIFFormat();
	GIFFormat(char * thePath);

	virtual bool Load(char * thePath);
};