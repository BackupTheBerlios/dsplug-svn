
#ifndef DSPLUG_PRIVATE_H
#define DSPLUG_PRIVATE_H


#include "dsplug/dsplug_types.h"
#include "dsplug_port_info_private.h"
/* ////////////////////////////////////////////////////////// */

/* Plugin Handle */

typedef struct {





} DSPLUG_Plugin_Handle_Private;

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
	
	DSPLUG_Plugin_Type type;

	/* Ports */

		/* Array of pointers to AUDIO PORT Info */
	DSPLUG_Audio_Port_Info_Private **audio_ports_info;
	int audio_port_count;

		/* Array of pointers to EVENT PORT Info */
	DSPLUG_Event_Port_Info_Private **event_port_info;
	int event_port_count;

		/* Array of pointers to CONTROL PORT Info */
	DSPLUG_Control_Port_Info_Private **control_port_info;
	int control_port_count;


	/* Bitmask for Features */
	
	unsigned long int features;

	/* Table for Constants */
	
	int constants[MAX_PLUGIN_CAPS_CONSTANTS];

	
	
	
} DSPLUG_Plugin_Caps_Private;


/* ////////////////////////////////////////////////////////// */

/* Plugin Instance */

typedef struct {





} DSPLUG_Plugin_Instance_Private;


#endif /* dsplug private */
