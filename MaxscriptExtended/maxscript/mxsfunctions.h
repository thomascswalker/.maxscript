#ifndef MXSEXT_MXSFUNCTIONS_H
#define MXSEXT_MXSFUNCTIONS_H

#include <helpers.h>

// Define custom Maxscript argument names
#define n_pretty (Name::intern( _T("pretty")))

Value* rFileIn_cf(Value **arg_list, int count);
Value* readJson_cf(Value **arg_list, int count);

#endif
#pragma once