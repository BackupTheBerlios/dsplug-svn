#ifndef DSPLUG_PORT_PRIVATE_H
#define DSPLUG_PORT_PRIVATE_H


#include "dsplug/dsplug_types.h"

/* ////////////////////////////////////////////////// */

/* Port Info */

typedef struct {

	char * caption;

	int default_channels;
	

} DSPLUG_Audio_Port_Info_Private;

typedef struct {

	char * caption;

	DSPLUG_Event_Port_Type type;
	

} DSPLUG_Event_Port_Info_Private;

typedef struct {

	/* Strings */
	char * caption;
	char * name;
	char * path;

	/* Type */
	DSPLUG_Control_Port_Type type;

	/* Flags */
	DSPLUG_Boolean is_multipart;
	DSPLUG_Boolean is_realtime_safe;

	/* Display Helpers */
	
	
	char ** option_names; /* for option ports */
	int option_count;

	
	int integer_steps; /* for int ports */

	
	void (*float_display_function)(float, char *); /* value display function */

	/* Defaults */

	float default_float;
	char * default_string;
	void * default_data;
	int default_data_len;

	/* Callback to Plugin when setting and getting ports */
	
	void (*set_callback)(DSPLUG_Plugin *, int, float); /* float value set */
	void (*set_callback_string)(DSPLUG_Plugin *, int, const char *); /* string value set */
	void (*set_callback_data)(DSPLUG_Plugin *, int, void *, int); /* data value set */

	float (*get_callback)(DSPLUG_Plugin *, int, float); /* float value get */
	char * (*get_callback_string)(DSPLUG_Plugin *, int); /* string value get */
	void (*get_callback_string_rtsafe)(DSPLUG_Plugin *, int, char *); /* string value get, rtsafe */
	void (*get_callback_data)(DSPLUG_Plugin *, int, void **, int*); /* data value get */
	
} DSPLUG_Control_Port_Info_Private;


/* ////////////////////////////////////////////////// */

	
/* Port Data */

typedef struct {

	float ** channel_buffer_ptr; /* channel buffer connections array */
	int channel_buffer_count; /* array size */

} DSPLUG_Audio_Port_Data_Private;


typedef struct {

	/* just the event queue */
	DSPLUG_Event_Queue * queue;

} DSPLUG_Event_Port_Data_Private;


typedef struct {

	void (*UI_changed_callback)(int, void *); /* ui changed port index, */
	void * UI_changed_callback_userdata;


} DSPLUG_Control_Port_Data_Private;





#endif /* dsplug_port_info_private.h */


