/*
  Copyright (c) 2021, Moscow Center for Diagnostics & Telemedicine

  This is a modified version of the QtWebApp software.
  The original license terms (GNU LGPLv3) are effective. See copyright.txt.
*/
/**
  @file
  @author Stefan Frings
*/

#ifndef HTTPGLOBAL_H
#define HTTPGLOBAL_H

#include <QtCore/QtGlobal>

// This is specific to Windows dll's
#if defined(Q_OS_WIN)
    #if defined(QTWEBAPPLIB_EXPORT)
        #define DECLSPEC Q_DECL_EXPORT
    #elif defined(QTWEBAPPLIB_IMPORT)
        #define DECLSPEC Q_DECL_IMPORT
    #endif
#endif
#if !defined(DECLSPEC)
    #define DECLSPEC
#endif

/** Get the library version number */
DECLSPEC const char* getQtWebAppLibVersion();


#endif // HTTPGLOBAL_H

