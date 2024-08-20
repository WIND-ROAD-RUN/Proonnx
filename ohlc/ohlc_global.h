#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(OHLC_LIB)
#  define OHLC_EXPORT Q_DECL_EXPORT
# else
#  define OHLC_EXPORT Q_DECL_IMPORT
# endif
#else
# define OHLC_EXPORT
#endif
