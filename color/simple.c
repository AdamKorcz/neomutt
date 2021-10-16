/**
 * @file
 * Simple colour
 *
 * @authors
 * Copyright (C) 2021 Richard Russon <rich@flatcap.org>
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @page color_simple Simple colour
 *
 * Manage the colours of the 'simple' graphical objects -- those that can only
 * have one colour, plus attributes.
 */

#include "config.h"
#include <stddef.h>
#include <stdbool.h>
#include "mutt/lib.h"
#include "gui/lib.h"
#include "debug.h"

struct AttrColor SimpleColors[MT_COLOR_MAX]; ///< Array of Simple colours

/**
 * simple_colors_init - Initialise the simple colour definitions
 */
void simple_colors_init(void)
{
  // Set some defaults
  color_debug("init indicator, markers, etc\n");
  SimpleColors[MT_COLOR_INDICATOR].attrs = A_REVERSE;
  SimpleColors[MT_COLOR_MARKERS].attrs = A_REVERSE;
  SimpleColors[MT_COLOR_SEARCH].attrs = A_REVERSE;
#ifdef USE_SIDEBAR
  SimpleColors[MT_COLOR_SIDEBAR_HIGHLIGHT].attrs = A_UNDERLINE;
#endif
  SimpleColors[MT_COLOR_STATUS].attrs = A_REVERSE;
}

/**
 * simple_colors_clear - Reset the simple colour definitions
 */
void simple_colors_clear(void)
{
  color_debug("clean up defs\n");
  for (size_t i = 0; i < MT_COLOR_MAX; i++)
  {
    attr_color_clear(&SimpleColors[i]);
  }
}

/**
 * simple_colors_get - Get the colour of an object by its ID
 * @param id Colour ID, e.g. #MT_COLOR_SEARCH
 * @retval ptr AttrColor of the object
 *
 * @note Do not free the returned object
 */
struct AttrColor *simple_colors_get(enum ColorId id)
{
  if (id >= MT_COLOR_MAX)
  {
    mutt_error("colour overflow %d", id);
    return NULL;
  }
  if (id <= MT_COLOR_NONE)
  {
    mutt_error("colour underflow %d", id);
    return NULL;
  }

  return &SimpleColors[id];
}

/**
 * simple_color_is_set - Is the object coloured?
 * @param id Colour ID, e.g. #MT_COLOR_SEARCH
 * @retval true Yes, a 'color' command has been used on this object
 */
bool simple_color_is_set(enum ColorId id)
{
  return attr_color_is_set(simple_colors_get(id));
}

/**
 * simple_color_is_header - Colour is for an Email header
 * @param color_id Colour, e.g. #MT_COLOR_HEADER
 * @retval true Colour is for an Email header
 */
bool simple_color_is_header(enum ColorId color_id)
{
  return (color_id == MT_COLOR_HEADER) || (color_id == MT_COLOR_HDRDEFAULT);
}
