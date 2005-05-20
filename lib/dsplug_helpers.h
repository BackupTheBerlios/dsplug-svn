/***************************************************************************
    This file is part of the DSPlug DSP Plugin Architecture
    url                  : http://www.dsplug.org
    copyright            : (C) 2005 by Juan Linietsky
    email                : coding -dontspamme- *AT* -please- reduz *DOT* com *DOT* ar
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License (LGPL)    *
 *   as published by the Free Software Foundation; either version 2.1 of   *
 *   the License, or (at your option) any later version.                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DSPLUG_HELPERS_H
#define DSPLUG_HELPERS_H

#include "dsplug_private.h"


void DSPlug_copy_to_newstring(char** p_dst,const char *p_src);
void DSPlug_validate_path_to_newstring(char** p_dst,const char *p_src);
void DSPlug_free_common_port_caps(DSPlug_CommonPortCapsPrivate *);
void DSPlug_free_plugin_caps(DSPlug_PluginCapsPrivate *);
DSPlug_Boolean DSPlug_check_features_bit(DSPlug_PluginCapsPrivate *,DSPlug_PluginFeature f);

#endif
