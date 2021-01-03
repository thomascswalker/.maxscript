#ifndef MXSEXT_MXSHELPERS_H
#define MXSEXT_MXSHELPERS_H

MaxSDK::Util::Path ToAbsFilename(Value* pFilename);
Value* QVariantToMxsValue(const QVariant& variant);
Value* QVariantMapToMxsDict(const QVariantMap& variantMap);
Value* QVariantListToMxsArray(const QVariantList& variantList);

#endif
#pragma once