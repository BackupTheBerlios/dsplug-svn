/**
 * \file dsplug_plugin.h
 * \author Juan Linietsky
 */

#ifndef DSPlUG_PLUGIN_H
#define DSPlUG_PLUGIN_H



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
 * Add an control port to the plugin.
 * \param label Visual string for the port
 * \param name Name of the port, this is UNIQUE for all audio ports
 * \param path UNIX-Like Path of the port, for example "/" (global value), "/filter" (part of a filter) "/oscilA" (Part of oscillator A"). "/" is automatically prepended to any value you pass. The same way, "/" is removed from the end of a sublocation. So passing "" and "/" , or "filter", "/filter", "filter/" and "/filter/" are all the same.
 * \param cpc Control port creation object.
 */
void DSPlug_PluginCreation_add_control_port( DSPlug_PluginCreation * , const char *label, const char *name , const char *path, DSPlug_ControlPortCreation * cpc );


/**
 * Instance a control port creation object. For the specific type of port.
 * \param set_cbk User-Callback for setting a value
 * \param disp_func User-Callback for the display function, if supplied, it hints the host on how to display the port.
 * \return a Control Port Creation instance.
*/
DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_input_numerical_float( void (*set_cbk)(DSPlug_Plugin , int, float) , void (*disp_func)(float, char *)=NULL);
/**
 * Instance a control port creation object. For the specific type of port.
 * \param steps Integer steps. This is to hint the host that you want values of type integer, and the range of them. The actual value is obtained by doing port_value*steps formula.
 * \param is_enum Set to true if the port must be displayed as an ennumerated options value.
 * \param set_cbk User-Callback for setting a value
 * \param disp_func User-Callback for the display function, if supplied, it hints the host on how to display the port.
 * \return a Control Port Creation instance.
 */
DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_input_numerical_integer( int steps, DSPlug_Boolean is_enum, void (*set_cbk)(DSPlug_Plugin , int, float) , void (*disp_func)(float, char *)=NULL);
DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_input_numerical_bool( void (*set_cbk)(DSPlug_Plugin , int, float) , void (*disp_func)(float, char *)=NULL);

DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_input_string( (*set_cbk)(DSPlug_Plugin , int, const char*) );
DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_input_data( (*set_cbk)(DSPlug_Plugin , int, void *, int ) );

DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_output_numerical_float( float (*get_cbk)(DSPlug_Plugin , int) , void (*disp_func)(float, char *)=NULL);
DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_output_numerical_integer( int steps,(*set_cbk)(DSPlug_Plugin , int, float) , void (*disp_func)(float, char *)=NULL);
DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_output_numerical_enum( int opcount, float (*get_cbk)(DSPlug_Plugin , int) , void (*disp_func)(float, char *)=NULL);
DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_output_numerical_bool( float (*get_cbk)(DSPlug_Plugin , int) , void (*disp_func)(float, char *)=NULL);

DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_output_string( char * (*get_cbk)(DSPlug_Plugin , int) );
DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_output_string_realtime( void (*get_cbk)(DSPlug_Plugin , int, char*) );
DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_output_data( void (*set_cbk)(DSPlug_Plugin , int, void **, int* ) );

void DSPlug_ControlPortCreation_set_realtime(DSPlug_ControlPortCreation *);
void DSPlug_ControlPortCreation_set_editable(DSPlug_ControlPortCreation *);

//detail the RT safeness stuff
void DSPlug_PluginCreation_set_process_callback( DSPlug_PluginCreation * , void (*c)(DSPlug_Plugin *, int f) ); /**< *REQUIRED* */

void DSPlug_PluginCreation_set_instance_process_userdata_callback( DSPlug_PluginCreation * , void* (*cbk)(DSPlug_PluginCaps, DSPlug_Boolean ui) ); /**< *REQUIRED* */
void DSPlug_PluginCreation_set_destroy_process_userdata_callback( DSPlug_PluginCreation * , void (*c)(DSPlug_Plugin *, int f) ); /**< *REQUIRED* */


void DSPlug_PluginCreation_set_reset_callback( DSPlug_PluginCreation * , void (*c)(DSPlug_Plugin *) );

void DSPlug_PluginCreation_set_output_delay_callback( int (*get_output_delay_callback)(DSPlug_Plugin *) );






/* Audio Ports

