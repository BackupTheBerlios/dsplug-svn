/**
 * \file dsplug_plugin.h
 * \author Juan Linietsky
 */

#ifndef DSPlUG_PLUGIN_H
#define DSPlUG_PLUGIN_H


/**************************
* Plugin Library Creation *
***************************/
/**
 * Instance a Plugin Creation object from a Library Creation.
 * This object is used to, later on, create a plugin and it's capabilities.
 *
 * \return a plugin creation ojbect
 */
DSPlug_PluginCreation * DSPlug_LibraryCreation_instance_plugin_creation( DSPlug_LibraryCreation );

/**
 * If something happens in the middle of the creation process which makes the plugin creation
 * impossible, then the process can be aborted by calling this function
 *
 * \param pc plugin creation object
 */
void DSPlug_LibraryCreation_abort_plugin_creation(DSPlug_LibraryCreation, DSPlug_PluginCreation * pc);

/**
 * Add the newly defined plugin to the plugin library.
 * If the plugin hasnt been properly created (misses calling some calling functions mainly)
 * the return value will be positive, indicating error, and the reason will be sent to stderr.
 * To avoid this to happen, be sure to call all the functions that include the tag *REQUIRED*
 * in the docummentation.
 * \param pc plugin creation object
 */
DSPlug_Boolean DSPlug_LibraryCreation_add_plugin( DSPlug_LibraryCreation , DSPlug_PluginCreation * pc );


/******************
* Plugin Creation *
*******************/

 /**
 * Set the caption of the plugin. This is the visual string for identifying the plugin.
 * For example: "Johnny's Ultimate Sound Madness Creator!!!"
 * \param s caption
 *
 */
void DSPlug_PluginCreation_set_caption( DSPlug_PluginCreation * , const char *s );

/**
 * Set the plugin author string.
 * For example: "John R. Maniac"
 * \param s pointer to a C-string of max length (including ending 0 char ) DSPLUG_STRING_MAX_LEN. If larger than this, it will be clipped.
 *
 */
void DSPlug_PluginCreation_set_author( DSPlug_PluginCreation * , const char *s );

/**
 * Set the plugin copyright string.
 * For example: "© 2016 John F. Maniac"
 * \param s pointer to a C-string of max length (including ending 0 char ) DSPLUG_STRING_MAX_LEN. If larger than this, it will be clipped.
 *
 */
void DSPlug_PluginCreation_set_copyright( DSPlug_PluginCreation * , const char *s );
/**
 * Set the plugin version string.
 * The naming scheme doesn't matter, but a newer version must be alphanumerically greater
 * than an older version if compared , like:  "0.99.31a" is lesser than "1.02.32".
 * "3.10" is lesser than "3.9" so care must be taken, use "3.09" instead.
 * \param s pointer to a C-string of max length (including ending 0 char ) DSPLUG_STRING_MAX_LEN. If larger than this, it will be clipped.
 *
 */
void DSPlug_PluginCreation_set_version( DSPlug_PluginCreation * , const char *s );

/**
 * Set the version up to where this plugin is compatible, it must be minor
 * or equal than the current version.
 * This is done to avoid incompatibilities if a new version of the plugin adds more ports, channels, etc.
 * \param s pointer to a C-string of max length (including ending 0 char ) DSPLUG_STRING_MAX_LEN. If larger than this, it will be clipped.
 *
 */
void DSPlug_PluginCreation_set_compatible_version( DSPlug_PluginCreation * , const char *s );
/**
 * Set the Unique ID string for this plugin. Write wathever you want for development
 * that doesnt conflict with an existing naming scheme. Official unique IDs can be
 * requested to http://www.dsplug.org.
 * NOTE: To identify a plugin (or saving the identification for later reopening)
 *    ALWAYS use ID _AND_ VERSION.
 * \param s pointer to a C-string of max length (including ending 0 char ) DSPLUG_STRING_MAX_LEN. If larger than this, it will be clipped.
 *
 */
void DSPlug_PluginCreation_set_unique_ID( DSPlug_PluginCreation * , const char *s ); /**< *REQUIRED* */

/**
 * Set a short description for the plugin, so users can find out
 * what this plugin is meant for.
 * \param s pointer to a C-string of max length (including ending 0 char ) DSPLUG_STRING_MAX_LEN. If larger than this, it will be clipped.
 *
 */
void DSPlug_PluginCreation_set_description( DSPlug_PluginCreation * , const char *s );

/**
 * Set location of the home page url, for obtaining docummentation and info.
 * If the host is connected to the internet, then it can open a web page
 * with the full and more complete docummentation for this plugin.
 * for example: "http://www.myplugin.org/doc", or "" for no URL.
 * \param s pointer to a C-string of max length (including ending 0 char ) DSPLUG_STRING_MAX_LEN. If larger than this, it will be clipped.
 *
 */
void DSPlug_PluginCreation_set_HTTP_URL( DSPlug_PluginCreation * , const char *s );

/**
 * Set the category path on where the plugin shall be displayed.
 * As well as plugin IDs , categories will be defined by the standard mantainer of the API
 * \param s pointer to a C-string of max length (including ending 0 char ) DSPLUG_STRING_MAX_LEN. If larger than this, it will be clipped.
 *
 */
void DSPlug_PluginCreation_set_category_path( DSPlug_PluginCreation * , const char *s );


void DSPlug_PluginCreation_set_usage_hint( DSPlug_PluginCreation * , DSPlug_PluginUsageHint h );

/**
 * Plugin Features:
 *
 *	Plugins can have many features, which can be individually tested
 *	using the following functions. The API is designed so a
 *	simple host can easily ignore any of these features, or
 *	just make good use of them if needed.
 *
 * Check which of the features the plugin supports and set them accordingly.
 *	\param f feature from enum
 */
void DSPlug_PluginCreation_add_feature( DSPlug_PluginCreation * , DSPlug_PluginFeature f );

/**
 * Plugin Constants:
 *
 *	Plugin limits allow you to check the integer "constant values" on what the plugins
 *	supports.
 * Here you can set the constant and its value.
 *	\param c constant from enum
 * 	\param cv value for the constant
 */

void DSPlug_PluginCreation_add_constant( DSPlug_PluginCreation * , DSPlug_PluginConstant c ,int cv );


 /**
 * Add an audio port to the plugin.
 * \param plug Plug type (input/output)
 * \param label Visual string for the port
 * \param name Name of the port, this is UNIQUE for all audio ports
 * \param path UNIX-Like Path of the port, for example "/" (global value), "/filter" (part of a filter) "/oscilA" (Part of oscillator A"). "/" is automatically prepended to any value you pass. The same way, "/" is removed from the end of a sublocation. So passing "" and "/" , or "filter", "/filter", "filter/" and "/filter/" are all the same.
 * \param ch Amount of audio channels that this port has
 */
void DSPlug_PluginCreation_add_audio_port( DSPlug_PluginCreation * , DSPlug_PlugType plug, const char *label, const char *name , const char *path, int ch );

/**
 * Add an event port to the plugin.
 * \param plug Plug type (input/output)
 * \param label Visual string for the port
 * \param name Name of the port, this is UNIQUE for all audio ports
 * \param path UNIX-Like Path of the port, for example "/" (global value), "/filter" (part of a filter) "/oscilA" (Part of oscillator A"). "/" is automatically prepended to any value you pass. The same way, "/" is removed from the end of a sublocation. So passing "" and "/" , or "filter", "/filter", "filter/" and "/filter/" are all the same.
 * \param evt Type of the event (check available types)
 */
void DSPlug_PluginCreation_add_event_port( DSPlug_PluginCreation * , DSPlug_PlugType plug, const char *label, const char *name , const char *path, DSPlug_EventType evt );

/**
 * Add a control port to the plugin. If the port plug is if of type input,
 * then it means that it will receive parameters from the host.
 * If of type output, then it means that host can get from here
 * information that was computed inside the plugin.
 * The host will use the information of input ports when
 * saving the status of the plugin.
 *
 * \param plug adds the hint wether this is an input or output port
 * \param label Visual string for the port
 * \param name Name of the port, this is UNIQUE for all audio ports
 * \param path UNIX-Like Path of the port, for example "/" (global value), "/filter" (part of a filter) "/oscilA" (Part of oscillator A"). "/" is automatically prepended to any value you pass. The same way, "/" is removed from the end of a sublocation. So passing "" and "/" , or "filter", "/filter", "filter/" and "/filter/" are all the same.
 * \param cpc Control port creation object.
 */
void DSPlug_PluginCreation_add_control_port( DSPlug_PluginCreation * , DSPlug_PlugType plug, const char *label, const char *name , const char *path, DSPlug_ControlPortCreation * cpc );

/**
 * Instance a control port creation object. For the specific type of port.
 * \param set_cbk User-Callback for setting a value
 * \param get_cbk User-Callback for getting a value
 * \param disp_func User-Callback for the display function, if supplied, it hints the host on how to display the port.
 * \return a Control Port Creation instance.
*/
DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_numerical_float( void (*set_cbk)(DSPlug_Plugin , int, float) ,  float (*get_cbk)(DSPlug_Plugin , int) , void (*disp_func)(float, char *)=NULL);
/**
 * Instance a control port creation object. For the specific type of port.
 * \param steps Integer steps. This is to hint the host that you want values of type integer, and the range of them. The actual value is obtained by doing port_value*steps formula.
 * \param is_enum Set to true if the port must be displayed as an ennumerated options value.
 * \param set_cbk User-Callback for setting a value
 * \param get_cbk User-Callback for getting a value
 * \param disp_func User-Callback for the display function, if supplied, it hints the host on how to display the port.
 * \return a Control Port Creation instance.
 */
DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_numerical_integer( int steps, DSPlug_Boolean is_enum, void (*set_cbk)(DSPlug_Plugin , int, float) ,  float (*get_cbk)(DSPlug_Plugin , int) , void (*disp_func)(float, char *)=NULL);
/**
 * Instance a control port creation object. For the specific type of port.
 * \param set_cbk User-Callback for setting a value
 * \param get_cbk User-Callback for getting a value
 * \param disp_func User-Callback for the display function, if supplied, it hints the host on how to display the port.
 * \return a Control Port Creation instance.
 */

DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_numerical_bool( void (*set_cbk)(DSPlug_Plugin , int, float) , float (*get_cbk)(DSPlug_Plugin , int) , void (*disp_func)(float, char *)=NULL);

/**
 * Instance a control port creation object. For the specific type of port.
 * When using this function, a normal non-realtime able port will be created,
 * trying to set the port realtime property later will have no effect. The advantage
 * of this kind of port is that it has not limits on the length of the string,
 * the disadvantage is that -due to this- you will not be able to use it on a RT-Thread.
 * For realtime ports, use:
 * 	DSPlug_ControlPortCreation_create_string_realtime
 * \param set_cbk User-Callback for setting a value
 * \param get_cbk User-Callback for getting a value
 * \return a Control Port Creation instance.
 */
 DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_string( (*set_cbk)(DSPlug_Plugin , int, const char*) , char * (*get_cbk)(DSPlug_Plugin , int) );

 /**
  * Instance a control port creation object. For the specific type of port.
  * When using this function, a normal realtime able port will be created.
  * This port can be used to retrieve string information in realtime-threads.
  * 	*WARNING*: Trying to use sprintf or *printf*, or other standard library
  * 	string operations in the realtime thread may be dangerous for realtime!!
  *
  * \param maxlen max length of the string, defaults to DSPLUG_STRING_PARAM_MAX_LEN
  * \param set_cbk User-Callback for setting a value
  * \param get_cbk User-Callback for getting a value
  * \return a Control Port Creation instance.
  */

 DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_string_realtime( (*set_cbk)(DSPlug_Plugin , int, const char*) ,  void (*get_cbk)(DSPlug_Plugin , int, char*) , int maxlen = DSPLUG_STRING_PARAM_MAX_LEN );

 /**
 * Instance a control port creation object. For the specific type of port.
 * Output data ports cant be realtime.
 * \param set_cbk User-Callback for setting a value
 * \param get_cbk User-Callback for getting a value
 * \return a Control Port Creation instance.
 */

DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_data( (*set_cbk)(DSPlug_Plugin , int, void *, int ) , void (*get_cbk)(DSPlug_Plugin , int, void **, int* ) );

/**
 * Setting this property to a port means that it will become valid
 * for having it's value changed or retrieved in the realtime thread.
 * If the plugin defines this it means that it will:
 *
 *	* Not do any operation that may result in the realtime
 *	thread to block operation at the kernel scheduler.
 *	This usually happens when doing a libcall/syscall
 *	that may set the process in wait state, such as
 *	handling files, sleeping, try to grab a blocking mutex,
 * 	allocating/deallocating memory, etc,
 *
 *	* Warrant that setting a port is reentrant and thread-safe, as in, setting the
 *	port from the UI or multiple threads will only result in setting
 *	a shared memory value, and the function can be safely be called
 *	simultaneously from them (many threads at the same time)
 *
 *	* Make sure that the port doesnt need blocking. Some ports will attempt
 *	to block the internal state of the plugin when being set, to prevent
 *	the process() function to use that state until it's done reconfiguring it,
 *	because this may lead to a crash. Such ports cannot be made RT-Safe
 *
 */
void DSPlug_ControlPortCreation_set_realtime(DSPlug_ControlPortCreation *);

/**
 * Setting this property to a port means that the host will NOT
 * let its users know of the existence of this port, this way
 * they cant edit it. Still though, the may exist for other
 * purposes, such as saving/restoring the plugin state, or
 * being a plugin that belongs to a specific configuration.
 *
 */
void DSPlug_ControlPortCreation_set_hidden(DSPlug_ControlPortCreation *);

/**
 * Set the pointer to the process function. This function is called
 * by the host to process a given chunk of data. Since realtime-capable hosts
 * will call this function from the realtime-audio-process-thread, the
 * plugin must be careful to not do any any operation that could
 * cause the OS to suspend the process, such as opening/handling files,
 * allocating/deallocating memory, or generally using syscalls/libcalls
 * that are not warranted to be realtime-safe.
 *
 * Inside this function you can grab the input and audio ports from the plugin and do
 * all the processing that you need to.
 */
void DSPlug_PluginCreation_set_process_callback( DSPlug_PluginCreation * , void (*c)(DSPlug_Plugin *, int f) ); /**< *REQUIRED* */

/**
 * When the plugin is instanced, you will probably want
 * to setup your variables, arrays, etc in some place for each plugin.
 * The API will request you to instance all that in here, plus it will
 * let you know if it actually wants you to instance the User Interface
 * or not. Plugins may want to run from a graphical-less environment, such
 * as console. or work controlled via network, for which the graphical UI
 * is not necesary, so attempting to initialize the UI anyway and ignore this
 * flag will most likely cause your plugin to not work properly in all conditions.
 */
void DSPlug_PluginCreation_set_instance_process_userdata_callback( DSPlug_PluginCreation * , void* (*cbk)(DSPlug_PluginCaps, float samplerate, DSPlug_Boolean ui) ); /**< *REQUIRED* */

/**
 * This funcion is the opposite to:
 *
 *  DSPlug_PluginCreation_set_instance_process_userdata_callback
 *
 * It will ask you to uninitialize the plugin instance.
 */

void DSPlug_PluginCreation_set_destroy_process_userdata_callback( DSPlug_PluginCreation * , void (*c)(DSPlug_Plugin *) ); /**< *REQUIRED* */


/**
 * Sometimes the host will want to set the plugin processing back to it's initial
 * state. For example, a reverb or echo may want to restart without any sound from
 * before, or the same basically for any algorithm that uses an historial of values.
 * This callback will be called in such cases. It is not mandatory to implement if
 * not needed.
 */
void DSPlug_PluginCreation_set_reset_callback( DSPlug_PluginCreation * , void (*c)(DSPlug_Plugin *) );

/**
 * The host may want to ask the plugin for the delay in the output. If between
 * the input and output the plugin introduces a delay (due to some algorithm, FFT, etc)
 * you can let the host know about it by using this. It is not mandatory to implement.
 */
void DSPlug_PluginCreation_set_output_delay_callback( int (*get_output_delay_callback)(DSPlug_Plugin *) );

/*********
* Plugin *
**********/

/**
 * Simple function to obtain the sampling rate from the plugin
 */
float DSPlug_Plugin_get_sample_rate( DSPlug_Plugin );

/**
 * Obtain the userdata from the plugin
 */
void * DSPlug_Plugin_get_userdata( DSPlug_Plugin );


/**
 * Obtain the plugin capabilities
 */
DSPlug_PluginCaps DSPlug_Plugin_get_plugin_caps( DSPlug_Plugin );

/* Audio */

/**
 * Obtain the pointer to a specific audio port channel buffer
 * Dont forget that you cant write values to an input buffer, as it may be shared with another port, channel or plugin.
 * \return pointer to an array of floats, for reading (an input) or writing (output port). NULL if the host didnt connect the port or the port doesnt exist.
 */
float * DSPlug_Plugin_get_audio_port_channel_buffer( DSPlug_Plugin , int p, int c);

/**
 * Obtain the pointer to a pointer to a specific audio port channel buffer (ack! x_x )
 * This is used mainly for speedup purposes.
 * Dont forget that you cant write values to an input buffer, as it may be shared with another port, channel or plugin.
 * \return pointer pointer to an array of floats, for reading (an input) or writing (output port). a pointer to NULL if the host didnt connect the port. NULL if the port/channel doesnt exist
 */
float ** DSPlug_Plugin_get_audio_port_channel_buffer_pointer( DSPlug_Plugin , int p, int c);

/* Event */

/**
 * Obtain the pointer to the event queue of the port.
 * Dont forget that you must not do any other than reading events from an input port, since it may be shared with another port or plugin
 * \return pointer to an event queue, for reading (an input) or writing (output port). NULL if the host didnt connect the port or the port doesnt exist
 */
DSPlug_EventQueue* DSPlug_Plugin_get_event_port_queue( DSPlug_Plugin , int p);

/**
 * Obtain the pointer to pointer to the event queue of the port.
 * This is used mainly for speedup purposes.
 * Dont forget that you must not do any other than reading events from an input port, since it may be shared with another port or plugin
 * \return pointer to an event queue, for reading (an input) or writing (output port). a pointer to NULL if the host didnt connect the port. NULL if the port/channel doesnt exist
 */
DSPlug_EventQueue** DSPlug_Plugin_get_event_port_queue_pointer( DSPlug_Plugin , int p);

/* Control */

/**
 * The UI must call this function upon modification of a port that can edit
 * This is used mainly for speedup purposes.
 * Dont forget that you must not do any other than reading events from an input port, since it may be shared with another port or plugin
 * \return pointer to an event queue, for reading (an input) or writing (output port). a pointer to NULL if the host didnt connect the port. NULL if the port/channel doesnt exist
 */
void DSPlug_Plugin_UI_value_changed_notify( DSPlug_Plugin , int p);



#endif