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

#ifndef DSPLUG_LIBRARY_H
#define DSPLUG_LIBRARY_H


#include "dsplug_types.h"
#include "dsplug_private.h"


/* *
   * the LIBRARY CACHE resolves and keeps a library open
   */

char * DSPlug_LibraryCache_get_full_path(const char *p_path);
DSPlug_PluginLibraryPrivate *DSPlug_LibraryCache_get_library(const char *p_full_path);
void DSPlug_LibraryCache_add_library(DSPlug_PluginLibraryPrivate *p_library);
void DSPlug_LibraryCache_remove_library(DSPlug_PluginLibraryPrivate *p_library);

/* *
   * the LIBRARY FILE is in charge of managing and opening files and creating DSPlugs
   */

void DSPlug_LibraryFile_handler_initialize();
int DSPlug_LibraryFile_handler_count();
DSPlug_PluginLibraryPrivate *DSPlug_get_LibraryFile_handler_open(int p_handler_index,const char *p_full_path);
void DSPlug_LibraryFile_handler_close(DSPlug_PluginLibraryPrivate *p_library);

typedef struct {
	const char *name;
	DSPlug_PluginLibraryPrivate * (*open_callback)(const char *p_full_path);
	void (*close_callback)(DSPlug_PluginLibraryPrivate *p_library);
} DSPlug_LibraryHandler;

void DSPlug_LibraryFile_handler_register(DSPlug_LibraryHandler p_library_handler);
void DSPlug_LibraryFile_handler_uninitialize();


#endif /* DSPLUG_LIBRARY_H */
