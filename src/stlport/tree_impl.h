/*
 *
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
 * Modified CRP 7/10/00 for improved conformance / efficiency on insert_unique /
 * insert_equal with valid hint -- efficiency is improved all around, and it is
 * should now be standard conforming for complexity on insert point immediately
 * after hint (amortized constant time).
 *
 */
/* Modified for this project: a focused excerpt of STLport 4.5.3.
 * Configuration/include scaffolding and unrelated templates are omitted.
 * _STLP_CALL is empty and _STLP_STD is _STL for the verified target ABI.
 * Member declarations, node fields and algorithms retain upstream spelling.
 * This is not a recovered complete original translation unit.
 */
#ifndef MOH_STLPORT_TREE_IMPL_H
#define MOH_STLPORT_TREE_IMPL_H
#include "tree.h"
#include "swap.h"

namespace _STL {

template <class _Dummy> void 
_Rb_global<_Dummy>::_Rotate_left(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
  _Rb_tree_node_base* __y = __x->_M_right;
  __x->_M_right = __y->_M_left;
  if (__y->_M_left !=0)
    __y->_M_left->_M_parent = __x;
  __y->_M_parent = __x->_M_parent;

  if (__x == __root)
    __root = __y;
  else if (__x == __x->_M_parent->_M_left)
    __x->_M_parent->_M_left = __y;
  else
    __x->_M_parent->_M_right = __y;
  __y->_M_left = __x;
  __x->_M_parent = __y;
}

template <class _Dummy> void  
_Rb_global<_Dummy>::_Rotate_right(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
  _Rb_tree_node_base* __y = __x->_M_left;
  __x->_M_left = __y->_M_right;
  if (__y->_M_right != 0)
    __y->_M_right->_M_parent = __x;
  __y->_M_parent = __x->_M_parent;

  if (__x == __root)
    __root = __y;
  else if (__x == __x->_M_parent->_M_right)
    __x->_M_parent->_M_right = __y;
  else
    __x->_M_parent->_M_left = __y;
  __y->_M_right = __x;
  __x->_M_parent = __y;
}

template <class _Dummy> void 
_Rb_global<_Dummy>::_Rebalance(_Rb_tree_node_base* __x, 
			       _Rb_tree_node_base*& __root)
{
  __x->_M_color = _S_rb_tree_red;
  while (__x != __root && __x->_M_parent->_M_color == _S_rb_tree_red) {
    if (__x->_M_parent == __x->_M_parent->_M_parent->_M_left) {
      _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_right;
      if (__y && __y->_M_color == _S_rb_tree_red) {
        __x->_M_parent->_M_color = _S_rb_tree_black;
        __y->_M_color = _S_rb_tree_black;
        __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
        __x = __x->_M_parent->_M_parent;
      }
      else {
        if (__x == __x->_M_parent->_M_right) {
          __x = __x->_M_parent;
          _Rotate_left(__x, __root);
        }
        __x->_M_parent->_M_color = _S_rb_tree_black;
        __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
        _Rotate_right(__x->_M_parent->_M_parent, __root);
      }
    }
    else {
      _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_left;
      if (__y && __y->_M_color == _S_rb_tree_red) {
        __x->_M_parent->_M_color = _S_rb_tree_black;
        __y->_M_color = _S_rb_tree_black;
        __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
        __x = __x->_M_parent->_M_parent;
      }
      else {
        if (__x == __x->_M_parent->_M_left) {
          __x = __x->_M_parent;
          _Rotate_right(__x, __root);
        }
        __x->_M_parent->_M_color = _S_rb_tree_black;
        __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
        _Rotate_left(__x->_M_parent->_M_parent, __root);
      }
    }
  }
  __root->_M_color = _S_rb_tree_black;
}

template <class _Dummy> _Rb_tree_node_base* 
_Rb_global<_Dummy>::_Rebalance_for_erase(_Rb_tree_node_base* __z,
					 _Rb_tree_node_base*& __root,
					 _Rb_tree_node_base*& __leftmost,
					 _Rb_tree_node_base*& __rightmost)
{
  _Rb_tree_node_base* __y = __z;
  _Rb_tree_node_base* __x = 0;
  _Rb_tree_node_base* __x_parent = 0;
  if (__y->_M_left == 0)     // __z has at most one non-null child. y == z.
    __x = __y->_M_right;     // __x might be null.
  else
    if (__y->_M_right == 0)  // __z has exactly one non-null child. y == z.
      __x = __y->_M_left;    // __x is not null.
    else {                   // __z has two non-null children.  Set __y to
      __y = __y->_M_right;   //   __z's successor.  __x might be null.
      while (__y->_M_left != 0)
        __y = __y->_M_left;
      __x = __y->_M_right;
    }
  if (__y != __z) {          // relink y in place of z.  y is z's successor
    __z->_M_left->_M_parent = __y; 
    __y->_M_left = __z->_M_left;
    if (__y != __z->_M_right) {
      __x_parent = __y->_M_parent;
      if (__x) __x->_M_parent = __y->_M_parent;
      __y->_M_parent->_M_left = __x;      // __y must be a child of _M_left
      __y->_M_right = __z->_M_right;
      __z->_M_right->_M_parent = __y;
    }
    else
      __x_parent = __y;  
    if (__root == __z)
      __root = __y;
    else if (__z->_M_parent->_M_left == __z)
      __z->_M_parent->_M_left = __y;
    else 
      __z->_M_parent->_M_right = __y;
    __y->_M_parent = __z->_M_parent;
    _STL::swap(__y->_M_color, __z->_M_color);
    __y = __z;
    // __y now points to node to be actually deleted
  }
  else {                        // __y == __z
    __x_parent = __y->_M_parent;
    if (__x) __x->_M_parent = __y->_M_parent;   
    if (__root == __z)
      __root = __x;
    else 
      if (__z->_M_parent->_M_left == __z)
        __z->_M_parent->_M_left = __x;
      else
        __z->_M_parent->_M_right = __x;
    if (__leftmost == __z) 
      if (__z->_M_right == 0)        // __z->_M_left must be null also
        __leftmost = __z->_M_parent;
    // makes __leftmost == _M_header if __z == __root
      else
        __leftmost = _Rb_tree_node_base::_S_minimum(__x);
    if (__rightmost == __z)  
      if (__z->_M_left == 0)         // __z->_M_right must be null also
        __rightmost = __z->_M_parent;  
    // makes __rightmost == _M_header if __z == __root
      else                      // __x == __z->_M_left
        __rightmost = _Rb_tree_node_base::_S_maximum(__x);
  }
  if (__y->_M_color != _S_rb_tree_red) { 
    while (__x != __root && (__x == 0 || __x->_M_color == _S_rb_tree_black))
      if (__x == __x_parent->_M_left) {
        _Rb_tree_node_base* __w = __x_parent->_M_right;
        if (__w->_M_color == _S_rb_tree_red) {
          __w->_M_color = _S_rb_tree_black;
          __x_parent->_M_color = _S_rb_tree_red;
          _Rotate_left(__x_parent, __root);
          __w = __x_parent->_M_right;
        }
        if ((__w->_M_left == 0 || 
             __w->_M_left->_M_color == _S_rb_tree_black) && (__w->_M_right == 0 || 
             __w->_M_right->_M_color == _S_rb_tree_black)) {
          __w->_M_color = _S_rb_tree_red;
          __x = __x_parent;
          __x_parent = __x_parent->_M_parent;
        } else {
          if (__w->_M_right == 0 || 
              __w->_M_right->_M_color == _S_rb_tree_black) {
            if (__w->_M_left) __w->_M_left->_M_color = _S_rb_tree_black;
            __w->_M_color = _S_rb_tree_red;
            _Rotate_right(__w, __root);
            __w = __x_parent->_M_right;
          }
          __w->_M_color = __x_parent->_M_color;
          __x_parent->_M_color = _S_rb_tree_black;
          if (__w->_M_right) __w->_M_right->_M_color = _S_rb_tree_black;
          _Rotate_left(__x_parent, __root);
          break;
        }
      } else {                  // same as above, with _M_right <-> _M_left.
        _Rb_tree_node_base* __w = __x_parent->_M_left;
        if (__w->_M_color == _S_rb_tree_red) {
          __w->_M_color = _S_rb_tree_black;
          __x_parent->_M_color = _S_rb_tree_red;
          _Rotate_right(__x_parent, __root);
          __w = __x_parent->_M_left;
        }
        if ((__w->_M_right == 0 || 
             __w->_M_right->_M_color == _S_rb_tree_black) && (__w->_M_left == 0 || 
             __w->_M_left->_M_color == _S_rb_tree_black)) {
          __w->_M_color = _S_rb_tree_red;
          __x = __x_parent;
          __x_parent = __x_parent->_M_parent;
        } else {
          if (__w->_M_left == 0 || 
              __w->_M_left->_M_color == _S_rb_tree_black) {
            if (__w->_M_right) __w->_M_right->_M_color = _S_rb_tree_black;
            __w->_M_color = _S_rb_tree_red;
            _Rotate_left(__w, __root);
            __w = __x_parent->_M_left;
          }
          __w->_M_color = __x_parent->_M_color;
          __x_parent->_M_color = _S_rb_tree_black;
          if (__w->_M_left) __w->_M_left->_M_color = _S_rb_tree_black;
          _Rotate_right(__x_parent, __root);
          break;
        }
      }
    if (__x) __x->_M_color = _S_rb_tree_black;
  }
  return __y;
}

template <class _Dummy> _Rb_tree_node_base* 
_Rb_global<_Dummy>::_M_decrement(_Rb_tree_node_base* _M_node)
{
  if (_M_node->_M_color == _S_rb_tree_red && _M_node->_M_parent->_M_parent == _M_node)
    _M_node = _M_node->_M_right;
  else if (_M_node->_M_left != 0) {
    _Base_ptr __y = _M_node->_M_left;
    while (__y->_M_right != 0)
      __y = __y->_M_right;
    _M_node = __y;
  }
  else {
    _Base_ptr __y = _M_node->_M_parent;
    while (_M_node == __y->_M_left) {
      _M_node = __y;
      __y = __y->_M_parent;
    }
    _M_node = __y;
  }
  return _M_node;
}

template <class _Dummy> _Rb_tree_node_base* 
_Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
{
  if (_M_node->_M_right != 0) {
    _M_node = _M_node->_M_right;
    while (_M_node->_M_left != 0)
      _M_node = _M_node->_M_left;
  }
  else {
    _Base_ptr __y = _M_node->_M_parent;
    while (_M_node == __y->_M_right) {
      _M_node = __y;
      __y = __y->_M_parent;
    }
    if (_M_node->_M_right != __y)
      _M_node = __y;
  }
  return _M_node;
}

}

#endif
