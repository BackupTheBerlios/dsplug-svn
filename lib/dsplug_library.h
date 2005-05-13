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

int DSPlug_LibraryFile_handler_count();
DSPlug_PluginLibraryPrivate *DSPlug_get_LibraryFile_handler_open(int p_handler_index,const char *p_full_path);
void DSPlug_LibraryFile_handler_close(DSPlug_PluginLibraryPrivate *p_library);

typedef struct {

	DSPlug_PluginLibraryPrivate * (*open_callback)(const char *p_full_path);
	void (*close_callback)(DSPlug_PluginLibraryPrivate *p_library);
} DSPlug_LibraryHandler;

void DSPlug_LibraryFile_handler_register(DSPlug_LibraryHandler p_library_handler);
void DSPlug_LibraryFile_handler_uninitialize();


#endif /* DSPLUG_LIBRARY_H */
