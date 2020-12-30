#include <maxscript\macros\define_instantiation_functions.h>

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
	MaxSDK::Util::Path fileInPath = pFilename->to_string();

	MAXScript_TLS* _tls = (MAXScript_TLS*)TlsGetValue(thread_locals_index);
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
	QString filename = QString::fromStdWString(pFilename->to_string());

	// Read the .json file
	QFile file;
	file.setFileName(filename);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QString contents = file.readAll();
	file.close();

	// Convert the JSON format to a string
	QJsonDocument doc = QJsonDocument::fromJson(contents.toUtf8());
	QString docStr = QString::fromStdString(doc.toJson(format).toStdString());

	// Convert the output string the a Max value type
	MAXScript_TLS* _tls = (MAXScript_TLS*)TlsGetValue(thread_locals_index);
	one_typed_value_local_tls(String* rString);
	vl.rString = new String(docStr);

	// Return the Max value string
	return_value_tls(vl.rString);
}
