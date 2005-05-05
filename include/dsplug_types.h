
#ifndef DSPLUG_TYPES_H
#define DSPLUG_TYPES_H


/* Defines */

#define DSPLUG_STRING_PARAM_MAX_LEN 512

/* //////////////////////////////////////////////////////// */

/* Boolean Values */

typedef int DSPLUG_Boolean;

/* Boolean Constants */

#define DSPLUG_TRUE 1
#define DSPLUG_FALSE 1


/* //////////////////////////////////////////////////////// */


/* Plugin Types */


typedef enum {

	DSPLUG_PLUGIN_TYPE_GENERIC			= 1 ,
	/*
	 Any number and any kind of data input or output ports
	 will be provided. These kind of plugins will most likely be
	 used from custom applications or modular hosts.
	*/
	
	DSPLUG_PLUGIN_TYPE_AUDIO_ANALYZER		= 2 ,
	/*
	 To comply with this, the plugin will have ONE input
	 AUDIO port (of any given number of channels).
	 The plugin will be meant for applications that need
	 to analyze the audio at some point, and provide
	 feedback through the control ports or the UI.
	*/

	DSPLUG_PLUGIN_TYPE_SIMPLE_AUDIO_PROCESSOR	= 3 ,
	/*
	 Plugins that comply with this will provide
	 ONE input audio port and ONE output audio port,
	 both the SAME number of channels each.
	 The plugin will take an input signal, apply a
	 process, then return the modified signal.
	*/
	
	DSPLUG_PLUGIN_TYPE_AUDIO_MODULATOR		= 4 ,
	/*
	 Modulator Plugins will need to provide any
	 number of Input AUDIO ports and only ONE
	 Output AUDIO Port. All the SAME number of
	 channels.  Most common examples of this
	 are a vocoder, a ring modulator, FM
	 modulator, sidechained compressor, etc.
	 The FIRST input audio port will contain the
	 signal to be modulated, if that applies. (carrier in FM, for example)
	*/


	DSPLUG_PLUGIN_TYPE_AUDIO_SPLITTER		= 5 ,
	/*
	 One AUDIO input, ANY amount of outputs.
	 All the SAME amount of channels.
	 Mostly used for signal decomposers.
	 Examples of this are multiband/filterbank splitter,
	*/
	
	DSPLUG_PLUGIN_TYPE_AUDIO_GENERATOR		= 6 ,
	/*
	 To comply with this, the plugin will only have output ports.
	 This is common for white noise sources, oscillators, or
	 plugins that take sound from somewhere else in the system
	 (for example, speech/sing synthesis from a text file).
	*/

	DSPLUG_PLUGIN_TYPE_MUSIC_EVENT_EXTRACTOR	= 7 ,
	/*
	 These kind of plugins will have only ONE audio input
	 and will produce events using ONE or MORE music event ports.
	 This is mostly used in pitch or beat recognition.
	*/
										
	DSPLUG_PLUGIN_TYPE_MUSIC_EVENT_FILTER		= 8 ,
	/* 
	 This kind of plugins will have ONE music event
	 input and ONE music event output, both in OMNI mode,
	 without audio ports. Most common uses would be event filters,
	 arpeggiator, etc
	*/
	
	DSPLUG_PLUGIN_TYPE_MUSIC_EVENT_MATRIX		= 9,
	/*
	 Plugins of this kind can have multiple event inputs
	 and outputs, and they are meant for routing matrices,
	 automatic arrangement generation, etc. Parts are
	 supported for this type.
	*/
	
	DSPLUG_PLUGIN_TYPE_MUSIC_EVENT_ANALYZER		= 8,
	/*
	 Just like an audio analyzer, but with events.
	 This is useful for analysis tools.
	*/

	DSPLUG_PLUGIN_TYPE_MUSIC_EVENT_GENERATOR	= 9,
	/*
	 Plugins that comply with this will have one or
	 more music event ouputs only, which can be used
	 to generate beats
	*/

	DSPLUG_PLUGIN_TYPE_MUSIC_EVENT_MODULATOR	= 10,
	/*
	 This plugin is like an audio processor plugin,
	 except it also provides an input music event
	 port in OMNI mode. The most common use for
	 this device is to modulate the audio input
	 using musical events, as a type of auto-tuner,
	 or applying parameters (like pan/volume/vibrato)
	 to the audio.
	*/

	DSPLUG_PLUGIN_TYPE_SYNTHESIZER			= 11,
	/*
	 Finally This plugin takes ONE music event port (in OMNI mode) and
	 outputs to ONE audio port (two if stereo, etc). The use for
	 this is software musical synthesis. Again, music event port provided
	 will work in OMNI mode, meaning it will discard the midi channel info.
	 This plugin could also be used for creating virtual singers, since
	 DSPLUG supports string-based control ports, you can input strings together
	 with the midi events.
	*/
										
	DSPLUG_PLUGIN_TYPE_MULTIPART_SYNTHESIZER 	= 12,
	/*
	 The same as above, but with any number of input
	 music event ports and any number of audio outputs.
	 This exists because many of these plugins will need to hold
	 shared data, such as samples, oscillators, etc that the preset use,
	 which can be very memory intensive, or simply want to
	 emulate a full music device. Since such type of plugin has parts
	 (ports per each music channel) the part for a midi channel for a
	 given port is: number of midi port * 16 + midi channel.
	*/
 
} DSPLUG_Plugin_Type;


/* //////////////////////////////////////////////////////// */


/* Event Port Types */


typedef enum {

	DSPLUG_EVENT_PORT_TYPE_MASTERTRACK	= 1,
	/*
	 This port receives sequencer mastertrack events such as
	 timebase/tempo/key/etc.
	 Even if the plugin supports this port, the host is not
	 forced to provide it (though it may be a good idea to
	 do so, if possible). The plugin may not work as well
	 without this. (or may not work at all).
	 Anyway, you can provide this no matter what the plugin type is.
	*/

	DSPLUG_EVENT_PORT_TYPE_MIDI		= 2,
	/*
	 MIDI  Ports are regular EVENT ports that can send
	 or receive musical data. So far only MIDI ports are
	 implemented.
	*/
	
	
} DSPLUG_Event_Port_Type;


/* //////////////////////////////////////////////////////// */


/* Control Port Types */


typedef enum {

/* Simple Port Types */
	DSPLUG_CONTROL_PORT_TYPE_FLOAT		=1,
	/* Normal port, sets/reads values from 0 to 1 */
	DSPLUG_CONTROL_PORT_TYPE_INTEGER	=2,
	/* Same as float port, except it has stepping */
	 DSPLUG_CONTROL_PORT_TYPE_ENUM		=3,
	/* Same as integer, but you can retrieve names for each value */
	DSPLUG_CONTROL_PORT_TYPE_BOOL		=4,
	/* Only accepts >=0.5 as true, otherwise false */
	DSPLUG_CONTROL_PORT_TYPE_TRIGGER	=5,
	/*
	 Write any value, and it will trigger a process
	 Whathever is read can be ignored
	 This port exists mainly to ease graphical representation
	*/

/* Complex Port Types */

	DSPLUG_CONTROL_PORT_TYPE_CSTRING	=100,
	/*
	 Read/Write C-Strings
	 Reading from automation port is imited to a fixed size for realtime issues
	*/
	DSPLUG_PORT_TYPE_DATA		=200,
	/*
	 Read/Write anything, but cant be user for automation
	*/

} DSPLUG_Control_Port_Type;


/* //////////////////////////////////////////////////////// */

/* Plugin Features */

typedef enum {

	DSPLUG_PLUGIN_FEATURE_INPLACE 			= 1 ,
	/*
	 Supports inplace replacement when plugin is
	 of type DSPLUG_PLUGIN_TYPE_SIMPLE_AUDIO_PROCESSOR.
	*/
	
	DSPLUG_PLUGIN_FEATURE_HARD_REALTIME		= 2 ,
	/*
	 The plugin will not do any system/library call
	 that can result in the blocking of the process
	 from where it is being run.
	*/
	
	DSPLUG_PLUGIN_FEATURE_LINEARITY_PROCESSING 	= 3 ,
	/*
	 if the plugin algorithm is of type:
	 DSPLUG_PLUGIN_TYPE_SIMPLE_AUDIO_PROCESSOR
	 and complies with the linearity requirement
	 that c*out=c*in then it can define this
	 feature.
	*/

	DSPLUG_PLUGIN_FEATURE_HAS_GUI			= 4 ,
	/*
	 Plugin has built-in graphical user interface editor
	*/

	DSPLUG_PLUGIN_FEATURE_VARIABLE_AUDIO_CHANNELS	= 5 ,
	/*
	 Plugin can be instanced with any given number of
	 channels per port. Only valid for some plugin types.
	*/
	
	DSPLUG_PLUGIN_FEATURE_LOCKING			= 6 ,
	/*
	 Plugin features locking. It is not of much use
	 to the host except for, maybe optimization purposes.
	 Locking is handled internally by the plugin.
	 For more info see plugin-api.
	*/
	
	DSPLUG_PLUGIN_FEATURE_LIVE_CONTROLS		= 7 ,
	/*
	 Plugin Supports Live Controls mode, see more info on this
	 on host.h or plugin.h
	*/

} DSPLUG_Plugin_Features;


/* //////////////////////////////////////////////////////// */


/* Plugin Constants */



typedef enum {
	DSPLUG_PLUGIN_CONSTANT_MAX_SAMPLING_RATE	= 1,
	/* Maximum sample rate that the plugin can work at, default is 0 (any) */
	DSPLUG_PLUGIN_CONSTANT_MIN_SAMPLING_RATE	= 2,
	/* Minimum sample rate that the plugin can work at default is 1 */
	
	DSPLUG_PLUGIN_CONSTANT_DEFAULT_CHANNELS		= 3,
	/*
	 Default amount of channels the plugin works with, when 
	 features DSPLUG_PLUGIN_FEATURE_VARIABLE_AUDIO_CHANNELS
	*/
} DSPLUG_Plugin_Constant;
	

/* //////////////////////////////////////////////////////// */
/* //////////////////////////////////////////////////////// */
/* //////////////////////////////////////////////////////// */
/* //////////////////////////////////////////////////////// */
/* //////////////////////////////////////////////////////// */

/* Sorry, but you can't access the internals yourself, they are compiled-in */


typedef struct {
	void * _private;
} DSPLUG_HOST_Plugin_Handle;

typedef struct {
	void * _private;
} DSPLUG_HOST_Plugin_Caps;

typedef struct {
	void * _private;
	void * _plugin_user_private;
} DSPLUG_Plugin;

typedef struct {
	void * _private;
} DSPLUG_Event_Queue;


#endif /* dsplug_types.h */
