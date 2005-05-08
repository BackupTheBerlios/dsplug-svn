#ifndef DSPlug_PORT_PRIVATE_H
#define DSPlug_PORT_PRIVATE_H


#include "dsplug_types.h"

/* ////////////////////////////////////////////////// */

/* Port Info */

/**
 * Common data for all plugins
 */
typedef struct {

	char * caption;
	char * name;
	char * path;

	DSPlug_PlugType plug_type;

} DSPlug_CommonPortCapsPrivate;


typedef struct {

	DSPlug_CommonPortCapsPrivate common; /**< Basic Inheritance form */
	
	int channel_count;	

} DSPlug_AudioPortCapsPrivate;

typedef struct {

	DSPlug_CommonPortCapsPrivate common; /**< Basic Inheritance form */
	
	DSPlug_EventType event_type;

} DSPlug_EventPortCapsPrivate;

/**
 *	I'm not going to comment much on this class, as everything is pretty well mapped
 *	in the host API
 */
typedef struct {

	
	/* Type */
	DSPlug_ControlPortType type;
	DSPlug_ControlPortNumericalHint numerical_hint; /**< This is only used if the type is numerical */

	/* Flags */
	DSPlug_Boolean is_realtime_safe;
	DSPlug_Boolean is_editable;

	/* Display Helpers */
	
	
	char ** option_names; /**< array to strings for option names, only for numerical-enum port type */
	int integer_steps; /**< amount of either integer steps, or amount of strings for option names, only for numerical-enum and numerical-int port type */


	void (*float_display_function)(float, char *); /**< Pointer tot he display function */

	/* Defaults */

	float default_numerical; /**< Default value for numerical port */
	char * default_string; /**< Default value for string port */
	void * default_data; /**< Default value for data port */
	int default_data_len; /**< Default size for data port */

	/* Callback to Plugin when setting and getting ports */
	
	void (*set_callback)(DSPlug_Plugin *, int, float); /**< Callback to float value set */
	void (*set_callback_string)(DSPlug_Plugin *, int, const char *); /**< Callback to string value set */
	void (*set_callback_data)(DSPlug_Plugin *, int, void *, int); /**< Callback to data value set */

	float (*get_callback)(DSPlug_Plugin *, int, float); /**< Callback to float value get */
	char * (*get_callback_string_norealtime)(DSPlug_Plugin *, int); /**< Callback to string value get, not realtime */
	void (*get_callback_string)(DSPlug_Plugin *, int, char *); /**< Callback to string value get */
	void (*get_callback_data)(DSPlug_Plugin *, int, void **, int*); /**< Callback to data value get */
	
} DSPlug_ControlPortCapsPrivate;


/* ////////////////////////////////////////////////// */

	
/* Port Data */

typedef struct {

	float ** channel_buffer_ptr; /**< array of pointers to the channel buffers */

} DSPlug_AudioPortDataPrivate;


typedef struct {

	DSPlug_EventQueue * queue; /**< Pointer to the Event Queue */

} DSPlug_EventPortDataPrivate;


typedef struct {

	void (*UI_changed_callback)(int, void *); /* ui changed port index, */
	void * UI_changed_callback_userdata;


} DSPlug_ControlPortDataPrivate;





#endif /* dsplug_port_info_private.h */


