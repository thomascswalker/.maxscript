#pragma once

#include <stdlib.h>
#include <string>
#include <maxscript\foundation\arrays.h>
#include <qdebug.h>

Value* QVariantToMxsValue(const QVariant& variant);
Value* QVariantMapToMxsDict(const QVariantMap& variantMap);
Value* QVariantListToMxsArray(const QVariantList& variantList);

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

Value* QVariantToMxsValue(const QVariant& variant)
{
	Value* mxsValue = nullptr;

	switch (variant.type())
	{
	// Strings
	case (QMetaType::QString):
		qInfo() << "QString found.";
		mxsValue = new String(variant.toString());
		break;
	// Booleans
	case (QMetaType::Bool):
		qInfo() << "QBool found.";
		variant.toBool() ? mxsValue = &true_value : mxsValue = &false_value;
		break;
	// Integers (doubles)
	case (QMetaType::Double):
		qInfo() << "QDouble found.";
		mxsValue = new Integer(variant.toInt());
		break;
	// Arrays
	case (QMetaType::QStringList):
	{
		qInfo() << "QStringList found.";
		mxsValue = QVariantListToMxsArray(variant.toList());
		break;
	}
	// Dictionaries
	case (QMetaType::QVariantMap):
	{
		qInfo() << "QVariantMap found.";
		mxsValue = QVariantMapToMxsDict(variant.toMap());
		break;
	}
	// Default to a string if it can't be resolved
	// to a MAXScript value type
	default:
		break;
	}

	return mxsValue;
}

// Dict to dict mapper
Value* QVariantMapToMxsDict(const QVariantMap& variantMap)
{
	MXSDictionaryValue *mxsDict = new MXSDictionaryValue(MXSDictionaryValue::key_type::key_string);

    // Get all the keys in the current level
    QList<QString> keys = variantMap.keys();

    // For each key...
    for (QString key : keys)
    {
        // We'll create the holder variables for the data
        // key : value pair
        Value* pairKey = new String(key);

        // Now we'll get the type of Q variable the variant
        // is, then convert the corresponding variable
        // to its MAXScript type synonym
        QVariant curValue = variantMap.value(key);
		Value* mxsValue = QVariantToMxsValue(curValue);

		if (mxsValue)
		{
			mxsDict->put(pairKey, mxsValue);
		}
    }

	return mxsDict;
}

// List to array mapper
Value* QVariantListToMxsArray(const QVariantList& variantList)
{
	Array *mxsArray = new Array(variantList.count());
	for (QVariant variant : variantList)
	{
		Value* mxsValue = QVariantToMxsValue(variant);
		mxsArray->append(mxsValue);
	}

	return mxsArray;
}
