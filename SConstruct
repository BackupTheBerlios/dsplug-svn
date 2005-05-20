
unix_flags=['-Wall','-std=c89','-Iinclude/'];

targets = [\
	'lib/dsplug_host.c',
	'lib/dsplug_plugin.c',
        'lib/dsplug_library.c',
        'lib/dsplug_helpers.c',
        'lib/dsplug_error_report.c',
        'lib/dsplug_default_loader.c',
        ];
        
StaticLibrary('DSPlug', targets, CCFLAGS=unix_flags)
SharedLibrary('DSPlug', targets, CCFLAGS=unix_flags)
        

        






