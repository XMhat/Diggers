; UFMOD.ASM
; ---------
; uFMOD public source code release. Provided as-is.

totalblocks       equ 16
FSOUND_BufferSize equ (FSOUND_BlockSize)*totalblocks

XM_MEMORY                equ 1
XM_FILE                  equ 2
XM_NOLOOP                equ 8
FSOUND_MIXDIR_FORWARDS   equ 0
FMUSIC_ENVELOPE_SUSTAIN  equ 2
FMUSIC_ENVELOPE_LOOP     equ 4
FMUSIC_FREQ              equ 1
FMUSIC_VOLUME            equ 2
FMUSIC_PAN               equ 4
FMUSIC_TRIGGER           equ 8
FMUSIC_STOP              equ 16
FMUSIC_VOLUME_OR_FREQ    equ 3
FMUSIC_VOLUME_OR_PAN     equ 6
FMUSIC_VOL_OR_FREQ_OR_TR equ 11
FMUSIC_FREQ_OR_TRIGGER   equ 9
NOT_FMUSIC_TRIGGER       equ 0F7h
NOT_FMUSIC_FREQ          equ 0FEh

; FMUSIC_XMCOMMANDS enum:
FMUSIC_XM_ARPEGGIO        equ 0
FMUSIC_XM_PORTAUP         equ 1
FMUSIC_XM_PORTADOWN       equ 2
FMUSIC_XM_PORTATO         equ 3
FMUSIC_XM_VIBRATO         equ 4
FMUSIC_XM_PORTATOVOLSLIDE equ 5
FMUSIC_XM_VIBRATOVOLSLIDE equ 6
FMUSIC_XM_TREMOLO         equ 7
FMUSIC_XM_SETPANPOSITION  equ 8
FMUSIC_XM_SETSAMPLEOFFSET equ 9
FMUSIC_XM_VOLUMESLIDE     equ 10
FMUSIC_XM_PATTERNJUMP     equ 11
FMUSIC_XM_SETVOLUME       equ 12
FMUSIC_XM_PATTERNBREAK    equ 13
FMUSIC_XM_SPECIAL         equ 14
FMUSIC_XM_SETSPEED        equ 15
FMUSIC_XM_SETGLOBALVOLUME equ 16
FMUSIC_XM_GLOBALVOLSLIDE  equ 17
FMUSIC_XM_KEYOFF          equ 20
FMUSIC_XM_SETENVELOPEPOS  equ 21
FMUSIC_XM_PANSLIDE        equ 25
FMUSIC_XM_MULTIRETRIG     equ 27
FMUSIC_XM_TREMOR          equ 29
FMUSIC_XM_EXTRAFINEPORTA  equ 33

; FMUSIC_XMCOMMANDSSPECIAL enum:
FMUSIC_XM_SETFILTER           equ 0
FMUSIC_XM_FINEPORTAUP         equ 1
FMUSIC_XM_FINEPORTADOWN       equ 2
FMUSIC_XM_SETGLISSANDO        equ 3
FMUSIC_XM_SETVIBRATOWAVE      equ 4
FMUSIC_XM_SETFINETUNE         equ 5
FMUSIC_XM_PATTERNLOOP         equ 6
FMUSIC_XM_SETTREMOLOWAVE      equ 7
FMUSIC_XM_SETPANPOSITION16    equ 8
FMUSIC_XM_RETRIG              equ 9
FMUSIC_XM_FINEVOLUMESLIDEUP   equ 10
FMUSIC_XM_FINEVOLUMESLIDEDOWN equ 11
FMUSIC_XM_NOTECUT             equ 12
FMUSIC_XM_NOTEDELAY           equ 13
FMUSIC_XM_PATTERNDELAY        equ 14

if WMM
pcm       dd 20001h           ; linear PCM, stereo
          dd FSOUND_MixRate
          dd FSOUND_MixRate*4
          dd 100004h          ; 16-bit, align 4
endif
if XM_RC_ON
uFMOD_rc  dd res_open
          dd mem_read
          dd mem_seek
endif
uFMOD_mem dd mem_open
          dd mem_read
          dd mem_seek
if XM_FILE_ON
          dd mem_open
uFMOD_fs  dd file_open
          dd file_read
          dd file_seek
          dd file_close
endif

if VOL_CONTROL_ON

	; Set current global volume [0 - silence, 64 - max. volume]
	if PBASIC
		if DSOUND
			; The following symbol lets uFMOD_DSSetVolume to be called from PureBasic
			PUBLIC PB_uFMOD_DSSetVolume
			PB_uFMOD_DSSetVolume:
		else
			; The following symbol lets uFMOD_SetVolume to be called from PureBasic
			PUBLIC PB_uFMOD_SetVolume
			PB_uFMOD_SetVolume:
		endif
	else
		if VB6
			; The following symbol lets uFMOD_SetVolume to be called from VB6
			if DSOUND
				PUBLIC ?uFMOD_SetVolume@DSuF_vb@@AAGXXZ
				?uFMOD_SetVolume@DSuF_vb@@AAGXXZ:
			else
				PUBLIC ?uFMOD_SetVolume@uF_vb@@AAGXXZ
				?uFMOD_SetVolume@uF_vb@@AAGXXZ:
			endif
		else
			PUBLIC uFMOD_SetVolume
			uFMOD_SetVolume:
		endif
	endif
		pop edx
		pop eax ; vol
		; rescale the given argument
		cmp eax,64
		jna setvol_bounds_ok
		push 64
		pop eax
	setvol_bounds_ok:
		neg eax
		add eax,64
		shl eax,25
		sar eax,24
		shr eax,1
		add eax,16
		mov [ufmod_vol],eax
		jmp edx
endif ; VOL_CONTROL_ON

if PAUSE_RESUME_ON

	; Pause the currently playing song.
	if DSOUND
		if PBASIC
			; The following symbol lets uFMOD_DSPause to be called from PureBasic
			PUBLIC PB_uFMOD_DSPause
			PB_uFMOD_DSPause:
		else
			if VB6
				; The following symbol lets uFMOD_Pause to be called from VB6
				PUBLIC ?uFMOD_Pause@DSuF_vb@@AAGXXZ
				?uFMOD_Pause@DSuF_vb@@AAGXXZ:
			else
				PUBLIC uFMOD_Pause
				uFMOD_Pause:
			endif
		endif
		mov eax,[hWaveOut]
		test eax,eax
		jz pause_R
		push eax ; this
		mov ecx,[eax]
		call DWORD PTR [ecx+72] ; IDirectSoundBuffer::Stop
	else
		if PBASIC
			; The following symbol lets uFMOD_Pause to be called from PureBasic
			PUBLIC PB_uFMOD_Pause
			PB_uFMOD_Pause:
		else
			if VB6
				; The following symbol lets uFMOD_Pause to be called from VB6
				PUBLIC ?uFMOD_Pause@uF_vb@@AAGXXZ
				?uFMOD_Pause@uF_vb@@AAGXXZ:
			else
				PUBLIC uFMOD_Pause
				uFMOD_Pause:
			endif
		endif
		mov eax,[hWaveOut]
		test eax,eax
		jz pause_R
		push eax
		call waveOutPause
	endif
	pause_R:
		ret

	; Resume the currently paused song.
	if DSOUND
		if PBASIC
			; The following symbol lets uFMOD_DSResume to be called from PureBasic
			PUBLIC PB_uFMOD_DSResume
			PB_uFMOD_DSResume:
		else
			if VB6
				; The following symbol lets uFMOD_Resume to be called from VB6
				PUBLIC ?uFMOD_Resume@DSuF_vb@@AAGXXZ
				?uFMOD_Resume@DSuF_vb@@AAGXXZ:
			else
				PUBLIC uFMOD_Resume
				uFMOD_Resume:
			endif
		endif
		mov eax,[hWaveOut]
		test eax,eax
		jz resume_R
		cdq
		push 1   ; dwFlags <= DSBPLAY_LOOPING
		push edx ; dwPriority
		push edx ; dwReserved1
		push eax ; this
		mov ecx,[eax]
		call DWORD PTR [ecx+48] ; IDirectSoundBuffer::Play
	else
		if PBASIC
			; The following symbol lets uFMOD_Resume to be called from PureBasic
			PUBLIC PB_uFMOD_Resume
			PB_uFMOD_Resume:
		else
			if VB6
				; The following symbol lets uFMOD_Resume to be called from VB6
				PUBLIC ?uFMOD_Resume@uF_vb@@AAGXXZ
				?uFMOD_Resume@uF_vb@@AAGXXZ:
			else
				PUBLIC uFMOD_Resume
				uFMOD_Resume:
			endif
		endif
		mov eax,[hWaveOut]
		test eax,eax
		jz resume_R
		push eax
		call waveOutRestart
	endif
	resume_R:
		ret
endif ; PAUSE_RESUME_ON

if INFO_API_ON

; Return currently playing signal stats:
; low-order word : RMS volume in R channel
; hi-order word  : RMS volume in L channel
	if PBASIC
		; The following symbol lets uFMOD_GetStats to be called from PureBasic
		if DSOUND
			PUBLIC PB_uFMOD_DSGetStats
			PB_uFMOD_DSGetStats:
		else
			PUBLIC PB_uFMOD_GetStats
			PB_uFMOD_GetStats:
		endif
	else
		if VB6
			; The following symbol lets uFMOD_GetStats to be called from VB6
			if DSOUND
				PUBLIC ?uFMOD_GetStats@DSuF_vb@@AAGXXZ
				?uFMOD_GetStats@DSuF_vb@@AAGXXZ:
			else
				PUBLIC ?uFMOD_GetStats@uF_vb@@AAGXXZ
				?uFMOD_GetStats@uF_vb@@AAGXXZ:
			endif
		else
			PUBLIC uFMOD_GetStats
			uFMOD_GetStats:
		endif
	endif
	xor eax,eax ; eax <= FMUSIC_TIMMEINFO.L_vol
	jmp $ + 5

; Return the time in milliseconds since the song was started. This is
; useful for synchronizing purposes.
	if PBASIC
		; The following symbol lets uFMOD_GetTime to be called from PureBasic
		if DSOUND
			PUBLIC PB_uFMOD_DSGetTime
			PB_uFMOD_DSGetTime:
		else
			PUBLIC PB_uFMOD_GetTime
			PB_uFMOD_GetTime:
		endif
	else
		if VB6
			; The following symbol lets uFMOD_GetTime to be called from VB6
			if DSOUND
				PUBLIC ?uFMOD_GetTime@DSuF_vb@@AAGXXZ
				?uFMOD_GetTime@DSuF_vb@@AAGXXZ:
			else
				PUBLIC ?uFMOD_GetTime@uF_vb@@AAGXXZ
				?uFMOD_GetTime@uF_vb@@AAGXXZ:
			endif
		else
			PUBLIC uFMOD_GetTime
			uFMOD_GetTime:
		endif
	endif
	push 4
	pop eax ; eax <= FMUSIC_TIMMEINFO.ms
	mov edx,OFFSET tInfo
	add eax,edx
	mov edx,[edx-4] ; RealBlock
	shl edx,3 ; x FMUSIC_TIMMEINFO_size
	mov eax,[eax+edx]
	ret

; Return the currently playing track title, if any.
	if PBASIC
		; The following symbol lets uFMOD_GetTitle to be called from PureBasic
		if DSOUND
			PUBLIC PB_uFMOD_DSGetTitle
			PB_uFMOD_DSGetTitle:
		else
			PUBLIC PB_uFMOD_GetTitle
			PB_uFMOD_GetTitle:
		endif
		push esi
		push edi
		push 6
		mov esi,OFFSET szTtl
		mov edi,[_PB_StringBase]
		pop ecx
		mov eax,edi
		rep movsd
		pop edi
		pop esi
		ret
	else
		if VB6
			; The following symbol lets uFMOD_GetTitle to be called from VB6
			if DSOUND
				PUBLIC ?uFMOD_GetTitle@DSuF_vb@@AAGXXZ
				?uFMOD_GetTitle@DSuF_vb@@AAGXXZ:
			else
				PUBLIC ?uFMOD_GetTitle@uF_vb@@AAGXXZ
				?uFMOD_GetTitle@uF_vb@@AAGXXZ:
			endif
		else
			PUBLIC uFMOD_GetTitle
			uFMOD_GetTitle:
		endif
		mov eax,OFFSET szTtl
		ret
	endif

endif ; INFO_API_ON

; Dynamic memory allocation
alloc:
; EAX - how many bytes to allocate
	push eax
	push 9 ; HEAP_ZERO_MEMORY | HEAP_NO_SERIALIZE
	push DWORD PTR [hHeap]
	call HeapAlloc
	ret

; ***************************
; *** XM_MEMORY CALLBACKS ***
; ***************************
mem_open:
; pszName in ESI
	mov [ebp+28],esi ; mmf+8 <= pMem
	ret

mem_read:
; buf in EAX
	pop edx
	pop ecx ; uSize
	push edx
	push edi
	push esi
	xchg eax,edi ; buf
	mov esi,OFFSET mmf
	lodsd
	mov edx,ecx
	add edx,[esi]
	cmp edx,eax
	jl copy
	sub eax,[esi]
	xchg eax,ecx
copy:
	test ecx,ecx
	jle mem_read_Ret
	lodsd
	add eax,[esi]
	mov [esi-4],edx
	xchg eax,esi
	rep movsb
mem_read_Ret:
	pop esi
	pop edi
	ret

mem_seek:
; pos in EAX
	pop edx
	pop ecx ; mode
	push edx
	mov edx,OFFSET mmf+4
	jecxz mem_ok
	add eax,[edx]
mem_ok:
	mov ecx,[edx-4]
	cmp eax,ecx
	jle mem_seek_Ret
	xchg eax,ecx
mem_seek_Ret:
	mov [edx],eax
	ret

; *****************************
; *** XM_RESOURCE CALLBACKS ***
; *****************************
if XM_RC_ON
res_open:
; pszName in ESI
	push 10              ; RT_RCDATA
	push esi             ; pszName
	mov esi,OFFSET mmf
	and DWORD PTR [esi+4],0
	push DWORD PTR [esi] ; HMODULE
	call FindResource
	push eax
	push DWORD PTR [esi]
	push eax
	push DWORD PTR [esi]
	call SizeofResource
	mov [esi],eax
	call LoadResource
	mov [esi+8],eax
	ret
endif ; XM_RC_ON

; *************************
; *** XM_FILE CALLBACKS ***
; *************************
if XM_FILE_ON
file_open:
; pszName in ESI
	push 0         ; CreateFile.hTemplateFile
	push 8000080h  ; CreateFile.dwFlagsAndAttributes   <= FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN
	push 3         ; CreateFile.dwCreationDistribution <= OPEN_EXISTING
	push 0         ; CreateFile.lpSecurityAttributes
	push 1         ; CreateFile.dwShareMode            <= FILE_SHARE_READ
	push 80000000h ; CreateFile.dwDesiredAccess        <= GENERIC_READ
	push esi       ; CreateFile.lpFileName
	call CreateFile
	mov [ebx+8],eax ; SW_Exit
	ret

file_close:
	push DWORD PTR [ebx+8] ; CloseHandle.hObject <- SW_Exit
	call CloseHandle
	ret

file_read:
; buf in EAX
	pop edx
	pop ecx      ; uSize
	push 0       ; ReadFile.lpOverlapped
	push esp     ; ReadFile.lpNumberOfBytesRead
	push ecx     ; ReadFile.nNumberOfBytesToRead
	push eax     ; ReadFile.lpBuffer
	push DWORD PTR [SW_Exit] ; ReadFile.hFile
	push edx
	jmp ReadFile

file_seek:
; pos in EAX
	pop edx
	push 0       ; SetFilePointer.lpDistanceToMoveHigh
	push eax     ; SetFilePointer.lDistanceToMove
	push DWORD PTR [SW_Exit] ; SetFilePointer.hFile
	push edx     ; restore EIP
	jmp SetFilePointer
endif ; XM_FILE_ON

; Starts a song playing.
if DSOUND
	if PBASIC
		PUBLIC PB_uFMOD_DSStopSong
		PB_uFMOD_DSStopSong:
			push eax
			push eax
			push eax
			push 0
			call PB_uFMOD_DSPlaySong
			ret

		; The following symbol lets uFMOD_DSPlaySong to be called from PureBasic
		PUBLIC PB_uFMOD_DSPlaySong
		PB_uFMOD_DSPlaySong:
	else
		if VB6
			; The following symbol lets uFMOD_DSPlaySong to be called from VB6
			PUBLIC ?uFMOD_DSPlaySong@DSuF_vb@@AAGXXZ
			?uFMOD_DSPlaySong@DSuF_vb@@AAGXXZ:
		else
			PUBLIC uFMOD_DSPlaySong
			uFMOD_DSPlaySong:
		endif
	endif
else
	if PBASIC
		PUBLIC PB_uFMOD_StopSong
		PB_uFMOD_StopSong:
			push eax
			push eax
			push 0
			call PB_uFMOD_PlaySong
			ret

		; The following symbol lets uFMOD_PlaySong to be called from PureBasic
		PUBLIC PB_uFMOD_PlaySong
		PB_uFMOD_PlaySong:
	else
		if VB6
			; The following symbol lets uFMOD_PlaySong to be called from VB6
			PUBLIC ?uFMOD_PlaySong@uF_vb@@AAGXXZ
			?uFMOD_PlaySong@uF_vb@@AAGXXZ:
		else
			PUBLIC uFMOD_PlaySong
			uFMOD_PlaySong:
		endif
	endif
endif
	; FREE PREVIOUS TRACK
	call uFMOD_FreeSong
	pop eax
	pop edx ; lpXM
	pop DWORD PTR [mmf] ; param
if DSOUND
	pop DWORD PTR [hWaveOut] ; fdwSong
endif
	pop ecx ; IDirectSoundBuffer / fdwSong
	test edx,edx
	push eax
	jnz do_play
	ret ; user requested to stop playback
do_play:
	; SET CALLBACKS
	push ebx
	push esi
	push edi
	push ebp
	mov ebx,OFFSET hWaveOut
	push eax
	mov ebp,OFFSET ufmod_noloop
if DSOUND
	mov eax,[ebx]
	mov [ebx],ecx
	xchg ecx,eax ; IDirectSoundBuffer <-> fdwSong
endif
	xor eax,eax
	mov [ebp+24],eax ; mmf+4
if NOLOOP_ON
	test cl,XM_NOLOOP
	setnz [ebp] ; ufmod_noloop
endif
	test cl,XM_MEMORY
	mov esi,OFFSET uFMOD_mem
if XM_FILE_ON
	jnz set_callbacks
	test cl,XM_FILE
	lea esi,[esi+(uFMOD_fs-uFMOD_mem)]
	jnz set_callbacks
else
	if XM_RC_ON
		jnz set_callbacks
		test cl,XM_FILE
		jnz uFMOD_FreeSong+5 ; File-loading unsupported (return error)
	else
		jz uFMOD_FreeSong+5  ; Only memory
	endif
endif
if XM_RC_ON
	if XM_FILE_ON
		lea esi,[esi+(uFMOD_rc-uFMOD_fs)]
	else
		lea esi,[esi+(uFMOD_rc-uFMOD_mem)]
	endif
endif
set_callbacks:
	lea edi,[ebp+32] ; uFMOD_fopen
	movsd
	movsd
	movsd
if XM_FILE_ON
	movsd
endif
	mov esi,edx ; uFMOD_fopen:lpXM <= ESI
	; ALLOC GLOBAL CHANNEL POOL
	mov ecx,(FSOUND_CHANNEL_size*64+FMUSIC_CHANNEL_size*32)/4
	mov edi,OFFSET Channels
	rep stosd
	; ALLOC A PRIVATE HEAP
	push eax ; HeapCreate:dwMaximumSize <- make it "growable"
	push eax ; HeapCreate:dwInitialSize <- 1 page
	push eax ; HeapCreate:flOptions
if WMM
	; INIT OUTPUT PCM DRIVER
	push eax ; waveOutOpen:fdwOpen
	push eax ; waveOutOpen:dwCallbackInstance
	push eax ; waveOutOpen:dwCallback
	push OFFSET pcm ; waveOutOpen:pwfx
	push -1  ; waveOutOpen:uDeviceID
	push ebx ; waveOutOpen:phwo
	call waveOutOpen
endif
	call HeapCreate
	test eax,eax
	mov [ebx-8],eax ; hHeap
	jz uFMOD_FreeSong+5
	; LOAD NEW TRACK
	call DWORD PTR [ebp+32] ; uFMOD_fopen
	mov esi,edi ; <- OFFSET _mod
	; VERIFY
	push FMUSIC_MODULE_size
	mov eax,edi
	; uFMOD_fread:buf <= EAX
	call DWORD PTR [ebp+36] ; uFMOD_fread
	xor eax,eax
	call LoadXM
if XM_FILE_ON
	xchg eax,edi
	call DWORD PTR [ebp+44] ; uFMOD_fclose
	test edi,edi
else
	test eax,eax
endif
	jz uFMOD_FreeSong+5
	; SET UP CHANNELS
	xor eax,eax
	push 64
	mov edx,OFFSET Channels+FSOUND_CHANNEL_size*64 ; uFMOD_Ch
if VOL_CONTROL_ON
	mov ecx,OFFSET ufmod_vol
	cmp [ecx],eax
	jne play_vol_ok
	mov BYTE PTR [ecx],16
play_vol_ok:
endif
	pop ecx ; ecx <= 64
	inc eax ; eax <= 1
	mov [esi+FMUSIC_MODULE.globalvolume],ecx
loop_1:
	sub edx,FSOUND_CHANNEL_size
	dec ecx
	mov [edx+FSOUND_CHANNEL.speedhi],eax ; Channels[count].speedhi <= 1
	mov [edx],ecx ; Channels[count].index <= count
	jnz loop_1
	dec eax ; eax <= 0
	movzx ecx,WORD PTR [esi+FMUSIC_MODULE.defaultspeed]
	mov [esi+FMUSIC_MODULE.speed],ecx ; <= defaultspeed
	mov [esi+FMUSIC_MODULE.mixer_samplesleft],eax ; <= 0
	mov [esi+FMUSIC_MODULE.tick],eax ; <= 0
if PATTERNDELAY_ON
	lea edi,[esi+FMUSIC_MODULE.patterndelay]
	stosd
else
	lea edi,[esi+FMUSIC_MODULE.nextorder]
endif
	stosd
	stosd
if INFO_API_ON
	stosd
endif
	mov ecx,[esi+FMUSIC_MODULE.numchannels] ; loop counter
	; edx == OFFSET Channels
loop_2:
	mov [eax+uFMOD_Ch+FMUSIC_CHANNEL.cptr],edx
	add edx,FSOUND_CHANNEL_size
	add eax,FMUSIC_CHANNEL_size
	loop loop_2
play_loop_2_end:
	movzx ecx,WORD PTR [esi+FMUSIC_MODULE.defaultbpm]
	; SetBPM:bpm = ECX
	call SetBPM
if WMM
	; CREATE AND START LOOPING WAVEOUT BLOCK
	mov esi,OFFSET MixBlock
	mov DWORD PTR [esi+FSOUND_SoundBlock.dwBufferLength],FSOUND_BufferSize*4
	or DWORD PTR [esi+FSOUND_SoundBlock.dwLoops],-1
	push 12 ; WHDR_BEGINLOOP or WHDR_ENDLOOP
	mov DWORD PTR [esi+FSOUND_SoundBlock.lpData],OFFSET databuf
	pop DWORD PTR [esi+FSOUND_SoundBlock.dwFlags]
	push 32 ; SIZEOF WAVEHDR
	push esi
	push DWORD PTR [ebx]
	call waveOutPrepareHeader
endif
	xor edi,edi
	; PREFILL THE MIXER BUFFER
loop_3:
	call uFMOD_SW_Fill
	cmp [ebx+4],edi ; uFMOD_FillBlk
	jnz loop_3
	mov [ebx+8],edi ; SW_Exit <= 0
	; START THE OUTPUT
if DSOUND
	mov eax,[ebx]
	push 1   ; dwFlags <= DSBPLAY_LOOPING
	push edi ; dwPriority
	push edi ; dwReserved1
	push eax ; this
	mov ecx,[eax]
	call DWORD PTR [ecx+48] ; IDirectSoundBuffer::Play
	xchg eax,ebx
else
	push 32 ; SIZEOF WAVEHDR
	push esi
	push DWORD PTR [ebx] ; hWaveOut
	call waveOutWrite
	test eax,eax
	jnz uFMOD_FreeSong+5
endif
	; CREATE THE THREAD
	push esp ; lpThreadID
	push edi
	push edi
	push OFFSET uFMOD_Thread
	push edi
	push edi
	call CreateThread
	test eax,eax
	mov [hThread],eax
	jz uFMOD_FreeSong+5
	push 15 ; THREAD_PRIORITY_TIME_CRITICAL
	push eax
	call SetThreadPriority
	xchg eax,ebx ; OFFSET hWaveOut
	pop edx
	pop ebp
	pop edi
	pop esi
	pop ebx
	ret

; Frees memory allocated for a song
uFMOD_FreeSong:
	push ebx
	push esi
	push edi
	push ebp
	push eax
; uFMOD_FreeSong+5
	mov ebp,OFFSET hThread
	mov eax,[ebp]
	test eax,eax
	jz thread_finished
	mov [ebp+12],eax ; SW_Exit
	; Wait for thread to finish
	push eax ; CloseHandle:hObject
	push -1  ; WaitForSingleObject:dwTimeout <= INFINITE
	push eax ; WaitForSingleObject:hObject   <= hThread
	call WaitForSingleObject
	call CloseHandle
thread_finished:
if NOLOOP_ON
	mov ebp,OFFSET hThread
endif
	xor ebx,ebx
	mov [ebp],ebx   ; hThread
	mov edi,[ebp+4] ; hWaveOut
	mov [ebp+8],ebx ; uFMOD_FillBlk
	test edi,edi
	jz snd_closed
	mov [ebp+4],ebx
if DSOUND
	push ebx ; 0
	push edi ; this
	push edi ; this
	mov edi,[edi]
	call DWORD PTR [edi+72] ; IDirectSoundBuffer::Stop
	call DWORD PTR [edi+52] ; IDirectSoundBuffer::SetCurrentPosition
else
	push edi ; waveOutClose:hwo
	push 32  ; waveOutUnprepareHeader:cbwh <= SIZEOF WAVEHDR
	push OFFSET MixBlock ; waveOutUnprepareHeader:pwh
	push edi ; waveOutUnprepareHeader:hwo
	push edi ; waveOutReset:hwo
	call waveOutReset
	call waveOutUnprepareHeader
	; SHUT DOWN PCM DRIVER
	call waveOutClose
endif
snd_closed:
	; FREE PRIVATE HEAP
	mov eax,[ebp-4] ; hHeap
	test eax,eax
	jz free_R
	mov [ebp-4],ebx ; hHeap
	push eax ; HeapDestroy:hHeap
	call HeapDestroy
if INFO_API_ON
	xor eax,eax
	mov ecx,FMUSIC_TIMMEINFO_size*totalblocks/4+2
	mov edi,OFFSET RealBlock
	rep stosd
endif
free_R:
if DSOUND
	mov eax,80070057h ; DSERR_INVALIDPARAM
else
	if INFO_API_ON
	else
		xor eax,eax
	endif
endif
	pop edx
	pop ebp
	pop edi
	pop esi
	pop ebx
	ret

; Internal thread function
uFMOD_Thread:
	push ebp
	push esi
	mov ebp,OFFSET mmt
thread_loop_1:
if DSOUND
	push 0   ; GetCurrentPosition:pdwCurrentWriteCursor
	push ebp ; GetCurrentPosition:pdwCurrentPlayCursor
	mov eax,[ebp+20] ; hWaveOut
	push eax ; this
	mov ecx,[eax]
	call DWORD PTR [ecx+16] ; IDirectSoundBuffer::GetCurrentPosition
	mov esi,[ebp]
else
	push 4
	pop DWORD PTR [ebp]
	push 12 ; SIZEOF MMTIME
	push ebp
	push DWORD PTR [ebp+20] ; hWaveOut
	call waveOutGetPosition
	mov esi,[ebp+4] ; u.cb
endif
	shr esi,12   ; / (FSOUND_BlockSize * 4)
	and esi,0Fh  ; % FSOUND_BufferSize
thread_loop_2:
	push 5
	call Sleep
	cmp DWORD PTR [ebp+28],0 ; SW_Exit
	je thread_loop_2_continue
	pop esi
	pop ebp
	ret 4
thread_loop_2_continue:
	cmp [ebp+24],esi ; uFMOD_FillBlk
	je thread_loop_1
if INFO_API_ON
	mov eax,OFFSET RealBlock
	inc DWORD PTR [eax]
	cmp DWORD PTR [eax],totalblocks
	jl thread_realblock_ok
	and DWORD PTR [eax],0
thread_realblock_ok:
endif
	push thread_loop_2 ; EIP

; Internal function
uFMOD_SW_Fill:
MixPtr equ 8
	mov ecx,FSOUND_BlockSize
	push ebx
	push esi
	push edi
	push ebp
	mov edi,OFFSET MixBuf
	mov eax,[edi-8] ; uFMOD_FillBlk
	mul ecx
	lea edi,[edi+eax*8]
	push edi ; <- MixPtr
	push eax ; pmixpos <= uFMOD_FillBlk*FSOUND_BlockSize
	xchg eax,edx
	shl ecx,1
	push edi ; mixbuffer <= OFFSET MixBuf+(pmixpos<<3)
	; MIXBUFFER CLEAR
	mov esi,OFFSET _mod
	rep stosd
	xchg eax,ebp ; MixedSoFar <= 0
	; UPDATE MUSIC
	mov ebx,[esi+FMUSIC_MODULE.mixer_samplesleft]
fill_loop_1:
	test ebx,ebx
	jnz mixedleft_nz
	; UPDATE XM EFFECTS
	cmp [esi+FMUSIC_MODULE.tick],ebx ; new note
	mov ecx,[esi+FMUSIC_MODULE.pattern]
	jne update_effects
	dec ebx
	; process any rows commands to set the next order/row
	mov edx,[esi+FMUSIC_MODULE.nextorder]
	mov eax,[esi+FMUSIC_MODULE.nextrow]
	mov [esi+FMUSIC_MODULE.nextorder],ebx
	test edx,edx
	mov [esi+FMUSIC_MODULE.nextrow],ebx
	jl fill_nextrow
	mov [esi+FMUSIC_MODULE.order],edx
fill_nextrow:
	test eax,eax
	jl update_note
	mov [esi+FMUSIC_MODULE.row],eax
update_note:
	; UPDATE AND PLAY THE NOTE
	; DoNote:mod = ESI
	call DoNote
	; if there were no row commands
if ROWCOMMANDS_ON
	mov eax,[esi+FMUSIC_MODULE.nextrow]
	test eax,eax
	jns check_row_bounds
endif
	mov eax,[esi+FMUSIC_MODULE.row]
	inc eax
check_row_bounds:
	; if end of pattern
	; "if(mod->nextrow >= mod->pattern[mod->orderlist[mod->order]].rows)"
	cmp ax,[ebx]
	jl set_nextrow
	mov edx,[esi+FMUSIC_MODULE.order]
	movzx ecx,WORD PTR [esi+FMUSIC_MODULE.numorders]
	inc edx
	xor eax,eax
	cmp edx,ecx
	jl set_nextorder
	; We've reached the end of the order list. So, stop playing, unless looping is enabled.
if NOLOOP_ON
	cmp [ufmod_noloop],al
	je set_restart
	pop DWORD PTR [SW_Exit] ; remove mixbuffer - signal thread to stop
	pop edx             ; remove pmixpos
	jmp thread_finished
set_restart:
endif
	movzx edx,WORD PTR [esi+FMUSIC_MODULE.restart]
	cmp edx,ecx
	jl set_nextorder
	cdq
set_nextorder:
	mov [esi+FMUSIC_MODULE.nextorder],edx
set_nextrow:
	mov [esi+FMUSIC_MODULE.nextrow],eax
	jmp inc_tick
update_effects:
	; update the inbetween row effects
	; DoEffs:mod = ESI
	call DoEffs
inc_tick:
	mov eax,[esi+FMUSIC_MODULE.speed]
	mov ebx,[esi+FMUSIC_MODULE.mixer_samplespertick]
	inc DWORD PTR [esi+FMUSIC_MODULE.tick]
if PATTERNDELAY_ON
	add eax,[esi+FMUSIC_MODULE.patterndelay]
endif
	cmp [esi+FMUSIC_MODULE.tick],eax
	jl mixedleft_nz
if PATTERNDELAY_ON
	and DWORD PTR [esi+FMUSIC_MODULE.patterndelay],0
endif
	and DWORD PTR [esi+FMUSIC_MODULE.tick],0
mixedleft_nz:
	mov edi,FSOUND_BlockSize
	sub edi,ebp
	cmp ebx,edi
	jae fill_ramp
	mov edi,ebx
fill_ramp:
	add ebp,edi ; MixedSoFar += SamplesToMix
	mov edx,[esp+MixPtr]
	lea eax,[edx+edi*8]
	mov [esp+MixPtr],eax ; MixPtr += (SamplesToMix<<3)
	; Ramp:tail   == EAX
	; Ramp:len    == EDI
	; Ramp:mixptr == EDX
	call Ramp
if INFO_API_ON
	lea eax,[edi+edi*4]
	cdq
	shl eax,2
	mov ecx,FSOUND_MixRate/50
	div ecx
	; _mod.time_ms += SamplesToMix*FSOUND_OOMixRate*1000
	add [esi+FMUSIC_MODULE.time_ms],eax
endif
	sub ebx,edi ; MixedLeft -= SamplesToMix
	cmp ebp,FSOUND_BlockSize
	jb fill_loop_1
	mov [esi+FMUSIC_MODULE.mixer_samplesleft],ebx ; <= MixedLeft
if INFO_API_ON
	mov eax,[uFMOD_FillBlk]
	shl eax,3 ; x FMUSIC_TIMMEINFO_size
	mov edi,OFFSET tInfo+4
	mov ecx,[esi+FMUSIC_MODULE.time_ms]
	mov [eax+edi],ecx
endif
	; CLIP AND COPY BLOCK TO OUTPUT BUFFER
	pop esi ; <- mixbuffer
	pop eax ; <- pmixpos
if INFO_API_ON
	xor ebx,ebx ; L channel RMS volume
	xor ebp,ebp ; R channel RMS volume
endif
if DSOUND
	; Lock the output buffer
	cdq
	shl eax,2
	mov edi,OFFSET mmt
	push edx        ; Lock:dwFlags
	push edx        ; Lock:pdwAudioBytes2
	push edx        ; Lock:ppvAudioPtr2
	push edi        ; Lock:pdwAudioBytes1
	stosd
	push edi        ; Lock:ppvAudioPtr1
	push FSOUND_BlockSize*4 ; Lock:dwBytes
	push eax        ; Lock:dwOffset
	mov eax,[edi+16]
	push eax ; this
	mov ecx,[eax]
	call DWORD PTR [ecx+44] ; IDirectSoundBuffer::Lock
	test eax,eax
	jns DS_lock_ok
	cmp eax,88780096h ; DSERR_BUFFERLOST
	jne DS_R
	; Try to restore the buffer
	mov eax,[edi+16]
	push eax ; this
	mov ecx,[eax]
	call DWORD PTR [ecx+80] ; IDirectSoundBuffer::Restore
	jmp DS_R
DS_lock_ok:
	mov ecx,[edi-4]
	mov edi,[edi]   ; <dest. ptr>
else
	lea edi,[eax*4+OFFSET databuf] ; <dest. ptr> = MixBlock.data+pmixpos*4
endif
if INFO_API_ON
if DSOUND
	shr ecx,2
	jz SW_Fill_Ret
else
	mov ecx,FSOUND_BlockSize
endif
fill_loop_2:
	fld DWORD PTR [esi]
	mov edx,0FFFF8000h
	fistp DWORD PTR [esi]
	lodsd
	cmp eax,edx
	jge lower_bound_ok
	mov eax,edx
lower_bound_ok:
	not edx
	cmp eax,edx
	jle upper_bound_ok
	xchg eax,edx
upper_bound_ok:
if VOL_CONTROL_ON
	shl eax,4
	cdq
	idiv DWORD PTR [ufmod_vol]
endif
	; sum to L channel RMS volume
	cdq
	stosw
	xor eax,edx
	sub eax,edx
	add ebx,eax ; += |vol|
	fld DWORD PTR [esi]
	mov edx,0FFFF8000h
	fistp DWORD PTR [esi]
	lodsd
	cmp eax,edx
	jge lower_bound_ok2
	mov eax,edx
lower_bound_ok2:
	not edx
	cmp eax,edx
	jle upper_bound_ok2
	xchg eax,edx
upper_bound_ok2:
if VOL_CONTROL_ON
	shl eax,4
	cdq
	idiv DWORD PTR [ufmod_vol]
endif
	; sum to R channel RMS volume
	cdq
	stosw
	xor eax,edx
	sub eax,edx
	add ebp,eax ; += |vol|
	dec ecx
	jnz fill_loop_2
else
if DSOUND
	shr ecx,1
	jz SW_Fill_Ret
else
	mov ecx,FSOUND_BlockSize*2
endif
	mov ebx,0FFFF8000h
	mov ebp,ebx
	not ebp
fill_loop_2:
	fld DWORD PTR [esi]
	fistp DWORD PTR [esi]
	lodsd
	cmp eax,ebx
	jge lower_bound_ok
	mov eax,ebx
lower_bound_ok:
	cmp eax,ebp
	jle upper_bound_ok
	mov eax,ebp
upper_bound_ok:
if VOL_CONTROL_ON
	shl eax,4
	cdq
	idiv DWORD PTR [ufmod_vol]
endif
	dec ecx
	stosw
	jnz fill_loop_2
endif
SW_Fill_Ret:
if DSOUND
	; Unlock the output buffer
	mov edi,OFFSET mmt
	push ecx                ; Unlock:pdwAudioBytes2
	push ecx                ; Unlock:ppvAudioPtr2
	push DWORD PTR [edi]    ; Unlock:pdwAudioBytes1
	push DWORD PTR [edi+4]  ; Unlock:ppvAudioPtr1
	mov eax,[edi+20]
	push eax ; this
	mov ecx,[eax]
	call DWORD PTR [ecx+76] ; IDirectSoundBuffer::Unlock
DS_R:
endif
	mov eax,OFFSET uFMOD_FillBlk
	inc DWORD PTR [eax]
	cmp DWORD PTR [eax],totalblocks
	jl SW_Fill_R
	and DWORD PTR [eax],0
SW_Fill_R:
if INFO_API_ON
	mov edx,[eax]
	shr ebp,10 ; R_vol / FSOUND_BlockSize
	shl edx,3  ; x FMUSIC_TIMMEINFO_size
	shl ebx,6  ; (L_vol / FSOUND_BlockSize) << 16
	add edx,OFFSET tInfo
	mov bx,bp
	mov [edx],ebx
endif
	pop eax
	pop ebp
	pop edi
	pop esi
	pop ebx
	ret