#ifndef DSPLUG_LIBRARY_H
#define DSPLUG_LIBRARY_H

/*
	Functions in charge of managing the libraries from files
*/


char * DSPlug_LibraryCache_get_full_path(const char *p_path);
DSPlug_Boolean DSPlug_LibraryCache_has_library(const char *p_full_path);
DSPlug_PluginLibraryPrivate *DSPlug_LibraryCache_get_library(const char *p_full_path);
void DSPlug_LibraryCache_add_library(DSPlug_PluginLibraryPrivate *p_library);
void DSPlug_LibraryCache_remove_library(DSPlug_PluginLibraryPrivate *p_library);

int DSPlug_LibraryFile_handler_count();
DSPlug_PluginLibraryPrivate *DSPlug_get_LibraryFile_handler_open(int p_handler_index,const char *p_full_path);
void DSPlug_LibraryFile_handler_close(DSPlug_PluginLibraryPrivate *p_library);

/* void DSPlug_LibraryFile_(DSPlug_PluginLibraryPrivate *p_library); */



#endif /* DSPLUG_LIBRARY_H */
