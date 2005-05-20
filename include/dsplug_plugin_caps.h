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

/**
 * \file dsplug_plugin_caps.h
 * \author Juan Linietsky
 */
#ifndef DSPLUG_PLUGIN_CAPS_H
#define DSPLUG_PLUGIN_CAPS_H


/****************************/

/* PLUGIN CAPABILITIES */

/****************************/


/**
 *	Get the caption of the plugin. This is the visual string for identifying the plugin.
 *	For example: "Johnny's Ultimate Sound Madness Creator!!!"
 *	\param s pointer to a char buffer of size: DSPLUG_STRING_MAX_LEN
 *
 */

void DSPlug_PluginCaps_get_caption( DSPlug_PluginCaps, char * s );


/**
 *	Get the plugin author string.
 *	For example: "John R. Maniac"
 *	\param s pointer to a char buffer of size: DSPLUG_STRING_MAX_LEN
 *
 */

void DSPlug_PluginCaps_get_author( DSPlug_PluginCaps, char * s );

/**
 *	Get the plugin copyright string.
 *	For example: "© 2016 John F. Maniac"
 *	\param s pointer to a char buffer of size: DSPLUG_STRING_MAX_LEN
 *
 */

void DSPlug_PluginCaps_get_copyright( DSPlug_PluginCaps, char * s );

/**
 *	Get the plugin version string.
 *	The naming scheme doesn't matter, but a newer version must be alphanumerically greater
 *	than an older version if compared , like:  "0.99.31a" is lesser than "1.02.32".
 *	"3.10" is lesser than "3.9" so care must be taken, use "3.09" instead.
 *	\param s pointer to a char buffer of size: DSPLUG_STRING_MAX_LEN
 *
 */
void DSPlug_PluginCaps_get_version( DSPlug_PluginCaps, char * s );

/**
 *	Get the version up to where this plugin is compatible, it must be minor
 *	or equal than the current version.
 *	This is done to avoid incompatibilities if a new version of the plugin adds more ports, channels, etc.
 *	\param s pointer to a char buffer of size: DSPLUG_STRING_MAX_LEN
 *
 */
void DSPlug_PluginCaps_get_compatible_version( DSPlug_PluginCaps, char *s );


/**
 *	Return the Unique ID string for this plugin. Write wathever you want for development
 *	that doesnt conflict with an existing naming scheme. Official unique IDs can be
 *	requested to http://www.dsplug.org.
 *	NOTE: To identify a plugin (or saving the identification for later reopening)
 *	   ALWAYS use ID _AND_ VERSION.
 *	\param s pointer to a char buffer of size: DSPLUG_STRING_MAX_LEN
 *
 */
void DSPlug_PluginCaps_get_unique_ID( DSPlug_PluginCaps, char *s );

/**
 *	Return a short description for the plugin, so users can find out
 *	what this plugin is meant for.
 *	\param s pointer to a char buffer of size: DSPLUG_STRING_MAX_LEN
 *
 */
void DSPlug_PluginCaps_get_description( DSPlug_PluginCaps, char *s );

/**
 *	Return location of the home page url, for obtaining docummentation and info.
 *	If the host is connected to the internet, then it can open a web page
 * 	with the full and more complete docummentation for this plugin.
 * 	for example: "http://www.myplugin.org/doc", or "" for no URL.
 *	\param s pointer to a char buffer of size: DSPLUG_STRING_MAX_LEN
 * *
 */
void DSPlug_PluginCaps_get_HTTP_URL( DSPlug_PluginCaps, char *s );

/**
 *	Return the category path on where the plugin shall be displayed.
 *	As well as plugin IDs , categories will be defined by the standard mantainer
 *	\param s pointer to a char buffer of size: DSPLUG_STRING_MAX_LEN
 *
 */
void DSPlug_PluginCaps_get_category_path( DSPlug_PluginCaps, char *s );

/**
 *	Plugin Usage Hint:
 *
 *	As we have just seen, there can be a endless configurations for
 *	the plugins.  Not all hosts will be able to make use of all
 *	these configurations, so they need to know beforehand what
 *	they are dealing with. This is why this function has been introduced.
 *
 *	\return the plugin usage hint, which must be defined by plugins.
 */
DSPlug_PluginUsageHint DSPlug_PluginCaps_get_plugin_usage_hint( DSPlug_PluginCaps );

/**
 *	Plugin Features:
 *
 *	Plugins can have many features, which can be individually tested
 *	using the following functions. The API is designed so a
 *	simple host can easily ignore any of these features, or
 *	just make good use of them if needed.
 *	\param f feature from enum
 *	\return true if the feature is supported, false otherwise
 */

DSPlug_Boolean DSPlug_PluginCaps_has_feature( DSPlug_PluginCaps , DSPlug_PluginFeature f);

/**
 *	Plugin Constants:
 *
 *	Plugin limits allow you to check the integer "constant values" on what the plugins
 *	supports,
 *	\param c constant from enum
 * 	\return zero or greater positive constant, DSPLUG_NO_CONSTANT if constant is not defined by the plugin.
 */
int DSPlug_PluginCaps_get_constant( DSPlug_PluginCaps , DSPlug_PluginConstant c);

/****************************/

/* GENERAL PORT CAPS */

/****************************/


/**
 *	Get the amount of ports in the plugin
 *	\param t type of ports being accessed
 *	\return an integer value containing the number of ports
 *
 */

 int DSPlug_PluginCaps_get_port_count( DSPlug_PluginCaps, DSPlug_PortType t);


/**
  *	Get the caption of the port, the one used for user visibility
  *	\param t type of ports being accessed
  *	\param i the port index, begining from zero
  *	\param s pointer to a char buffer of size: DSPLUG_STRING_MAX_LEN
  *
  *	If the port index is invalid, an error message will be sent to stderr, and the function will do nothing.  By design, the API does not offer ways to handle this silly level of human errors.
 */

 void DSPlug_PluginCaps_get_port_caption( DSPlug_PluginCaps, DSPlug_PortType t, int i, char * s );


/**
  *	Get the name of the port, this name should be used as reference for saving the
  *	status of a port (and NOT the index). In the case of a port supporting multipart
  *	then the port name and the part index have to be saved.
  *	\param t type of ports being accessed
  *	\param i the port index, begining from zero
  *	\param s pointer to achar buffer of size: DSPLUG_STRING_MAX_LEN
  *
  *	If the port index is invalid, an error message will be sent to stderr, and the function will do nothing.  By design, the API does not offer ways to handle this silly level of human errors.
 */

 void DSPlug_PluginCaps_get_port_name( DSPlug_PluginCaps, DSPlug_PortType t, int i, char * s );


/**
  *	Get the "plug" type of the port, it can be input, output or both
  *	\param t type of ports being accessed
  *	\param i the port index, begining from zero
  *	\return the "plug" type
  *
  *	If the port index is invalid, an error message will be sent to stderr, and the function will do nothing. By design, the API does not offer ways to handle this silly level of human errors.
  *	The return value, can then be ignored.
 */

 DSPlug_PlugType DSPlug_PluginCaps_get_port_plug_type( DSPlug_PluginCaps, DSPlug_PortType t, int i);


/**
  *	Get the port path. This is useful for organizing the ports in the host,
  *	so they can be shown as a tree, or directories. The path will use "/" as
  *	separator. Example paths are:
  *		- "/"
  *		- "/filter/cutoff" , "/filter/resonance"
  *		- "/oscil/saw"
  *	\param t type of ports being accessed
  *	\param i the port index, begining from zero
  *	\param s pointer to achar buffer of size: DSPLUG_STRING_MAX_LEN
  *
  *	If the port index is invalid, an error meessage will be sent to stderr, and the function will do nothing. By design, the API does not offer ways to handle this silly level of human errors.
 */

 void DSPlug_PluginCaps_get_port_path( DSPlug_PluginCaps, DSPlug_PortType t, int i, char * s);

/**
  *	Get AUDIO port capabilities.
  *	\param i the audio port index, begining from zero
  *	\return an audio port capabilities object
  *
  *	If the port type or index is invalid, an error meessage will be sent to stderr, and the function will do nothing.  By design, the API does not offer ways to handle this silly level of human errors.
  *	The return value, can then be ignored.
 */

 DSPlug_AudioPortCaps DSPlug_PluginCaps_get_audio_port_caps( DSPlug_PluginCaps, int i );

/**
  *	Get EVENT port capabilities.
  *	\param i the event port index, begining from zero
  *	\return an event port capabilities object
  *
  *	If the port type or index is invalid, an error meessage will be sent to stderr, and the function will do nothing.  By design, the API does not offer ways to handle this silly level of human errors.
  *	The return value, can then be ignored.
 */

 DSPlug_EventPortCaps DSPlug_PluginCaps_get_event_port_caps( DSPlug_PluginCaps, int i );

/**
  *	Get CONTROL port capabilities.
  *	\param i the control port index, begining from zero
  *	\return an control port capabilities object
  *
  *	If the port type or index is invalid, an error meessage will be sent to stderr, and the function will do nothing.  By design, the API does not offer ways to handle this silly level of human errors.
  *	The return value, can then be ignored.
 */

 DSPlug_ControlPortCaps DSPlug_PluginCaps_get_control_port_caps( DSPlug_PluginCaps, int i );

 /****************************/

 /* AUDIO PORT CAPS */

 /****************************/

/**
  *	Get the amount of channels for the audio port.
  *	Audio ports can have any number of channels.
  *	\return number of channels, starting from 1
  *
 */

 int DSPlug_AudioPortCaps_get_channel_count( DSPlug_AudioPortCaps );


 /****************************/

 /* EVENT PORT CAPS */

 /****************************/

/**
  *	Get the type of events handled by the event port
  *	\return type of event enum
  *
  *	If the port type or index is invalid, an error meessage will be sent to stderr, and the function will do nothing.  By design, the API does not offer ways to handle this silly level of human errors.
  *	The return value, can then be ignored.
 */

 DSPlug_EventType DSPlug_EventPortCaps_get_event_type( DSPlug_EventPortCaps );

 /****************************/

 /* CONTROL PORT CAPS */

 /****************************/

/**
  *	Get the control port value type. Supported types are numerical, string, data.
  *	\return the value type.
 */

 DSPlug_ControlPortType DSPlug_ControlPortCaps_get_type( DSPlug_ControlPortCaps );

 /* Numerical Ports */
/**
  *	Get the numerical port default value.
  *	\returns floating point value between 0 and 1
  *
  *	If the control port is not of type numerical-enum, then an error message is sent to stderr and the function does nothing. The return value can also be ignored. By design, the API does not offer ways to handle this silly level of human errors.
 */

 float DSPlug_ControlPortCaps_get_numerical_default( DSPlug_ControlPortCaps );

/**
  *	Determine how a numerical port must display a value.
  *	Just pass a regular port value (from 0 to 1) and obtain the
  *	display string. Example: you pass "0.33" then obtain "1240 hz",
  *      "33%" , "-9.6 dB", etc. The plugin determines h
  *	\param v real value from 0 to 1
  *	\param s pointer to achar buffer of size: DSPLUG_STRING_MAX_LEN
  *
  *	If the control port is not of type numerical, then an error message is sent to stderr and the function does nothing. By design, the API does not offer ways to handle this silly level of human errors.
 */

 void DSPlug_ControlPortCaps_get_numerical_display( DSPlug_ControlPortCaps , float v , char *s);

/**
  *	Get the edition hint for the numerical port, this helps the host
  *	to determine how the numerical port will be edited.
  *	\return hint to determine how the numerical port is used
  *
  *	If the control port is not of type numerical, then an error message is sent to stderr and the function does nothing. By design, the API does not offer ways to handle this silly level of human errors.
 */

 DSPlug_ControlPortNumericalHint DSPlug_ControlPortCaps_get_numerical_hint( DSPlug_ControlPortCaps );


 /* Numerical Port - Integer Options */

/**
  *	Get the steps for the integer value.
  * 	NOTE: **WARNING** When saving the port value, *ALWAYS* save either both the port value and the steps, or the integer final value. This way if the plugin adds more steps (as in, more possible values) you can remain compatible with it.
  *	\return amount of steps
  *
  *	If the control port is not of type numerical-integer, then an error message is sent to stderr and the function does nothing. The return value can also be ignored. By design, the API does not offer ways to handle this silly level of human errors.
 */

 int DSPlug_ControlPortCaps_get_numerical_integer_steps( DSPlug_ControlPortCaps );

/**
  *	Check if the integer port is actually an enumerated value (a set of options).
  * 	This is just to hint the host on a better way to display it to the user.
  *
  *	If the control port is not of type numerical-integer, then an error message is sent to stderr and the function does nothing. The return value can also be ignored. By design, the API does not offer ways to handle this silly level of human errors.
 */

 DSPlug_Boolean DSPlug_ControlPortCaps_is_numerical_integer_enum( DSPlug_ControlPortCaps );

 /* String Port */

/**
  *	Get the default value for the string control port as a zero-terminated C-String
  *	the string will be kept in memory by the plugin, since it is supposed to be a constant
  *	the plugin will return NULL if the port is not of this type.
  *	\return a constant string for the default value, NULL on error.
 */
 const char * DSPlug_ControlPortCaps_get_string_default( DSPlug_ControlPortCaps );

 /* Data Port */

/**
  *	Get the default value for the control port, and its length.
  *	The data will be kept in memory by the plugin, as it is supposed to be a constant
  *	the plugin will return NULL if the port is not of this type.
  *	\param l pointer the length of the data, will be set by the function if no error occurs
  *	\return pointer constant to the data
 */

 const void* DSPlug_ControlPortCaps_get_raw_data_default( DSPlug_ControlPortCaps , int *l );

 /* Misc Port Settings */

/**
  *
  *	Return true if the plugin does not want the host-user to know about
  * 	the existence of this port. If so, then this port will only used
  * 	when saving/restoring the status of the plugin.
  *	\return true if the port can be editable by the host
 */

 DSPlug_Boolean DSPlug_ControlPortCaps_is_hidden( DSPlug_ControlPortCaps );

/**
  *
  *	Many hosts will attempt to change control ports during playback,
  *	to assign different parameters during the length of a song/sound/etc
  *	This is perfectly normal, but also many hosts will need to do so
  *	from a realtime thread. Since realtime-safety and thread-safety
  *	may be needed, plugins may define when they are safe to be set
  *	and read by defining the realtime safe flag.
 *	If the plugin defines this it means that it will:
  *
  *		* Not do any operation that may result in the realtime
  *		thread to block operation at the kernel scheduler.
  *		This usually happens when doing a libcall/syscall
  *		that may set the process in wait state, such as
  *		handling files, sleeping, try to grab a blocking mutex, etc.
  *
  *		* Warrant that setting a port is reentrant and thread-safe, as in, setting the
  *		port from the UI or multiple threads will only result in setting
  *		a shared memory value, and the function can be safely be called
  *		simultaneously from them (many threads at the same time)
  *
  *		* Make sure that the port doesnt need blocking. Some ports will attempt
  *		to block the internal state of the plugin when being set, to prevent
  *		the process() function to use that state until it's done reconfiguring it,
  *		because this may lead to a crash. Such ports cannot be made RT-Safe
  *
  *	\return true if the port can be set in realtime, false otherwise
 */

 DSPlug_Boolean DSPlug_ControlPortCaps_is_realtime_safe( DSPlug_ControlPortCaps );

/**
  *
  *	If a port has many MIDI event input ports, then you may want to
  *	let the host know that a certain port will only affect the processing
  *	of a certain MIDI channel.
  *	You can check if a port will work specifically for a music part
  *	by calling this function.
  *	This is made as a hint for hosts, so they can easily assign a control port
  *	to a music part.
  *	Currently this only works for MIDI, but in the future, DSPlug may work with other
  *	music formats.
  *
  *
  *	The amount of PARTS that the plugin has is computed by checking
  *	how many MUSIC EVENT Ports does it have, multiplied by 16
  *	(-max- midi channels, and only if the port isnt on OMNI mode).
  *	Anyway, In the end these are just helpers for the host.. the host
  *	can do whathever it wants.
  *	\return the part number if it is assigned to one, or a negative value otherwise
 */

 int DSPlug_ControlPortCaps_get_music_part( DSPlug_ControlPortCaps );




#endif /* plugin caps.h */
