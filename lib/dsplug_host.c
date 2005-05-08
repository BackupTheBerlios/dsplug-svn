
/**
 * \file dsplug_host.h
 * \author Juan Linietsky
 */


#include <string.h>
#include <stdlib.h>

#include "dsplug_private.h"
#include "dsplug_host.h"
#include "dsplug_error_report.h"




/****************************/

/* PLUGIN LIBRARY */

/****************************/

/**
 *	Open a Plugin Library (plugin access object and shared data for
 *     the plugin instances).
 */

DSPlug_PluginLibrary * DSPlug_Host_open_plugin_library( const char * p ) {





}

/**
 *	Close a plugin handle. Call this when you dont want to use the plugin anymore.
 *     WARNING: This will invalidate any open plugin instances!
 *
 */

void DSPlug_Host_close_plugin_library( DSPlug_PluginLibrary *  p_library) {






}


int DSPlug_PluginLibrary_get_plugin_count( DSPlug_PluginLibrary * p_library) {

	DSPlug_PluginLibraryPrivate *library = (DSPlug_PluginLibraryPrivate *) (p_library->_private);

	return library->plugin_count;
	


}
DSPlug_PluginCaps DSPlug_PluginLibrary_get_plugin_caps( DSPlug_PluginLibrary * p_library, int i ) {

	DSPlug_PluginLibraryPrivate *library = (DSPlug_PluginLibraryPrivate *) (p_library->_private);
	DSPlug_PluginCaps c;

	/* Check if plugin exists */	
	if (i<0 || i>=library->plugin_count) {

		DSPlug_report_error("HOST: DSPlug_PluginLibrary_get_plugin_caps - Invalid Plugin Index Parameter");
		c._private=NULL;
		return c;
	}

	/* Assign the private component */
	c._private=library->plugin_caps_array[i];
			
	return c;

}


/**
 * This is one of the most complex functions in the api, as it is in charge of the plugin instance initialization
 */

DSPlug_PluginInstance * DSPlug_PluginLibrary_get_plugin_instance( DSPlug_PluginLibrary * p_library, int i , int r, DSPlug_Boolean ui) {


	DSPlug_PluginLibraryPrivate *library = (DSPlug_PluginLibraryPrivate *) (p_library->_private);
	DSPlug_PluginInstance * plugin_instance=NULL;

	/* Check if plugin exists */
	if (i<0 || i>=library->plugin_count) {

		DSPlug_report_error("HOST: DSPlug_PluginLibrary_get_plugin_instance - Invalid Plugin Index Parameter");
		return NULL;
	}

	/* Check wether the plugin really has UI */
	if (ui && ! ( library->plugin_caps_array[i]->features & (1<<DSPLUG_PLUGIN_FEATURE_HAS_GUI) ) ) {

		DSPlug_report_error("HOST: DSPlug_PluginLibrary_get_plugin_instance - Attempt to instance UI of UI-Less Plugin");
		return NULL;
	}
	
	/* Create plugin instance */

	plugin_instance = (DSPlug_PluginInstance *)malloc(sizeof(DSPlug_PluginInstance));

	/* Create the Actual Plugin */
	{
		DSPlug_Plugin *plugin=NULL;
		DSPlug_PluginPrivate *plugin_private=NULL;
		DSPlug_PluginCaps aux_caps=DSPlug_PluginLibrary_get_plugin_caps(p_library,i);
		DSPlug_PluginCapsPrivate *caps_private=(DSPlug_PluginCapsPrivate*)aux_caps._private;
		int i=0,j=0;
		

		/* User Data */
		
		void * plugin_userdata = library->plugin_caps_array[i]->instance_plugin_userdata(aux_caps,ui);
		if (plugin_userdata==NULL) {

			free(plugin_instance);
			DSPlug_report_error("HOST: DSPlug_PluginLibrary_get_plugin_instance - Plugin Failed Initialization");
			return NULL;

		}

		plugin = (DSPlug_Plugin *)malloc(sizeof(DSPlug_Plugin));
		plugin->_user_private = plugin_userdata;

		/* Plugin Data */

		plugin_private = (DSPlug_PluginPrivate *)malloc(sizeof(DSPlug_PluginPrivate));
		plugin_private->plugin_caps=caps_private;

		/* Create the port structures */

		/* * Audio Ports * */
		plugin_private->audio_port_count=caps_private->audio_port_count;
		plugin_private->audio_ports=(DSPlug_AudioPortPrivate**)malloc( sizeof(DSPlug_AudioPortPrivate*)*plugin_private->audio_port_count);

		for (i=0;i<plugin_private->audio_port_count;i++) {

			DSPlug_AudioPortPrivate* aport = plugin_private->audio_ports[i]; /* audio port */
			
			aport = (DSPlug_AudioPortPrivate*)malloc( sizeof(DSPlug_AudioPortPrivate*));
			aport->channel_count = caps_private->audio_port_caps[i]->channel_count;
			aport->channel_buffer_ptr = (float**)malloc( sizeof(float*)*aport->channel_count);
			for(j=0;j<aport->channel_count;j++)
				aport->channel_buffer_ptr[j] = NULL; /* unconnected port channel by default */
		}
                  
		/* * Event Ports * */
		plugin_private->event_port_count=caps_private->event_port_count;
		plugin_private->event_ports=(DSPlug_EventPortPrivate**)malloc( sizeof(DSPlug_EventPortPrivate*)*plugin_private->event_port_count);

		for (i=0;i<plugin_private->event_port_count;i++) {

			plugin_private->event_ports[i] = (DSPlug_EventPortPrivate*)malloc( sizeof(DSPlug_EventPortPrivate*));
			plugin_private->event_ports[i]->queue = NULL; /* unconnected queue by default */

		}

		/* * Control Ports * */

		plugin_private->control_port_count=caps_private->control_port_count;
		plugin_private->control_ports=(DSPlug_ControlPortPrivate**)malloc( sizeof(DSPlug_ControlPortPrivate*)*plugin_private->control_port_count);

		for (i=0;i<plugin_private->control_port_count;i++) {

			plugin_private->control_ports[i] = (DSPlug_ControlPortPrivate*)malloc( sizeof(DSPlug_ControlPortPrivate*));
			plugin_private->control_ports[i]->UI_changed_callback_userdata = NULL;
			plugin_private->control_ports[i]->UI_changed_callback = NULL;
			
		}

		plugin->_private=plugin_private;		
           
		/* Assign to instance */

		plugin_instance->_private=plugin;

	}		
		

	/* Assign the private component */
	
		
	return plugin_instance;

}

void DSPlug_PluginLibrary_destroy_plugin_instance( DSPlug_PluginLibrary *  p_library, DSPlug_PluginInstance * p) {



}


void DSPlug_PluginCaps_get_caption( DSPlug_PluginCaps *p_caps, char * s ) {





}


void DSPlug_PluginCaps_get_author( DSPlug_PluginCaps *p_caps, char * s ) {




}


void DSPlug_PluginCaps_get_copyright( DSPlug_PluginCaps *p_caps, char * s ) {




}

void DSPlug_PluginCaps_get_version( DSPlug_PluginCaps *p_caps, char * s ) {




}

void DSPlug_PluginCaps_get_compatible_version( DSPlug_PluginCaps *p_caps, char *s ) {




}


void DSPlug_PluginCaps_get_unique_ID( DSPlug_PluginCaps *p_caps, char *s ) {





}

void DSPlug_PluginCaps_get_category_path( DSPlug_PluginCaps *p_caps, char *s ) {




}

DSPlug_PluginUsageHint DSPlug_PluginCaps_get_plugin_usage_hint( DSPlug_PluginCaps *p_caps ) {





}


DSPlug_Boolean DSPlug_PluginCaps_has_feature( DSPlug_PluginCaps *p_caps , DSPlug_PluginFeature f) {




}

int DSPlug_PluginCaps_get_constant( DSPlug_PluginCaps *p_caps , DSPlug_PluginConstant c) {





}

/****************************/

/* GENERAL PORT CAPS */

/****************************/


  
 int DSPlug_PluginCaps_get_port_count( DSPlug_PluginCaps *p_caps, DSPlug_PortType t) {




 }


 void DSPlug_PluginCaps_get_port_caption( DSPlug_PluginCaps *p_caps, DSPlug_PortType t, int i, char * s ) {




 }

  
 void DSPlug_PluginCaps_get_port_name( DSPlug_PluginCaps *p_caps, DSPlug_PortType t, int i, char * s ) {




 }


 DSPlug_PlugType DSPlug_PluginCaps_get_port_plug_type( DSPlug_PluginCaps *p_caps, DSPlug_PortType t, int i) {





 }

 void DSPlug_PluginCaps_get_port_path( DSPlug_PluginCaps *p_caps, DSPlug_PortType t, int i, char * s) {





 }


 DSPlug_AudioPortCaps DSPlug_PluginCaps_get_audio_port_caps( DSPlug_PluginCaps *p_caps, int i ) {





 }


 

 DSPlug_EventPortCaps DSPlug_PluginCaps_get_event_port_caps( DSPlug_PluginCaps *p_caps, int i ) {




 }
  

 DSPlug_ControlPortCaps DSPlug_PluginCaps_get_control_port_caps( DSPlug_PluginCaps *p_caps, int i ) {




 }
 
 /****************************/

 /* AUDIO PORT CAPS */

 /****************************/


 int DSPlug_AudioPortCaps_get_channel_count( DSPlug_AudioPortCaps * p_audio_caps ) {



 }


 /****************************/

 /* EVENT PORT CAPS */

 /****************************/


 DSPlug_EventType DSPlug_EventPortCaps_get_event_type( DSPlug_EventPortCaps *p_event_caps ) {



 }

 /****************************/

 /* CONTROL PORT CAPS */

 /****************************/

 
 DSPlug_ControlPortType DSPlug_ControlPortCaps_get_type( DSPlug_ControlPortCaps *p_control_caps) {




 }

 /* Numerical Ports */

 float DSPlug_ControlPortCaps_get_numerical_default( DSPlug_ControlPortCaps *p_control_caps ) {

	 

 }

 
 void DSPlug_ControlPortCaps_get_numerical_display( DSPlug_ControlPortCaps *p_control_caps , float v , char *s) {


	 

 }


 
 DSPlug_ControlPortNumericalHint DSPlug_ControlPortCaps_get_numerical_hint( DSPlug_ControlPortCaps *p_control_caps ) {


	 

 }


 int DSPlug_ControlPortCaps_get_numerical_option_count( DSPlug_ControlPortCaps *p_control_caps ) {


	 

 }


 void DSPlug_ControlPortCaps_get_numerical_option_caption( DSPlug_ControlPortCaps *p_control_caps , int o, char * s) {


	 

 }


 

 /* Numerical Port - Integer Options */

 int DSPlug_ControlPortCaps_get_numerical_integer_steps( DSPlug_ControlPortCaps *p_control_caps ) {


	 

 }


 

 /* String Port */

 
 const char * DSPlug_ControlPortCaps_get_string_default( DSPlug_ControlPortCaps *p_control_caps ) {


	 

 }


 

 /* Data Port */


 const void* DSPlug_ControlPortCaps_get_raw_data_default( DSPlug_ControlPortCaps *p_control_caps , int *l ) {


	 

 }


 

 /* Misc Port Settings */


 DSPlug_Boolean DSPlug_ControlPortCaps_is_editable( DSPlug_ControlPortCaps *p_control_caps ) {


	 

 }


 DSPlug_Boolean DSPlug_ControlPortCaps_is_realtime_safe( DSPlug_ControlPortCaps *p_control_caps ) {


	 

 }


 int DSPlug_ControlPortCaps_get_music_part( DSPlug_ControlPortCaps *p_control_caps ) {


	 

 }


 


 /****************************/

 /* PLUGIN INSTANCE */

 /****************************/

 /* SETTING UP AUDIO PORTS */


 void DSPlug_PluginInstance_connect_audio_port( DSPlug_PluginInstance *p_instance, int i, int c, float * b) {


	 

 }


 void DSPlug_PluginInstance_connect_event_port( DSPlug_PluginInstance *p_instance, int i, DSPlug_EventQueue *q ) {


	 

 }



 /* SETTING UP CONTROL PORTS */

 void DSPlug_PluginInstance_set_control_numerical_port( DSPlug_PluginInstance *p_instance, int i , float v ) {


	 

 }


 void DSPlug_PluginInstance_set_control_string_port( DSPlug_PluginInstance *p_instance, int i , const char * s ) {


	 

 }


 void DSPlug_PluginInstance_set_control_data_port( DSPlug_PluginInstance *p_instance, int i , const void * d, int l ) {


	 

 }


 
 float DSPlug_PluginInstance_get_control_numerical_port( DSPlug_PluginInstance *p_instance, int i ) {


	 

 }


 char * DSPlug_PluginInstance_get_control_string_port_no_realtime( DSPlug_PluginInstance *p_instance, int i ) {


	 

 }

 void DSPlug_PluginInstance_get_control_string_port( DSPlug_PluginInstance *p_instance, int i , char * s ) {


	 

 }


 void DSPlug_PluginInstance_get_control_port_data( DSPlug_PluginInstance *p_instance, int i , void ** d, int * l ) {


	 

 }


 void DSPlug_PluginInstance_set_control_port_changed_callback( DSPlug_PluginInstance *p_instance, int i , void (*c)(int, void *) , void * u) {


	 

 }


 
 /****************************/

 /* PROCESSING THE AUDIO */

 /****************************/

 void DSPlug_PluginInstance_process( DSPlug_PluginInstance *p_instance, int f ) {


	 

 }


 


 /* RESETTING THE STATE */

 void DSPlug_PluginInstance_reset( DSPlug_PluginInstance *p_instance) {


	 

 }


 


 /*******************/

 /* MISCELANEOUS	 */

 /*******************/


 int DSPlug_PluginInstance_get_output_delay( DSPlug_PluginInstance *p_instance) {


	 

 }



 int DSPlug_PluginInstance_get_skipped_initial_frames( DSPlug_PluginInstance *p_instance) {


	 

 }


 

 /**********************/

 /* USER INTERFACE  */

 /**********************/


