#ifndef DSPLUG_HELPERS_H
#define DSPLUG_HELPERS_H

#include "dsplug_private.h"


void DSPlug_copy_to_newstring(char** p_dst,const char *p_src);

void DSPlug_free_common_port_caps(DSPlug_CommonPortCapsPrivate *);
void DSPlug_free_plugin_caps(DSPlug_PluginCapsPrivate *);
DSPlug_Boolean DSPlug_check_features_bit(DSPlug_PluginCapsPrivate *,DSPlug_PluginFeature f);

#endif