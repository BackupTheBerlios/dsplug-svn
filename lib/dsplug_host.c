
/**
 * \file dsplug_host.h
 * \author Juan Linietsky
 */


#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "dsplug_private.h"
#include "dsplug_host.h"
#include "dsplug_error_report.h"
#include "dsplug_library.h"
#include "dsplug_helpers.h"


/****************************/

/* PLUGIN LIBRARY */

/****************************/

/**
 *	Open a Plugin Library (plugin access object and shared data for
 *     the plugin instances).
 */

DSPlug_PluginLibrary * DSPlug_Host_open_plugin_library( const char * p ) {

	int i;

	DSPlug_PluginLibraryPrivate * library;
	DSPlug_PluginLibrary * library_public;

	char * full_path = DSPlug_LibraryCache_get_full_path(p);

	/* Attempt to see if the library cache has this */
	library = DSPlug_LibraryCache_get_library(full_path);

	/* It doesnt have it.. */
	if (!library) {

		for (i=0;i<DSPlug_LibraryFile_handler_count();i++) {

			library = DSPlug_get_LibraryFile_handler_open(i,full_path);
			if (library)
				break;


		}

		if (!library) {
			free(full_path);
			return NULL; /* no library handler for this library*/
		} else
			DSPlug_LibraryCache_add_library(library);


	}

	library->reference_count++;

	library_public = (DSPlug_PluginLibrary*)malloc(sizeof(DSPlug_PluginLibrary));;
	library_public->_private=library;

	free(full_path);

	return library_public;

}

/**
 *	Close a plugin handle. Call this when you dont want to use the plugin anymore.
 *     WARNING: This will invalidate any open plugin instances!
 *
 */

void DSPlug_Host_close_plugin_library( DSPlug_PluginLibrary *  p_library) {

	DSPlug_PluginLibraryPrivate *library = (DSPlug_PluginLibraryPrivate *) (p_library->_private);

	free(p_library); /* just free the library */
	library->reference_count--; /* dereference the library */

	if (library->reference_count==0) { /* no one is using the library anymore */

		DSPlug_LibraryCache_remove_library(library);
		DSPlug_LibraryFile_handler_close(library);
	}
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
	if (ui && !DSPlug_check_features_bit(library->plugin_caps_array[i],DSPLUG_PLUGIN_FEATURE_HAS_GUI)  ) {

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

void DSPlug_PluginLibrary_destroy_plugin_instance( DSPlug_PluginLibrary *  p_library, DSPlug_PluginInstance * p_instance) {

	DSPlug_Plugin *plugin_public = (DSPlug_Plugin *)p_instance->_private;
	DSPlug_PluginPrivate *plugin = (DSPlug_PluginPrivate *)plugin_public->_private;
	int i;

	if (plugin_public==NULL || plugin==NULL) {

		DSPlug_report_error("HOST: DSPlug_PluginLibrary_destroy_plugin_instance: Calling with NULL PluginInstance object ");
		return;
	}

	if (plugin->inside_process_callback_flag) {

		DSPlug_report_error("GRRR: STUPID PROGRAMMER! DONT DELETE THE INSTANCE WHILE YOU ARE STILL USING IT! I'M EXITING YOUR APP, NOW GO READ THE API DOCS!");
		exit(255);
	}

	/* first, get rid of the programmer userdata for the plugin */
	plugin->plugin_caps->destroy_plugin_userdata(plugin_public);

	for (i=0;i<plugin->audio_port_count;i++) {

		/* free the channel buffer connections of the port */
		free(plugin->audio_ports[i]->channel_buffer_ptr);
		/* free the port */
		free(plugin->audio_ports[i]);
	}

	for (i=0;i<plugin->event_port_count;i++) {

		/* free the port */
		free(plugin->event_ports[i]);
	}

	for (i=0;i<plugin->control_port_count;i++) {

		/* free the port */
		free(plugin->control_ports[i]);
	}

	free(plugin);
	free(plugin_public);

	/* Successful Deinitialization! */
}


void DSPlug_PluginCaps_get_caption( DSPlug_PluginCaps p_caps, char * s ) {

	DSPlug_PluginCapsPrivate *caps = (DSPlug_PluginCapsPrivate *)p_caps._private;

	if (caps==NULL) {

		DSPlug_report_error("HOST: DSPlug_PluginCaps_get_caption: Calling with NULL PluginCaps object ");
		return;
	}

	strcpy(s,caps->info_caption);


}


void DSPlug_PluginCaps_get_author( DSPlug_PluginCaps p_caps, char * s ) {

	DSPlug_PluginCapsPrivate *caps = (DSPlug_PluginCapsPrivate *)p_caps._private;
	if (caps==NULL) {

		DSPlug_report_error("HOST: DSPlug_PluginCaps_get_author: Calling with NULL PluginCaps object ");
		return;
	}

	strcpy(s,caps->info_author);
}


void DSPlug_PluginCaps_get_copyright( DSPlug_PluginCaps p_caps, char * s ) {

	DSPlug_PluginCapsPrivate *caps = (DSPlug_PluginCapsPrivate *)p_caps._private;
	if (caps==NULL) {

		DSPlug_report_error("HOST: DSPlug_PluginCaps_get_copyright: Calling with NULL PluginCaps object ");
		return;
	}


	strcpy(s,caps->info_copyright);

}

void DSPlug_PluginCaps_get_version( DSPlug_PluginCaps p_caps, char * s ) {

	DSPlug_PluginCapsPrivate *caps = (DSPlug_PluginCapsPrivate *)p_caps._private;
	if (caps==NULL) {

		DSPlug_report_error("HOST: DSPlug_PluginCaps_get_version: Calling with NULL PluginCaps object ");
		return;
	}

	strcpy(s,caps->info_version);

}

void DSPlug_PluginCaps_get_compatible_version( DSPlug_PluginCaps p_caps, char *s ) {

	DSPlug_PluginCapsPrivate *caps = (DSPlug_PluginCapsPrivate *)p_caps._private;
	if (caps==NULL) {

		DSPlug_report_error("HOST: DSPlug_PluginCaps_get_compatible_version: Calling with NULL PluginCaps object ");
		return;
	}

	strcpy(s,caps->info_compatible_version);

}


void DSPlug_PluginCaps_get_unique_ID( DSPlug_PluginCaps p_caps, char *s ) {

	DSPlug_PluginCapsPrivate *caps = (DSPlug_PluginCapsPrivate *)p_caps._private;
	if (caps==NULL) {

		DSPlug_report_error("HOST: DSPlug_PluginCaps_get_unique_ID: Calling with NULL PluginCaps object ");
		return;
	}

	strcpy(s,caps->info_unique_ID);

}

void DSPlug_PluginCaps_get_category_path( DSPlug_PluginCaps p_caps, char *s ) {

	DSPlug_PluginCapsPrivate *caps = (DSPlug_PluginCapsPrivate *)p_caps._private;
	if (caps==NULL) {

		DSPlug_report_error("HOST: DSPlug_PluginCaps_get_category_path: Calling with NULL PluginCaps object ");
		return;
	}

	strcpy(s,caps->info_category_path);
}

DSPlug_PluginUsageHint DSPlug_PluginCaps_get_plugin_usage_hint( DSPlug_PluginCaps p_caps ) {

	DSPlug_PluginCapsPrivate *caps = (DSPlug_PluginCapsPrivate *)p_caps._private;
	if (caps==NULL) {

		DSPlug_report_error("HOST: DSPlug_PluginCaps_get_plugin_usage_hint: Calling with NULL PluginCaps object ");
		return DSPLUG_USAGE_HINT_GENERIC; /* just return something */
	}


	return caps->usage_hint;

}


DSPlug_Boolean DSPlug_PluginCaps_has_feature( DSPlug_PluginCaps p_caps , DSPlug_PluginFeature f) {

	DSPlug_PluginCapsPrivate *caps = (DSPlug_PluginCapsPrivate *)p_caps._private;
	if (caps==NULL) {

		DSPlug_report_error("HOST: DSPlug_PluginCaps_has_feature: Calling with NULL PluginCaps object ");
		return DSPLUG_FALSE; /* just return something */
	}

	return DSPlug_check_features_bit(caps,f);

}

int DSPlug_PluginCaps_get_constant( DSPlug_PluginCaps p_caps , DSPlug_PluginConstant c) {

	DSPlug_PluginCapsPrivate *caps = (DSPlug_PluginCapsPrivate *)p_caps._private;
	if (caps==NULL) {

		DSPlug_report_error("HOST: DSPlug_PluginCaps_has_feature: Calling with NULL PluginCaps object ");
		return 0; /* just return something */
	}

	if (c>=MAX_PLUGIN_CAPS_CONSTANTS) {

		DSPlug_report_error("API: DSPlug_PluginCaps_has_feature: Constant exceeds array, bug? ");
		return 0; /* just return something */

	}

	return caps->constants[c];
}

/****************************/

/* GENERAL PORT CAPS */

/****************************/



 int DSPlug_PluginCaps_get_port_count( DSPlug_PluginCaps p_caps, DSPlug_PortType t) {

	 DSPlug_PluginCapsPrivate *caps = (DSPlug_PluginCapsPrivate *)p_caps._private;
	 if (caps==NULL) {

		 DSPlug_report_error("HOST: DSPlug_PluginCaps_get_port_count: Calling with NULL PluginCaps object ");
		 return 0; /* just return something */
	 }


	 switch (t) {
		 case DSPLUG_PORT_AUDIO: return caps->audio_port_count; break;
		 case DSPLUG_PORT_EVENT: return caps->event_port_count; break;
		 case DSPLUG_PORT_CONTROL: return caps->control_port_count; break;
	 }

	 DSPlug_report_error("API: DSPlug_PluginCaps_get_port_count: Something Wicked Happened! ");

	 return 0;
 }


 void DSPlug_PluginCaps_get_port_caption( DSPlug_PluginCaps p_caps, DSPlug_PortType t, int i, char * s ) {

	 DSPlug_PluginCapsPrivate *caps = (DSPlug_PluginCapsPrivate *)p_caps._private;
	 int count=0;
	 DSPlug_CommonPortCapsPrivate **common_port_caps;
	 if (caps==NULL) {

		 DSPlug_report_error("HOST: DSPlug_PluginCaps_get_port_caption: Calling with NULL PluginCaps object ");
		 return;
	 }

	 switch (t) {
		 case DSPLUG_PORT_AUDIO: {
		 	count = caps->audio_port_count;
			common_port_caps = (DSPlug_CommonPortCapsPrivate **)caps->audio_port_caps;
		 } break;
		 case DSPLUG_PORT_EVENT: {
			 count = caps->event_port_count;
			 common_port_caps = (DSPlug_CommonPortCapsPrivate **)caps->event_port_caps;
		 } break;
		 case DSPLUG_PORT_CONTROL: {
			 count = caps->control_port_count;
			 common_port_caps = (DSPlug_CommonPortCapsPrivate **)caps->control_port_caps;
		 } break;
	 }

	 if (i<0 || i>=count) {

		 DSPlug_report_error("HOST: DSPlug_PluginCaps_get_port_caption: Invalid port index ");
		 return;

	 }

	 strcpy(s,common_port_caps[i]->caption);

 }


 void DSPlug_PluginCaps_get_port_name( DSPlug_PluginCaps p_caps, DSPlug_PortType t, int i, char * s ) {

	 DSPlug_PluginCapsPrivate *caps = (DSPlug_PluginCapsPrivate *)p_caps._private;
	 int count;
	 DSPlug_CommonPortCapsPrivate **common_port_caps;
	 if (caps==NULL) {

		 DSPlug_report_error("HOST: DSPlug_PluginCaps_get_port_name: Calling with NULL PluginCaps object ");
		 return;
	 }

	 switch (t) {
		 case DSPLUG_PORT_AUDIO: {
			 count = caps->audio_port_count;
			 common_port_caps = (DSPlug_CommonPortCapsPrivate **)caps->audio_port_caps;
		 } break;
		 case DSPLUG_PORT_EVENT: {
			 count = caps->event_port_count;
			 common_port_caps = (DSPlug_CommonPortCapsPrivate **)caps->event_port_caps;
		 } break;
		 case DSPLUG_PORT_CONTROL: {
			 count = caps->control_port_count;
			 common_port_caps = (DSPlug_CommonPortCapsPrivate **)caps->control_port_caps;
		 } break;
	 }

	 if (i<0 || i>=count) {

		 DSPlug_report_error("HOST: DSPlug_PluginCaps_get_port_caption: Invalid port index ");
		 return;

	 }

	 strcpy(s,common_port_caps[i]->name);


 }


 DSPlug_PlugType DSPlug_PluginCaps_get_port_plug_type( DSPlug_PluginCaps p_caps, DSPlug_PortType t, int i) {

	 DSPlug_PluginCapsPrivate *caps = (DSPlug_PluginCapsPrivate *)p_caps._private;
	 int count;
	 DSPlug_CommonPortCapsPrivate **common_port_caps;
	 if (caps==NULL) {

		 DSPlug_report_error("HOST: DSPlug_PluginCaps_get_port_plug_type: Calling with NULL PluginCaps object ");
		 return DSPLUG_PLUG_OUTPUT; /* just return something */
	 }

	 switch (t) {
		 case DSPLUG_PORT_AUDIO: {
			 count = caps->audio_port_count;
			 common_port_caps = (DSPlug_CommonPortCapsPrivate **)caps->audio_port_caps;
		 } break;
		 case DSPLUG_PORT_EVENT: {
			 count = caps->event_port_count;
			 common_port_caps = (DSPlug_CommonPortCapsPrivate **)caps->event_port_caps;
		 } break;
		 case DSPLUG_PORT_CONTROL: {
			 count = caps->control_port_count;
			 common_port_caps = (DSPlug_CommonPortCapsPrivate **)caps->control_port_caps;
		 } break;
	 }

	 if (i<0 || i>=count) {

		 DSPlug_report_error("HOST: DSPlug_PluginCaps_get_port_plug_type: Invalid port index ");
		 return DSPLUG_PLUG_OUTPUT; /* just return something */

	 }

	 return common_port_caps[i]->plug_type;

 }

 void DSPlug_PluginCaps_get_port_path( DSPlug_PluginCaps p_caps, DSPlug_PortType t, int i, char * s) {

	 DSPlug_PluginCapsPrivate *caps = (DSPlug_PluginCapsPrivate *)p_caps._private;
	 int count;
	 DSPlug_CommonPortCapsPrivate **common_port_caps;
	 if (caps==NULL) {

		 DSPlug_report_error("HOST: DSPlug_PluginCaps_get_port_path: Calling with NULL PluginCaps object ");
		 return ;
	 }


	 switch (t) {
		 case DSPLUG_PORT_AUDIO: {
			 count = caps->audio_port_count;
			 common_port_caps = (DSPlug_CommonPortCapsPrivate **)caps->audio_port_caps;
		 } break;
		 case DSPLUG_PORT_EVENT: {
			 count = caps->event_port_count;
			 common_port_caps = (DSPlug_CommonPortCapsPrivate **)caps->event_port_caps;
		 } break;
		 case DSPLUG_PORT_CONTROL: {
			 count = caps->control_port_count;
			 common_port_caps = (DSPlug_CommonPortCapsPrivate **)caps->control_port_caps;
		 } break;
	 }

	 if (i<0 || i>=count) {

		 DSPlug_report_error("HOST: DSPlug_PluginCaps_get_port_path: Invalid port index ");
		 return;

	 }

	 strcpy(s,common_port_caps[i]->path);


 }


 DSPlug_AudioPortCaps DSPlug_PluginCaps_get_audio_port_caps( DSPlug_PluginCaps p_caps, int i ) {

	 DSPlug_PluginCapsPrivate *caps = (DSPlug_PluginCapsPrivate *)p_caps._private;
	 DSPlug_AudioPortCaps port_caps;
	 port_caps._private=NULL;
	 if (caps==NULL) {

		 DSPlug_report_error("HOST: DSPlug_PluginCaps_get_audio_port_caps: Calling with NULL PluginCaps object ");
		 return port_caps;
	 }

	 if (i<0 || i>=caps->audio_port_count) {

		 DSPlug_report_error("HOST: DSPlug_PluginCaps_get_audio_port_caps: invalid port index ");
		 return port_caps;
	 }

	 port_caps._private=caps->audio_port_caps[i];

	 return port_caps;

 }




 DSPlug_EventPortCaps DSPlug_PluginCaps_get_event_port_caps( DSPlug_PluginCaps p_caps, int i ) {

	 DSPlug_PluginCapsPrivate *caps = (DSPlug_PluginCapsPrivate *)p_caps._private;
	 DSPlug_EventPortCaps port_caps;
	 port_caps._private=NULL;
	 if (caps==NULL) {

		 DSPlug_report_error("HOST: DSPlug_PluginCaps_get_event_port_caps: Calling with NULL PluginCaps object ");
		 return port_caps;
	 }

	 if (i<0 || i>=caps->event_port_count) {

		 DSPlug_report_error("HOST: DSPlug_PluginCaps_get_event_port_caps: invalid port index ");
		 return port_caps;
	 }

	 port_caps._private=caps->event_port_caps[i];

	 return port_caps;
 }


 DSPlug_ControlPortCaps DSPlug_PluginCaps_get_control_port_caps( DSPlug_PluginCaps p_caps, int i ) {

	 DSPlug_PluginCapsPrivate *caps = (DSPlug_PluginCapsPrivate *)p_caps._private;
	 DSPlug_ControlPortCaps port_caps;
	 port_caps._private=NULL;
	 if (caps==NULL) {

		 DSPlug_report_error("HOST: DSPlug_PluginCaps_get_control_port_caps: Calling with NULL PluginCaps object ");
		 return port_caps;
	 }

	 if (i<0 || i>=caps->control_port_count) {

		 DSPlug_report_error("HOST: DSPlug_PluginCaps_get_control_port_caps: invalid port index ");
		 return port_caps;
	 }

	 port_caps._private=caps->control_port_caps[i];

	 return port_caps;

 }

 /****************************/

 /* AUDIO PORT CAPS */

 /****************************/


 int DSPlug_AudioPortCaps_get_channel_count( DSPlug_AudioPortCaps p_audio_caps ) {

	 DSPlug_AudioPortCapsPrivate *audio_caps = (DSPlug_AudioPortCapsPrivate *)p_audio_caps._private;
	 if (audio_caps==NULL) {

		 DSPlug_report_error("HOST: DSPlug_AudioPortCaps_get_channel_count: Calling with NULL AudioPortCaps ");
		 return 0; /* return something */
	 }

	 return audio_caps->channel_count;

 }


 /****************************/

 /* EVENT PORT CAPS */

 /****************************/


 DSPlug_EventType DSPlug_EventPortCaps_get_event_type( DSPlug_EventPortCaps p_event_caps ) {

	 DSPlug_EventPortCapsPrivate *event_caps = (DSPlug_EventPortCapsPrivate *)p_event_caps._private;
	 if (event_caps==NULL) {

		 DSPlug_report_error("HOST: DSPlug_EventPortCaps_get_event_type: Calling with NULL EventPortCaps ");
		 return DSPLUG_EVENT_TYPE_AUDIO; /* return something */
	 }

	 return event_caps->event_type;

 }

 /****************************/

 /* CONTROL PORT CAPS */

 /****************************/


 DSPlug_ControlPortType DSPlug_ControlPortCaps_get_type( DSPlug_ControlPortCaps p_control_caps) {

	 DSPlug_ControlPortCapsPrivate *control_caps = (DSPlug_ControlPortCapsPrivate *)p_control_caps._private;

	 if (control_caps==NULL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_type: Calling with NULL ControlPortCaps ");
		 return DSPLUG_CONTROL_PORT_TYPE_NUMERICAL; /* return something */
	 }


	 return control_caps->type;

 }

 /* Numerical Ports */


 void DSPlug_ControlPortCaps_get_numerical_display( DSPlug_ControlPortCaps p_control_caps , float v , char *s) {

	 DSPlug_ControlPortCapsPrivate *control_caps = (DSPlug_ControlPortCapsPrivate *)p_control_caps._private;

	 if (control_caps==NULL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_numerical_display: Calling with NULL ControlPortCaps ");
		 return ; /* return something */
	 }

	 if (control_caps->type!=DSPLUG_CONTROL_PORT_TYPE_NUMERICAL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_numerical_display: Port is not DSPLUG_CONTROL_PORT_TYPE_NUMERICAL ");
		 return ;

	 }

	 /* If a function for display was provided by the plugin then use that */

	 if (control_caps->float_display_function!=NULL) {

		 control_caps->float_display_function(v,s);

		 /* If not, then we just use the default percentage display */
	 } else {
		 /* Display from 0 to 100 percentage */
		 sprintf(s,"%.2f%%",v*100.0f);

	 }

 }



 DSPlug_ControlPortNumericalHint DSPlug_ControlPortCaps_get_numerical_hint( DSPlug_ControlPortCaps p_control_caps ) {


	 DSPlug_ControlPortCapsPrivate *control_caps = (DSPlug_ControlPortCapsPrivate *)p_control_caps._private;

	 if (control_caps==NULL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_numerical_hint: Calling with NULL ControlPortCaps ");
		 return DSPLUG_CONTROL_PORT_HINT_TYPE_FLOAT; /* return something */
	 }

	 if (control_caps->type!=DSPLUG_CONTROL_PORT_TYPE_NUMERICAL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_numerical_hint: Port is not DSPLUG_CONTROL_PORT_TYPE_NUMERICAL ");
		 return DSPLUG_CONTROL_PORT_HINT_TYPE_FLOAT;

	 }

	 return control_caps->numerical_hint;


 }






 /* Numerical Port - Integer Options */

 int DSPlug_ControlPortCaps_get_numerical_integer_steps( DSPlug_ControlPortCaps p_control_caps ) {


	 DSPlug_ControlPortCapsPrivate *control_caps = (DSPlug_ControlPortCapsPrivate *)p_control_caps._private;

	 if (control_caps==NULL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_numerical_integer_steps: Calling with NULL ControlPortCaps ");
		 return 0; /* return something */
	 }

	 if (control_caps->type!=DSPLUG_CONTROL_PORT_TYPE_NUMERICAL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_numerical_integer_steps: Port is not DSPLUG_CONTROL_PORT_TYPE_NUMERICAL ");
		 return 0;

	 }

	 if (control_caps->numerical_hint!=DSPLUG_CONTROL_PORT_HINT_TYPE_INTEGER) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_numerical_integer_steps: Port is not DSPLUG_CONTROL_PORT_HINT_TYPE_INTEGER ");
		 return 0;

	 }



	 return control_caps->integer_steps;
 }


 DSPlug_Boolean DSPlug_ControlPortCaps_is_numerical_integer_enum( DSPlug_ControlPortCaps p_control_caps) {

	 DSPlug_ControlPortCapsPrivate *control_caps = (DSPlug_ControlPortCapsPrivate *)p_control_caps._private;

	 if (control_caps==NULL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_is_numerical_integer_enum: Calling with NULL ControlPortCaps ");
		 return 0; /* return something */
	 }

	 if (control_caps->type!=DSPLUG_CONTROL_PORT_TYPE_NUMERICAL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_is_numerical_integer_enum: Port is not DSPLUG_CONTROL_PORT_TYPE_NUMERICAL ");
		 return 0;

	 }

	 if (control_caps->numerical_hint!=DSPLUG_CONTROL_PORT_HINT_TYPE_INTEGER) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_is_numerical_integer_enum: Port is not DSPLUG_CONTROL_PORT_HINT_TYPE_INTEGER ");
		 return 0;

	 }

	 return control_caps->integer_is_enum;


 }

 /* String Port */




 /* Data Port */



 /* Misc Port Settings */


 DSPlug_Boolean DSPlug_ControlPortCaps_is_hidden( DSPlug_ControlPortCaps p_control_caps ) {


	 DSPlug_ControlPortCapsPrivate *control_caps = (DSPlug_ControlPortCapsPrivate *)p_control_caps._private;

	 if (control_caps==NULL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_is_hidden: Calling with NULL ControlPortCaps ");
		 return DSPLUG_FALSE; /* return anything */
	 }

	 return control_caps->is_hidden;


 }


 DSPlug_Boolean DSPlug_ControlPortCaps_is_realtime_safe( DSPlug_ControlPortCaps p_control_caps ) {


	 DSPlug_ControlPortCapsPrivate *control_caps = (DSPlug_ControlPortCapsPrivate *)p_control_caps._private;

	 if (control_caps==NULL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_is_realtime_safe: Calling with NULL ControlPortCaps ");
		 return DSPLUG_FALSE; /* return anything */
	 }

	 return control_caps->is_realtime_safe;


 }


 int DSPlug_ControlPortCaps_get_music_part( DSPlug_ControlPortCaps p_control_caps ) {

	 DSPlug_ControlPortCapsPrivate *control_caps = (DSPlug_ControlPortCapsPrivate *)p_control_caps._private;

	 if (control_caps==NULL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_musical_part: Calling with NULL ControlPortCaps ");
		 return 0; /* return anything */
	 }

	 return control_caps->musical_part;

 }





 /****************************/

 /* PLUGIN INSTANCE */

 /****************************/

 /* SETTING UP AUDIO PORTS */


 void DSPlug_PluginInstance_connect_audio_port( DSPlug_PluginInstance *p_instance, int i, int c, float * b) {

	 DSPlug_Plugin *plugin_public = (DSPlug_Plugin *)p_instance->_private;
	 DSPlug_PluginPrivate *plugin = (DSPlug_PluginPrivate *)plugin_public->_private;

	 if (plugin_public==NULL || plugin==NULL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_connect_audio_port: Calling with NULL PluginInstance ");
		 return ; /* return anything */
	 }

	 if (i<0 || i>=plugin->audio_port_count) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_connect_audio_port: Invalid Audio Port Index ");
		 return ; /* return anything */
	 }

	 if (c<0 || c>=plugin->audio_ports[i]->channel_count) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_connect_audio_port: Invalid Audio Port Channel Index ");
		 return ; /* return anything */
	 }

	 plugin->audio_ports[i]->channel_buffer_ptr[c]=b;
 }


 void DSPlug_PluginInstance_connect_event_port( DSPlug_PluginInstance *p_instance, int i, DSPlug_EventQueue *q ) {

	 DSPlug_Plugin *plugin_public = (DSPlug_Plugin *)p_instance->_private;
	 DSPlug_PluginPrivate *plugin = (DSPlug_PluginPrivate *)plugin_public->_private;

	 if (plugin_public==NULL || plugin==NULL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_connect_event_port: Calling with NULL PluginInstance ");
		 return ; /* return anything */
	 }

	 if (i<0 || i>=plugin->event_port_count) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_connect_event_port: Invalid Event Port Index ");
		 return ; /* return anything */
	 }

	 plugin->event_ports[i]->queue=q;

 }



 /* SETTING UP CONTROL PORTS */

 void DSPlug_PluginInstance_set_control_numerical_port( DSPlug_PluginInstance *p_instance, int i , float v ) {

	 DSPlug_Plugin *plugin_public = (DSPlug_Plugin *)p_instance->_private;
	 DSPlug_PluginPrivate *plugin = (DSPlug_PluginPrivate *)plugin_public->_private;

	 if (plugin_public==NULL || plugin==NULL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_connect_event_port: Calling with NULL PluginInstance ");
		 return ; /* return anything */
	 }

	 if (i<0 || i>=plugin->control_port_count) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_set_control_numerical_port: Invalid Control Port Index ");
		 return ; /* return anything */
	 }

	 if (plugin->plugin_caps->control_port_caps[i]->type!=DSPLUG_CONTROL_PORT_TYPE_NUMERICAL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_set_control_numerical_port: Port is not of numerical type ");
		 return ; /* return anything */

	 }

	 if (plugin->plugin_caps->control_port_caps[i]->set_callback_numerical) {

		 plugin->plugin_caps->control_port_caps[i]->set_callback_numerical(*plugin_public,i,v);
	 } else {

		 DSPlug_report_error("API: DSPlug_ControlPortCaps_set_control_numerical_port: Control Port not configured, Bug? ");
	 }

 }


 void DSPlug_PluginInstance_set_control_string_port( DSPlug_PluginInstance *p_instance, int i , const char * s ) {

	 DSPlug_Plugin *plugin_public = (DSPlug_Plugin *)p_instance->_private;
	 DSPlug_PluginPrivate *plugin = (DSPlug_PluginPrivate *)plugin_public->_private;

	 if (plugin_public==NULL || plugin==NULL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_connect_event_port: Calling with NULL PluginInstance ");
		 return ; /* return anything */
	 }

	 if (i<0 || i>=plugin->control_port_count) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_set_control_string_port: Invalid Control Port Index ");
		 return ; /* return anything */
	 }

	 if (plugin->plugin_caps->control_port_caps[i]->type!=DSPLUG_CONTROL_PORT_TYPE_STRING) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_set_control_string_port: Port is not of string type ");
		 return ; /* return anything */

	 }

	 if (plugin->plugin_caps->control_port_caps[i]->set_callback_string) {

		 plugin->plugin_caps->control_port_caps[i]->set_callback_string(*plugin_public,i,s);
	 } else {

		 DSPlug_report_error("API: DSPlug_ControlPortCaps_set_string_string_port: Control Port not configured, Bug? ");
	 }


 }


 void DSPlug_PluginInstance_set_control_data_port( DSPlug_PluginInstance *p_instance, int i , const void * d, int l ) {

	 DSPlug_Plugin *plugin_public = (DSPlug_Plugin *)p_instance->_private;
	 DSPlug_PluginPrivate *plugin = (DSPlug_PluginPrivate *)plugin_public->_private;

	 if (plugin_public==NULL || plugin==NULL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_set_control_data_port: Calling with NULL PluginInstance ");
		 return ; /* return anything */
	 }

	 if (i<0 || i>=plugin->control_port_count) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_set_control_data_port: Invalid Control Port Index ");
		 return ; /* return anything */
	 }

	 if (plugin->plugin_caps->control_port_caps[i]->type!=DSPLUG_CONTROL_PORT_TYPE_DATA) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_set_control_data_port: Port is not of data type ");
		 return ; /* return anything */

	 }

	 if (plugin->plugin_caps->control_port_caps[i]->set_callback_data) {

		 plugin->plugin_caps->control_port_caps[i]->set_callback_data(*plugin_public,i,d,l);
	 } else {

		 DSPlug_report_error("API: DSPlug_ControlPortCaps_set_data_data_port: Control Port not configured, Bug? ");
	 }


 }



 float DSPlug_PluginInstance_get_control_numerical_port( DSPlug_PluginInstance *p_instance, int i ) {

	 DSPlug_Plugin *plugin_public = (DSPlug_Plugin *)p_instance->_private;
	 DSPlug_PluginPrivate *plugin = (DSPlug_PluginPrivate *)plugin_public->_private;

	 if (plugin_public==NULL || plugin==NULL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_control_numerical_port: Calling with NULL PluginInstance ");
		 return 0; /* return anything */
	 }

	 if (i<0 || i>=plugin->control_port_count) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_control_numerical_port: Invalid Control Port Index ");
		 return 0; /* return anything */
	 }

	 if (plugin->plugin_caps->control_port_caps[i]->type!=DSPLUG_CONTROL_PORT_TYPE_NUMERICAL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_control_numerical_port: Port is not of numerical type ");
		 return 0; /* return anything */
	 }

	 if (plugin->plugin_caps->control_port_caps[i]->set_callback_numerical) {

		 return plugin->plugin_caps->control_port_caps[i]->get_callback_numerical(*plugin_public,i);
	 } else {

		 DSPlug_report_error("API: DSPlug_ControlPortCaps_get_control_numerical_port: Control Port not configured, Bug? ");
		 return 0;
	 }
 }


 char * DSPlug_PluginInstance_get_control_string_port( DSPlug_PluginInstance *p_instance, int i ) {

	 DSPlug_Plugin *plugin_public = (DSPlug_Plugin *)p_instance->_private;
	 DSPlug_PluginPrivate *plugin = (DSPlug_PluginPrivate *)plugin_public->_private;

	 if (plugin_public==NULL || plugin==NULL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_control_string_port: Calling with NULL PluginInstance ");
		 return NULL; /* return anything */
	 }

	 if (i<0 || i>=plugin->control_port_count) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_control_string_port: Invalid Control Port Index ");
		 return NULL; /* return anything */
	 }

	 if (plugin->plugin_caps->control_port_caps[i]->type!=DSPLUG_CONTROL_PORT_TYPE_STRING) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_control_string_port: Port is not of string type ");
		 return NULL; /* return anything */

	 }

	 if (plugin->plugin_caps->control_port_caps[i]->is_realtime_safe) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_control_string_port: Port is of type realtime safe, not allowing call. ");
		 return ; /* return anything */

	 }
	 

	 if (plugin->plugin_caps->control_port_caps[i]->get_callback_string) {

		 return plugin->plugin_caps->control_port_caps[i]->get_callback_string(*plugin_public,i);
	 } else {

		 DSPlug_report_error("API: DSPlug_ControlPortCaps_get_control_string_port: Control Port not configured, Bug? ");
		 return NULL;
	 }



 }

 void DSPlug_PluginInstance_get_control_string_port_realtime( DSPlug_PluginInstance *p_instance, int i , char * s ) {

	 DSPlug_Plugin *plugin_public = (DSPlug_Plugin *)p_instance->_private;
	 DSPlug_PluginPrivate *plugin = (DSPlug_PluginPrivate *)plugin_public->_private;

	 if (plugin_public==NULL || plugin==NULL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_control_string_port_realtime: Calling with NULL PluginInstance ");
		 return ; /* return anything */
	 }

	 if (i<0 || i>=plugin->control_port_count) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_control_string_port_realtime: Invalid Control Port Index ");
		 return ; /* return anything */
	 }

	 if (plugin->plugin_caps->control_port_caps[i]->type!=DSPLUG_CONTROL_PORT_TYPE_STRING) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_control_string_port_realtime: Port is not of string type ");
		 return ; /* return anything */

	 }

	 if (!plugin->plugin_caps->control_port_caps[i]->is_realtime_safe) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_control_string_port_realtime: Port is not realtime safe, not allowing call. ");
		 return ; /* return anything */

	 }
	 
	 if (plugin->plugin_caps->control_port_caps[i]->get_callback_string) {

		 plugin->plugin_caps->control_port_caps[i]->get_callback_string_realtime(*plugin_public,i,s);
	 } else {

		 DSPlug_report_error("API: DSPlug_ControlPortCaps_get_control_string_port_realtime: Control Port not configured, Bug? ");
	 }



 }


 void DSPlug_PluginInstance_get_control_port_data( DSPlug_PluginInstance *p_instance, int i , void ** d, int * l ) {

	 DSPlug_Plugin *plugin_public = (DSPlug_Plugin *)p_instance->_private;
	 DSPlug_PluginPrivate *plugin = (DSPlug_PluginPrivate *)plugin_public->_private;

	 if (plugin_public==NULL || plugin==NULL) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_control_data_port: Calling with NULL PluginInstance ");
		 return ; /* return anything */
	 }

	 if (i<0 || i>=plugin->control_port_count) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_control_data_port: Invalid Control Port Index ");
		 return ; /* return anything */
	 }

	 if (plugin->plugin_caps->control_port_caps[i]->type!=DSPLUG_CONTROL_PORT_TYPE_STRING) {

		 DSPlug_report_error("HOST: DSPlug_ControlPortCaps_get_control_data_port: Port is not of data type ");
		 return ; /* return anything */

	 }

	 if (plugin->plugin_caps->control_port_caps[i]->get_callback_data) {

		 plugin->plugin_caps->control_port_caps[i]->get_callback_data(*plugin_public,i,d,l);
	 } else {

		 DSPlug_report_error("API: DSPlug_ControlPortCaps_get_control_data_port: Control Port not configured, Bug? ");
	 }


 }


 void DSPlug_PluginInstance_set_UI_changed_control_port_callback( DSPlug_PluginInstance *p_instance, int i , void (*c)(int, void *) , void * u) {

	 DSPlug_Plugin *plugin_public = (DSPlug_Plugin *)p_instance->_private;
	 DSPlug_PluginPrivate *plugin = (DSPlug_PluginPrivate *)plugin_public->_private;

	 if (plugin_public==NULL || plugin==NULL) {

		 DSPlug_report_error("HOST: DSPlug_PluginInstance_set_control_port_changed_callback: Calling with NULL PluginInstance ");
		 return ; /* return anything */
	 }

	 if (i<0 || i>=plugin->control_port_count) {

		 DSPlug_report_error("HOST: DSPlug_PluginInstance_set_control_port_changed_callback: Invalid Control Port Index ");
		 return ; /* return anything */
	 }

	 plugin->control_ports[i]->UI_changed_callback=c;
	 plugin->control_ports[i]->UI_changed_callback_userdata=u;

 }



 /****************************/

 /* PROCESSING THE AUDIO */

 /****************************/

 void DSPlug_PluginInstance_process( DSPlug_PluginInstance *p_instance, int f ) {

	 DSPlug_Plugin *plugin_public = (DSPlug_Plugin *)p_instance->_private;
	 DSPlug_PluginPrivate *plugin = (DSPlug_PluginPrivate *)plugin_public->_private;

	 if (plugin_public==NULL || plugin==NULL) {

		 DSPlug_report_error("HOST: DSPlug_PluginInstance_process: Calling with NULL PluginInstance ");
		 return ; /* return anything */
	 }

	 if (plugin->plugin_caps->process_callback && plugin->inside_process_callback_flag) {

		 plugin->inside_process_callback_flag=DSPLUG_TRUE;
		 plugin->plugin_caps->process_callback(plugin_public,f);
		 plugin->inside_process_callback_flag=DSPLUG_FALSE;
	 } else {

		 if (plugin->inside_process_callback_flag)
			 DSPlug_report_error("API: DSPlug_PluginInstance_process: Attempt to call process() when already processing! ");
		 else
			 DSPlug_report_error("API: DSPlug_PluginInstance_process: Plugin lacks process() callback, bug? ");
	 }
 }





 /* RESETTING THE STATE */

 void DSPlug_PluginInstance_reset( DSPlug_PluginInstance *p_instance) {

	 DSPlug_Plugin *plugin_public = (DSPlug_Plugin *)p_instance->_private;
	 DSPlug_PluginPrivate *plugin = (DSPlug_PluginPrivate *)plugin_public->_private;

	 if (plugin_public==NULL || plugin==NULL) {

		 DSPlug_report_error("HOST: DSPlug_PluginInstance_reset: Calling with NULL PluginInstance ");
		 return ; /* return anything */
	 }

	 if (plugin->plugin_caps->reset_callback) {

		 plugin->plugin_caps->reset_callback(plugin_public);
	 } else {

		 DSPlug_report_error("API: DSPlug_PluginInstance_reset: Plugin lacks reset() callback, bug? ");
	 }

 }





 /*******************/

 /* MISCELANEOUS	 */

 /*******************/


 int DSPlug_PluginInstance_get_output_delay( DSPlug_PluginInstance *p_instance) {

	 DSPlug_Plugin *plugin_public = (DSPlug_Plugin *)p_instance->_private;
	 DSPlug_PluginPrivate *plugin = (DSPlug_PluginPrivate *)plugin_public->_private;

	 if (plugin_public==NULL || plugin==NULL) {

		 DSPlug_report_error("HOST: DSPlug_PluginInstance_get_output_delay: Calling with NULL PluginInstance ");
		 return 0; /* return anything */
	 }

	 if (plugin->plugin_caps->get_output_delay_callback) {

		 return plugin->plugin_caps->get_output_delay_callback(plugin_public);
	 }

	 return 0;

	 /* It's fine if a plugin lacks get_output_delay_callback callback.. some may not need it */

 }




 /**********************/

 /* USER INTERFACE  */

 /**********************/


