#pragma once
#include "GerberRender.h"
class CGerberRenderPNG :
	public CGerberRender
{
public:
	CGerberRenderPNG();
	virtual ~CGerberRenderPNG();
	// Implement this to test that produced image looks as expected
};

