#ifndef MXSEXT_HELPERS_H
#define MXSEXT_HELPERS_H

MaxSDK::Util::Path ToAbsFilename(Value* pFilename);
Value* QVariantToMxsValue(const QVariant& variant);
Value* QVariantMapToMxsDict(const QVariantMap& variantMap);
Value* QVariantListToMxsArray(const QVariantList& variantList);

#endif
#pragma once