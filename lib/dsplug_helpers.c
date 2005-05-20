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

#include "dsplug_helpers.h"
#include <stdlib.h>
#include <string.h>

void DSPlug_copy_to_newstring(char** p_dst,const char *p_src) {

	if (*p_dst)
		free(*p_dst);
	*p_dst=(char*)malloc( strlen(p_src) + 1 );
	strcpy(*p_dst,p_src);
}

void DSPlug_validate_path_to_newstring(char** p_dst,const char *p_src) {


	char * auxbuf;

	if (!p_src)
		return;

	if (strlen(p_src)==0) { /* if none, set "/" */

		DSPlug_copy_to_newstring(p_dst,"/");
		return;
	} else if (strlen(p_src)==1 && p_src[0]=='/') {
		/* nothing to do */
		return;
	}

	auxbuf=(char*)malloc( strlen(p_src) + 3 ); /* +pre+post+null */

	if (p_src[0]!='/') {
		strcpy(&auxbuf[1],p_src);
		auxbuf[0]='/';
	}

	if (p_src[ strlen(p_src) - 1 ]!='/')
		auxbuf[ strlen(auxbuf) - 2 ] = 0; /* make last char '/' dissapear */

	DSPlug_copy_to_newstring(p_dst,auxbuf);

	free(auxbuf);


}

DSPlug_Boolean DSPlug_check_features_bit(DSPlug_PluginCapsPrivate *p_caps,DSPlug_PluginFeature f) {

	int byte=f/8;
	int bit=f%8;

	if (byte>=MAX_PLUGIN_CAPS_FEATURE_BYTES) {

		return DSPLUG_FALSE;
	}

	return (p_caps->features[byte]&(1<<bit) ) ? DSPLUG_TRUE : DSPLUG_FALSE;

}

void DSPlug_free_common_port_caps(DSPlug_CommonPortCapsPrivate *p_port_caps) {

	free(p_port_caps->name);
	free(p_port_caps->caption);
	free(p_port_caps->path);

}


void DSPlug_free_plugin_caps(DSPlug_PluginCapsPrivate *p_plugin_caps) {

        int i;

	free(p_plugin_caps->info_caption);
	free(p_plugin_caps->info_author);
	free(p_plugin_caps->info_copyright);
	free(p_plugin_caps->info_version);
	free(p_plugin_caps->info_compatible_version);
	free(p_plugin_caps->info_unique_ID);
	free(p_plugin_caps->info_description);
	free(p_plugin_caps->info_HTTP_URL);
	free(p_plugin_caps->info_category_path);

	for (i=0;i<p_plugin_caps->audio_port_count;i++) {

		DSPlug_free_common_port_caps(&p_plugin_caps->audio_port_caps[i]->common);
		free(p_plugin_caps->audio_port_caps[i]);
	}
	free(p_plugin_caps->audio_port_caps);

	for (i=0;i<p_plugin_caps->event_port_count;i++) {

		DSPlug_free_common_port_caps(&p_plugin_caps->event_port_caps[i]->common);
		free(p_plugin_caps->event_port_caps[i]);
	}
	free(p_plugin_caps->event_port_caps);

	for (i=0;i<p_plugin_caps->control_port_count;i++) {

		DSPlug_free_common_port_caps(&p_plugin_caps->control_port_caps[i]->common);
		free(p_plugin_caps->control_port_caps[i]);
	}
	free(p_plugin_caps->control_port_caps);


	free(p_plugin_caps);
}

