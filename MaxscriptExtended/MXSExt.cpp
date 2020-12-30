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
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "MXSExt_pch.h"
#include "MXSExt.h"
#include "resource.h"
#include "maxscript/mxsfunctions.h"

#define MXSExtUtilClassDesc_CLASS_ID	Class_ID(0x75723824, 0x68792b2a)

// Define Maxscript functions
#include <maxscript\macros\define_instantiation_functions.h>
	def_visible_primitive(rFileIn, "RFileIn");
	def_visible_primitive(readJson, "ReadJson");

ClassDesc2* GetMXSExtUtilClassDesc() {
	static MXSExtUtilClassDesc MXSExtUtilDesc;
	return &MXSExtUtilDesc;
}

//
// MXSExtUtil Class Description
//
Class_ID MXSExtUtilClassDesc::ClassID()
{
	return MXSExtUtilClassDesc_CLASS_ID;
}

const TCHAR* MXSExtUtilClassDesc::ClassName()
{
	return GetString(IDS_CLASS_NAME);
}

SClass_ID MXSExtUtilClassDesc::SuperClassID()
{
	return GUP_CLASS_ID;
}

const TCHAR* MXSExtUtilClassDesc::Category()
{
	return GetString(IDS_CATEGORY);
}

//
//  MXSExtUtil 
//
MXSExtUtil::MXSExtUtil()
{
}

MXSExtUtil::~MXSExtUtil()
{
}

void MXSExtUtil::DeleteThis()
{
	delete this;
}

// Activate and Stay Resident
DWORD MXSExtUtil::Start()
{
	DebugPrint(_T("Simple Utilities Started\n"));
	return GUPRESULT_KEEP;
}

void MXSExtUtil::Stop()
{
	
}

DWORD_PTR MXSExtUtil::Control( DWORD /*parameter*/ )
{
	return 0;
}
