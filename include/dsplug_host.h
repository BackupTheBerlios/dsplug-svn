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
  * \file dsplug_host.h
  * \author Juan Linietsky
  */

#ifndef DSPlug_host_H
#define DSPlug_host_H


#include "dsplug_types.h"
#include "dsplug_plugin_caps.h"

/****************************/

/* PLUGIN LIBRARY */

/****************************/

/**
  *	Open a Plugin Library (plugin access object and shared data for
  *     the plugin instances).
  *     This warranties that the library on which the plugin is remains open,
  *	and, optionally, mantain a reference count to the times the plugin
  *	is accessed.
  *	\param p path to plugin library file
  *
  */

DSPlug_PluginLibrary * DSPlug_Host_open_plugin_library( const char * p );

/**
  *	Close a plugin handle. Call this when you dont want to use the plugin anymore.
  *     WARNING: This will invalidate any open plugin instances!
  *
  */

void DSPlug_Host_close_plugin_library( DSPlug_PluginLibrary * );


/**
 *	A plugin library can have many plugins inside
 *	as in.. an equalize plugin may contain 6,8,10,21,31 bands
 *	versions, as well as a plugin having a low quality and
 *	hi quality version, and so on. Or just a library of misc plugins.
 *	The amount of supported plugins can be retrived with this function
 * 	WARNING: The index of a plugin inside the library can change freely, so
 * 		ALWAYS reference it for it's unique_ID+version ONLY.
 *	\return a number equal or greater to one
 */

int DSPlug_PluginLibrary_get_plugin_count( DSPlug_PluginLibrary * );

/**
 *	Using the handle, one can instance the plugin capabilities object
 *	for one of multiple plugins of the library.
 *	This is used to obtain all sorts of static information on the specific plugin
 *	without the need of instancing one. This object exists for the life of
 *	the library and does not need to be deinitialized.
 *	\param i plugin index
 *	\return plugin capabilities, NULL on error
 */

DSPlug_PluginCaps DSPlug_PluginLibrary_get_plugin_caps( DSPlug_PluginLibrary * , int i );

/**
	This will instance a plugin from the plugin library. The plugin is
	what it does the actual processing. We can hint the plugin that the
	host is or not going to be using the graphical interface. If the plugin
	cant work at all without the user interface, then will let API
	know and NULL will be returned here.
	\param i plugin index
	\param r sampling rate at which the plugin will work at
	\param ui instance the plugin UI hint, if true is sent, the plugin can instnce the UI too. Plugins without UI can ignore this.
	\return a plugin instance, NULL on error.


*/
DSPlug_PluginInstance * DSPlug_PluginLibrary_get_plugin_instance( DSPlug_PluginLibrary * , int i , int r, DSPlug_Boolean ui);

/**
 *	Uninitialize a plugin. Call this when you dont need it anymore
 *	WARNING: DONT CALL THIS IF ANOTHER THREAD IS STILL USING THE PLUGIN
 *	\param p plugin
 */

void DSPlug_PluginLibrary_destroy_plugin_instance( DSPlug_PluginLibrary * , DSPlug_PluginInstance * p);

/****************************/

/* PLUGIN INSTANCE */

/****************************/

/* SETTING UP AUDIO PORTS */



/**
 *	Connect an audio port channel to a given audio buffer.
 *	If the port is bidirectional, then the processing is done inplace.
 *
 *	\param i audio port index
 *	\param c audio channel
 *	\param b buffer to float values
 */

void DSPlug_PluginInstance_connect_audio_port( DSPlug_PluginInstance * , int i, int c, float * b);

/**
 *	Connect an event port to a given event queue
 *	If the port is bidirectional, then the processing is done inplace,
 *      as in, the port will free the queue and fill it with new events.
 *	If not, you can safely feed the same input event queue instance to
 *	many plugins, as they should not modify it. This is useful
 *	for connecting the mastertrack and audio master event queues.
 *
 *	\param i event port index
 *	\param q event queue channel
 */

void DSPlug_PluginInstance_connect_event_port( DSPlug_PluginInstance *, int i, DSPlug_EventQueue *q );
/* connect input event queue */


/* SETTING UP CONTROL PORTS */

/**
 *	Set a numerical value to the port, from 0 to 1
 *	This is ignored on output ports.
 *	If the port supports realtime, this can safely called on a RT-Thread
 *
 *	\param i control port index
 *	\param v value as float, from 0.0f to 1.0f
 */
void DSPlug_PluginInstance_set_control_numerical_port( DSPlug_PluginInstance * , int i , float v );

/**
 *	Set a string value.
 *	This is ignored on output ports.
 *	If the port supports realtime, this can safely called on a RT-Thread
 *
 *	\param i control port index
 *	\param s constant pointer to a C-String, the plugin is not expected to keep it
 */
void DSPlug_PluginInstance_set_control_string_port( DSPlug_PluginInstance * , int i , const char * s );

/**
 *	Set a data value.
 *	This is ignored on output ports.
 *	If the port supports realtime, this can safely called on a RT-Thread
 *
 *	\param i control port index
 *	\param d data pointer, the plugin is not expected to keep it
 *	\param l data length in bytes
 */
void DSPlug_PluginInstance_set_control_data_port( DSPlug_PluginInstance * , int i , const void * d, int l );

/**
 *	Get a numerical value from the port, from 0 to 1
 *	This is ignored on input ports.
 *	If the port supports realtime, this can safely called on a RT-Thread
 *
 *	\param i control port index
 *	\return a float value, from 0.0f to 1.0f
 */
float DSPlug_PluginInstance_get_control_numerical_port( DSPlug_PluginInstance * , int i );


/**
 *	Get a C-String, the data is given for you to handle (free it yourself)
 *      Some complex plugins can use this for saving their configuration in text-based format
 *	You must use this function to get strings if the port is NOT defined realtime.
 *	WARNING THIS FUNCTION CANT BE CALLED ON A REALTIME THREAD!
 *
 *	\param i control port index
 *	\return a C-String, the host is in charge of freeing it
 */
char * DSPlug_PluginInstance_get_control_string_port( DSPlug_PluginInstance * , int i );

/**
 *	Get a string value. The max length of this string is predetermined by the plugin
 *	This is ignored on output ports.
 *	You must use this function instead of the above one when the ports are realtime.
 *
 *	\param i control port index
 *	\param s pointer to a char buffer of size specified in: DSPlug_PluginInstance_get_control_string_port_realtime_max_length
 */

 void DSPlug_PluginInstance_get_control_string_port_realtime( DSPlug_PluginInstance * , int i , char * s );

 /**
  *	Get the max length of the realtime port string. This is fixed for
  *	the port, so it is warranted that it is realtime safe.
  *	This function only works for reealtime string ports.
  *
  *	\param i control port index
  *	\return the length in characters for the string buffer + 0 chracter
  */

 int DSPlug_PluginInstance_get_control_string_port_realtime_max_length( DSPlug_PluginInstance * , int i );

/**
 *	Get a data chunk, this can only be used in non realtine,
 *	the data can have any length. Non editable ports
 *      may use this for saving their configuration in binary
 *	format.
 *	This is ignored on input ports.
 *	WARNING THIS FUNCTION CANT BE CALLED ON A REALTIME THREAD!
 *
 *	\param i control port index
 *	\param d pointer to pointer that will be set to the data location
 *	\param l pointer to a variable that will be set with the length of the data
 */
void DSPlug_PluginInstance_get_control_port_data( DSPlug_PluginInstance * , int i , void ** d, int * l );


/**
 *	Set a callback to be called when an output/bidi control port has changed inside the plugin.
 *	If the output control port is realtime enabled, then this method may have been
 *      called from the RT-Thread, so apply the usual cautions to it for RT-Thread Programming.
 *      may use this for saving their configuration in binary
 *	format.
 *	This is ignored on input ports.
 *	WARNING THIS FUNCTION CANT BE CALLED ON A REALTIME THREAD!
 *
 *	\param i control port index
 *	\param c callback function to be called
 *	\param u userdata, this pointer will be returned as parameter in the callback
 */
void DSPlug_PluginInstance_set_UI_changed_control_port_callback( DSPlug_PluginInstance * , int i , void (*c)(int, void *) , void * u);


/****************************/

/* PROCESSING THE AUDIO */

/****************************/

/**
	Once all is setup and all the ports are connected and configured,
	the plugin is ready to process data. This function
	will process a given amount of frames.
	\param f amount of frames to process

*/

void DSPlug_PluginInstance_process( DSPlug_PluginInstance * , int f );


/* RESETTING THE STATE */

/**
 *	When the host stops playback, or wants to reapply an effect to a buffer,
 *	or in many situations, it may want to reset the status of the plugin.
 *	This will happen in the case of audio plugins with history
 *	(reverb/chorus/echo/etc). Or midi synthesizer plugins
 *	(reset voices and stuff). In any case, this method will reset
 *	the plugin to it's initial status.
 */

void DSPlug_PluginInstance_reset( DSPlug_PluginInstance * );


/*******************/

/* MISCELANEOUS	 */

/*******************/


/*

	Functions that dont belong anywhere else are docummented here,
	they should be usually be related to plugin capabilities that
	can only determined in realtime or once instanced
*/

/**
 *	Get the output delay in frames. If the plugin performs FFT or some
 *	sort of internal buffering, which forces a delay in the output,
 *	or even a simple FIR filter that delays one frame.. it can be set here.
 *	By default, this will report 0 (zero, no delay)
 *
 *	\return amount of frames that the plugin delays the sound.
*/

int DSPlug_PluginInstance_get_output_delay( DSPlug_PluginInstance * );


/**********************/

/* USER INTERFACE  */

/**********************/



#endif /* DSPlug Host Header */
