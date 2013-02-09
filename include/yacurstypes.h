// -*- mode: c++ -*-
//
// $Id$

#ifndef YACURSTYPES_H
#define YACURSTYPES_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <vector>

// Forward declaration because we want to be independent.
class FocusGroup;

/**
 * @defgroup Focus Focus Management
 *
 * Focus Groups and Focus Manager.
 *
 * Focus Management is comprised of Focus Groups and the Focus
 * Manager. Focus Groups are organized in a list by the Focus Manager,
 * where only one Focus Group can be active, aka the Current Focus Group.
 */
typedef std::vector<FocusGroup*>::size_type fgid_t;

#endif // YACURSTYPES_H
