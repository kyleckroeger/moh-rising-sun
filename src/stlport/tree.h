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
#ifndef MOH_STLPORT_TREE_H
#define MOH_STLPORT_TREE_H

namespace _STL {

typedef bool _Rb_tree_Color_type;
//const _Rb_tree_Color_type _S_rb_tree_red = false;
//const _Rb_tree_Color_type _S_rb_tree_black = true;

#define _S_rb_tree_red false
#define _S_rb_tree_black true

struct _Rb_tree_node_base
{
  typedef _Rb_tree_Color_type _Color_type;
  typedef _Rb_tree_node_base* _Base_ptr;

  _Color_type _M_color; 
  _Base_ptr _M_parent;
  _Base_ptr _M_left;
  _Base_ptr _M_right;

  static _Base_ptr  _S_minimum(_Base_ptr __x)
  {
    while (__x->_M_left != 0) __x = __x->_M_left;
    return __x;
  }

  static _Base_ptr  _S_maximum(_Base_ptr __x)
  {
    while (__x->_M_right != 0) __x = __x->_M_right;
    return __x;
  }
};

template <class _Dummy> class _Rb_global {
public:
  typedef _Rb_tree_node_base* _Base_ptr;
  // those used to be global functions 
  static void  _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);
  static _Rb_tree_node_base*  _Rebalance_for_erase(_Rb_tree_node_base* __z,
                                                             _Rb_tree_node_base*& __root,
                                                             _Rb_tree_node_base*& __leftmost,
                                                             _Rb_tree_node_base*& __rightmost);
  // those are from _Rb_tree_base_iterator - moved here to reduce code bloat
  // moved here to reduce code bloat without templatizing _Rb_tree_base_iterator
  static _Rb_tree_node_base*   _M_increment(_Rb_tree_node_base*);
  static _Rb_tree_node_base*   _M_decrement(_Rb_tree_node_base*);
  static void  _Rotate_left(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);
  static void  _Rotate_right(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root); 
};

}

#endif
