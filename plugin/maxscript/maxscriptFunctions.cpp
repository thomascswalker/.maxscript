#include "MXSExt_pch.h"
#include <maxscript\maxscriptFunctions.h>

/*
 Extends the standard 'fileIn' MAXScript function to support
 relative file paths.
 
 @param <string> filepath - The path of the file to include. This can be written with relative pathing in mind (..\..\myFile.ms).
 @returns <OK>
 */
Value* rFileIn_cf(Value **arg_list, int count)
{
	// <ok> rFileIn <FileName:String>
	check_arg_count(rFileIn, 1, count);

	// Get the current script filepath that's being run
	Value* pFilename = arg_list[0];

	// Check to see if the filename arg is a string
	if (!(is_string(pFilename)))
	{
		throw RuntimeError(_T("Expected a string for the first argument, in function RFileIn."));
	}

	// Convert relative filename to absolute filename
	filein_script(ToAbsFilename(pFilename).GetCStr());

	// Generic return
	return &ok;
}

/*
Reads the provided .json file, either as a single line (no whitespace)
or with indents.

@param <string> filepath - The path of the file to read. This can be written with relative pathing in mind (..\..\myFile.ms).
@param <bool> pretty - Whether to format the returned string with indents (true) or not (false).
@returns <DictionaryValue> - The contents of the .json file.
*/
Value* readJson_cf(Value **arg_list, int count)
{
	check_arg_count_with_keys(readJson, 1, count);

	// Get the filename argument
	Value* pFilename = arg_list[0];

	// Get the optional 'pretty' argument, defining the QJsonDocument formatting style
	// Pretty:true = Indented
	// Pretty:false = Compact
	Value* tmp;
	BOOL pPretty = bool_key_arg(pretty, tmp, false);
	QJsonDocument::JsonFormat format = pPretty ? QJsonDocument::Indented : QJsonDocument::Compact;

	// Check to see if the filename arg is a string
	if (!(is_string(pFilename)))
	{
		throw RuntimeError(_T("Expected a string for the first argument, in function ReadJson."));
	}

	// Get the filename from the first argument
	// Also supports relative file pathing
	const wchar_t* absFilename = ToAbsFilename(pFilename).GetCStr();
	QString filename = QString::fromStdWString(absFilename);

	// Read the .json file
	QFile file;
	file.setFileName(filename);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QString contents = file.readAll();
	file.close();

	// Convert the JSON format to a string
	QJsonDocument jsonDoc = QJsonDocument::fromJson(contents.toUtf8());
	if (jsonDoc.isNull() | !jsonDoc.isObject())
	{
		throw RuntimeError(_T("Failed to read .json file."));
	}

	// Get the JSON object
	QJsonObject jsonObj = jsonDoc.object();
	if (jsonObj.isEmpty())
	{
		throw RuntimeError(_T("QJSONObject is empty."));
	}

	// Convert the JSON dictionary to a mapped QVariant, effectively
	// a Qt Dictionary
	const QVariantMap variantMap = jsonObj.toVariantMap();

	// Convert the QVariantMap to a MAXScript Dictionary value
	Value* mxsDict = QVariantMapToMxsDict(variantMap);

	// Return the dictionary
	return mxsDict;
}

/*
Writes the provided MAXScript dictionary to the file path specified.

@returns <OK>
*/
Value* writeJson_cf(Value **arg_list, int count)
{
	return &ok;
}