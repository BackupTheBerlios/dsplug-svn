/**
 * \file dsplug_types.h
 * \author Juan Linietsky
 */

#ifndef DSPLUG_TYPES_H
#define DSPLUG_TYPES_H


/* Defines */

/**< dsplug string max buffer size, including zero */
#define DSPLUG_STRING_PARAM_MAX_LEN 512
#define DSPLUG_NO_CONSTANT -1
#define DSPLUG_MAX_CHANNELS_PER_AUDIO_PORT 64

#define DSPLUG_MAX_AUDIO_PORTS 512
#define DSPLUG_MAX_EVENT_PORTS 512
#define DSPLUG_MAX_CONTROL_PORTS 16384

/* This constant avoids making more steps than a floating number can handle */
#define DSPLUG_NUMERICAL_PORT_MAX_STEPS ((1<<24)-1)


/* //////////////////////////////////////////////////////// */

/* Boolean Values */

typedef int DSPlug_Boolean;

/* Boolean Constants */

#define DSPLUG_TRUE 1
#define DSPLUG_FALSE 0


/* //////////////////////////////////////////////////////// */


/* Plugin Types */


typedef enum {

	/**
	* Any number and any kind of data input or output ports
	* will be provided. These kind of plugins will most likely be
	* used from custom applications or modular hosts.
	*/
	DSPLUG_USAGE_HINT_GENERIC			= 1 ,
	
	/**
	* To comply with this, the plugin will have ONE input
	* AUDIO port (of any given number of channels).
	* The plugin will be meant for applications that need
	* to analyze the audio at some point, and provide
	* feedback through the control ports or the UI.
	*/
	
	DSPLUG_USAGE_HINT_AUDIO_ANALYZER		= 2 ,

	/**
	* Plugins that comply with this will provide
	* ONE bidirectional audio port.
	* The plugin will take an input signal, apply a
	* process, then return the modified signal.
	*/
	DSPLUG_USAGE_HINT_SIMPLE_AUDIO_PROCESSOR	= 3 ,
	
	/**
	* Modulator Plugins will provide a bidirectional
	* AUDIO port, plus any number of extra AUDIO inputs.
	* All the SAME number of channels.
	* Most common examples of this
	* are a vocoder, a ring modulator, FM
	* modulator, sidechained compressor, etc.
	* The FIRST input audio port will contain the
	* signal to be modulated, if that applies. (carrier in FM, for example)
	*/
	DSPLUG_USAGE_HINT_AUDIO_MODULATOR		= 4 ,

	/**
	* One AUDIO input, ANY amount of outputs.
	* All the SAME amount of channels.
	* Mostly used for signal decomposers.
	* Examples of this are multiband/filterbank splitter,
	*/

	DSPLUG_USAGE_HINT_AUDIO_SPLITTER		= 5 ,
	
	/**
	* To comply with this, the plugin will only have output ports.
	* This is common for white noise sources, oscillators, or
	* plugins that take sound from somewhere else in the system
	* (for example, speech/sing synthesis from a text file, or
	* an audio capture api).
	*/
	DSPLUG_USAGE_HINT_AUDIO_GENERATOR		= 6 ,

	/**
	* To comply with this, the plugin will only have output ports.
	* This is common for white noise sources, oscillators, or
	* plugins that take sound from somewhere else in the system
	* (for example, speech/sing synthesis from a text file, or
	* an audio capture api).
	*/
	DSPLUG_USAGE_HINT_AUDIO_CHANNEL_REMAPPER	= 7 ,
	
	/**
	* This is for plugins that takes ONE input port of N channels
	* and ONE output port of M channels.
	* (for example stero->mono, mono with panpot to stereo, 5.1 encoder, etc)
      	*/
	DSPLUG_USAGE_HINT_MUSIC_EVENT_EXTRACTOR		= 8 ,
										
	/** 
	* This kind of plugins will have ONE bidirectional music event
	* port. in OMNI mode,
	* without audio ports. Most common uses would be event filters,
	* arpeggiator, etc
	*/
	DSPLUG_USAGE_HINT_MUSIC_EVENT_FILTER		= 9 ,
	
	/**
	* Plugins of this kind can have multiple event inputs
	* and outputs, and they are meant for routing matrices,
	* automatic arrangement generation, etc. Parts are
	* supported for this type.
	*/
	DSPLUG_USAGE_HINT_MUSIC_EVENT_MATRIX		= 10,
	
	/**
	* Just like an audio analyzer, but with events.
	* This is useful for analysis tools, but could be
	* used also to send the event to external ports.
	*/
	DSPLUG_USAGE_HINT_MUSIC_EVENT_ANALYZER		= 11,

	/**
	* Plugins that comply with this will have one or
	* more music event ouputs only, which can be used
	* to generate beats
	*/
	DSPLUG_USAGE_HINT_MUSIC_EVENT_GENERATOR		= 12,

	/**
	* This plugin is like an audio processor plugin,
	* except it also provides an input music event
	* port in OMNI mode. The most common use for
	* this device is to modulate the audio input
	* using musical events, as a type of auto-tuner,
	* or applying parameters (like pan/volume/vibrato)
	* to the audio.
	*/
	DSPLUG_USAGE_HINT_MUSIC_EVENT_MODULATOR		= 13,

	/**
	* Finally This plugin takes ONE music event port (in OMNI mode) and
	* outputs to ONE audio port . The use for
	* this is software musical synthesis. Again, music event port provided
	* will work in OMNI mode, meaning it will discard the midi channel info.
	* This plugin could also be used for creating virtual singers, since
	* DSPLUG supports string-based control ports, you can input strings together
	* with the midi events.
	*/
	DSPLUG_USAGE_HINT_SYNTHESIZER			= 14,
										
	/**
	* The same as above, but with any number of input
	* music event ports and any number of audio outputs.
	* This exists because many of these plugins will need to hold
	* shared data, such as samples, oscillators, etc that the preset use,
	* which can be very memory intensive, or simply want to
	* emulate a full music device. Since such type of plugin has parts
	* (ports per each music channel) the part for a midi channel for a
	* given port is: number of midi port * 16 + midi channel.
	*/
	DSPLUG_USAGE_HINT_MULTIPART_SYNTHESIZER 	= 15,
 
} DSPlug_PluginUsageHint;

/* //////////////////////////////////////////////////////// */


/* Plug Type */

typedef enum {

	DSPLUG_PLUG_INPUT 	= 0, /**< Port Provides Input */
	DSPLUG_PLUG_OUTPUT 	= 1, /**< Port Provides Output */

} DSPlug_PlugType;

/* //////////////////////////////////////////////////////// */

/* Port Tyoe */

typedef enum {

	DSPLUG_PORT_AUDIO 	= 0, /**< Audio Port */
	DSPLUG_PORT_EVENT 	= 1, /**< Control Port */
	DSPLUG_PORT_CONTROL 	= 2, /**< Event Port*/

} DSPlug_PortType;


/* //////////////////////////////////////////////////////// */

/* Event Port Types */


typedef enum {

	/**
	* This port receives sequencer mastertrack events such as
	* timebase/tempo/key/etc.
	* Even if the plugin supports this port, the host is not
	* forced to provide it (though it may be a good idea to
	* do so, if possible). The plugin may not work as well
	* without this. (or may not work at all).
	* Anyway, you can provide this no matter what the plugin type is.
	*/
	DSPLUG_EVENT_TYPE_MASTERTRACK	= 1,

	/**
	* MIDI  Ports are regular EVENT ports that can send
	* or receive musical data. So far only MIDI ports are
	* implemented.
	*/
	DSPLUG_EVENT_TYPE_MIDI		= 2,
	
	/**
	 * AUDIO Ports are for plugins to receive information such
	 * as output/input latency, transport in frames, and other audio information.
	 */
	DSPLUG_EVENT_TYPE_AUDIO		= 3,
	
	
} DSPlug_EventType;





/* //////////////////////////////////////////////////////// */


/* Control Port Types */
	
typedef enum {


	DSPLUG_CONTROL_PORT_TYPE_NUMERICAL	= 0, /**< Port handles floating point values from 0.0f to 1.0f */
	DSPLUG_CONTROL_PORT_TYPE_STRING		= 1, /**< Port handles C-String values */
	DSPLUG_CONTROL_PORT_TYPE_DATA		= 2, /**< Port handles Binary Data */

} DSPlug_ControlPortType;

/* //////////////////////////////////////////////////////// */


/* Numerical Control Port Hints */

typedef enum {

	DSPLUG_CONTROL_PORT_HINT_TYPE_FLOAT		= 0, /**< Normal port, sets/reads values from 0.0f to 1.1f */
	DSPLUG_CONTROL_PORT_HINT_TYPE_INTEGER	= 1, /**< Same as float port, except it has stepping. **WARNING** When saving the port value, *ALWAYS* save either both the port value and the steps, or the integer value. This way if the plugin adds more steps (as in, more possible values) you can remain compatible with it. */
	DSPLUG_CONTROL_PORT_HINT_TYPE_BOOL		= 2, /**< Only accepts >=0.5f as true, otherwise false */

} DSPlug_ControlPortNumericalHint;

/* //////////////////////////////////////////////////////// */

/* Plugin Features */

typedef enum {

	
	/**
	* if the plugin algorithm is of type:
	* DSPLUG_USAGE_HINT_SIMPLE_AUDIO_PROCESSOR
	* and complies with the linearity requirement
	* that c*out=c*in then it can define this
	* feature.
	*/
	
	DSPLUG_PLUGIN_FEATURE_LINEARITY_PROCESSING 	= 0 ,

	/**
	* Plugin has built-in graphical user interface editor
	*/	
	DSPLUG_PLUGIN_FEATURE_HAS_GUI			= 1 ,
	
	/**
	* Plugin features locking. It is not of much use
	* to the host except for, maybe optimization purposes.
	* Locking is handled internally by the plugin.
	* For more info see plugin-api.
	*/	
	DSPLUG_PLUGIN_FEATURE_LOCKING			= 2 ,
	
	/**
	 * If the plugin needs to be run in realtime, because
	 * of any reason (most probably interacting with something
	 * external to the host) it needs to define this flag.
	 */
	DSPLUG_PLUGIN_FEATURE_ONLINE_PROCESSING_ONLY	= 3 ,

} DSPlug_PluginFeature;


/* //////////////////////////////////////////////////////// */


/* Plugin Constants */



typedef enum {
	DSPLUG_PLUGIN_CONSTANT_MAX_SAMPLING_RATE	= 1, /**< Maximum sample rate that the plugin can work at, default is 0 (any) */
	DSPLUG_PLUGIN_CONSTANT_MIN_SAMPLING_RATE	= 2, /**< Minimum sample rate that the plugin can work at default is 1 */
	
	DSPLUG_PLUGIN_CONSTANT_DEFAULT_CHANNELS		= 3, /**<  Default amount of channels the plugin works with, when  features DSPLUG_PLUGIN_FEATURE_VARIABLE_AUDIO_CHANNELS */

} DSPlug_PluginConstant;
	

/* //////////////////////////////////////////////////////// */
/* //////////////////////////////////////////////////////// */
/* //////////////////////////////////////////////////////// */
/* //////////////////////////////////////////////////////// */
/* //////////////////////////////////////////////////////// */

/* Sorry, but you can't access the internals yourself, they are compiled-in */
/* Still thought, you can use the following macro to test for the vailidty
   of the following objects, simlpy pass them normally,not as pointers:
*/

#define DSPLUG_IS_OBJECT_VALID(m_o) ( (m_o)._private!=NULL )

/**
 * Instance to the plugin library opened.
 */
typedef struct {
	const void * _private; /**< No access to the internals are provided */
} DSPlug_PluginLibrary;

/**
 * This object stores the capabilities of a given plugin.
 */
typedef struct {
	const void * _private; /**< No access to the internals are provided */
} DSPlug_PluginCaps;

/**
 * Instanced plugin. This object is using for the actual data processing.
 */
typedef struct {
	const void * _private; /**< No access to the internals are provided */
} DSPlug_PluginInstance;

/**
 * This object stores the capabilities of an audio port.
 */
typedef struct {
	const void * _private; /**< No access to the internals are provided */
} DSPlug_AudioPortCaps;

/**
 * This object stores the capabilities of an event port.
 */
typedef struct {
	const void * _private; /**< No access to the internals are provided */
} DSPlug_EventPortCaps;

/**
 * This object stores the capabilities of a control port.
 */
typedef struct {
	const void * _private; /**< No access to the internals are provided */
} DSPlug_ControlPortCaps;

/**
 * Event Queue. Holds a queue of events of a certain type.
 */
typedef struct {
	const void * _private; /**< No access to the internals are provided */
} DSPlug_EventQueue;

/**
 * This object stores the capabilities of a given plugin.
 */
typedef struct {
	const void * _private; /**< No access to the internals are provided */
	const void * _user_private;  /**< No access to the internals are provided */
} DSPlug_Plugin;

/**
 * This plugin is used upon creation of a plugin
 */
typedef struct {
	const void * _private; /**< No access to the internals are provided */
} DSPlug_PluginCreation;

/**
 * This plugin is used upon creation of a plugin library
 */
typedef struct {
	const void * _private; /**< No access to the internals are provided */
} DSPlug_LibraryCreation;

/**
 * This plugin is used upon creation of a plugin control port
 */
typedef struct {
	const void * _private; /**< No access to the internals are provided */
} DSPlug_ControlPortCreation;
		
#endif /* dsplug_types.h */
