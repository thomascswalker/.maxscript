#pragma once

#include <stdlib.h>
#include <string>

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

void QJsonToMxsDict(const QVariantMap& jsonMap, MXSDictionaryValue* mxsDict)
{
	// Get all the keys in the current level
	QList<QString> keys = jsonMap.keys();

	// For each key...
	for (QString key : keys)
	{
		// We'll create the holder variables for the data
		// key : value pair
		Value* pairKey = new String(key);
		Value* pairValue = nullptr;

		// Now we'll get the type of Q variable the variant
		// is, then convert the corresponding variable
		// to its MAXScript type synonym
		QVariant curValue = jsonMap.value(key);
		switch (curValue.type())
		{
			// Strings
			case (QMetaType::QString):
				pairValue = new String(curValue.toString());
				break;
			// Booleans
			case (QMetaType::Bool):
				curValue.toBool() ? pairValue = &true_value : pairValue = &false_value;
				break;
			// Integers
			case (QMetaType::Int):
				pairValue = new Integer(curValue.toInt());
				break;
			// Arrays
			case (QMetaType::QStringList):
				//QList<QVariant> curList = curValue.toList();
				break;
			// Dictionaries
			case (QMetaType::QVariantMap):
				QJsonToMxsDict(curValue.toMap(), mxsDict);
				break;
		}

		// Forcibly override (for now) because the above switch/case produces a broken result.
		pairValue = new String(curValue.toString());
		
		// Finally we'll use the 'put' function to add the key
		// value pair to the dictionary
		mxsDict->put(pairKey, pairValue);
	}
}