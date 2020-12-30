#pragma once

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
