#include "ImageFormat.h"

class SIM_EXP PNGFormat : public ImageFormat
{
public:
	PNGFormat();
	PNGFormat(char * thePath);
	~PNGFormat();

	virtual bool Load(char * thePath);
};