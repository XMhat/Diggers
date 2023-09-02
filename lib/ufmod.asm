; MASMSTUB.ASM
; ------------
; uFMOD public source code release. Provided as-is.

; *** This stub allows compiling uFMOD sources to COFF object format
; *** using MASM32 (ml.exe) or to OMF object format using TASM (tasm32.exe)

.386
.model flat

; Autodetect target
COFF = 1
ifdef ??version
  COFF = 0
endif
ifdef OMF
  COFF = 0
endif

; Let's get rid of defines, so we don't need to use ifdef/ifndef/...

; TASM is supposed to handle echoes, but it may print
; 'em multiple times (due to multiple passes).
; So, we'd better echo warnings only in MASM.

DSOUND = 0
WMM = 0
ifdef DIRECTSOUND
  DSOUND = 1
else
  ifndef WINMM
    if COFF
      echo UFMOD_OUTPUT not specified (defaulting to WINMM)
    endif
  endif
  WMM = 1
endif

PBASIC = 0
ifdef PUREBASIC
  PBASIC = 1
  EXTRN _PB_StringBase:DWORD
endif

VB6 = 0
ifdef VISUALBASIC
  VB6 = 1
endif

BLITZMAX = 0
FTOL = 0

UCODE = 0
ifdef UNICODE
  UCODE = 1
else
  ifndef ANSI
    if COFF
      echo UFMOD_UFS not specified (defaulting to ANSI)
    endif
  endif
endif

; *** CONSTANTS ***
; volumerampsteps  = [mix rate] * 128 / 44100
; fvolumerampsteps = 1.0f / volumerampsteps / 255.0f
ifdef f48000
  ; Mix rate = 48000 Hz
  FSOUND_MixRate   equ 48000
  volumerampsteps  equ 139
  fvolumerampsteps equ 2.8212724e-5
else
  ifdef f22050
    ; Mix rate = 22050 Hz
    FSOUND_MixRate   equ 22050
    volumerampsteps  equ 64
    fvolumerampsteps equ 6.127451e-5
  else
    ifndef f44100
      if COFF
        echo UFMOD_MIXRATE not specified (defaulting to 44.1KHz)
      endif
    endif
    ; Mix rate = 44100 Hz
    FSOUND_MixRate   equ 44100
    volumerampsteps  equ 128
    fvolumerampsteps equ 3.0637257e-5
  endif
endif
FSOUND_BlockSize equ 1024

include ufmod.inc

; *** STRUCTS ***

; Required for WINMM target
FSOUND_SoundBlock STRUC
  lpData          dd ?
  dwBufferLength  dd ?
  dwBytesRecorded dd ?
  dwUser          dd ?
  dwFlags         dd ?
  dwLoops         dd ?
  lpNext          dd ?
  Reserved        dd ?
FSOUND_SoundBlock ENDS

; Sample type - contains info on sample
FSOUND_SAMPLE STRUC

  ; Don't change order .:.
  _length   dd ? ; sample length
  loopstart dd ? ; loop start
  looplen   dd ? ; loop length
  defvol    db ? ; default volume
  finetune  db ? ; finetune value from -128 to 127
  bytes     db ? ; type [b 0-1] : 0 - no loop
                 ;                1 - forward loop
                 ;                2 - bidirectional loop (aka ping-pong)
                 ;      [b 4]   : 0 - 8-bit sample data
                 ;                1 - 16-bit sample data
  defpan    db ? ; default pan value from 0 to 255
  relative  db ? ; relative note (signed value)
  Reserved  db ? ; reserved, known values: 00h - regular delta packed sample data
                 ;                         ADh - ModPlug 4-bit ADPCM packed sample data
  ; .:.

  loopmode  db ?
  _align    db ?
  buff      db ?,? ; sound data
FSOUND_SAMPLE ENDS

; Channel type - contains information on a mixing channel
FSOUND_CHANNEL STRUC
  index            dd ? ; position in channel pool
  actualvolume     dd ? ; driver level current volume
  actualpan        dd ? ; driver level panning value
  fsampleoffset    dd ? ; sample offset (sample starts playing from here)
  leftvolume       dd ? ; mixing information. adjusted volume for left channel (panning involved)
  rightvolume      dd ? ; mixing information. adjusted volume for right channel (panning involved)
  mixpos           dd ? ; mixing information. high part of 32:32 fractional position in sample
  speedlo          dd ? ; mixing information. playback rate - low part fractional
  speedhi          dd ? ; mixing information. playback rate - high part fractional
  ramp_lefttarget  dd ?
  ramp_righttarget dd ?
  ramp_leftspeed   dd ?
  ramp_rightspeed  dd ?

  ; Don't change order .:.
  fsptr            dd ? ; pointer to FSOUND_SAMPLE currently playing sample
  mixposlo         dd ? ; mixing information. low part of 32:32 fractional position in sample
  speeddir         dd ? ; mixing information. playback direction - forwards or backwards
  ramp_leftvolume  dd ?
  ramp_rightvolume dd ?
  ramp_count       dd ?
  ; .:.

FSOUND_CHANNEL ENDS

; Single note type - contains info on 1 note in a pattern
FMUSIC_NOTE STRUC
  unote   db ? ; note to play at (0-97) 97=keyoff
  number  db ? ; sample being played (0-128)
  uvolume db ? ; volume column value (0-64)  255=no volume
  effect  db ? ; effect number (0-1Ah)
  eparam  db ? ; effect parameter (0-255)
FMUSIC_NOTE ENDS

FMUSIC_TIMMEINFO STRUC
  L_vol dw ? ; L channel RMS volume
  R_vol dw ? ; R channel RMS volume
  ms    dd ? ; sync. time in ms
FMUSIC_TIMMEINFO ENDS

; Pattern data type
FMUSIC_PATTERN STRUC
  rows        dw ?
  patternsize dw ?
  data        dd ? ; pointer to FMUSIC_NOTE
FMUSIC_PATTERN ENDS

; Multi sample extended instrument
FMUSIC_INSTRUMENT STRUC
  numsamples   dd ? ; number of samples in this instrument
  sample       dd 16 dup (?) ; 16 pointers to FSOUND_SAMPLE per instrument

  ; Don't change order .:.
  keymap       db 96 dup (?) ; sample keymap assignments
  VOLPoints    dw 24 dup (?) ; volume envelope points
  PANPoints    dw 24 dup (?) ; panning envelope points
  VOLnumpoints db ? ; number of volume envelope points
  PANnumpoints db ? ; number of panning envelope points
  VOLsustain   db ? ; volume sustain point
  VOLLoopStart db ? ; volume envelope loop start
  VOLLoopEnd   db ? ; volume envelope loop end
  PANsustain   db ? ; panning sustain point
  PANLoopStart db ? ; panning envelope loop start
  PANLoopEnd   db ? ; panning envelope loop end
  VOLtype      db ? ; type of envelope,bit 0:On 1:Sustain 2:Loop
  PANtype      db ? ; type of envelope,bit 0:On 1:Sustain 2:Loop
  VIBtype      db ? ; instrument vibrato type
  VIBsweep     db ? ; time it takes for vibrato to fully kick in
  iVIBdepth    db ? ; depth of vibrato
  VIBrate      db ? ; rate of vibrato
  VOLfade      dw ? ; fade out value
  ; .:.

FMUSIC_INSTRUMENT ENDS

; Channel type - contains information on a mod channel
FMUSIC_CHANNEL STRUC
  note          db ? ; last note set in channel
  samp          db ? ; last sample set in channel
  notectrl      db ? ; flags for DoFlags proc
  inst          db ? ; last instrument set in channel
  cptr          dd ? ; pointer to FSOUND_CHANNEL system mixing channel
  freq          dd ? ; current mod frequency period for this channel
  volume        dd ? ; current mod volume for this channel
  voldelta      dd ? ; delta for volume commands... tremolo/tremor, etc
  freqdelta     dd ? ; delta for frequency commands... vibrato/arpeggio, etc
  pan           dd ? ; current mod pan for this channel

  ; Don't change order .:.
  envvoltick    dd ? ; tick counter for envelope position
  envvolpos     dd ? ; envelope position
  envvoldelta   dd ? ; delta step between points
  envpanpos     dd ? ; envelope position
  envpantick    dd ? ; tick counter for envelope position
  envpandelta   dd ? ; delta step between points
  ivibsweeppos  dd ? ; instrument vibrato sweep position
  ivibpos       dd ? ; instrument vibrato position
  keyoff        db ? ; flag whether keyoff has been hit or not
  envvolstopped db ? ; flag to say whether envelope has finished or not
  envpanstopped db ? ; flag to say whether envelope has finished or not
  tremorpos     db ? ; tremor position
  ; .:.

  envvolfrac    dd ? ; fractional interpolated envelope volume
  envvol        dd ? ; final interpolated envelope volume
  envpanfrac    dd ? ; fractional interpolated envelope pan
  envpan        dd ? ; final interpolated envelope pan
  fadeoutvol    dd ? ; volume fade out
  period        dd ? ; last period set in channel
  sampleoffset  dd ? ; sample offset for this channel in SAMPLES
  portatarget   dd ? ; note to porta to
  patlooprow    dd ?
  patloopno     dd ? ; pattern loop variables for effect E6x
  realnote      db ? ; last realnote set in channel
  recenteffect  db ? ; previous row's effect... used to correct tremolo volume
  portadown     db ? ; last porta down value
  portaup       db ? ; last porta up value
  xtraportadown db ? ; last porta down value
  xtraportaup   db ? ; last porta up value
  volslide      db ? ; last volume slide value
  panslide      db ? ; pan slide parameter
  retrigx       db ? ; last retrig volume slide used
  retrigy       db ? ; last retrig tick count used
  portaspeed    db ? ; porta speed
  vibpos        db ? ; vibrato position
  vibspeed      db ? ; vibrato speed
  vibdepth      db ? ; vibrato depth
  tremolopos    db ? ; tremolo position
  tremolospeed  db ? ; tremolo speed
  tremolodepth  db ? ; tremolo depth
  tremoron      db ? ; remembered parameters for tremor
  tremoroff     db ? ; remembered parameters for tremor
  wavecontrol   db ? ; waveform type for vibrato and tremolo (4bits each)
  finevslup     db ? ; parameter for fine volume slide down
  fineportaup   db ? ; parameter for fine porta slide up
  fineportadown db ? ; parameter for fine porta slide down
  _align4       db ?
FMUSIC_CHANNEL ENDS

; Song type - contains info on song
FMUSIC_MODULE STRUC

  ; Don't change order .:.
  pattern              dd ? ; pointer to FMUSIC_PATTERN array for this song
  instrument           dd ? ; pointer to FMUSIC_INSTRUMENT array for this song
  mixer_samplesleft    dd ?
  globalvolume         dd ? ; global mod volume
  tick                 dd ? ; current mod tick
  speed                dd ? ; speed of song in ticks per row
  order                dd ? ; current song order position
  row                  dd ? ; current row in pattern
  patterndelay         dd ? ; pattern delay counter
  nextorder            dd ? ; current song order position
  nextrow              dd ? ; current row in pattern
  time_ms              dd ? ; time passed in milliseconds since song started
  numchannels          dd ? ; number of channels
  globalvsl            db ? ; global mod volume
  _align               db 7 dup (?)
  mixer_samplespertick dd ?
  numorders            dw ? ; number of orders (song length)
  restart              dw ? ; restart position
  numchannels_xm       db ?,?
  numpatternsmem       dw ? ; number of allocated patterns
  numinsts             dw ? ; number of instruments
  flags                dw ? ; flags such as linear frequency, format specific quirks, etc
  defaultspeed         dw ?
  defaultbpm           dw ?
  orderlist            db 256 dup (?) ; pattern playing order list
  ; .:.

FMUSIC_MODULE ENDS

if COFF
; MS COFF object format

uFMOD_PlaySong EQU <_uFMOD_PlaySong@12>
uFMOD_DSPlaySong EQU <_uFMOD_DSPlaySong@16>
uFMOD_Pause EQU <_uFMOD_Pause@0>
uFMOD_Resume EQU <_uFMOD_Resume@0>
uFMOD_GetTime EQU <_uFMOD_GetTime@0>
uFMOD_GetStats EQU <_uFMOD_GetStats@0>
uFMOD_GetTitle EQU <_uFMOD_GetTitle@0>
uFMOD_SetVolume EQU <_uFMOD_SetVolume@4>

EXTERN __imp__WaitForSingleObject@8:DWORD
WaitForSingleObject equ __imp__WaitForSingleObject@8
EXTERN __imp__CloseHandle@4:DWORD
CloseHandle equ __imp__CloseHandle@4
EXTERN __imp__CreateThread@24:DWORD
CreateThread equ __imp__CreateThread@24
EXTERN __imp__SetThreadPriority@8:DWORD
SetThreadPriority equ __imp__SetThreadPriority@8
EXTERN __imp__HeapAlloc@12:DWORD
HeapAlloc equ __imp__HeapAlloc@12
EXTERN __imp__HeapCreate@12:DWORD
HeapCreate equ __imp__HeapCreate@12
EXTERN __imp__HeapDestroy@4:DWORD
HeapDestroy equ __imp__HeapDestroy@4
EXTERN __imp__Sleep@4:DWORD
Sleep equ __imp__Sleep@4

ifndef DIRECTSOUND
  EXTERN __imp__waveOutClose@4:DWORD
  waveOutClose equ __imp__waveOutClose@4
  EXTERN __imp__waveOutGetPosition@12:DWORD
  waveOutGetPosition equ __imp__waveOutGetPosition@12
  EXTERN __imp__waveOutOpen@24:DWORD
  waveOutOpen equ __imp__waveOutOpen@24
  EXTERN __imp__waveOutPrepareHeader@12:DWORD
  waveOutPrepareHeader equ __imp__waveOutPrepareHeader@12
  EXTERN __imp__waveOutReset@4:DWORD
  waveOutReset equ __imp__waveOutReset@4
  EXTERN __imp__waveOutUnprepareHeader@12:DWORD
  waveOutUnprepareHeader equ __imp__waveOutUnprepareHeader@12
  EXTERN __imp__waveOutWrite@12:DWORD
  waveOutWrite equ __imp__waveOutWrite@12

  if PAUSE_RESUME_ON
    EXTERN __imp__waveOutPause@4:DWORD
    waveOutPause equ __imp__waveOutPause@4
    EXTERN __imp__waveOutRestart@4:DWORD
    waveOutRestart equ __imp__waveOutRestart@4
  endif
endif

if XM_FILE_ON
  if UCODE
    EXTERN __imp__CreateFileW@28:DWORD
    CreateFile equ __imp__CreateFileW@28
  else
    EXTERN __imp__CreateFileA@28:DWORD
    CreateFile equ __imp__CreateFileA@28
  endif
  EXTERN __imp__ReadFile@20:DWORD
  ReadFile equ __imp__ReadFile@20
  EXTERN __imp__SetFilePointer@16:DWORD
  SetFilePointer equ __imp__SetFilePointer@16
endif

if XM_RC_ON
  EXTERN __imp__FindResourceA@12:DWORD
  FindResource equ __imp__FindResourceA@12
  EXTERN __imp__LoadResource@8:DWORD
  LoadResource equ __imp__LoadResource@8
  EXTERN __imp__SizeofResource@8:DWORD
  SizeofResource equ __imp__SizeofResource@8
endif

else
; Borland OMF format for Delphi 4 or later
; Take into account the following issues:
;    Delphi doesn't like name decoration!
;    Delphi doesn't like direct dllimport references!
;       (it preferes old style call [<jmp addr>])

EXTRN WaitForSingleObject:NEAR
EXTRN CloseHandle:NEAR
EXTRN CreateThread:NEAR
EXTRN SetThreadPriority:NEAR
EXTRN HeapAlloc:NEAR
EXTRN HeapCreate:NEAR
EXTRN HeapDestroy:NEAR
EXTRN Sleep:NEAR

ifndef DIRECTSOUND
  EXTRN waveOutClose:NEAR
  EXTRN waveOutGetPosition:NEAR
  EXTRN waveOutOpen:NEAR
  EXTRN waveOutPrepareHeader:NEAR
  EXTRN waveOutReset:NEAR
  EXTRN waveOutUnprepareHeader:NEAR
  EXTRN waveOutWrite:NEAR

  if PAUSE_RESUME_ON
    EXTRN waveOutPause:NEAR
    EXTRN waveOutRestart:NEAR
  endif
endif

if XM_FILE_ON
  if UCODE
    EXTRN CreateFileW:NEAR
    CreateFile EQU CreateFileW
  else
    EXTRN CreateFileA:NEAR
    CreateFile EQU CreateFileA
  endif
  EXTRN ReadFile:NEAR
  EXTRN SetFilePointer:NEAR
endif

if XM_RC_ON
  EXTRN FindResourceA:NEAR
  FindResource EQU FindResourceA
  EXTRN LoadResource:NEAR
  EXTRN SizeofResource:NEAR
endif

endif

FMUSIC_MODULE_size     = SIZE FMUSIC_MODULE
FSOUND_CHANNEL_size    = SIZE FSOUND_CHANNEL
FMUSIC_CHANNEL_size    = SIZE FMUSIC_CHANNEL
FMUSIC_TIMMEINFO_size  = SIZE FMUSIC_TIMMEINFO
FMUSIC_INSTRUMENT_size = SIZE FMUSIC_INSTRUMENT
FMUSIC_PATTERN_size    = SIZE FMUSIC_PATTERN
FMUSIC_NOTE_size       = SIZE FMUSIC_NOTE

; FPU register stack
st0 TEXTEQU <st(0)>
st1 TEXTEQU <st(1)>
st2 TEXTEQU <st(2)>
st3 TEXTEQU <st(3)>
st4 TEXTEQU <st(4)>

.CODE
include ufmodm.asm
rampsteps dd fvolumerampsteps
include ufmodc.asm

.DATA?

; Don't change order!
Channels      FSOUND_CHANNEL 64 dup(<>) ; channel pool
uFMOD_Ch      = $
              FMUSIC_CHANNEL 32 dup(<>) ; channel array for this song
_mod          = $
              FMUSIC_MODULE<> ; currently playing track
mmt           dd ?,?,?
hHeap         dd ?
hThread       dd ?
hWaveOut      dd ? ; HWAVEOUT
uFMOD_FillBlk dd ?
SW_Exit       dd ?
; mix buffer memory block (align 16!)
MixBuf        db (FSOUND_BufferSize*8) dup (?)
ufmod_noloop  db ?,? ; enable/disable restart loop
; Made global to free ebp
mix_endflag   db ?,?
mix_count     dd ?
mix_count_old dd ?
mix_leftvol   dd ?
mix_rightvol  dd ?
mmf           dd ?,?,?
; * LPCALLBACKS *
uFMOD_fopen   dd ?
uFMOD_fread   dd ?
uFMOD_fseek   dd ?
uFMOD_fclose  dd ?
ifndef DIRECTSOUND
   databuf    db (FSOUND_BufferSize*4) dup (?) ; hold up to 100 samples (4 seconds of playback)
endif
MixBlock      FSOUND_SoundBlock<>
if INFO_API_ON
   RealBlock  dd ?
   tInfo      FMUSIC_TIMMEINFO totalblocks dup(<>) ; L_vol / R_vol / time
   szTtl      db 24 dup (?) ; track title
endif
if VOL_CONTROL_ON
   ufmod_vol  dd ? ; global volume scale
endif
DummySamp     FSOUND_SAMPLE<>

end