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

	DSPlug_CommonPortCapsPrivate common; /**< Basic Inheritance form, if GTK does this, I can too, this must always be the first member of the struct */

	int channel_count;

} DSPlug_AudioPortCapsPrivate;

typedef struct {

	DSPlug_CommonPortCapsPrivate common; /**< Basic Inheritance form, if GTK does this, I can too, this must always be the first member of the struct */

	DSPlug_EventType event_type;

} DSPlug_EventPortCapsPrivate;

/**
 *	I'm not going to comment much on this class, as everything is pretty well mapped
 *	in the host API
 */
typedef struct {

	DSPlug_CommonPortCapsPrivate common; /**< Basic Inheritance form, if GTK does this, I can too, this must always be the first member of the struct */

	/* Type */
	DSPlug_ControlPortType type;
	DSPlug_ControlPortNumericalHint numerical_hint; /**< This is only used if the type is numerical */

	/* Flags */
	DSPlug_Boolean is_realtime_safe;
	DSPlug_Boolean is_hidden;

	/* Multipart */

	int musical_part;

	/* Display Helpers */


	int integer_steps; /**< amount of either integer steps, or amount of strings for option names, only for  numerical-int port type */
	DSPlug_Boolean integer_is_enum;


	void (*float_display_function)(float, char *); /**< Pointer tot he display function */

	/* Defaults */

	int realtime_port_max_len;

	/* Callback to Plugin when setting and getting ports */

	void (*set_callback_numerical)(DSPlug_Plugin , int, float); /**< Callback to float value set */
	void (*set_callback_string)(DSPlug_Plugin , int, const char *); /**< Callback to string value set */
	void (*set_callback_data)(DSPlug_Plugin , int, const void *, int); /**< Callback to data value set */

	float (*get_callback_numerical)(DSPlug_Plugin , int); /**< Callback to float value get */
	char * (*get_callback_string_norealtime)(DSPlug_Plugin , int); /**< Callback to string value get, not realtime */
	void (*get_callback_string)(DSPlug_Plugin , int, char *); /**< Callback to string value get */
	void (*get_callback_data)(DSPlug_Plugin , int, void **, int*); /**< Callback to data value get */

} DSPlug_ControlPortCapsPrivate;


/* ////////////////////////////////////////////////// */


/* Port Data */

typedef struct {

	float ** channel_buffer_ptr; /**< array of pointers to the channel buffers */
	int channel_count;

} DSPlug_AudioPortPrivate;


typedef struct {

	DSPlug_EventQueue * queue; /**< Pointer to the Event Queue */

} DSPlug_EventPortPrivate;


typedef struct {

	void (*UI_changed_callback)(int, void *); /* ui changed port index, */
	void * UI_changed_callback_userdata;


} DSPlug_ControlPortPrivate;





#endif /* dsplug_port_info_private.h */


