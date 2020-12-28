#pragma once

//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Includes for Plugins
// AUTHOR: 
//***************************************************************************/

// 3DS Max includes
#include <guplib.h>
#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h>
#include <maxtypes.h>

extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;

class MXSExtUtil : public GUP
{
public:

	//Constructor/Destructor
	MXSExtUtil();
	virtual ~MXSExtUtil();

	// GUP Methods
	virtual DWORD     Start();
	virtual void      Stop();
	virtual DWORD_PTR Control(DWORD parameter);
	virtual void      DeleteThis();

	// Singleton access
	static MXSExtUtil* GetInstance() {
		static MXSExtUtil theMXSExtUtil;
		return &theMXSExtUtil;
	}

private:

};

class MXSExtUtilClassDesc : public ClassDesc2
{
public:
	virtual int IsPublic() { return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) { return new MXSExtUtil(); }
	virtual const TCHAR *ClassName();
	virtual SClass_ID SuperClassID();
	virtual Class_ID ClassID();
	virtual const TCHAR* Category();

	virtual const TCHAR* InternalName() { return _T("MXSExtUtil"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() { return hInstance; }				// returns owning module handle

};

ClassDesc2* GetMXSExtUtilClassDesc();
