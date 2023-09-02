#define XM_RESOURCE 0
#define XM_MEMORY   1
#define XM_FILE     2
#define XM_NOLOOP   8
#define uFMOD_BUFFER_SIZE 65536 // don't modify!

#ifdef __cplusplus
	extern "C" {
#endif

/* The uFMOD_DSPlaySong function starts playing an XM song.
   --------------
   Parameters:
     lpXM
        Specifies the song to play. If this parameter is NULL,
        any currently playing song is stopped. In such a case, function
        does not return a meaningful value.
        fdwSong parameter determines whether this value is interpreted
        as a filename, as a resource identifier or a pointer to an image
        of the song in memory.
     param
        Handle to the executable file that contains the resource to be
        loaded or size of the image of the song in memory. This parameter
        is ignored unless XM_RESOURCE or XM_MEMORY is specified in fdwSong.
     fdwSong
        Flags for playing the song. The following values are defined:
        Value        Meaning
        XM_FILE      lpXM points to filename.
                     param is ignored.
        XM_MEMORY    lpXM points to an image of a song in memory.
                     param is the image size. Once, uFMOD_DSPlaySong
                     returns, it's safe to free/discard the memory buffer.
        XM_RESOURCE  lpXM Specifies the name of the resource.
                     param identifies the module whose executable file
                     contains the resource. Could be NULL if the given
                     resource is located in the current module.
                     The resource type must be RT_RCDATA.
        XM_NOLOOP    An XM track plays repeatedly by default. Specify
                     this flag to play it only once.
     lpDSBuffer
        A pointer to an IDirectSoundBuffer. uFMOD assumes
        that it has been properly initialized to 16-bit stereo.
  Return Values:
     Returns DS_OK on success or an error value otherwise:
        DSERR_BUFFERLOST
        DSERR_INVALIDCALL
        DSERR_INVALIDPARAM
        DSERR_PRIOLEVELNEEDED
  NOTE: Once playback has started, it's not necessary to check for "buffer
  lost" condition, since uFMOD performs buffer restoring automatically.
*/
int __stdcall uFMOD_DSPlaySong(void*, void*, int, void*);

// The uFMOD_StopSong function stops the currently playing song, if any.
#define uFMOD_StopSong() uFMOD_DSPlaySong(0, 0, 0, 0)

// The uFMOD_DSPause function pauses the currently playing song.
void __stdcall uFMOD_Pause();

// The uFMOD_DSResume function resumes the currently paused song.
void __stdcall uFMOD_Resume();

/* The uFMOD_GetStats function returns the current RMS volume coefficients
   in L and R channels.
   --------------
   Return Values:
      low-order word : RMS volume in R channel
      hi-order  word : RMS volume in L channel
*/
unsigned int __stdcall uFMOD_GetStats();

/* The uFMOD_GetTime function returns the time in milliseconds since the
   song was started. This is useful for synchronizing purposes.
   --------------
   Return Values:
      Returns the time in milliseconds since the song was started.
*/
unsigned int __stdcall uFMOD_GetTime();

/* The uFMOD_GetTitle function returns the current track's title, if any.
   --------------
   Return Values:
      Returns the track's title in ASCIIZ format.
*/
const char* __stdcall uFMOD_GetTitle();

/* The uFMOD_SetVolume function sets the global volume.
   --------------
   0:  silence (muting)
   64: maximum volume
   NOTE: The volume scale is linear. Maximum volume is set by default.
*/
void __stdcall uFMOD_SetVolume(unsigned int);

#ifdef __cplusplus
	}
#endif
