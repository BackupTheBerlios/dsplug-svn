
#include "dsplug_plugin.h"
#include "dsplug_private.h"
#include "dsplug_helpers.h"
#include "dsplug_error_report.h"

#include <stdlib.h>
#include <string.h>




DSPlug_PluginCreation* DSPlug_LibraryCreation_instance_plugin_creation( DSPlug_LibraryCreation p_lib_creation) {

	DSPlug_PluginCreation *plugin_creation = (DSPlug_PluginCreation*) malloc(sizeof(DSPlug_PluginCreation));
	DSPlug_PluginCapsPrivate *plugin_caps = (DSPlug_PluginCapsPrivate*) malloc(sizeof(DSPlug_PluginCapsPrivate));
	int i;
	memset(plugin_caps,0,sizeof(DSPlug_PluginCapsPrivate));

	DSPlug_copy_to_newstring(&plugin_caps->info_caption,"Unnamed Plugin");
	DSPlug_copy_to_newstring(&plugin_caps->info_author,"Unauthored Plugin");
	DSPlug_copy_to_newstring(&plugin_caps->info_copyright,"Uncopyrighted Plugin");
	DSPlug_copy_to_newstring(&plugin_caps->info_version,"00.00.00");
	DSPlug_copy_to_newstring(&plugin_caps->info_compatible_version,"");
	
	DSPlug_copy_to_newstring(&plugin_caps->info_description,"");
	DSPlug_copy_to_newstring(&plugin_caps->info_HTTP_URL,"http://www.dsplug.org");
	DSPlug_copy_to_newstring(&plugin_caps->info_category_path,"/Uncategorized");
				
	for (i=0;i<MAX_PLUGIN_CAPS_CONSTANTS;i++)
		plugin_caps->constants[i]=DSPLUG_NO_CONSTANT;
	

	plugin_creation->_private=plugin_caps;

	return plugin_creation;
}

void DSPlug_LibraryCreation_abort_plugin_creation( DSPlug_LibraryCreation p_lib_creation, DSPlug_PluginCreation * pc) {

	DSPlug_PluginCapsPrivate *plugin_caps = (DSPlug_PluginCapsPrivate *)pc->_private;

	if (!pc || !plugin_caps) {

		DSPlug_report_error("PLUGIN: DSPlug_LibraryCreation_abort_plugin_creation: Invalid PluginCreation object (NULL)");
		return;
	}

	if (plugin_caps->created_succesfully) {

		DSPlug_report_error("PLUGIN: DSPlug_LibraryCreation_abort_plugin_creation: attempt to abort a succesfully created plugin");
		return;
	}
	
	DSPlug_free_plugin_caps(plugin_caps);
	free(pc);
}

DSPlug_Boolean DSPlug_LibraryCreation_add_plugin( DSPlug_LibraryCreation p_lib_creation, DSPlug_PluginCreation * pc ) {




	return DSPLUG_FALSE;
}


/******************
* Plugin Creation *
*******************/
void DSPlug_PluginCreation_set_caption( DSPlug_PluginCreation *p_plugin_creation , const char *s ) {

	DSPlug_PluginCapsPrivate *plugin_caps = (DSPlug_PluginCapsPrivate *)p_plugin_creation->_private;

	if (!p_plugin_creation || !plugin_caps) {

		DSPlug_report_error("PLUGIN: DSPlug_PluginCreation_set_caption: Invalid PluginCreation object (NULL)");
		return;
	}

	DSPlug_copy_to_newstring(&plugin_caps->info_caption,s);

}

void DSPlug_PluginCreation_set_author( DSPlug_PluginCreation *p_plugin_creation , const char *s ) {

	DSPlug_PluginCapsPrivate *plugin_caps = (DSPlug_PluginCapsPrivate *)p_plugin_creation->_private;

	if (!p_plugin_creation || !plugin_caps) {

		DSPlug_report_error("PLUGIN: DSPlug_PluginCreation_set_author: Invalid PluginCreation object (NULL)");
		return;
	}

	DSPlug_copy_to_newstring(&plugin_caps->info_author,s);

}

void DSPlug_PluginCreation_set_copyright( DSPlug_PluginCreation *p_plugin_creation , const char *s ) {

	DSPlug_PluginCapsPrivate *plugin_caps = (DSPlug_PluginCapsPrivate *)p_plugin_creation->_private;

	if (!p_plugin_creation || !plugin_caps) {

		DSPlug_report_error("PLUGIN: DSPlug_PluginCreation_set_copyright: Invalid PluginCreation object (NULL)");
		return;
	}

	DSPlug_copy_to_newstring(&plugin_caps->info_copyright,s);

}

void DSPlug_PluginCreation_set_version( DSPlug_PluginCreation *p_plugin_creation , const char *s ) {

	DSPlug_PluginCapsPrivate *plugin_caps = (DSPlug_PluginCapsPrivate *)p_plugin_creation->_private;

	if (!p_plugin_creation || !plugin_caps) {

		DSPlug_report_error("PLUGIN: DSPlug_PluginCreation_set_version: Invalid PluginCreation object (NULL)");
		return;
	}
	
	DSPlug_copy_to_newstring(&plugin_caps->info_version,s);

}

void DSPlug_PluginCreation_set_compatible_version( DSPlug_PluginCreation *p_plugin_creation , const char *s ) {

	DSPlug_PluginCapsPrivate *plugin_caps = (DSPlug_PluginCapsPrivate *)p_plugin_creation->_private;

	if (!p_plugin_creation || !plugin_caps) {

		DSPlug_report_error("PLUGIN: DSPlug_PluginCreation_set_compatible_version: Invalid PluginCreation object (NULL)");
		return;
	}
	
	DSPlug_copy_to_newstring(&plugin_caps->info_version,s);

}

void DSPlug_PluginCreation_set_unique_ID( DSPlug_PluginCreation *p_plugin_creation , const char *s ) {

	DSPlug_PluginCapsPrivate *plugin_caps = (DSPlug_PluginCapsPrivate *)p_plugin_creation->_private;

	if (!p_plugin_creation || !plugin_caps) {

		DSPlug_report_error("PLUGIN: DSPlug_PluginCreation_set_unique_ID: Invalid PluginCreation object (NULL)");
		return;
	}
	
	DSPlug_copy_to_newstring(&plugin_caps->info_unique_ID,s);

}

void DSPlug_PluginCreation_set_description( DSPlug_PluginCreation *p_plugin_creation , const char *s ) {

	DSPlug_PluginCapsPrivate *plugin_caps = (DSPlug_PluginCapsPrivate *)p_plugin_creation->_private;

	if (!p_plugin_creation || !plugin_caps) {

		DSPlug_report_error("PLUGIN: DSPlug_PluginCreation_set_description: Invalid PluginCreation object (NULL)");
		return;
	}
	
	DSPlug_copy_to_newstring(&plugin_caps->info_description,s);


}

void DSPlug_PluginCreation_set_HTTP_URL( DSPlug_PluginCreation *p_plugin_creation , const char *s ) {

	DSPlug_PluginCapsPrivate *plugin_caps = (DSPlug_PluginCapsPrivate *)p_plugin_creation->_private;

	if (!p_plugin_creation || !plugin_caps) {

		DSPlug_report_error("PLUGIN: DSPlug_PluginCreation_set_HTTP_URL: Invalid PluginCreation object (NULL)");
		return;
	}

	DSPlug_copy_to_newstring(&plugin_caps->info_HTTP_URL,s);

}

void DSPlug_PluginCreation_set_category_path( DSPlug_PluginCreation *p_plugin_creation , const char *s ) {

	DSPlug_PluginCapsPrivate *plugin_caps = (DSPlug_PluginCapsPrivate *)p_plugin_creation->_private;

	if (!p_plugin_creation || !plugin_caps) {

		DSPlug_report_error("PLUGIN: DSPlug_PluginCreation_set_category_path: Invalid PluginCreation object (NULL)");
		return;
	}

	DSPlug_copy_to_newstring(&plugin_caps->info_category_path,s);

	
}


void DSPlug_PluginCreation_set_usage_hint( DSPlug_PluginCreation *p_plugin_creation , DSPlug_PluginUsageHint h ) {


	
}


void DSPlug_PluginCreation_add_feature( DSPlug_PluginCreation *p_plugin_creation , DSPlug_PluginFeature f ) {

	DSPlug_PluginCapsPrivate *plugin_caps = (DSPlug_PluginCapsPrivate *)p_plugin_creation->_private;
	int fbyte=(int)f/8;
	int fbit=(int)f%8;
	
	if (!p_plugin_creation || !plugin_caps) {

		DSPlug_report_error("PLUGIN: DSPlug_PluginCreation_add_feature Invalid PluginCreation object (NULL)");
		return;
	}

	if (f>=(MAX_PLUGIN_CAPS_FEATURE_BYTES*8)) {


		DSPlug_report_error("API: DSPlug_PluginCreation_add_feature: More constants than app can handle?");
		return;

	}

	plugin_caps->features[fbyte]|=(1<<fbit);

}


void DSPug_PluginCreation_add_constant( DSPlug_PluginCreation *p_plugin_creation , DSPlug_PluginConstant c ,int cv ) {

	DSPlug_PluginCapsPrivate *plugin_caps = (DSPlug_PluginCapsPrivate *)p_plugin_creation->_private;
	
	if (!p_plugin_creation || !plugin_caps) {

		DSPlug_report_error("PLUGIN: DSPlug_PluginCreation_add_feature Invalid PluginCreation object (NULL)");
		return;
	}

	if (c>=MAX_PLUGIN_CAPS_CONSTANTS) {

		DSPlug_report_error("API: DSPlug_PluginCreation_add_constant: More constants than app can handle?");
		return;

	}

	if (cv<0) {

		DSPlug_report_error("API: DSPlug_PluginCreation_add_constant: Negative constants not allowed");
		return;
	}
	
		
	plugin_caps->constants[c]=cv;

}

void DSPlug_PluginCreation_add_audio_port( DSPlug_PluginCreation *p_plugin_creation , DSPlug_PlugType plug, const char *label, const char *name , const char *path, int ch ) {

	DSPlug_PluginCapsPrivate *plugin_caps = (DSPlug_PluginCapsPrivate *)p_plugin_creation_private->_private;
	
	if (!p_plugin_creation || !plugin_caps) {

		DSPlug_report_error("PLUGIN: DSPlug_PluginCreation_add_audio_port: Invalid PluginCreation object (NULL)");
		return;
	}

	if (plugin_caps->audio_port_count>=DSPLUG_MAX_AUDIO_PORTS) {

		DSPlug_report_error("PLUGIN: DSPlug_PluginCreation_add_audio_port: Maximum number of audio ports reached");
		return;
	}

	if (ch<=0 || ch >DSPLUG_MAX_CHANNELS_PER_AUDIO_PORT) {

		DSPlug_report_error("PLUGIN: DSPlug_PluginCreation_add_audio_port: Invalid number of channels");
		return;
	}

	plugin_caps->audio_port_count++;
	plugin_caps->audio_port_caps=(DSPlug_AudioPortCapsPrivate**)realloc(plugin_caps->audio_port_caps,sizeof(DSPlug_AudioPortCapsPrivate*)*plugin_caps->audio_port_count);
	
	plugin_caps->audio_port_caps[plugin_caps->audio_port_count-1]=(DSPlug_AudioPortCapsPrivate*)malloc(sizeof(DSPlug_AudioPortCapsPrivate));
	DSPlug_CommonPortCapsPrivate *cpc=&plugin_caps->audio_port_caps[plugin_caps->audio_port_count-1]->common;
	DSPlug_copy_to_newstring(&cpc->caption,label);
	DSPlug_copy_to_newstring(&cpc->name,name);
	DSPlug_copy_to_newstring(&cpc->path,path);
	cpc->plug_type=plug;

	plugin_caps->audio_port_caps[plugin_caps->audio_port_count-1]->channel_count=ch;


}

void DSPlug_PluginCreation_add_event_port( DSPlug_PluginCreation *p_plugin_creation , DSPlug_PlugType plug, const char *label, const char *name , const char *path, DSPlug_EventType evt ) {

	DSPlug_PluginCapsPrivate *plugin_caps = (DSPlug_PluginCapsPrivate *)p_plugin_creation->_private;
	
	if (!p_plugin_creation || !plugin_caps) {

		DSPlug_report_error("PLUGIN: DSPlug_PluginCreation_add_event_port: Invalid PluginCreation object (NULL)");
		return;
	}

	if (plugin_caps->event_port_count>=DSPLUG_MAX_EVENT_PORTS) {

		DSPlug_report_error("PLUGIN: DSPlug_PluginCreation_add_event_port: Maximum number of control ports reached");
		return;
	}

	plugin_caps->event_port_count++;
	plugin_caps->event_port_caps=(DSPlug_EventPortCapsPrivate**)realloc(plugin_caps->event_port_caps,sizeof(DSPlug_EventPortCapsPrivate*)*plugin_caps->event_port_count);
	
	plugin_caps->event_port_caps[plugin_caps->event_port_count-1]=(DSPlug_EventPortCapsPrivate*)malloc(sizeof(DSPlug_EventPortCapsPrivate));
	DSPlug_CommonPortCapsPrivate *cpc=&plugin_caps->event_port_caps[plugin_caps->event_port_count-1]->common;
	DSPlug_copy_to_newstring(&cpc->caption,label);
	DSPlug_copy_to_newstring(&cpc->name,name);
	DSPlug_copy_to_newstring(&cpc->path,path);
	cpc->plug_type=plug;

	plugin_caps->event_port_caps[plugin_caps->event_port_count-1]->event_type=evt;

}

void DSPlug_PluginCreation_add_control_port( DSPlug_PluginCreation *p_plugin_creation , DSPlug_PlugType plug, const char *label, const char *name , const char *path, DSPlug_ControlPortCreation * ctpc ) {

	DSPlug_PluginCapsPrivate *plugin_caps = (DSPlug_PluginCapsPrivate *)p_plugin_creation->_private;
	DSPlug_ControlPortCapsPrivate *control_port_caps = (DSPlug_ControlPortCapsPrivate *)ctpc->_private;
	
	
	if (!p_plugin_creation || !plugin_caps) {

		DSPlug_report_error("PLUGIN: DSPlug_PluginCreation_add_control_port: Invalid PluginCreation object (NULL)");
		return;
	}

	if ( !ctpc || !control_port_caps ) {

		DSPlug_report_error("PLUGIN: DSPlug_PluginCreation_add_control_port: Invalid ControlPortCaps object (NULL)");
		return;
	}
		
	if (plugin_caps->control_port_count>=DSPLUG_MAX_CONTROL_PORTS) {

		DSPlug_report_error("PLUGIN: DSPlug_PluginCreation_add_control_port: Maximum number of control ports reached");
		return;
	}


	plugin_caps->control_port_count++;
	plugin_caps->control_port_caps=(DSPlug_ControlPortCapsPrivate**)realloc(plugin_caps->control_port_caps,sizeof(DSPlug_ControlPortCapsPrivate*)*plugin_caps->control_port_count);
	
	plugin_caps->control_port_caps[plugin_caps->control_port_count-1]=control_port_caps;
	
	DSPlug_CommonPortCapsPrivate *cpc=&plugin_caps->control_port_caps[plugin_caps->control_port_count-1]->common;
	DSPlug_copy_to_newstring(&cpc->caption,label);
	DSPlug_copy_to_newstring(&cpc->name,name);
	DSPlug_copy_to_newstring(&cpc->path,path);
	cpc->plug_type=plug;

	
	free(ctpc);




}

/*** HELPER ****/
static void DSPlug_instance_control_port_creation(DSPlug_ControlPortCreation *ctpc,DSPlug_ControlPortCapsPrivate *cpc ) {

	ctpc =(DSPlug_ControlPortCreation*)malloc(sizeof(DSPlug_ControlPortCreation));
	cpc = (DSPlug_ControlPortCapsPrivate*)malloc(sizeof(DSPlug_ControlPortCapsPrivate*));
	ctpc->_private=cpc;

	memset(cpc,0,sizeof(DSPlug_ControlPortCapsPrivate));
}

DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_numerical_float( void (*set_cbk)(DSPlug_Plugin , int, float) ,  float (*get_cbk)(DSPlug_Plugin , int) , void (*disp_func)(float, char *)) {

	DSPlug_ControlPortCreation *control_port_creation;
	DSPlug_ControlPortCapsPrivate *control_port_caps;

	if (!get_cbk || !set_cbk) {

		DSPlug_report_error("PLUGIN: DSPlug_ControlPortCreation_create_numerical_float: NULL get/set callbacks provided");
		return NULL;
	}


	DSPlug_instance_control_port_creation( control_port_creation, control_port_caps );

	control_port_caps->type=DSPLUG_CONTROL_PORT_TYPE_NUMERICAL;
	control_port_caps->numerical_hint=DSPLUG_CONTROL_PORT_HINT_TYPE_FLOAT;

	control_port_caps->set_callback_numerical=set_cbk;
	control_port_caps->get_callback_numerical=get_cbk;
	
	control_port_caps->float_display_function=disp_func;

	return control_port_creation;
	
}

DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_numerical_integer( int steps, DSPlug_Boolean is_enum, void (*set_cbk)(DSPlug_Plugin , int, float) ,  float (*get_cbk)(DSPlug_Plugin , int) , void (*disp_func)(float, char *)) {


	DSPlug_ControlPortCreation *control_port_creation;
	DSPlug_ControlPortCapsPrivate *control_port_caps;

	if (!get_cbk || !set_cbk) {

		DSPlug_report_error("PLUGIN: DSPlug_ControlPortCreation_create_numerical_integer: NULL get/set callbacks provided");
		return NULL;
	}

	if (steps<1 || steps>=DSPLUG_NUMERICAL_PORT_MAX_STEPS) {

		DSPlug_report_error("PLUGIN: DSPlug_ControlPortCreation_create_numerical_integer: Invalid number of steps");
		return NULL;
	}

	DSPlug_instance_control_port_creation( control_port_creation, control_port_caps );

	control_port_caps->type=DSPLUG_CONTROL_PORT_TYPE_NUMERICAL;
	control_port_caps->numerical_hint=DSPLUG_CONTROL_PORT_HINT_TYPE_INTEGER;
	control_port_caps->integer_is_enum=is_enum;

	control_port_caps->set_callback_numerical=set_cbk;
	control_port_caps->get_callback_numerical=get_cbk;
	
	control_port_caps->float_display_function=disp_func;

	return control_port_creation;

}

DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_numerical_bool( void (*set_cbk)(DSPlug_Plugin , int, float) , float (*get_cbk)(DSPlug_Plugin , int) , void (*disp_func)(float, char *)) {

	DSPlug_ControlPortCreation *control_port_creation;
	DSPlug_ControlPortCapsPrivate *control_port_caps;

	if (!get_cbk || !set_cbk) {

		DSPlug_report_error("PLUGIN: DSPlug_ControlPortCreation_create_numerical_bool: NULL get/set callbacks provided");
		return NULL;
	}

	DSPlug_instance_control_port_creation( control_port_creation, control_port_caps );

	control_port_caps->type=DSPLUG_CONTROL_PORT_TYPE_NUMERICAL;
	control_port_caps->numerical_hint=DSPLUG_CONTROL_PORT_HINT_TYPE_BOOL;

	control_port_caps->set_callback_numerical=set_cbk;
	control_port_caps->get_callback_numerical=get_cbk;
	
	control_port_caps->float_display_function=disp_func;

	return control_port_creation;

}

 DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_string( void (*set_cbk)(DSPlug_Plugin , int, const char*) , char * (*get_cbk)(DSPlug_Plugin , int) ) {


	 DSPlug_ControlPortCreation *control_port_creation;
	 DSPlug_ControlPortCapsPrivate *control_port_caps;

	 if (!get_cbk || !set_cbk) {

		 DSPlug_report_error("PLUGIN: DSPlug_ControlPortCreation_create_string: NULL get/set callbacks provided");
		 return NULL;
	 }

	 DSPlug_instance_control_port_creation( control_port_creation, control_port_caps );

	 control_port_caps->type=DSPLUG_CONTROL_PORT_TYPE_STRING;

	 control_port_caps->set_callback_string=set_cbk;
	 control_port_caps->get_callback_string=get_cbk;

	 control_port_caps->is_realtime_safe=DSPLUG_FALSE;

	 return control_port_creation;


 }

 DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_string_realtime( void (*set_cbk)(DSPlug_Plugin , int, const char*) ,  void (*get_cbk)(DSPlug_Plugin , int, char*) , int maxlen ) {

	 DSPlug_ControlPortCreation *control_port_creation;
	 DSPlug_ControlPortCapsPrivate *control_port_caps;

	 if (!get_cbk || !set_cbk) {

		 DSPlug_report_error("PLUGIN: DSPlug_ControlPortCreation_create_string_realtime: NULL get/set callbacks provided");
		 return NULL;
	 }

	 DSPlug_instance_control_port_creation( control_port_creation, control_port_caps );

	 control_port_caps->type=DSPLUG_CONTROL_PORT_TYPE_STRING;

	 control_port_caps->set_callback_string=set_cbk;
	 control_port_caps->get_callback_string_realtime=get_cbk;

	 control_port_caps->is_realtime_safe=DSPLUG_TRUE;

	 return control_port_creation;
 }


 DSPlug_ControlPortCreation * DSPlug_ControlPortCreation_create_data( void (*set_cbk)(DSPlug_Plugin , int, void *, int ) , void (*get_cbk)(DSPlug_Plugin , int, void **, int* ) ) {


	 DSPlug_ControlPortCreation *control_port_creation;
	 DSPlug_ControlPortCapsPrivate *control_port_caps;

	 if (!get_cbk || !set_cbk) {

		 DSPlug_report_error("PLUGIN: DSPlug_ControlPortCreation_create_data: NULL get/set callbacks provided");
		 return NULL;
	 }

	 DSPlug_instance_control_port_creation( control_port_creation, control_port_caps );

	 control_port_caps->type=DSPLUG_CONTROL_PORT_TYPE_DATA;

	 control_port_caps->set_callback_data=set_cbk;
	 control_port_caps->get_callback_data=get_cbk;

	 control_port_caps->is_realtime_safe=DSPLUG_FALSE;

	 return control_port_creation;


 }

 
 void DSPlug_ControlPortCreation_set_realtime(DSPlug_ControlPortCreation *p_port) {

	 DSPlug_ControlPortCapsPrivate *control_port_caps = (DSPlug_ControlPortCapsPrivate *)p_port->_private;

	
	 if (!p_port || !control_port_caps) {

		 DSPlug_report_error("PLUGIN: DSPlug_ControlPortCreation_set_realtime: Invalid ControlPortCreation object (NULL)");
		 return;
	 }

	 if (control_port_caps->type!=DSPLUG_CONTROL_PORT_TYPE_NUMERICAL) {

		 DSPlug_report_error("PLUGIN: DSPlug_ControlPortCreation_set_realtime: Cant's set a port of this type to be realtime with this function");
		 return;
	 }


	 control_port_caps->is_realtime_safe=DSPLUG_TRUE;

 }

 void DSPlug_ControlPortCreation_set_hidden(DSPlug_ControlPortCreation *p_port) {


	 DSPlug_ControlPortCapsPrivate *control_port_caps = (DSPlug_ControlPortCapsPrivate *)p_port->_private;

	
	 if (!p_port || !control_port_caps) {

		 DSPlug_report_error("PLUGIN: DSPlug_ControlPortCreation_set_hidden: Invalid ControlPortCreation object (NULL)");
		 return;
	 }

	 control_port_caps->is_hidden=DSPLUG_TRUE;

 }

 void DSPlug_ControlPortCreation_set_musical_part(DSPlug_ControlPortCreation *p_port, int p_part) {

	 DSPlug_ControlPortCapsPrivate *control_port_caps = (DSPlug_ControlPortCapsPrivate *)p_port->_private;

	
	 if (!p_port || !control_port_caps) {

		 DSPlug_report_error("PLUGIN: DSPlug_ControlPortCreation_set_musical_part: Invalid ControlPortCreation object (NULL)");
		 return;
	 }

	 if (p_part<0) {

		 DSPlug_report_error("PLUGIN: DSPlug_ControlPortCreation_set_musical_part: Invalid Musical Part");
		 return;
	 }
	 
	 control_port_caps->musical_part=p_part+1; //plus one, so I know zero is unused

 }

 

 void DSPlug_PluginCreation_set_process_callback( DSPlug_PluginCreation *p_plugin_creation , void (*c)(DSPlug_Plugin *, int f) ) {





 }

 void DSPlug_PluginCreation_set_instance_process_userdata_callback( DSPlug_PluginCreation *p_plugin_creation , void* (*cbk)(DSPlug_PluginCaps, float samplerate, DSPlug_Boolean ui) ) {




 }

 void DSPlug_PluginCreation_set_destroy_process_userdata_callback( DSPlug_PluginCreation *p_plugin_creation , void (*c)(DSPlug_Plugin *) ) {



 }

 void DSPlug_PluginCreation_set_reset_callback( DSPlug_PluginCreation *p_plugin_creation , void (*c)(DSPlug_Plugin *) ) {




 }

 void DSPlug_PluginCreation_set_output_delay_callback( int (*get_output_delay_callback)(DSPlug_Plugin *) ) {



 }

/*********
 * Plugin *
**********/

 float DSPlug_Plugin_get_sample_rate( DSPlug_Plugin p_plugin ) {



 }

 void * DSPlug_Plugin_get_userdata( DSPlug_Plugin p_plugin ) {



 }
 
 DSPlug_PluginCaps DSPlug_Plugin_get_plugin_caps( DSPlug_Plugin p_plugin ) {


 }

 /* Audio */

 float * DSPlug_Plugin_get_audio_port_channel_buffer( DSPlug_Plugin p_plugin , int p, int c) {




 }

 float ** DSPlug_Plugin_get_audio_port_channel_buffer_pointer( DSPlug_Plugin p_plugin , int p, int c) {





 }

 /* Event */

 DSPlug_EventQueue* DSPlug_Plugin_get_event_port_queue( DSPlug_Plugin p_plugin , int p) {



 }

 DSPlug_EventQueue** DSPlug_Plugin_get_event_port_queue_pointer( DSPlug_Plugin p_plugin , int p) {



 }

 /* Control */

 void DSPlug_Plugin_UI_value_changed_notify( DSPlug_Plugin p_plugin , int p) {



 }


