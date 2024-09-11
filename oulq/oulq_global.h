#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(OULQ_LIB)
#  define OULQ_EXPORT Q_DECL_EXPORT
# else
#  define OULQ_EXPORT Q_DECL_IMPORT
# endif
#else
# define OULQ_EXPORT
#endif
