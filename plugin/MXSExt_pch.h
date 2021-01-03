//**************************************************************************/
// Copyright (c) 1998-2005 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Include this file before including any 3ds Max SDK files. It 
//              define the macros required to add linkable todo compile-time 
//              messages. Therefore if you use this TODO macro, it will emit
//              a message that you can click on, and visual studio will open
//              the file and line where the message is.
// AUTHOR: Jean-Francois Yelle - created Mar.20.2007
//***************************************************************************/

// useful for #pragma message
#define STRING2(x) #x
#define STRING(x) STRING2(x)
#define TODO(x) __FILE__ "(" STRING(__LINE__) "): TODO: "x

// FREE_BUILD should be checked in as 0.
#ifndef FREE_BUILD
#define FREE_BUILD 0
#endif // FREE_BUILD

#pragma warning(disable : 4100) // disable 'unreferenced parameter'
#pragma warning(disable : 4718) // disable 'recursive call has no side effects, deleting'
#pragma warning(disable : 4800) // disable 'forcing value to bool 'true' or 'false''

// STL Headers
#include <array>
#include <condition_variable>
#include <deque>
#include <functional>
#include <thread>
#include <sstream>
#include <vector>
#include <string>
#include <stdlib.h>


// 3ds Max Headers
#pragma warning(push)  
#pragma warning(disable : 4702)
#include <maxscript\kernel\value.h>
#include <maxscript\foundation\arrays.h>
#include <maxscript\foundation\strings.h>
#include <maxscript/foundation/numbers.h>
#include <maxscript\foundation\MXSDictionaryValue.h>
#include <maxscript\foundation\DataPair.h>
#include <maxscript/macros/generic_class.h>
#include <maxscript/maxwrapper/mxsobjects.h>
#include <maxscript/maxscript.h>

#include <maxscript\macros\define_instantiation_functions.h>
#include <maxscript\macros\define_implementations.h>

#pragma warning(pop)

#include <Qt/QmaxDockWidget.h>

// QT Headers
// Qt headers commonly generate the following warnings. As such, we provide
// an easy way to disable these warnings for any file that needs to include
// Qt headers. Standard Qt headers are all included here, but some generated 
// headers are included directly by the related .cpp files (for example, those 
// generated to represent UIs built in Qt Designer).
#define BEGIN_QT_HEADERS \
    __pragma(warning(push)) \
    __pragma(warning(disable: 4251)) /* needs to have dll-interface to be used */

#define END_QT_HEADERS \
    __pragma(warning(pop))

// Standard Qt headers
BEGIN_QT_HEADERS
#include <QAbstractNativeEventFilter>
#include <QAction>
#include <QApplication>
#include <QBoxLayout>
#include <QClipboard>
#include <QDebug>
#include <QDesktopServices>
#include <QDialog>
#include <QDir>
#include <QFileInfo>
#include <qglobal.h>
#include <QHttpMultiPart>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QMenu>
#include <QMessagebox>
#include <QModelIndex>
#include <QModelIndexList>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkCookieJar>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QProcess>
#include <QSortFilterProxyModel>
#include <QString>
#include <QThread>
#include <QTimer>
#include <QUrl>
#include <QVector>
#include <QWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariant>
#include <QHash>
#include <QHashFunctions>
END_QT_HEADERS

// Universal helper functions
template <class C>
inline void SafeDelete(C*& p)
{
	delete p;
	p = nullptr;
}

template <class C>
inline void SafeDeleteArray(C*& p)
{
	delete[] p;
	p = nullptr;
}

// Use this to selectively suppress unused variable warnings.
#define UNUSED(x) (void)x
