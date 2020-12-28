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

#define MXSExtUtilClassDesc_CLASS_ID	Class_ID(0x75723824, 0x68792b2a)

#include <maxscript\macros\define_instantiation_functions.h>
	def_visible_primitive(rFileIn, "rFileIn");
	def_visible_primitive(readJson, "readJson");

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

Value* readJson_cf(Value **arg_list, int count)
{
	check_arg_count(readJson, 1, count);
	QString file = QString::fromStdWString(arg_list[0]->to_string());
	QJsonDocument::JsonFormat format = QJsonDocument::Indented;
	QJsonDocument jsonDoc;

	QFile loadFile(file);
	QByteArray readData = loadFile.readAll();
	loadFile.close();

	QJsonParseError error;
	jsonDoc = QJsonDocument::fromJson(readData, &error);

	QString output = QString::fromStdString(jsonDoc.toJson(format).toStdString());
	return new String(output.toStdWString().c_str());
}

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
