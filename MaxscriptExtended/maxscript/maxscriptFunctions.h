#pragma once

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