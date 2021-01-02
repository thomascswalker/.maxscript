#include <maxscript\kernel\value.h>
#include <maxscript\foundation\MXSDictionaryValue.h>
#include <maxscript\foundation\DataPair.h>
#include <maxscript\macros\define_instantiation_functions.h>
#include <maxscript\macros\define_implementations.h>
#include <helpers.h>

// Define custom maxscript argument names
#define n_pretty (Name::intern( _T("pretty")))

/**
 * Extends the standard 'fileIn' MAXScript function to support
 * relative filepaths.
 *
 * @param arg_list The function arguments.
 * @returns OK
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

/**
 * Reads the provided .json file, either as a single line (no whitespace)
 * or with indents.
 *
 * @param arg_list The function arguments.
 * @param pretty Whether to format the returned string with indents (true)
 *               or not (false).
 * @returns MAXValueString The contents of the .json file.
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
	QVariantHash jsonMap = jsonObj.toVariantHash();
	MXSDictionaryValue jsonMxsDict = MXSDictionaryValue(MXSDictionaryValue::key_type::key_string);

	QList<QString> keys = jsonMap.keys();
	//while ()

	//QHash::const_iterator i = jsonMap.constBegin();
	//Value* jsonMapValue = Value::to_bitarray(jsonMap.keys());
	//to_bitarray(jsonMap.keys());
	//jsonMxsDict.set_keys(jsonMap.keys(), 1);

	MAXScript_TLS* _tls = (MAXScript_TLS*)TlsGetValue(thread_locals_index);
	one_typed_value_local_tls(MXSDictionaryValue* rDict);
	vl.rDict = new MXSDictionaryValue(MXSDictionaryValue::key_type::key_string);

	// Here is where we copy the QJsonMap/Hash to the MXS Dictionary value
	Value* key = new String(_T("testKey"));
	Value* value = new String(_T("testValue"));

	vl.rDict->put(key, value);

	// Return the dictionary
	return_value_tls(vl.rDict);


	//// deep copy hash table from qvarianthash?

	//QString jsonStr = QString::fromStdString(jsonDoc.toJson(format).toStdString());

	//// Convert the output string the a Max value type

	//MAXScript_TLS* _tls = (MAXScript_TLS*)TlsGetValue(thread_locals_index);
	//one_typed_value_local_tls(String* rString);
	//vl.rString = new String(jsonStr);

	//// Return the Max value string
	//return_value_tls(vl.rString);
}
