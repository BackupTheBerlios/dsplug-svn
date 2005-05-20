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

#include "dsplug_library.h"
#include "dsplug_error_report.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "dsplug_default_loader.h"

typedef struct {

	DSPlug_PluginLibraryPrivate * library;

} DSPlug_LibraryCacheElement;


static DSPlug_LibraryCacheElement *library_cache_elements=0;
static int library_cache_element_count=0;

/* *
   * the LIBRARY CACHE resolves and keeps a library open
   */

char * DSPlug_LibraryCache_get_full_path(const char *p_path) {

	char *full;
	static const int MAX_FULLCWD_SIZE=4096; /* enough? */
	char *aux_cwd;

	if (strlen(p_path)==0)
		return NULL;




	if (p_path[0]=='/') {

		full=(char*)malloc(strlen(p_path)+1);
		strcpy(full,p_path);

	} else {

		aux_cwd = (char*)malloc(MAX_FULLCWD_SIZE);
		if (getcwd(aux_cwd,MAX_FULLCWD_SIZE)) {
			DSPlug_report_error("API: DSPlug_LibraryCache_get_full_path: Current path is too big to retrieve? wtf?");
			free(aux_cwd);
			return NULL;
		}

		if ( (strlen(aux_cwd)+strlen(p_path)+1)>MAX_FULLCWD_SIZE ) {
			DSPlug_report_error("API: DSPlug_LibraryCache_get_full_path: Current path is too big to retrieve? wtf?");
			free(aux_cwd);
			return NULL;
		}

		strcat(aux_cwd,p_path);
		full=(char*)malloc(strlen(aux_cwd)+1);
		strcpy(full,aux_cwd);
		free(aux_cwd);

	}

	return full;
}

DSPlug_PluginLibraryPrivate *DSPlug_LibraryCache_get_library(const char *p_full_path) {

    int i;
	for (i=0;i<library_cache_element_count;i++)
		if (!strcmp(library_cache_elements[i].library->library_cache_full_path,p_full_path))
			return library_cache_elements[i].library;

	return NULL;
}
void DSPlug_LibraryCache_add_library(DSPlug_PluginLibraryPrivate *p_library) {

	library_cache_element_count++;
	/* looong line! */
	library_cache_elements=(DSPlug_LibraryCacheElement*)realloc(library_cache_elements,sizeof(DSPlug_LibraryCacheElement)*library_cache_element_count);
	library_cache_elements[library_cache_element_count-1].library=p_library;

}
void DSPlug_LibraryCache_remove_library(DSPlug_PluginLibraryPrivate *p_library) {

	int idx=-1;
	int i;
	for (i=0;i<library_cache_element_count;i++)
		if (library_cache_elements[i].library==p_library)
			idx=i;

	if (idx==-1) {

		DSPlug_report_error("API: DSPlug_LibraryCache_remove_library: Library to remove not found, bug?");
		return;
	}

	for (i=idx;i<(library_cache_element_count-1);i++) {

		library_cache_elements[i]=library_cache_elements[i+1];
	}

	library_cache_element_count--;

	library_cache_elements=(DSPlug_LibraryCacheElement*)realloc(library_cache_elements,sizeof(DSPlug_LibraryCacheElement)*library_cache_element_count);
}




/* *
   * the LIBRARY FILE is in charge of managing and opening files and creating DSPlugs
   */

static DSPlug_LibraryHandler *library_handler_elements=0;
static int library_handler_element_count=0;
static int library_handler_initialized=0;


int DSPlug_LibraryFile_handler_count() {

	return library_handler_element_count;
}

DSPlug_PluginLibraryPrivate *DSPlug_get_LibraryFile_handler_open(int p_handler_index,const char *p_full_path) {

	DSPlug_PluginLibraryPrivate * new_library;
	if (p_handler_index<0 || p_handler_index>=library_handler_element_count)
		return NULL;

	new_library = library_handler_elements[p_handler_index].open_callback(p_full_path);
	if (new_library==NULL)
		return NULL;

	/* Fill out the "even more private" properties */

	new_library->reference_count=0;
	new_library->library_cache_full_path=(char*)malloc(strlen(p_full_path)+1);
	strcpy(new_library->library_cache_full_path,p_full_path);
	new_library->library_file_handler_index=p_handler_index;

	return new_library;
}

void DSPlug_LibraryFile_handler_close(DSPlug_PluginLibraryPrivate *p_library) {

	library_handler_elements[p_library->library_file_handler_index].close_callback(p_library);

}

void DSPlug_LibraryFile_handler_register(DSPlug_LibraryHandler p_library_handler) {

	library_handler_element_count++;
	/* looong line! */
	library_handler_elements=(DSPlug_LibraryHandler*)realloc(library_handler_elements,sizeof(DSPlug_LibraryHandler)*library_handler_element_count);
	library_handler_elements[library_handler_element_count-1]=p_library_handler;

}

void DSPlug_LibraryFile_handler_initialize() {

	if (library_handler_initialized)
		return;

	DSPlug_DefaultLoader_register();

}

void DSPlug_LibraryFile_handler_uninitialize() {

	if (library_handler_elements)
		free(library_handler_elements);

	library_handler_element_count=0;

}
