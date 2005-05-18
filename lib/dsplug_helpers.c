
#include "dsplug_helpers.h"
#include <stdlib.h>

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

