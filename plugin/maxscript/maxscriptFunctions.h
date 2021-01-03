#ifndef MXSEXT_MXSFUNCTIONS_H
#define MXSEXT_MXSFUNCTIONS_H

#include <maxscript\maxscriptHelpers.h>

// Define custom Maxscript argument names
#define n_pretty (Name::intern( _T("pretty")))

Value* rFileIn_cf(Value **arg_list, int count);
Value* readJson_cf(Value **arg_list, int count);
Value* writeJson_cf(Value **arg_list, int count);

#endif
#pragma once