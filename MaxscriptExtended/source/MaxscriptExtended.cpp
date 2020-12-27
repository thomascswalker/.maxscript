//**************************************************************************/
// Copyright (c) 1998-2018 Autodesk, Inc.
// All rights reserved.
// 
// Use of this software is subject to the terms of the Autodesk license 
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "MaxscriptExtended.h"
#include <maxscript/maxscript.h>
#include <maxscript/foundation/strings.h>
#include <maxscript/foundation/numbers.h>
#include <maxscript/foundation/arrays.h>

#define MaxscriptExtended_CLASS_ID	Class_ID(0x3d840d3c, 0x3eb5f76d)

// Declare MAXScript functions
#include <maxscript\macros\define_instantiation_functions.h>
	def_visible_primitive(rFileIn, "rFileIn");

/*
 *  rFileIn
 *  Extends the standard 'fileIn' MAXScript function to support
 *  relative filepaths.
 *
 *  Example:
 *  rFileIn @"..\..\myFile.ms"
 */
Value* rFileIn_cf(Value **arg_list, int count)
{
	// <ok> relFileIn <FileName:String>
	check_arg_count(rFileIn, 1, count);
	MAXScript_TLS* _tls = (MAXScript_TLS*)TlsGetValue(thread_locals_index);

	// Get the current script filepath that's being run
	Value* pFilename = arg_list[0];
	MaxSDK::Util::Path fileInPath = pFilename->to_string();
	
	if (fileInPath.StartsWithUpDirectory() && _tls && _tls->source_file)
	{
		fileInPath.Normalize();

		// Strip filepath of leaves, add backslash, convert to absolute
		fileInPath = _tls->source_file->to_string();
		fileInPath.RemoveLeaf();
		fileInPath.AddTrailingBackslash();
		fileInPath.Append(arg_list[0]->to_string());
		fileInPath.Normalize();
	}
	
	// Macro for running the MAXScript function 'fileIn'
	filein_script(fileInPath.GetCStr());

	// Generic return
	return &ok;
}

class MaxscriptExtended : public GUP
{
public:
	//Constructor/Destructor
	MaxscriptExtended();
	virtual ~MaxscriptExtended();

	// GUP Methods
	virtual DWORD     Start();
	virtual void      Stop();
	virtual DWORD_PTR Control(DWORD parameter);
	virtual void      DeleteThis();

	// Loading/Saving
	virtual IOResult Save(ISave* isave);
	virtual IOResult Load(ILoad* iload);
};

class MaxscriptExtendedClassDesc : public ClassDesc2 
{
public:
	virtual int           IsPublic() override                       { return TRUE; }
	virtual void*         Create(BOOL /*loading = FALSE*/) override { return new MaxscriptExtended(); }
	virtual const TCHAR * ClassName() override                      { return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID     SuperClassID() override                   { return GUP_CLASS_ID; }
	virtual Class_ID      ClassID() override                        { return MaxscriptExtended_CLASS_ID; }
	virtual const TCHAR*  Category() override                       { return GetString(IDS_CATEGORY); }

	virtual const TCHAR*  InternalName() override                   { return _T("MaxscriptExtended"); } // Returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE     HInstance() override                      { return hInstance; } // Returns owning module handle


};

ClassDesc2* GetMaxscriptExtendedDesc()
{
	static MaxscriptExtendedClassDesc MaxscriptExtendedDesc;
	return &MaxscriptExtendedDesc; 
}

MaxscriptExtended::MaxscriptExtended()
{

}

MaxscriptExtended::~MaxscriptExtended()
{

}

void MaxscriptExtended::DeleteThis()
{
	delete this;
}

// Activate and Stay Resident
DWORD MaxscriptExtended::Start()
{
	#pragma message(TODO("Do plugin initialization here"))
	#pragma message(TODO("Return if you want remain loaded or not"))
	return GUPRESULT_KEEP;
}

void MaxscriptExtended::Stop()
{
	#pragma message(TODO("Do plugin un-initialization here"))
}

DWORD_PTR MaxscriptExtended::Control( DWORD /*parameter*/ )
{
	return 0;
}

IOResult MaxscriptExtended::Save(ISave* /*isave*/)
{
	return IO_OK;
}

IOResult MaxscriptExtended::Load(ILoad* /*iload*/)
{
	return IO_OK;
}

