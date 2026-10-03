/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1997
 * Moscow Center for SPARC Technology
 *
 * Copyright (c) 1999 
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted 
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

/* Modified for this project: a focused excerpt of STLport 4.5.3.
 * Configuration/include scaffolding and unrelated templates are omitted.
 * _STLP_CALL is empty and _STLP_STD is _STL for the verified target ABI.
 * Member declarations, node fields and algorithms retain upstream spelling.
 * This is not a recovered complete original translation unit.
 */
#ifndef MOH_STLPORT_SWAP_H
#define MOH_STLPORT_SWAP_H
namespace _STL {
template <class _Tp>
inline void swap(_Tp& __a, _Tp& __b) {
  _Tp __tmp = __a;
  __a = __b;
  __b = __tmp;
}

}
#endif
