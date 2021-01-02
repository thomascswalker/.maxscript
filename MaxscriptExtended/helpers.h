#pragma once
#include <maxscript/foundation/MXSDictionaryValue.h>

using namespace MaxSDK::Util;

Path ToAbsFilename(Value* pFilename)
{
	Path filename = pFilename->to_string();

	// Get the current 
	MAXScript_TLS* _tls = (MAXScript_TLS*)TlsGetValue(thread_locals_index);
	if (filename.StartsWithUpDirectory() && _tls && _tls->source_file)
	{
		filename.Normalize();

		// Strip filepath of leaves, add backslash, convert to absolute
		filename = _tls->source_file->to_string();
		filename.RemoveLeaf();
		filename.AddTrailingBackslash();
		filename.Append(pFilename->to_string());
		filename.Normalize();
	}

	return filename;
}

MXSDictionaryValue* QJsonToMxsDict(QVariantMap* jsonMap, MXSDictionaryValue* mxsDict)
{
	QList<QString> keys = jsonMap->keys();

	foreach(QString key, keys)
	{
		// Here is where we copy the QJsonMap/Hash to the MXS Dictionary value
		Value* pairKey = new String(key);
		Value* pairValue = new String(key);
		
		mxsDict->put(pairKey, pairValue);
	}

	return mxsDict;
}