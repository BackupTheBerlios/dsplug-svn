
#ifndef DSPlug_PRIVATE_H
#define DSPlug_PRIVATE_H


#include "dsplug_types.h"
#include "dsplug_port_info_private.h"

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

	DSPlug_PluginUsageHint usage_hint;


	/* Array of pointers to AUDIO PORT Caps */
	DSPlug_AudioPortCapsPrivate **audio_port_caps;
	int audio_port_count;

	/* Array of pointers to EVENT PORT Caps */
	DSPlug_EventPortCapsPrivate **event_port_caps;
	int event_port_count;

	/* Array of pointers to CONTROL PORT Caps */
	DSPlug_ControlPortCapsPrivate **control_port_caps;
	int control_port_count;


	/* Bitmask for Features */

	unsigned long int features;

	/* Table for Constants */

	int constants[MAX_PLUGIN_CAPS_CONSTANTS];

	/* Instance Plugin Userdata Callback */

	void* (*instance_plugin_userdata)(DSPlug_PluginCaps, DSPlug_Boolean ui); /**< This is used to create the processing instance */
	void (*destroy_plugin_userdata)(DSPlug_Plugin *); /**< This is used to destroy the processing instance*/

	/* Process Callbacks */

	void (*process_callback)(DSPlug_Plugin *, int f);
	void (*reset_callback)(DSPlug_Plugin *);

	/* Misc Callbacks */

	int  (*get_output_delay_callback)(DSPlug_Plugin *);
	int  (*get_skipped_initial_frames_callback)(DSPlug_Plugin *);


} DSPlug_PluginCapsPrivate;



/* ////////////////////////////////////////////////////////// */

/* Plugin Instance */

typedef struct {

	/**
	 * the plugin can always access its own caps,
	 * for ease of programming of this lib
	 */
	DSPlug_PluginCapsPrivate * plugin_caps;

	DSPlug_AudioPortPrivate **audio_ports;
	int audio_port_count;

	DSPlug_EventPortPrivate **event_ports;
	int event_port_count;

	DSPlug_ControlPortPrivate **control_ports;
	int control_port_count;

	DSPlug_Boolean inside_process_callback_flag; /* This flag is on when plugin is inside process callback */

} DSPlug_PluginPrivate;

/* ////////////////////////////////////////////////////////// */

/* Plugin Library */

typedef struct {


	DSPlug_PluginCapsPrivate **plugin_caps_array; /**< Plugin Capabilities for all plugins */
	int plugin_count;


	/* Library file handler stuff */
	int reference_count; /**< Amount of times this library has been opened */
	int library_file_handler_index; /* library file handler in charge of it */
	void * library_file_handler_private; /* library file handler private data */
	char * library_file_handler_full_path;


} DSPlug_PluginLibraryPrivate;

#endif /* dsplug private */
