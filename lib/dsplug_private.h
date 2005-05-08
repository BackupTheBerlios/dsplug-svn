
#ifndef DSPlug_PRIVATE_H
#define DSPlug_PRIVATE_H


#include "dsplug_types.h"
#include "dsplug_port_info_private.h"
/* ////////////////////////////////////////////////////////// */

/* Plugin Handle */

typedef struct {


	int plugin_count;


} DSPlug_PluginLibraryPrivate;

/* ////////////////////////////////////////////////////////// */

/* Plugin Caps */


#define MAX_PLUGIN_CAPS_CONSTANTS 32

typedef struct {


	/* Strings */
	

	char *info_caption;
	char *info_name;
	char *info_author;
	char *info_copyright;
	char *info_version;
	char *info_compatible_version;
	char *info_unique_ID;
	char *info_category_path;

	/* Plugin Type */
	
	DSPlug_PluginUsageHint type;

	/* Ports */

		/* Array of pointers to AUDIO PORT Info */
	DSPlug_AudioPortCapsPrivate **audio_ports_info;
	int audio_port_count;

		/* Array of pointers to EVENT PORT Info */
	DSPlug_EventPortCapsPrivate **event_port_info;
	int event_port_count;

		/* Array of pointers to CONTROL PORT Info */
	DSPlug_ControlPortCapsPrivate **control_port_info;
	int control_port_count;


	/* Bitmask for Features */
	
	unsigned long int features;

	/* Table for Constants */
	
	int constants[MAX_PLUGIN_CAPS_CONSTANTS];

	
	
	
} DSPlug_PluginCapsPrivate;


/* ////////////////////////////////////////////////////////// */

/* Plugin Instance */

typedef struct {





} DSPlug_PluginPrivate;


#endif /* dsplug private */
