#ifndef DSPLUG_HOST_H
#define DSPLUG_HOST_H


#include "dsplug_types.h"

/****************************/

/* PLUGIN HANDLE */

/****************************/

/*

	Instancing a Plugin Handle:
	
		This warranties that the library on which the plugin is
		remains open, and, optionally, a reference count to the
		times the plugin was opened is mantained. This is done
		automatically on most Operating Systems anyay, but
		I believe it's better to abstract this part from the OS
*/

DSPLUG_HOST_Plugin_Handle * DSPLUG_HOST_open_plugin( const char * p_path );



/*

	Closing a Plugin Handle:
	
		If you dont want to use the plugin anymore, close it
		using this function:


	WARNING: This will invalidate any open plugin instance!

*/		                                   

void DSPLUG_HOST_close_plugin( DSPLUG_HOST_Plugin_Handle * );


/*
	A plugin can have several multiple functionalities inside,
	as in.. an equalize plugin may contain 6,8,10,21,31 bands
	versions, as well as a plugin having a low quality and
	hi quality version, and so on. It is basically
	like having multiple sub-plugins inside. This will
	be called "instance type" (I have no better name, please
	give me one or this will go for stable version).
	The amount of instance types can be retrieved by calling:

*/	

int DSPLUG_HOST_plugin_handle_get_instance_types_count( DSPLUG_HOST_Plugin_Handle * );
/* get the amount of instance types.. must be 1 or greater. */

/****************************/

/* PLUGIN CAPABILITIES */

/****************************/


/*
	Using the handle, one can instance the plugin capabilities object
	for one of multiple instance types.
	This is used to obtain all sorts of static information on the specific plugin
	instance type,	without the need of instancing one.
	
*/

DSPLUG_HOST_Plugin_Caps * DSPLUG_HOST_plugin_handle_get_caps( DSPLUG_HOST_Plugin_Handle * , int p_instance_type );
/* get plugin caps structure for the specific instance type */

/*
	Basic information about the plugin can be retrieved by
	the following functions:
	(it is to note that functions will never set into the
	char buffer parameters a stringth with length greater
	than DSPLUG_STRING_PARAM_MAX_LEN (plus zero).
*/


void DSPLUG_HOST_plugin_caps_get_caption( DSPLUG_HOST_Plugin_Caps *, char * );
/*
returns the visual name of the plugin, like
"Johnny's Ultimate Sound Madness Creator!!!"
*/

void DSPLUG_HOST_plugin_caps_get_name( DSPLUG_HOST_Plugin_Caps *, char * );
/* returns the Unix-like name of the plugin, like "madness_creator" */

void DSPLUG_HOST_plugin_caps_get_author( DSPLUG_HOST_Plugin_Caps *, char * );
/* returns the plugin author, "John Maniac" */

void DSPLUG_HOST_plugin_caps_get_copyright( DSPLUG_HOST_Plugin_Caps *, char * );
/* returns the plugin copyright holder "© 2006 John F. Maniac" */

void DSPLUG_HOST_plugin_caps_get_version( DSPLUG_HOST_Plugin_Caps *, char * );
/*
returns the plugin version,
the naming scheme doesn't matter, but a newer version must be alphanumerically greater than an older
version if compared , like:  "0.99.31a" is lesser than "1.02.32".  "3.10" is lesser than "3.9" so care must be
taken, use "3.09" instead.
*/

void DSPLUG_HOST_plugin_caps_get_compatible_version( DSPLUG_HOST_Plugin_Caps *, char * );
/*
Return the version up to where this plugin is compatible, it must be minor or equal than the current version.
This is done to avoid incompatibilities if a new version of the plugin adds more ports, channels, etc.
*/

void DSPLUG_HOST_plugin_caps_get_unique_ID( DSPLUG_HOST_Plugin_Caps *, char * );
/*
Return the Unique ID number for this plugin. 0 to 1000 are reserved for development.
It is recommended that applications rely both on the Unique ID and Version when saving
plugin information. Plugin IDs can be requested to whoever mantains this standard
*/

void DSPLUG_HOST_plugin_caps_get_category_path( DSPLUG_HOST_Plugin_Caps *, char * );
/*
Return the category path on where the plugin shall be displayed.
As well as plugin IDs , categories will be defined by the standard mantainer
*/



/****************************/

/* PORT CAPS */

/****************************/


/**
  *	Get the amount of ports in the plugin
  *	\return an integer value containing the number of ports
  *
  */
  
 int DSPLUG_HOST_plugin_caps_get_port_count( DSPLUG_HOST_Plugin_Caps *);


/**
  *	Get the port type. Supported types are audio, event and control
  *	\param i the port index, begining from zero
  *	\return the port type
  *
  *	If the port index is invalid, an error message will be sent to stderr, and the function will do nothing. By design, the API does not offer ways to handle this silly level of human errors.
  *	The return value, can then be ignored.
  */
 
DSPLUG_Port_Type DSPLUG_HOST_plugin_caps_get_port_type( DSPLUG_HOST_Plugin_Caps *, int i );


/**
  *	Get the caption of the port, the one used for user visibility
  *	\param i the port index, begining from zero
  *	\param s Pointer to a char buffer of size: DSPLUG_STRING_MAX_LEN
  *
  *	If the port index is invalid, an error message will be sent to stderr, and the function will do nothing.  By design, the API does not offer ways to handle this silly level of human errors.
  */
  
void DSPLUG_HOST_plugin_caps_get_port_caption( DSPLUG_HOST_Plugin_Caps *, int i, char * s );


/**
  *	Get the name of the port, this name should be used as reference for saving the
  *	status of a port (and NOT the index). In the case of a port supporting multipart
  *	then the port name and the part index have to be saved.
  *	\param i the port index, begining from zero
  *	\param s Pointer to a char buffer of size: DSPLUG_STRING_MAX_LEN
  *
  *	If the port index is invalid, an error message will be sent to stderr, and the function will do nothing.  By design, the API does not offer ways to handle this silly level of human errors.
  */
  
void DSPLUG_HOST_plugin_caps_get_port_name( DSPLUG_HOST_Plugin_Caps *, int i, char * s );


/**
  *	Get the "plug" type of the port, it can be input, output or both
  *	\param i the port index, begining from zero
  *	\return the "plug" type
  *
  *	If the port index is invalid, an error message will be sent to stderr, and the function will do nothing. By design, the API does not offer ways to handle this silly level of human errors.
  *	The return value, can then be ignored.
  */
  
DSPLUG_Plug_Type DSPLUG_HOST_plugin_caps_get_port_plug_type( DSPLUG_HOST_Plugin_Caps *, int i);


/**
  *	Get the port path. This is useful for organizing the ports in the host,
  *	so they can be shown as a tree, or directories. The path will use "/" as
  *	separator. Example paths are:
  *		- "/"
  *		- "/filter/cutoff" , "/filter/resonance"
  *		- "/oscil/saw"
  *	\param i the port index, begining from zero
  *	\param s Pointer to a char buffer of size: DSPLUG_STRING_MAX_LEN
  *
  *	If the port index is invalid, an error meessage will be sent to stderr, and the function will do nothing. By design, the API does not offer ways to handle this silly level of human errors.
  */
  
void DSPLUG_HOST_plugin_caps_get_port_path( DSPLUG_HOST_Plugin_Caps *, int i, char * s);

  

/****************************/

/* AUDIO PORT CAPS */

/****************************/

/**
 *	Get AUDIO port capabilities.
 *	\param i the port index, begining from zero
 *	\return an audio port capabilities object
 *
 *	If the port type or index is invalid, an error meessage will be sent to stderr, and the function will do nothing.  By design, the API does not offer ways to handle this silly level of human errors.
 *	The return value, can then be ignored.
 */

DSPLUG_HOST_Audio_Port_Caps DSPLUG_HOST_plugin_caps_get_audio_port_caps( DSPLUG_HOST_Plugin_Caps *, int i );

/**
 *	Get the default amount of channels for the audio port.
 *	If the plugin features variable audio channels,
 *      (DSPLUG_PLUGIN_FEATURE_VARIABLE_AUDIO_CHANNELS) then this will
 *      report the default amount (when instancing the plugin normally).
 *	\return number of channels, starting from 1
 *
 */

int DSPLUG_HOST_audio_caps_get_channel_count( DSPLUG_HOST_Audio_Port_Caps * );


/****************************/

/* EVENT PORT CAPS */

/****************************/

/**
 *	Get EVENT port capabilities.
 *	\param i the port index, begining from zero
 *	\return an event port capabilities object
 *
 *	If the port type or index is invalid, an error meessage will be sent to stderr, and the function will do nothing.  By design, the API does not offer ways to handle this silly level of human errors.
 *	The return value, can then be ignored.
 */

DSPLUG_HOST_Event_Port_Caps DSPLUG_HOST_plugin_caps_get_event_port_caps( DSPLUG_HOST_Plugin_Caps *, int i );

/**
 *	Get the type of events handled by the event port
 *	\return type of event enum
 *
 *	If the port type or index is invalid, an error meessage will be sent to stderr, and the function will do nothing.  By design, the API does not offer ways to handle this silly level of human errors.
 *	The return value, can then be ignored.
 */

DSPLUG_Event_Type DSPLUG_HOST_event_caps_get_handled_event_type( DSPLUG_HOST_Event_Port_Caps * );

/****************************/

/* CONTROL PORT CAPS */

/****************************/

/**
 *	Get CONTROL port capabilities.
 *	\param i the port index, begining from zero
 *	\return an control port capabilities object
 *
 *	If the port type or index is invalid, an error meessage will be sent to stderr, and the function will do nothing.  By design, the API does not offer ways to handle this silly level of human errors.
 *	The return value, can then be ignored.
 */


DSPLUG_HOST_Control_Port_Caps DSPLUG_HOST_plugin_caps_get_control_port_caps( DSPLUG_HOST_Plugin_Caps *, int i );

/**
 *	Get the control port value type. Supported types are float, string, data.
 *	\return the value type.
 */
 
DSPLUG_Control_Port_Type DSPLUG_HOST_control_caps_get_value_type( DSPLUG_HOST_Control_Port_Caps * );

/**
 *	Determine how a float port must display a float value.
 *	Just pass a regular port value (from 0 to 1) and obtain the	
 *	display string. Example: you pass "0.33" then obtain "1240 hz",
 *      "33%" , "-9.6 dB", etc. The plugin determines what the value is.
 *	\param v real value from 0 to 1 
 *	\param s Pointer to a char buffer of size: DSPLUG_STRING_MAX_LEN
 *
 *	If the control port is not of type float, then an error message is sent to stderr and the function does nothing. By design, the API does not offer ways to handle this silly level of human errors.
 */

void DSPLUG_HOST_control_caps_float_value_display( DSPLUG_HOST_Control_Port_Caps * ,  float v , char *s);
/* This will return , for example, for 0.5 , 3123hz */



/*
	A port could be an ennumerated type, like the kind of distortion
	in a guitar, or a State Variable Filter Mode.
*/

DSPLUG_Control_Port_Type DSPLUG_HOST_plugin_caps_get_control_port_type( DSPLUG_HOST_Plugin_Caps * , int p_port_index );
/* Return the type of control port */


/* ENUM PORT Options */

int DSPLUG_HOST_plugin_caps_get_control_port_enum_option_count( DSPLUG_HOST_Plugin_Caps * , int p_port_index );
/* returns how many options the enumeration may have, ie: 3 */

void DSPLUG_HOST_plugin_caps_get_control_port_enum_option_caption( DSPLUG_HOST_Plugin_Caps * , int p_port_index , int p_option, char *);
/* returns the caption of each option, like "Lowpass" , "Bandpass" , "HiPass" */

/* INTEGER PORT Options */

DSPLUG_Boolean DSPLUG_HOST_plugin_caps_is_control_port_integer( DSPLUG_HOST_Plugin_Caps * , int p_port_index );
/* Check if this is integer */

int DSPLUG_HOST_plugin_caps_get_control_port_integer_steps( DSPLUG_HOST_Plugin_Caps * , int p_port_index );
/* Get how many integer steps */

/*

	Many hosts will attempt to change control ports during playback,
	to assign different parameters during the length of a song/sound/etc
	This is perfectly normal, but also many hosts will need to do so
	from a realtime thread. Since realtime-safety and thread-safety
	may be needed, plugins may define when they are safe to be set
	and read by defining the realtime safe flag.
	If the plugin defines this it means that it will:

		* Not do any operation that may result in the realtime
		thread to block operation at the kernel scheduler.
		This usually happens when doing a libcall/syscall
		that may set the process in wait state, such as
		handling files, sleeping, try to grab a blocking mutex, etc.

		* Warrant that setting a port is thread-safe, as in, setting the
		port from the UI or multiple threads will only result in setting
		a shared memory value, and the function can be safely be called
		simultaneously from them (many threads at the same time)

		* Make sure that the port doesnt need blocking. Some ports will attempt
		to block the internal state of the plugin when being set, to prevent
		the process() function to use that state until it's done reconfiguring it,
		because this may lead to a crash. Such ports cannot be made RT-Safe
		
*/


DSPLUG_Boolean DSPLUG_HOST_plugin_caps_is_control_realtime_safe( DSPLUG_HOST_Plugin_Caps * , int p_port_index );
/* return whether reading or writing from the control ports is REALTIME SAFE */

/*
	* Defaults and Parts *

	If the plugin is of type API_PLUGIN_TYPE_GENERIC,
	API_PLUGIN_TYPE_MULTIPART_SYNTHESIZER
	or API_PLUGIN_TYPE_MUSIC_EVENT_MATRIX then a port could
	be assignable to multiple parts. You can check if a port will
	work specifically for a part by calling this function.
	This is made so hosts can easily assign a control port
	to a part.

	The amount of PARTS that the plugin has is computed by checking
	how many MUSIC EVENT Ports does it have, multiplied by 16
	(-max- midi channels, and only if the port isnt on OMNI mode).
	Anyway, In the end these are just helpers for the host.. the host
	can do whathever it wants.
*/

int DSPLUG_HOST_plugin_caps_get_control_port_part( DSPLUG_HOST_Plugin_Caps * , int p_port_index );
/* return the part for which the port will work, a negative value is default and means: "works for any part" */

/*
	And finally, we can always retrieve the default value for a port:
	part can be ignored for non multipart plugins
*/

float DSPLUG_HOST_plugin_caps_get_control_port_default( DSPLUG_HOST_Plugin_Caps * , int p_port_index);
/* return the default value for the control port, -1.0 if the port is not floating point */

void DSPLUG_HOST_plugin_caps_get_control_port_default_string( DSPLUG_HOST_Plugin_Caps * , int p_port_index, const char **p_string );
/*
get the default value for the control port, which is set to p_string, a zero-terminated C-String
the string must be kept in memory by the plugin, since it is supposed to be a constant
the plugin will return NULL if the port is not of this type
*/



void DSPLUG_HOST_plugin_caps_get_control_port_default_raw_data( DSPLUG_HOST_Plugin_Caps * , int p_port_index, const void **p_raw_data, int *p_data_len );
/*
get the default value for the control port, which is set to p_data, with bytes-length to p_data_len
the data must be kept in memory by the plugin, as it is supposed to be a constant
the plugin will return NULL if the port is not of this type
*/



/*
return the number of channels the port handles. If the plugin features
DSPLUG_PLUGIN_FEATURE_VARIABLE_AUDIO_CHANNELS this will return the
DEFAULT amount of channels for the port.
*/








/* Old Plugin Idea */






/* EVENT PORTS */

/*
	To query the amount of event ports in a plugin,
	the following functions are provided:
*/

int DSPLUG_HOST_plugin_caps_get_event_ports_count( DSPLUG_HOST_Plugin_Caps * );
/* This will return the number of event ports that a plugin can receive. */


DSPLUG_Event_Port_Type DSPLUG_HOST_plugin_caps_event_port_get_type( DSPLUG_HOST_Plugin_Caps * , int p_port_index );
/* Return wether the port inputs or outputs events */

void DSPLUG_HOST_plugin_caps_get_event_port_caption( DSPLUG_HOST_Plugin_Caps *, char * );

/*
	Some capabilities of the ports can be tested too:
*/


DSPLUG_Event_Port_Event_Type DSPLUG_HOST_plugin_caps_event_port_get_event_type( DSPLUG_HOST_Plugin_Caps * , int p_port_index );
/* Get the type of events that the port handles */

/* AUDIO Ports */

/*
	These are AUDIO ports, they deal with digital audio signals in
	32-Bits floating point format.
*/

/*
	You can query for the amount of audio ports in the plugin:
*/


int DSPLUG_HOST_plugin_caps_get_audio_ports_count( DSPLUG_HOST_Plugin_Caps * );
/* This will return the amount of audio ports that the plugin handles. */


/* 
	Additionally, we can query the default number of channels handled by the port:
*/

DSPLUG_Audio_Port_Type DSPLUG_HOST_plugin_caps_get_audio_port_type( DSPLUG_HOST_Plugin_Caps * , int p_input_port_index );


int DSPLUG_HOST_plugin_caps_get_audio_port_channels( DSPLUG_HOST_Plugin_Caps * , int p_input_port_index );
/*
return the number of channels the port handles. If the plugin features
DSPLUG_PLUGIN_FEATURE_VARIABLE_AUDIO_CHANNELS this will return the
DEFAULT amount of channels for the port.
*/

/* 
	We can check the name of the port too
*/

void DSPLUG_HOST_plugin_caps_get_input_port_audio_caption( DSPLUG_HOST_Plugin_Caps * , char * );
void DSPLUG_HOST_plugin_caps_get_output_port_audio_caption( DSPLUG_HOST_Plugin_Caps * , char * );
/* get the visible name of the port */

/*

	Feel free to use DSPLUG_Audio_Channels enumeration
	as reference (in types.h)
*/ 


/* CONTROL Ports */

/*

	A plugin can have any number of control ports.
	A control port is customizable parameter of the processes
	going on inside the plugin. Ports can only be assigned values
	from 0.0 to 1.0 (floating point)

	Usuaully, control ports can have many uses:

	If the plugin lacks an interface, usually the host will build one
	for it by using them, as they control the parameters of the synthesis.

	The plugin configuration can always be stored by saving all the ports,
	and restored by setting all the saved ports later.

*/

int DSPLUG_HOST_plugin_caps_get_control_ports_count( DSPLUG_HOST_Plugin_Caps * );
/* This will return the amount of control ports */

/*
	We can fetch some information about the port itself:
*/

void DSPLUG_HOST_plugin_caps_get_control_port_caption( DSPLUG_HOST_Plugin_Caps * , int p_port_index , char * );
/* This will return the displayable name of the control port, like "Frecuency Cutoff (hz)" */

void DSPLUG_HOST_plugin_caps_get_control_port_name( DSPLUG_HOST_Plugin_Caps * , int p_port_index , char *);
/* This will return the Unix-like name of the plugin, and should be unique for the plugin, like "freq_cutoff" */

void DSPLUG_HOST_plugin_caps_get_control_port_path( DSPLUG_HOST_Plugin_Caps * , int p_port_index , char *);
/*
Control ports can be organized by path, this eases searching for a control port or organizing them while
developing hosts. Examples could be: /Filter , /Generation/Oscilator 1 , etc.
"slash" (/) is the delimiter
*/

/*
	Since in most cases we want to know what the port value is actually setting, not just 0 .. 1 , we can ask the plugin how to display it.
*/
void DSPLUG_HOST_plugin_caps_get_control_port_display( DSPLUG_HOST_Plugin_Caps * , int p_port_index , float p_val , char *);
/* This will return , for example, for 0.5 , 3123hz */

/*
	A port could be an ennumerated type, like the kind of distortion
	in a guitar, or a State Variable Filter Mode.
*/

DSPLUG_Control_Port_Type DSPLUG_HOST_plugin_caps_get_control_port_type( DSPLUG_HOST_Plugin_Caps * , int p_port_index );
/* Return the type of control port */


/* ENUM PORT Options */

int DSPLUG_HOST_plugin_caps_get_control_port_enum_option_count( DSPLUG_HOST_Plugin_Caps * , int p_port_index );
/* returns how many options the enumeration may have, ie: 3 */

void DSPLUG_HOST_plugin_caps_get_control_port_enum_option_caption( DSPLUG_HOST_Plugin_Caps * , int p_port_index , int p_option, char *);
/* returns the caption of each option, like "Lowpass" , "Bandpass" , "HiPass" */

/* INTEGER PORT Options */

DSPLUG_Boolean DSPLUG_HOST_plugin_caps_is_control_port_integer( DSPLUG_HOST_Plugin_Caps * , int p_port_index );
/* Check if this is integer */

int DSPLUG_HOST_plugin_caps_get_control_port_integer_steps( DSPLUG_HOST_Plugin_Caps * , int p_port_index );
/* Get how many integer steps */

/*

	Many hosts will attempt to change control ports during playback,
	to assign different parameters during the length of a song/sound/etc
	This is perfectly normal, but also many hosts will need to do so
	from a realtime thread. Since realtime-safety and thread-safety
	may be needed, plugins may define when they are safe to be set
	and read by defining the realtime safe flag.
	If the plugin defines this it means that it will:

		* Not do any operation that may result in the realtime
		thread to block operation at the kernel scheduler.
		This usually happens when doing a libcall/syscall
		that may set the process in wait state, such as
		handling files, sleeping, try to grab a blocking mutex, etc.

		* Warrant that setting a port is thread-safe, as in, setting the
		port from the UI or multiple threads will only result in setting
		a shared memory value, and the function can be safely be called
		simultaneously from them (many threads at the same time)

		* Make sure that the port doesnt need blocking. Some ports will attempt
		to block the internal state of the plugin when being set, to prevent
		the process() function to use that state until it's done reconfiguring it,
		because this may lead to a crash. Such ports cannot be made RT-Safe
		
*/


DSPLUG_Boolean DSPLUG_HOST_plugin_caps_is_control_realtime_safe( DSPLUG_HOST_Plugin_Caps * , int p_port_index );
/* return whether reading or writing from the control ports is REALTIME SAFE */

/*
	* Defaults and Parts *

	If the plugin is of type API_PLUGIN_TYPE_GENERIC,
	API_PLUGIN_TYPE_MULTIPART_SYNTHESIZER
	or API_PLUGIN_TYPE_MUSIC_EVENT_MATRIX then a port could
	be assignable to multiple parts. You can check if a port will
	work specifically for a part by calling this function.
	This is made so hosts can easily assign a control port
	to a part.

	The amount of PARTS that the plugin has is computed by checking
	how many MUSIC EVENT Ports does it have, multiplied by 16
	(-max- midi channels, and only if the port isnt on OMNI mode).
	Anyway, In the end these are just helpers for the host.. the host
	can do whathever it wants.
*/

int DSPLUG_HOST_plugin_caps_get_control_port_part( DSPLUG_HOST_Plugin_Caps * , int p_port_index );
/* return the part for which the port will work, a negative value is default and means: "works for any part" */

/*
	And finally, we can always retrieve the default value for a port:
	part can be ignored for non multipart plugins
*/

float DSPLUG_HOST_plugin_caps_get_control_port_default( DSPLUG_HOST_Plugin_Caps * , int p_port_index);
/* return the default value for the control port, -1.0 if the port is not floating point */

void DSPLUG_HOST_plugin_caps_get_control_port_default_string( DSPLUG_HOST_Plugin_Caps * , int p_port_index, const char **p_string );
/*
get the default value for the control port, which is set to p_string, a zero-terminated C-String
the string must be kept in memory by the plugin, since it is supposed to be a constant
the plugin will return NULL if the port is not of this type
*/



void DSPLUG_HOST_plugin_caps_get_control_port_default_raw_data( DSPLUG_HOST_Plugin_Caps * , int p_port_index, const void **p_raw_data, int *p_data_len );
/*
get the default value for the control port, which is set to p_data, with bytes-length to p_data_len
the data must be kept in memory by the plugin, as it is supposed to be a constant
the plugin will return NULL if the port is not of this type
*/



/****************************/

/* PLUGIN GENERAL CAPABILITIES */

/****************************/

/*
	**PLUGIN TYPE**

	As we have just seen, there can be a endless configurations for
	the plugins.  Not all hosts will be able to make use of all
	these configurations, so they need to know beforehand what
	they are dealing with. This is why the following function
	has been introduced:
*/

DSPLUG_Plugin_Type DSPLUG_HOST_plugin_caps_get_plugin_type( DSPLUG_HOST_Plugin_Caps * );
/*
returns the enumeration value for determining the plugin type
Check types.h for possible DSPLUG_Plugin_Type values
*/

/*

	Some plugin configurations can be instanced with any number of channels
	per audio port. This is up to the plugin to support, of course,
	Configurations that accept this are: API_PLUGIN_TYPE_AUDIO_ANALYZER,
	API_PLUGIN_TYPE_AUDIO_PROCESSOR, API_PLUGIN_TYPE_AUDIO_MODULATOR,
	API_PLUGIN_TYPE_AUDIO_SPLITTER, and API_PLUGIN_TYPE_AUDIO_GENERATOR.
	Since writing code that deals with any number of channels is harder
	to optimize in some algorithms, it is advised for plugin writers to
	write two versions of the processing loop: One optimized for
	mono/stereo. And another for any generic number or channels,
	again if this is the one algorithm that can be optimized better
	in such cases.

	Anyway, if the plugin supports this, it will have the following
	feature defined:  API_PLUGIN_FEATURE_VARIABLE_AUDIO_CHANNELS
	(see next)

*/

/*
	**PLUGIN FEATURES**

	Plugins can have many features, which can be individually tested
	using the following functions. The API is designed so a
	simple host can easily ignore any of these features, or
	just make good use of them if needed. 
*/

DSPLUG_Boolean DSPLUG_HOST_plugin_caps_has_feature( DSPLUG_HOST_Plugin_Caps * , DSPLUG_Plugin_Features);
/*
returns true if the feature is supported, false otherwise
see types.h for the list of features
*/

/*
	**PLUGIN CONSTANTS**

	Plugin limits allow you to check the integer "constant values" on what the plugins
	supports.
*/

int DPLUG_HOST_plugin_caps_get_constant( DSPLUG_HOST_Plugin_Caps * , DSPLUG_Plugin_Constant);
/* get a limit, see types.h for more information */


/****************************/

/* PLUGIN INSTANCE */

/****************************/

/*
	Instancing a plugin is very easy, the initial
	plugin handle, instance type and the sampling rate must be provided:

*/

DSPLUG_Plugin * DSPLUG_HOST_plugin_handle_create_plugin_instance( DSPLUG_HOST_Plugin_Handle * , int p_instance_type , int p_sampling_rate);
/* return the plugin */
/*
	Optionally, an audio plugin can be attempted to be instanced
	with different audio channel settings, only if it features:
	DSPLUG_PLUGIN_FEATURE_VARIABLE_AUDIO_CHANNELS
*/

DSPLUG_Plugin * DSPLUG_HOST_plugin_handle_create_plugin_instance_with_channels( DSPLUG_HOST_Plugin_Handle * , int p_instance_type , int p_sampling_rate, int p_num_channels);
/*  num_channels must be > 0 */


/*
	When no longer needed, we can destroy the instance by calling:
*/

void DSPLUG_HOST_destroy_plugin_instance( DSPLUG_Plugin * );

/*

	Of course, make sure the instance will no longer be used in another
	thread or similar, as it will immediately become invalid.
*/



/* SETTING UP AUDIO PORTS */

/*

	if the feature DSPLUG_PLUGIN_FEATURE_VARIABLE_AUDIO_CHANNELS
	you may want to check how many channels has the plugin has
	been initialized for later.. this is done using this:
*/

int DSPLUG_HOST_plugin_get_instanced_audio_channels( DSPLUG_Plugin * );
/*
return the number of instanced channels per ports
otherwise returns -1 if the plugin does not support this
*/

/*
	Now we may want to connect the ports to some buffers of us
*/ 

void DSPLUG_HOST_plugin_connect_audio_input_port( DSPLUG_Plugin * , int p_port_index, int p_channel, float *);
/* tell the port where it is going to get the data from */

void DSPLUG_HOST_plugin_connect_audio_output_port( DSPLUG_Plugin * , int p_port_index, int p_channel, float *);
/* tell the port where it is going to write the data to */
/*
	NOTE: from and to can be the same if API_PLUGIN_FEATURE_INPLACE is supported on API_PLUGIN_TYPE_AUDIO_PROCESSOR plugin.
*/
	
/* SETTING UP EVENT PORTS */

/*
	The number of event ports won't change depending on
	the way one instances the plugin. So anyway, we can
	just connect them.
*/

void DSPLUG_HOST_plugin_connect_event_input_port( DSPLUG_Plugin *, int p_port_index, DSPLUG_Event_Queue *p_queue_in);
/* connect input event queue */

void DSPLUG_HOST_plugin_connect_event_output_port( DSPLUG_Plugin *, int p_port_index, DSPLUG_Event_Queue *p_queue_out);
/* connect output event queue */
 
/*

	Check event_queue.h for detail on event queues

*/

/* SETTING UP CONTROL PORTS */


/*
	The instanced plugin will contain the control ports that
	we checked in the capabilities earlier. Reading and Writing
	to them is very easy:
*/

void DSPLUG_HOST_plugin_set_control_port( DSPLUG_Plugin * , int p_port_index , float p_val );
/* set a port with a float value from 0 to 1 */

void DSPLUG_HOST_plugin_set_control_port_string( DSPLUG_Plugin * , int p_port_index , const char * p_string );
/* set the port the value of the string */

void DSPLUG_HOST_plugin_set_control_port_data( DSPLUG_Plugin * , int p_port_index , const void * p_data, int p_data_len );
/* set a port with a data value of specified len */

float DSPLUG_HOST_plugin_get_control_port( DSPLUG_Plugin * , int p_port_index );
/* get a float value from 0 to 1 */

char * DSPLUG_HOST_plugin_get_control_port_string_no_realtime( DSPLUG_Plugin * , int p_port_index );
/*
 return a buffer containing the any-size C-String to be handled by the host (the plugin allocates the string for the host).
 THIS FUNCTION MUST NOT BE CALLED FROM A REALTIME THREAD
*/

 void DSPLUG_HOST_plugin_get_control_port_string( DSPLUG_Plugin * , int p_port_index , char * );
/*
 fill a buffer containing the string value
 it is RT safe, but limited only to DSPLUG_STRING_PORT_GET_MAX_LEN characters
*/

void DSPLUG_HOST_plugin_get_control_port_data( DSPLUG_Plugin * , int p_port_index , void ** p_data, int *p_data_len );
/*
 return a data chunk to be handled by the host
 raw data ports cant be realtime anyway so no worries
 THIS FUNCTION MUST NOT BE CALLED FROM A REALTIME THREAD, though these ports
 will never be able to be marked as RT-Safe anyway
*/


/****************************/

/* PROCESSING THE AUDIO */

/****************************/

/*
	Once we have all setup and all ports connected and configured,
	we are ready to use the plugin.
	The Plugin will process one block by simply calling: 
*/ 
		
void DSPLUG_HOST_plugin_process( DSPLUG_Plugin * , int p_frames );
/* process inputs and generate outputs (always replacing) */


/* RESETTING THE STATE */

/*
	When we stop playback, or want to reapply an effect to a buffer,
	or in many situations, we may want to reset the status of the plugin.
	This will happen in the case of audio plugins with history
	(reverb/chorus/echo/etc). Or midi synthesizer plugins
	(reset voices and stuff). In any case, this method will reset
	the plugin to it's initial status.
*/


void DSPLUG_HOST_plugin_reset( DSPLUG_Plugin * );


/*******************/

/* MISCELANEOUS	 */

/*******************/


/*

	Functions that dont belong anywhere else are docummented here,
	they should be usually be related to plugin capabilities that
	can only determined in realtime or once instanced
*/


int DSPLUG_HOST_plugin_get_output_delay( DSPLUG_Plugin * );
/*
 Get the output delay in frames. If the plugin performs FFT or some
 sort of internal buffering, which forces a delay in the output,
 or even a simple FIR filter that delays one frame.. it can be set here.
 By default, this will report 0 (zero, no delay)
*/

int DSPLUG_HOST_plugin_get_skipped_initial_frames( DSPLUG_Plugin * );
/*
 Returns the skipped initial frames when processing, some plugins will ask you
 to skip some of the initial frames of processing because they are junk
 default is 0 (zero, no skip)
*/

/**********************/

/* USER INTERFACE  */

/**********************/



#endif /* DSPlug Host Header */
