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

#include "dsplug_default_loader.h"
#include "dsplug_library.h"
#include "dsplug_error_report.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

DSPlug_PluginLibraryPrivate * DSPlug_default_open_callback(const char *p_full_path) {

	DSPlug_PluginLibraryPrivate *library;
	DSPlug_LibraryCreation library_creation;

	void * handle = dlopen(p_full_path, RTLD_NOW);
	if (!handle) {
		DSPlug_report_error("LOADER: DSPlug_default_open_callback: cannot open file");
		return NULL;
	}
	void (*creation_func)(DSPlug_LibraryCreation);
	creation_func=dlsym(handle,"creation_callback");

	if (!creation_func) {
		DSPlug_report_error("LOADER: DSPlug_default_open_callback: file lacks creation function");
		return NULL;
	}

	library = (DSPlug_PluginLibraryPrivate *)malloc( sizeof(DSPlug_PluginLibraryPrivate) );
	memset(library,0,sizeof(DSPlug_PluginLibraryPrivate));
	library->library_file_handler_private=handle;

	library_creation._private=library;

	creation_func(library_creation); /* call creation func on the plugin */

	if (library->plugin_count==0) {

		DSPlug_report_error("LOADER: DSPlug_default_open_callback: library has zero plugins");

		dlclose(handle);
		free(library);
		return NULL;
	}

	return library;
}

void DSPlug_default_close_callback(DSPlug_PluginLibraryPrivate *p_library) {

	dlclose(p_library->library_file_handler_private);

}


/****************************/


void DSPlug_DefaultLoader_register() {

	DSPlug_LibraryHandler handler;

	handler.name="Default Internal DSPlug Loader";
	handler.open_callback=DSPlug_default_open_callback;
	handler.close_callback=DSPlug_default_close_callback;

	DSPlug_LibraryFile_handler_register(handler);
}
