#pragma once
#include <Windows.h>
#include "../core/CoreDef.h"
class ENGINE_DLL GDIPlusManager
{
public:
	GDIPlusManager();
	~GDIPlusManager();
private:
	static ULONG_PTR token;
	static int refCount;
};