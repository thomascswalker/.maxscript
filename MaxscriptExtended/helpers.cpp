#include "MXSExt_pch.h"
#include <helpers.h>

MaxSDK::Util::Path ToAbsFilename(Value* pFilename)
{
	MaxSDK::Util::Path filename = pFilename->to_string();

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
	default:
	case (QMetaType::QString):
		mxsValue = new String(variant.toString());
		break;
		// Booleans
	case (QMetaType::Bool):
		variant.toBool() ? mxsValue = &true_value : mxsValue = &false_value;
		break;
		// Integers (doubles)
	case (QMetaType::Int):
	case (QMetaType::Double):
		mxsValue = new Integer(variant.toInt());
		break;
		// Floats
	case (QMetaType::Float):
		mxsValue = new Float(variant.toFloat());
		break;
		// Arrays
	case (QMetaType::QVariantList):
	{
		mxsValue = QVariantListToMxsArray(variant.toList());
		break;
	}
	// Dictionaries
	case (QMetaType::QVariantMap):
	{
		mxsValue = QVariantMapToMxsDict(variant.toMap());
		break;
	}
	}

	return mxsValue;
}

// JSON associative array to MAXScript Dictionary mapper
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

// JSON list to MAXScript Array mapper
Value* QVariantListToMxsArray(const QVariantList& variantList)
{
	Array *mxsArray = new Array(0);
	for (QVariant variant : variantList)
	{
		Value* mxsValue = QVariantToMxsValue(variant);
		mxsArray->append(mxsValue);
	}

	return mxsArray;
}
