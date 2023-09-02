; CORE.ASM:
; --------
; uFMOD public source code release. Provided as-is.

f1over255 dd 3.9215689e-3 ; 1.0f / 255.0f
f255      dd 255.0e0

; Mixer ramping
; PARAMS: mixer buffer (in EDX), it's length (in EDI) and a pointer to it's tail (in EAX)
Ramp:
	mov [mmf],eax
	; LOOP THROUGH CHANNELS
	mov ecx,OFFSET Channels
	push esi
	push ebx
	push ebp
loop_ch:
	mov esi,[ecx+FSOUND_CHANNEL.fsptr] ; load the correct SAMPLE pointer for this channel
	test esi,esi ; if(!fsptr) then skip this channel!
	mov [mmf+4],edx
	jz MixExit_1
	push edx
	lea edx,[esi+FSOUND_SAMPLE.buff] ; get pointer to sample buffer
	shr edx,1
	mov ebx,[ecx+FSOUND_CHANNEL.mixpos]
	mov [mmf+8],edx
	; the following code sets up a mix counter. it sees what will happen first, will the output buffer
	; end be reached first? or will the end of the sample be reached first? whatever is smallest will
	; be the mixcount.
	push edi
CalculateLoopCount:
	xor ebp,ebp
	mov edx,[esi+FSOUND_SAMPLE.loopstart]
	cmp [ecx+FSOUND_CHANNEL.speeddir],ebp ; compare to FSOUND_MIXDIR_FORWARDS
	mov [uFMOD_fread],ebp
	mov [uFMOD_fopen],ebp
	mov eax,[ecx+FSOUND_CHANNEL.mixposlo]
	jne samplesleftbackwards
	; work out how many samples left from mixpos to loop end
	add edx,[esi+FSOUND_SAMPLE.looplen]
	cmp ebx,edx
	jle submixpos
	mov edx,[esi+FSOUND_SAMPLE._length]
submixpos:
	sub edx,ebx ; edx = samples left (loopstart+looplen-mixpos)
	neg eax
	sbb edx,ebp
	adc edx,ebp
	jmp samplesleftfinish
samplesleftbackwards:
	; work out how many samples left from mixpos to loop start
	neg edx
	add edx,ebx
samplesleftfinish:
	; edx:eax now contains number of samples left to mix
	cmp edx,1000000h
	jae staywithoutputbuffend
	; now samples left = EDX:EAX -> hhhhhlll
	shrd eax,edx,8
	mov ebx,[ecx+FSOUND_CHANNEL.speedhi]
	mov ebp,[ecx+FSOUND_CHANNEL.speedlo]
	shr edx,8
	shrd ebp,ebx,8
	jnz speedok ; divide by 0 check
	mov ebp,17C6Fh ; 100Hz
speedok:
	div ebp
	test edx,edx
	jz dontaddbyte
	inc eax
dontaddbyte:
	cmp eax,edi
	ja staywithoutputbuffend
	stc
	xchg eax,edi
staywithoutputbuffend:
	mov eax,[ecx+FSOUND_CHANNEL.ramp_count]
	; set a flag to say mix will end when end of output buffer is reached
	setc [mix_endflag]
	; VOLUME RAMP SETUP
	; Reasons to ramp
	; 1 volume change
	; 2 sample starts (just treat as volume change - 0 to volume)
	; 3 sample ends (ramp last n number of samples from volume to 0)
	; now if the volume has changed, make end condition equal a volume ramp
	test eax,eax
	mov [mix_count_old],edi ; remember mix count before modifying it
	mov edx,[ecx+FSOUND_CHANNEL.leftvolume]
	mov ebp,[ecx+FSOUND_CHANNEL.rightvolume]
	jz volumerampstart
	; if it tries to continue an old ramp, but the target has changed,
	; set up a new ramp
	cmp edx,[ecx+FSOUND_CHANNEL.ramp_lefttarget]
	jne volumerampstart
	cmp ebp,[ecx+FSOUND_CHANNEL.ramp_righttarget]
	jne volumerampstart
	; restore old ramp
	mov edx,[ecx+FSOUND_CHANNEL.ramp_leftspeed]
	mov [uFMOD_fread],edx
	mov edx,[ecx+FSOUND_CHANNEL.ramp_rightspeed]
	mov [uFMOD_fopen],edx
	jmp novolumerampR
volumerampstart:
	mov [ecx+FSOUND_CHANNEL.ramp_lefttarget],edx
	sub edx,[ecx+FSOUND_CHANNEL.ramp_leftvolume]
	jz novolumerampL
	mov [uFMOD_fread],edx
	xor eax,eax
	fild DWORD PTR [uFMOD_fread]
	fmul DWORD PTR [rampsteps]
	fst DWORD PTR [uFMOD_fread]
	fstp DWORD PTR [ecx+FSOUND_CHANNEL.ramp_leftspeed]
	mov al,volumerampsteps
novolumerampL:
	mov [ecx+FSOUND_CHANNEL.ramp_righttarget],ebp
	sub ebp,[ecx+FSOUND_CHANNEL.ramp_rightvolume]
	jz novolumerampR
	mov [uFMOD_fopen],ebp
	xor eax,eax
	fild DWORD PTR [uFMOD_fopen]
	fmul DWORD PTR [rampsteps]
	fst DWORD PTR [uFMOD_fopen]
	fstp DWORD PTR [ecx+FSOUND_CHANNEL.ramp_rightspeed]
	mov al,volumerampsteps
novolumerampR:
	test eax,eax
	jle volumerampend
	cmp edi,eax
	mov [ecx+FSOUND_CHANNEL.ramp_count],eax
	jbe volumerampend ; dont clamp mixcount
	xchg eax,edi
volumerampend:
	mov [mix_count],edi
	; SET UP VOLUME MULTIPLIERS
	; right volume
	fild DWORD PTR [ecx+FSOUND_CHANNEL.rightvolume]
	fmul DWORD PTR [f1over255]
	fstp DWORD PTR [mix_rightvol]
	; left volume
	fild DWORD PTR [ecx+FSOUND_CHANNEL.leftvolume]
	fmul DWORD PTR [f1over255]
	fstp DWORD PTR [mix_leftvol]
	; right ramp volume
	fild DWORD PTR [ecx+FSOUND_CHANNEL.ramp_rightvolume]
	fmul DWORD PTR [f1over255] ; convert from 24:8 to 0-255 and make 0-1.0f
	fstp DWORD PTR [uFMOD_fseek]
	; left ramp volume
	fild DWORD PTR [ecx+FSOUND_CHANNEL.ramp_leftvolume]
	fmul DWORD PTR [f1over255] ; convert from 24:8 to 0-255 and make 0-1.0f
	fstp DWORD PTR [uFMOD_fclose]
	; SET UP ALL OF THE REGISTERS HERE FOR THE INNER LOOP
	; edi = speed low
	; edx = speed high
	; ebx = mixpos
	; esi = destination pointer
	; ebp = mixpos low
	mov ebp,[ecx+FSOUND_CHANNEL.mixposlo]
	mov ebx,[ecx+FSOUND_CHANNEL.mixpos]
	push esi
	cmp DWORD PTR [ecx+FSOUND_CHANNEL.speeddir],FSOUND_MIXDIR_FORWARDS
	mov esi,[mmf+4] ; point to 16bit output stream
	mov edi,[ecx+FSOUND_CHANNEL.speedlo]
	mov edx,[ecx+FSOUND_CHANNEL.speedhi]
	je NoChangeSpeed
	not edi
	not edx
	add edi,1
	adc edx,0
NoChangeSpeed:
	; 16 BIT NORMAL FUNCTIONS
	add ebx,[mmf+8]
	; ecx = loop counter
	push ecx
	cmp DWORD PTR [ecx+FSOUND_CHANNEL.ramp_count],0
	mov ecx,[mix_count]
	jne MixLoopStart16_2
	push ecx
	shr ecx,1
	jz MixLoopStart16 ; no groups of 2 samples to mix!
	; START
	call deltas ; compute volume deltas
	jmp MixLoopUnroll16CoilEntry
MixLoopUnroll16:
	call deltas
	fxch st3
	fstp DWORD PTR [esi-28]
	fxch st3
	fstp DWORD PTR [esi-24]
	fstp DWORD PTR [esi-20]
MixLoopUnroll16CoilEntry:
	fxch st1
	fld st0
	fld st2
	fld DWORD PTR [mix_rightvol]
	fmul st2,st0
	fmulp st1,st0
	fld DWORD PTR [mix_leftvol]
	fmul st4,st0
	fmulp st3,st0
	fadd DWORD PTR [esi-16]
	dec ecx
	fstp DWORD PTR [esi-16]
	fadd DWORD PTR [esi-8]
	fxch st2
	fadd DWORD PTR [esi-12]
	fxch st1
	fadd DWORD PTR [esi-4]
	jnz MixLoopUnroll16
	fstp DWORD PTR [esi-4]
	fstp DWORD PTR [esi-12]
	fstp DWORD PTR [esi-8]
	; MIX 16BIT, ROLLED
MixLoopStart16:
	pop ecx ; ecx <- mix_count
	and ecx,1
MixLoopStart16_2:
	; if(!count) dont enter the mix loop
	test ecx,ecx
	jz MixLoopEnd16
	fld DWORD PTR [uFMOD_fread]
	fld DWORD PTR [uFMOD_fclose]
	fld DWORD PTR [uFMOD_fseek]
	fld DWORD PTR [uFMOD_fopen]
MixLoop16:
	shr ebp,1 ; make 31bit coz fpu only loads signed values
	mov [uFMOD_fclose],ebp
	fild WORD PTR [ebx+ebx+2]
	fild WORD PTR [ebx+ebx]
	shl ebp,1
	fsub st1,st0
	fild DWORD PTR [uFMOD_fclose]
	add ebp,edi
	adc ebx,edx
	fmul DWORD PTR [f1over2g]
	fmulp st2,st0
	faddp st1,st0
	fld st0
	fmul st0,st3
	fadd DWORD PTR [esi]
	fstp DWORD PTR [esi]
	lodsd
	fmul st0,st3
	fadd DWORD PTR [esi]
	dec ecx
	fstp DWORD PTR [esi]
	lodsd
	fadd st1,st0
	fld st3
	faddp st3,st0
	jnz MixLoop16
	fstp st0
	fmul DWORD PTR [f255]
	fistp DWORD PTR [uFMOD_fseek]
	fmul DWORD PTR [f255]
	fistp DWORD PTR [uFMOD_fclose]
	fstp st0
MixLoopEnd16:
	mov eax,[mmf] ; find out how many OUTPUT samples left to mix
	mov [mmf+4],esi ; update the new mixbuffer pointer
	sub ebx,[mmf+8]
	sub eax,esi
	shr eax,3
	xchg eax,edi ; edi now holds # of samples left
	; DID A VOLUME RAMP JUST HAPPEN
	pop ecx ; load ecx with channel pointer
	mov edx,[ecx+FSOUND_CHANNEL.ramp_count]
	mov [ecx+FSOUND_CHANNEL.mixpos],ebx
	test edx,edx
	mov [ecx+FSOUND_CHANNEL.mixposlo],ebp
	pop esi ; load esi with sample pointer
	jz DoOutputbuffEnd ; no, no ramp
	mov eax,[uFMOD_fclose]
	mov [ecx+FSOUND_CHANNEL.ramp_leftvolume],eax
	mov eax,[uFMOD_fseek]
	mov [ecx+FSOUND_CHANNEL.ramp_rightvolume],eax
	mov eax,[mix_count]
	sub edx,eax
	mov [ecx+FSOUND_CHANNEL.ramp_count],edx
	cdq
	; if(!rampcount) a ramp has FINISHED, so finish the rest of the mix
	jnz DoOutputbuffEnd
	; clear out the ramp speeds
	mov [ecx+FSOUND_CHANNEL.ramp_leftspeed],edx
	mov [ecx+FSOUND_CHANNEL.ramp_rightspeed],edx
	; is it 0 because ramp ended only? or both ended together?
	; if sample ended together with ramp... problems... loop isn't handled
	cmp [mix_count_old],eax ; ramp and output mode ended together
	mov eax,[ecx+FSOUND_CHANNEL.leftvolume]
	mov [ecx+FSOUND_CHANNEL.ramp_leftvolume],eax
	mov eax,[ecx+FSOUND_CHANNEL.rightvolume]
	mov [ecx+FSOUND_CHANNEL.ramp_rightvolume],eax
	je DoOutputbuffEnd
	; start again and continue rest of mix
	test edi,edi
	jnz CalculateLoopCount ; dont start again if nothing left
DoOutputbuffEnd:
	cmp [mix_endflag],dl
	je MixExit
	movzx eax,BYTE PTR [esi+FSOUND_SAMPLE.loopmode]
	; SWITCH ON LOOP MODE TYPE
	dec eax ; check for normal loop (FSOUND_LOOP_NORMAL == 1)
	jnz CheckBidiLoop
	mov eax,[esi+FSOUND_SAMPLE.loopstart]
	add eax,[esi+FSOUND_SAMPLE.looplen]
rewindsample:
	sub ebx,[esi+FSOUND_SAMPLE.looplen]
	cmp ebx,eax
	jae rewindsample
	test edi,edi
	mov [ecx+FSOUND_CHANNEL.mixpos],ebx
	jz MixExit
	jmp CalculateLoopCount
CheckBidiLoop:
	dec eax ; FSOUND_LOOP_BIDI == 2
	jnz NoLoop
	cmp [ecx+FSOUND_CHANNEL.speeddir],eax ; FSOUND_MIXDIR_FORWARDS
	je BidiForward
BidiBackwards:
	mov eax,[esi+FSOUND_SAMPLE.loopstart]
	cdq
	dec eax
	dec DWORD PTR [ecx+FSOUND_CHANNEL.speeddir] ; set FSOUND_MIXDIR_FORWARDS
	dec edx
	sub edx,ebp
	sbb eax,ebx
	mov ebp,edx
	mov ebx,[esi+FSOUND_SAMPLE.loopstart]
	add ebx,eax
	cmp eax,[esi+FSOUND_SAMPLE.looplen]
	jl BidiFinish
BidiForward:
	mov eax,[esi+FSOUND_SAMPLE.loopstart]
	add eax,[esi+FSOUND_SAMPLE.looplen]
	neg ebp
	mov edx,eax
	sbb eax,ebx
	xchg eax,ebx
	xchg eax,edx
	cdq
	dec eax
	dec edx
	add ebp,edx
	adc ebx,eax
	inc DWORD PTR [ecx+FSOUND_CHANNEL.speeddir] ; go backwards
	cmp ebx,[esi+FSOUND_SAMPLE.loopstart]
	jl BidiBackwards
BidiFinish:
	test edi,edi
	mov [ecx+FSOUND_CHANNEL.mixpos],ebx
	mov [ecx+FSOUND_CHANNEL.mixposlo],ebp
	jz MixExit
	jmp CalculateLoopCount
NoLoop:
	mov [ecx+FSOUND_CHANNEL.fsptr],edx
	mov [ecx+FSOUND_CHANNEL.mixposlo],edx
	mov [ecx+FSOUND_CHANNEL.mixpos],edx
MixExit:
	pop edi
	pop edx
MixExit_1:
	add ecx,FSOUND_CHANNEL_size
	cmp ecx,OFFSET Channels+FSOUND_CHANNEL_size*64
	jl loop_ch ; iterate 64 times
	pop ebp
	pop ebx
	pop esi
	ret
f1over2g:
	dd 4.6566129e-10 ; 1.0f / 2147483648.0f
deltas:
	; volume deltas
	shr ebp,1 ; make 31bit coz fpu only loads signed values
	lea eax,[ebx+ebx]
	mov [mmf+4],ebp
	shl ebp,1
	add ebp,edi
	adc ebx,edx
	fild WORD PTR [ebx+ebx]
	add esi,16
	shr ebp,1
	fild WORD PTR [eax]
	fild DWORD PTR [mmf+4]
	mov [mmf+4],ebp
	fmul DWORD PTR [f1over2g]
	fild WORD PTR [eax+2]
	shl ebp,1
	fsub st0,st2
	fmulp st1,st0
	faddp st1,st0
	fild DWORD PTR [mmf+4]
	fmul DWORD PTR [f1over2g]
	fild WORD PTR [ebx+ebx+2]
	fsub st0,st3
	fmulp st1,st0
	add ebp,edi
	adc ebx,edx
	faddp st2,st0
	ret

if AMIGAPERIODS_ON
; DESCRIPTION: Returns a float
; PARAMETERS: Intrinsic real argument in ST0
; NOTE: no registers affected
f0_0833 dd 8.3333336e-2 ; 1/12.0f
f13_375 dd 1.3375e1
exp2:
	fmul DWORD PTR [f0_0833] ; /12.0f
exp2_2:
	fld st0       ; copy the logarithm
	frndint       ; keep only the characteristic
	fsub st1,st0  ; keeps only the mantissa
	fxch st1      ; get the mantissa on top
	f2xm1         ; ->2^(mantissa)-1
	fld1
	faddp st1,st0 ; add 1 back
	fscale        ; scale it with the characteristic
	fstp st1      ; overwrite the characteristic
	fmul DWORD PTR [f13_375] ; *13.375f
	ret

AmigaPeriod:
; note in EAX
	push esi
	cdq
	lea esi,[esp+8] ; sptr
	mov dl,132
	push edi
	sub edx,eax
	mov edi,edx
	test eax,eax
	mov eax,[esi]
	mov [esi],edx
	movsx eax,BYTE PTR [eax+FSOUND_SAMPLE.finetune]
	fild DWORD PTR [esi]
	call exp2
if FTOL
	push ebx
	mov ebx,OFFSET cw
	fldcw [ebx]   ; Set FPU rounding close to ftol()
	fistp DWORD PTR [esi]
	fldcw [ebx+2] ; Set FPU rounding to default
else
	fistp DWORD PTR [esi]
endif
	jz _do_inc
	test eax,eax
	cdq
	js _do_sub
_do_inc:
	inc edx
_do_sub:
	sub edi,edx
	push edi
	mov edi,[esi]
	fild DWORD PTR [esp]
	pop ecx
	call exp2
if FTOL
	fldcw [ebx]   ; Set FPU rounding close to ftol()
	fistp DWORD PTR [esi]
	fldcw [ebx+2] ; Set FPU rounding to default
	pop ebx
else
	fistp DWORD PTR [esi]
endif
	mov ecx,edi
	sub ecx,[esi]
	test edx,edx
	js _do_imul
	neg ecx
_do_imul:
	imul ecx
	sar eax,7
	add eax,edi
	pop edi
	pop esi
	ret 4
endif ; AMIGAPERIODS_ON

; Try to emulate ftol() behaviour. Only for testing!
if FTOL
cw     dw 0E7Fh ; FPU: round as ftol() does
       dw 027Fh ; FPU: default setting
endif

if VIBRATO_OR_TREMOLO
f0_098 dd 9.817475e-2 ; 2*3.141592/64.0f
f256   dd 2.56e2      ; 256.0f
endif

; DESCRIPTION: To carry out a vibrato at a certain depth and speed
if VIBRATO_OR_VOLSLIDE
Vibrato:
; cptr = ESI
	push ebx
	mov al,[esi+FMUSIC_CHANNEL.wavecontrol]
	movsx ebx,BYTE PTR [esi+FMUSIC_CHANNEL.vibpos]
	and eax,3 ; switch(cptr->wavecontrol&3)
	jz case_0
	cdq
	dec dl
	dec eax
	jnz default
	; case 1
	shl ebx,3
	jns default ; delta = cptr->vibpos<<3
	inc edx
	neg edx ; delta = 255-(cptr->vibpos<<3)
	jmp default
case_0:
	push eax
	mov [esp],ebx
	fild DWORD PTR [esp]
	fmul DWORD PTR [f0_098]
	fsin
	fmul DWORD PTR [f256]
	fabs
	fistp DWORD PTR [esp] ; delta = fabs((sin(cptr->vibpos*2*3.141592/64.0f))*256.0f)
	mov edx,[esp]
	pop eax
default:
	movzx eax,BYTE PTR [esi+FMUSIC_CHANNEL.vibdepth]
	imul edx ; delta *= cptr->vibdepth
	sar eax,5 ; delta >>= 7; delta <<= 2
	and al,0FCh
	test ebx,ebx
	js set_freqdelta
	neg eax
set_freqdelta:
	pop ebx
	mov [esi+FMUSIC_CHANNEL.freqdelta],eax
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_FREQ
	ret
endif ; VIBRATO_OR_VOLSLIDE

if TREMOLO_ON
Tremolo:
; cptr = ESI
	cdq
	dec dl ; voldelta = 255
	mov cl,[esi+FMUSIC_CHANNEL.wavecontrol]
	shr ecx,4
	and ecx,3 ; switch((cptr->wavecontrol>>4)&3)
	jz trem_case_0
	dec ecx
	jz trem_case_1
	dec ecx
	dec ecx
	jnz trem_end_switch
	; case 0 / case 3
trem_case_0:
	movsx ecx,BYTE PTR [esi+FMUSIC_CHANNEL.tremolopos]
	push ecx
	fild DWORD PTR [esp]
	fmul DWORD PTR [f0_098]
	fsin
	fmul DWORD PTR [f256]
	fabs
	fistp DWORD PTR [esp]
	pop edx ; fabs((sin(cptr->tremolopos*2*3.141592/64.0f))*256.0f)
	jmp trem_end_switch
trem_case_1:
	movzx ecx,BYTE PTR [esi+FMUSIC_CHANNEL.tremolopos]
	mov edx,ecx
	shl dl,3
	test ecx,ecx
	jge trem_end_switch ; voldelta = (cptr->tremolopos)<<3
	mov ecx,edx
	or dl,255
	sub edx,ecx ; voldelta = 255-(cptr->tremolopos)<<3
trem_end_switch:
	movzx eax,BYTE PTR [esi+FMUSIC_CHANNEL.tremolodepth]
	imul eax,[esi+FMUSIC_CHANNEL.voldelta]
	sar eax,6
	imul edx
	push ebx
	mov bl,[esi+FMUSIC_CHANNEL.tremolopos]
	test bl,bl
	mov [esi+FMUSIC_CHANNEL.voldelta],eax ; cptr->voldelta = voldelta*((cptr->voldelta*cptr->tremolodepth)>>6)
	jl tremolopos_lz
	mov edx,[esi+FMUSIC_CHANNEL.volume]
	add eax,edx
	cmp eax,64
	jle trem_end_if
	push 64
	pop ecx
	sub ecx,edx ; cptr->voldelta = 64-cptr->volume
	jmp set_voldelta
tremolopos_lz:
	mov cx,WORD PTR [esi+FMUSIC_CHANNEL.volume]
	sub cx,WORD PTR [esi+FMUSIC_CHANNEL.voldelta]
	jns inv_voldelta
	mov ecx,[esi+FMUSIC_CHANNEL.volume]
	mov [esi+FMUSIC_CHANNEL.voldelta],ecx ; cptr->voldelta = cptr->volume
inv_voldelta:
	mov ecx,[esi+FMUSIC_CHANNEL.voldelta]
	neg ecx ; cptr->voldelta = -cptr->voldelta
set_voldelta:
	mov [esi+FMUSIC_CHANNEL.voldelta],ecx
trem_end_if:
	mov cl,[esi+FMUSIC_CHANNEL.tremolospeed]
	add cl,bl ; cptr->tremolopos += cptr->tremolospeed
	pop ebx
	cmp cl,31
	jle tremolopos_ok
	sub cl,64 ; cptr->tremolopos -=64
tremolopos_ok:
	mov [esi+FMUSIC_CHANNEL.tremolopos],cl
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_VOLUME
	ret
endif ; TREMOLO_ON

if PORTATO_OR_VOLSLIDE
Portamento:
; cptr = ESI
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_FREQ
	and DWORD PTR [esi+FMUSIC_CHANNEL.freqdelta],0
	movzx edx,BYTE PTR [esi+FMUSIC_CHANNEL.portaspeed]
	mov ecx,[esi+FMUSIC_CHANNEL.portatarget]
	mov eax,[esi+FMUSIC_CHANNEL.freq]
	shl edx,2
	cmp eax,ecx
	jg porta_do_sub
	add eax,edx
	cmp eax,ecx
	jg _do_trim
	jmp _no_trim
porta_do_sub:
	sub eax,edx
	cmp eax,ecx
	jl _do_trim
_no_trim:
	xchg eax,ecx
_do_trim:
	mov [esi+FMUSIC_CHANNEL.freq],ecx
	ret
endif ; PORTATO_OR_VOLSLIDE

if VOLUME_OR_PANENVELOPE
Envelope:
; cptr = ESI
control      equ 12
env_iptr     equ 8
env_tick     equ -12
envdelta     equ -16
envstopped   equ -20
valfrac      equ -24
env_value    equ -28
sustain_l2   equ -30
sustain_l1   equ -31
sustain_loop equ -32
env_type     equ -36
	push ebp
	mov ebp,esp
	; Initialize local vars with PAN/VOL data
	push edi
	push ebx
	mov edi,[ebp+env_iptr]
	add edi,FMUSIC_INSTRUMENT.PANPoints
if PANENVELOPE_ON
	lea ecx,[esi+FMUSIC_CHANNEL.envpanpos]
	lea ebx,[esi+FMUSIC_CHANNEL.envpantick]
	lea eax,[esi+FMUSIC_CHANNEL.envpandelta]
	mov [ebp+envdelta],eax
	lea eax,[esi+FMUSIC_CHANNEL.envpan]
	mov [ebp+env_value],eax
	lea eax,[esi+FMUSIC_CHANNEL.envpanfrac]
	mov [ebp+valfrac],eax
	mov eax,DWORD PTR [edi+FMUSIC_INSTRUMENT.PANsustain-FMUSIC_INSTRUMENT.PANPoints]
	mov [ebp+sustain_loop],eax ; load PANsustain, PANLoopStart and PANLoopEnd
	mov al,BYTE PTR [edi+FMUSIC_INSTRUMENT.PANtype-FMUSIC_INSTRUMENT.PANPoints]
	mov [ebp+env_type],eax
	movzx edx,BYTE PTR [edi+FMUSIC_INSTRUMENT.PANnumpoints-FMUSIC_INSTRUMENT.PANPoints]
endif
	mov eax,[ebp+control]
	or [esi+FMUSIC_CHANNEL.notectrl],al ; cptr->notectrl |= control
if PANENVELOPE_ON
	if VOLUMEENVELOPE_ON
		cmp al,FMUSIC_VOLUME ; is it FMUSIC_VOLUME or FMUSIC_PAN?
	endif
	lea eax,[esi+FMUSIC_CHANNEL.envpanstopped]
endif
if VOLUMEENVELOPE_ON
	if PANENVELOPE_ON
		jnz pan_or_vol_ok
	endif
	; control == FMUSIC_VOLUME
	lea ecx,[esi+FMUSIC_CHANNEL.envvolpos]
	lea ebx,[esi+FMUSIC_CHANNEL.envvoltick]
	lea eax,[esi+FMUSIC_CHANNEL.envvoldelta]
	mov [ebp+envdelta],eax
	lea eax,[esi+FMUSIC_CHANNEL.envvol]
	mov [ebp+env_value],eax
	lea eax,[esi+FMUSIC_CHANNEL.envvolfrac]
	mov [ebp+valfrac],eax
	mov eax,DWORD PTR [edi+FMUSIC_INSTRUMENT.VOLsustain-FMUSIC_INSTRUMENT.PANPoints]
	mov [ebp+sustain_loop],eax ; load VOLsustain, VOLLoopStart and VOLLoopEnd
	mov al,BYTE PTR [edi+FMUSIC_INSTRUMENT.VOLtype-FMUSIC_INSTRUMENT.PANPoints]
	mov [ebp+env_type],eax
	movzx edx,BYTE PTR [edi+FMUSIC_INSTRUMENT.VOLnumpoints-FMUSIC_INSTRUMENT.PANPoints]
	lea eax,[esi+FMUSIC_CHANNEL.envvolstopped]
	add edi,FMUSIC_INSTRUMENT.VOLPoints-FMUSIC_INSTRUMENT.PANPoints
pan_or_vol_ok:
endif
	cmp BYTE PTR [eax],0
	jne goto_envelope_ret
	mov [ebp+envstopped],eax
	; Local vars initialized
	mov eax,[ecx]
	cmp eax,edx ; if(*pos>=numpoints) envelop out of bound
	jge envelope_done
	movzx eax,WORD PTR [edi+eax*4]
	cmp [ebx],eax ; if(*tick == points[(*pos)<<1]) we are at the correct tick for the position
	mov [ebp+env_tick],ebx
	jnz add_envdelta
	test BYTE PTR [ebp+env_type],FMUSIC_ENVELOPE_LOOP
	jz loop_ok
	movzx eax,BYTE PTR [ebp+sustain_l2]
	cmp [ecx],eax
	jnz loop_ok ; if((type&FMUSIC_ENVELOPE_LOOP) && *pos == loopend) handle loop
	movzx eax,BYTE PTR [ebp+sustain_l1]
	mov [ecx],eax ; *pos = loopstart
	movzx eax,WORD PTR [edi+eax*4]
	mov [ebx],eax ; *tick = points[(*pos)<<1]
loop_ok:
	mov eax,[ecx]
	mov [ebp+control],eax
	lea eax,[edi+eax*4]
	dec edx
	movzx ebx,WORD PTR [eax] ; get tick at this point
	cmp [ecx],edx
	mov edx,[eax+4]
	mov edi,edx
	movzx eax,WORD PTR [eax+2]
	mov [ebp+env_iptr],edx ; get tick at next point
	mov edx,[ebp+env_value]
	mov [edx],eax ; *value = points[(currpos<<1)+1]
	jne env_continue
	; if it is at the last position, abort the envelope and continue last value
	mov eax,[ebp+envstopped]
	inc BYTE PTR [eax] ; *envstopped = TRUE
goto_envelope_ret:
	jmp Envelope_Ret
env_continue:
	shl eax,16
	sub edi,eax
	xchg eax,edi
	xor ax,ax
	; sustain
	test BYTE PTR [ebp+env_type],FMUSIC_ENVELOPE_SUSTAIN
	jz not_sustain
	movzx edx,BYTE PTR [ebp+sustain_loop]
	cmp [ebp+control],edx
	jnz not_sustain
	cmp BYTE PTR [esi+FMUSIC_CHANNEL.keyoff],0
	jz Envelope_Ret
not_sustain:
	; interpolate 2 points to find delta step
	inc DWORD PTR [ecx] ; (*pos)++
	mov ecx,[ebp+valfrac]
	mov [ecx],edi ; *valfrac = curr
	mov edi,[ebp+envdelta]
	movzx ecx,WORD PTR [ebp+env_iptr]
	and DWORD PTR [edi],0 ; *envdelta = 0
	sub ecx,ebx
	jz envelope_done
	cdq
	idiv ecx
	mov [edi],eax ; *envdelta = (next-curr)/(nexttick-currtick)
	jmp envelope_done
add_envdelta:
	; interpolate
	mov eax,[ebp+envdelta]
	mov ecx,[eax]
	mov eax,[ebp+valfrac]
	add [eax],ecx ; *valfrac += *envdelta
envelope_done:
	mov eax,[ebp+valfrac]
	mov eax,[eax]
	mov ecx,[ebp+env_value]
	sar eax,16
	mov [ecx],eax ; *value = *valfrac >> 16
	mov eax,[ebp+env_tick]
	inc DWORD PTR [eax] ; (*tick)++
Envelope_Ret:
	pop ebx
	pop edi
	pop ebp
	ret 8
endif ; VOLUME_OR_PANENVELOPE

if VOLUMEBYTE_ON
VolByte:
; volume = EDX
; cptr   = ESI
	sub edx,10h
	jb switch_volume
	cmp edx,40h
	ja switch_volume
	; if(volume >= 0x10 && volume <= 0x50)
	mov [esi+FMUSIC_CHANNEL.volume],edx
switch_volume:
	mov ecx,edx
	and edx,0Fh
	shr ecx,4 ; switch(volume>>4)
	sub ecx,5
	jz case_6
	dec ecx
	jz case_7
	dec ecx
	jz case_6
	dec ecx
	jz case_7
	dec ecx
	jz case_A
	dec ecx
	jz case_B
	dec ecx
	jz case_C
	dec ecx
	jz case_D
	dec ecx
	jz case_E
	dec ecx
	jnz vol_default
	; case 0xF
	test edx,edx
	jz vol_z
	shl dl,4
	mov [esi+FMUSIC_CHANNEL.portaspeed],dl
vol_z:
	and BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],NOT_FMUSIC_TRIGGER
if PORTATO_OR_VOLSLIDE
	mov ecx,[esi+FMUSIC_CHANNEL.period]
	mov [esi+FMUSIC_CHANNEL.portatarget],ecx ; cptr->portatarget = cptr->period
endif
vol_default:
	ret
case_6: ; / case 8
	neg edx
case_7: ; / case 9
	add [esi+FMUSIC_CHANNEL.volume],edx
	ret
case_A:
	mov [esi+FMUSIC_CHANNEL.vibspeed],dl
	ret
case_B:
	mov [esi+FMUSIC_CHANNEL.vibdepth],dl
	ret
case_C:
	shl edx,4
	mov [esi+FMUSIC_CHANNEL.pan],edx
	jmp pan_break
case_D:
	neg edx
case_E:
	add [esi+FMUSIC_CHANNEL.pan],edx
pan_break:
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_PAN
	ret
endif ; VOLUMEBYTE_ON

if TREMOR_ON
Tremor:
; cptr = ESI
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_VOLUME
	mov dl,[esi+FMUSIC_CHANNEL.tremorpos]
	mov dh,[esi+FMUSIC_CHANNEL.tremoron]
	cmp dl,dh
	jbe inc_pos
	mov ecx,[esi+FMUSIC_CHANNEL.volume]
	neg ecx
	mov [esi+FMUSIC_CHANNEL.voldelta],ecx
inc_pos:
	inc dh
	inc dl
	add dh,[esi+FMUSIC_CHANNEL.tremoroff]
	cmp dl,dh
	jbe Tremor_Ret
	xor edx,edx
Tremor_Ret:
	mov [esi+FMUSIC_CHANNEL.tremorpos],dl
	ret
endif ; TREMOR_ON

; DESCRIPTION: Reset current channel
zcptr:
; cptr = ESI
; sptr = EAX
	push edi
	push 9
	movzx ecx,BYTE PTR [eax+FSOUND_SAMPLE.defvol]
	mov [esi+FMUSIC_CHANNEL.volume],ecx
	pop ecx
	movzx eax,BYTE PTR [eax+FSOUND_SAMPLE.defpan]
	mov [esi+FMUSIC_CHANNEL.pan],eax
	push 64
	xor eax,eax
	pop DWORD PTR [esi+FMUSIC_CHANNEL.envvol]
	push 32
	lea edi,[esi+FMUSIC_CHANNEL.envvoltick]
	pop DWORD PTR [esi+FMUSIC_CHANNEL.envpan]
	mov DWORD PTR [esi+FMUSIC_CHANNEL.fadeoutvol],65536
	rep stosd ; memset(&cptr->envvoltick,0,36)
	; Retrigger tremolo and vibrato waveforms
	mov cl,[esi+FMUSIC_CHANNEL.wavecontrol]
	pop edi
	cmp cl,4Fh
	jge z_tremolopos_ok
	mov BYTE PTR [esi+FMUSIC_CHANNEL.tremolopos],al ; <= 0
z_tremolopos_ok:
	and cl,0Ch
	jnz z_vibpos_ok
	mov BYTE PTR [esi+FMUSIC_CHANNEL.vibpos],al ; <= 0
z_vibpos_ok:
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_VOLUME_OR_PAN
	ret

SetBPM:
; bpm = ECX
	test ecx,ecx
	mov eax,FSOUND_MixRate*5/2
	jz SetBPM_Ret
	cdq
	div ecx
SetBPM_Ret:
	mov DWORD PTR [_mod+FMUSIC_MODULE.mixer_samplespertick],eax
	ret

DoNote:
; mod = ESI
donote_iptr     equ -4
donote_sptr     equ -8
donote_paramx   equ -10
donote_porta    equ -12
donote_oldpan   equ -16
donote_currtick equ -20
donote_oldfreq  equ -24
donote_jumpflag equ -28
donote_mod      equ -32
	; Point our note pointer to the correct pattern buffer, and to the
	; correct offset in this buffer indicated by row and number of channels
	mov eax,[esi+FMUSIC_MODULE.order]
	push ebp
	movzx ebx,BYTE PTR [eax+esi+FMUSIC_MODULE.orderlist]
	mov ebp,esp
	mov eax,[esi+FMUSIC_MODULE.row]
	lea ebx,[ecx+ebx*FMUSIC_PATTERN_size]
	mov ecx,[esi+FMUSIC_MODULE.numchannels]
	mul ecx
	sub esp,28
	lea edi,[eax+eax*4] ; x SIZE FMUSIC_NOTE
	push esi
	add edi,[ebx+FMUSIC_PATTERN.data] ; mod->pattern[mod->orderlist[mod->order]].data+(mod->row*mod->numchannels)
if PATTERNBREAK_ON
	if PATTERNJUMP_ON
		and DWORD PTR [ebp+donote_jumpflag],0
	endif
endif
	; Loop through each channel in the row
	shl ecx,7 ; x FMUSIC_CHANNEL_size
	jz donote_R
	push esi
	push ebx
	mov esi,OFFSET uFMOD_Ch
	add ecx,esi
donote_for_channels:
	push ecx
	mov al,[edi+FMUSIC_NOTE.eparam]
	mov ecx,eax
	and al,0Fh
	xchg eax,ebx  ; get effect param y
	mov al,[edi+FMUSIC_NOTE.effect]
	shr cl,4      ; get effect param x
	cmp al,FMUSIC_XM_PORTATO
	mov [ebp+donote_paramx],cl
	je donote_doporta
	cmp al,FMUSIC_XM_PORTATOVOLSLIDE
donote_doporta:
	setz [ebp+donote_porta]
	; First store note and instrument number if there was one
	jz donote_rem_note
	mov cl,[edi+FMUSIC_NOTE.number]
	dec cl
	js donote_rem_inst
	mov [esi+FMUSIC_CHANNEL.inst],cl ; remember the instrument #
donote_rem_inst:
	mov cl,[edi] ; get current note
	dec ecx
	cmp cl,96
	jae donote_rem_note
	mov [esi],cl ; remember the note
donote_rem_note:
	movzx ecx,BYTE PTR [esi+FMUSIC_CHANNEL.inst]
	mov eax,[ebp+donote_mod]
	imul ecx,FMUSIC_INSTRUMENT_size
	add ecx,[eax+FMUSIC_MODULE.instrument]
	movzx eax,BYTE PTR [esi] ; get note
	cdq
	movzx eax,BYTE PTR [eax+ecx+FMUSIC_INSTRUMENT.keymap]
	cmp al,16
	mov [ebp+donote_iptr],ecx
	jae donote_set_sptr
	mov edx,[ecx+eax*4+FMUSIC_INSTRUMENT.sample]
donote_set_sptr:
	test edx,edx
	jnz donote_valid_sptr
	mov edx,OFFSET DummySamp
donote_valid_sptr:
	mov [ebp+donote_sptr],edx
if NOTEDELAY_ON
	mov ecx,[esi+FMUSIC_CHANNEL.freq]
	mov eax,[esi+FMUSIC_CHANNEL.volume]
	mov [ebp+donote_oldfreq],ecx
	mov ecx,[esi+FMUSIC_CHANNEL.pan]
	mov [ebp+donote_currtick],eax
	mov [ebp+donote_oldpan],ecx
endif
if TREMOLO_ON
	; if there is no more tremolo, set volume to volume + last tremolo delta
	mov al,[edi+FMUSIC_NOTE.effect]
	cmp al,FMUSIC_XM_TREMOLO
	je donote_tremolo_vol
	cmp BYTE PTR [esi+FMUSIC_CHANNEL.recenteffect],FMUSIC_XM_TREMOLO
	jne donote_tremolo_vol
	mov ecx,[esi+FMUSIC_CHANNEL.voldelta]
	add [esi+FMUSIC_CHANNEL.volume],ecx
donote_tremolo_vol:
	mov [esi+FMUSIC_CHANNEL.recenteffect],al
endif
	and DWORD PTR [esi+FMUSIC_CHANNEL.voldelta],0
	and DWORD PTR [esi+FMUSIC_CHANNEL.freqdelta],0
	mov BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_VOLUME_OR_FREQ
	; PROCESS NOTE
	mov al,[edi] ; note
	test al,al
	je donote_note_ok
	cmp al,97
	jae donote_note_ok
	; get note according to relative note
	movzx ecx,BYTE PTR [edx+FSOUND_SAMPLE.relative]
	add cl,al
	mov eax,[ebp+donote_mod]
	dec ecx
	mov [esi+FMUSIC_CHANNEL.realnote],cl
	; Get period according to realnote and finetune
	test BYTE PTR [eax+FMUSIC_MODULE.flags],1
	je donote_flagsn1
	mov eax,[ebp+donote_sptr]
	movsx eax,BYTE PTR [eax+FSOUND_SAMPLE.finetune]
	cdq
	neg ecx
	sub eax,edx
	sar eax,1
	shl ecx,6
	sub ecx,eax
	add ecx,7680
	mov [esi+FMUSIC_CHANNEL.period],ecx
if AMIGAPERIODS_ON
	jmp donote_chk_porta
donote_flagsn1:
	push DWORD PTR [ebp+donote_sptr]
	xchg eax,ecx ; note = EAX
	call AmigaPeriod
	mov [esi+FMUSIC_CHANNEL.period],eax
donote_chk_porta:
else
donote_flagsn1:
endif
	; Frequency only changes if there are no portamento effects
	cmp BYTE PTR [ebp+donote_porta],0
	jne donote_freq_ok
	mov eax,[esi+FMUSIC_CHANNEL.period]
	mov [esi+FMUSIC_CHANNEL.freq],eax
donote_freq_ok:
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_TRIGGER
donote_note_ok:
	; PROCESS INSTRUMENT NUMBER
	cmp BYTE PTR [edi+FMUSIC_NOTE.number],0
	je donote_zcptr_ok
	mov eax,[ebp+donote_sptr]
	; zcptr:cptr = ESI
	; zcptr:sptr = EAX
	call zcptr
donote_zcptr_ok:
if VOLUMEBYTE_ON
	; PROCESS VOLUME BYTE
	movzx edx,BYTE PTR [edi+FMUSIC_NOTE.uvolume]
	; VolByte:volume = EDX
	; VolByte:cptr   = ESI
	call VolByte
endif
	; PROCESS KEY OFF
	cmp BYTE PTR [edi],97 ; note
	jae donote_set_keyoff
	cmp BYTE PTR [edi+FMUSIC_NOTE.effect],FMUSIC_XM_KEYOFF
	jne donote_keyoff_ok
donote_set_keyoff:
	inc BYTE PTR [esi+FMUSIC_CHANNEL.keyoff]
donote_keyoff_ok:
	; PROCESS ENVELOPES
if VOLUMEENVELOPE_ON
	mov ecx,[ebp+donote_iptr]
	test BYTE PTR [ecx+FMUSIC_INSTRUMENT.VOLtype],1
	jz donote_no_voltype
	push FMUSIC_VOLUME
	push ecx
	; Envelope:cptr = ESI
	call Envelope
	jmp donote_volenv_ok
donote_no_voltype:
endif
	cmp BYTE PTR [esi+FMUSIC_CHANNEL.keyoff],0
	je donote_volenv_ok
	and DWORD PTR [esi+FMUSIC_CHANNEL.envvol],0
donote_volenv_ok:
if PANENVELOPE_ON
	mov ecx,[ebp+donote_iptr]
	test BYTE PTR [ecx+FMUSIC_INSTRUMENT.PANtype],1
	je donote_no_pantype
	push FMUSIC_PAN
	push ecx
	; Envelope:cptr = ESI
	call Envelope
donote_no_pantype:
endif
	; PROCESS VOLUME FADEOUT
	cmp BYTE PTR [esi+FMUSIC_CHANNEL.keyoff],0
	mov ecx,[ebp+donote_iptr]
	je donote_fadevol_ok
	movzx eax,WORD PTR [ecx+FMUSIC_INSTRUMENT.VOLfade]
	sub [esi+FMUSIC_CHANNEL.fadeoutvol],eax
	jns donote_fadevol_ok
	and DWORD PTR [esi+FMUSIC_CHANNEL.fadeoutvol],0
donote_fadevol_ok:
	; PROCESS TICK 0 EFFECTS
	movzx eax,BYTE PTR [edi+FMUSIC_NOTE.effect]
	dec eax  ; skip FMUSIC_XM_ARPEGGIO
if PORTAUP_ON
else
	dec eax
endif
	mov dl,[edi+FMUSIC_NOTE.eparam]
if EXTRAFINEPORTA_ON
	cmp al,32
else
	if TREMOR_ON
		cmp al,28
	else
		if MULTIRETRIG_ON
			cmp al,26
		else
			if PANSLIDE_ON
				cmp al,24
			else
				if SETENVELOPEPOS_ON
					cmp al,20
				else
					if GLOBALVOLSLIDE_ON
						cmp al,16
					else
						if SETGLOBALVOLUME_ON
							cmp al,15
						else
							if SETSPEED_ON
								cmp al,14
							else
								cmp al,13
							endif
						endif
					endif
				endif
			endif
		endif
	endif
endif
if PORTAUP_ON
	ja donote_s1_brk
else
	jae donote_s1_brk
endif
	call DWORD PTR [eax*4+S1_TBL]
donote_s1_brk:
if INSTRUMENTVIBRATO_ON
	push DWORD PTR [ebp+donote_iptr]
endif
	push DWORD PTR [ebp+donote_sptr]
	; cptr in ESI
	call DoFlags
	sub esi,-(FMUSIC_CHANNEL_size)
	pop ecx
	add edi,FMUSIC_NOTE_size
	cmp esi,ecx
	jl donote_for_channels
	pop ebx
	pop esi
donote_R:
	leave
S1_r:
	ret
S1_TBL:
if PORTAUP_ON
	dd S1_C1
endif
if PORTADOWN_ON
	dd S1_C2
else
	dd S1_r
endif
if PORTATO_ON
	dd S1_C3
else
	dd S1_r
endif
if VIBRATO_ON
	dd S1_C4
else
	dd S1_r
endif
if PORTATOVOLSLIDE_ON
	dd S1_C5
else
	dd S1_r
endif
if VIBRATOVOLSLIDE_ON
	dd S1_C6
else
	dd S1_r
endif
if TREMOLO_ON
	dd S1_C7
else
	dd S1_r
endif
if SETPANPOSITION_ON
	dd S1_C8
else
	dd S1_r
endif
if SETSAMPLEOFFSET_ON
	dd S1_C9
else
	dd S1_r
endif
if VOLUMESLIDE_ON
	dd S1_C10
else
	dd S1_r
endif
if PATTERNJUMP_ON
	dd S1_C11
else
	dd S1_r
endif
if SETVOLUME_ON
	dd S1_C12
else
	dd S1_r
endif
if PATTERNBREAK_ON
	dd S1_C13
else
	dd S1_r
endif
	dd S1_C14
if EXTRAFINEPORTA_ON
	if SETSPEED_ON
		dd S1_C15
	else
		dd S1_r
	endif
	if SETGLOBALVOLUME_ON
		dd S1_C16
	else
		dd S1_r
	endif
	if GLOBALVOLSLIDE_ON
		dd S1_C17
	else
		dd S1_r
	endif
	dd S1_r ; unassigned effect ordinal [17]
	dd S1_r ; unassigned effect ordinal [18]
	dd S1_r ; skip FMUSIC_XM_KEYOFF
	if SETENVELOPEPOS_ON
		dd S1_C21
	else
		dd S1_r
	endif
	dd S1_r ; unassigned effect ordinal [22]
	dd S1_r ; unassigned effect ordinal [23]
	dd S1_r ; unassigned effect ordinal [24]
	if PANSLIDE_ON
		dd S1_C25
	else
		dd S1_r
	endif
	dd S1_r ; unassigned effect ordinal [26]
	if MULTIRETRIG_ON
		dd S1_C27
	else
		dd S1_r
	endif
	dd S1_r ; unassigned effect ordinal [28]
	if TREMOR_ON
		dd S1_C29
	else
		dd S1_r
	endif
	dd S1_r ; unassigned effect ordinal [30]
	dd S1_r ; unassigned effect ordinal [31]
	dd S1_r ; unassigned effect ordinal [32]
	dd S1_C33
else
	if TREMOR_ON
		if SETSPEED_ON
			dd S1_C15
		else
			dd S1_r
		endif
		if SETGLOBALVOLUME_ON
			dd S1_C16
		else
			dd S1_r
		endif
		if GLOBALVOLSLIDE_ON
			dd S1_C17
		else
			dd S1_r
		endif
		dd S1_r
		dd S1_r
		dd S1_r
		if SETENVELOPEPOS_ON
			dd S1_C21
		else
			dd S1_r
		endif
		dd S1_r
		dd S1_r
		dd S1_r
		if PANSLIDE_ON
			dd S1_C25
		else
			dd S1_r
		endif
		dd S1_r
		if MULTIRETRIG_ON
			dd S1_C27
		else
			dd S1_r
		endif
		dd S1_r
		dd S1_C29
	else
		if MULTIRETRIG_ON
			if SETSPEED_ON
				dd S1_C15
			else
				dd S1_r
			endif
			if SETGLOBALVOLUME_ON
				dd S1_C16
			else
				dd S1_r
			endif
			if GLOBALVOLSLIDE_ON
				dd S1_C17
			else
				dd S1_r
			endif
			dd S1_r
			dd S1_r
			dd S1_r
			if SETENVELOPEPOS_ON
				dd S1_C21
			else
				dd S1_r
			endif
			dd S1_r
			dd S1_r
			dd S1_r
			if PANSLIDE_ON
				dd S1_C25
			else
				dd S1_r
			endif
			dd S1_r
			dd S1_C27
		else
			if PANSLIDE_ON
				if SETSPEED_ON
					dd S1_C15
				else
					dd S1_r
				endif
				if SETGLOBALVOLUME_ON
					dd S1_C16
				else
					dd S1_r
				endif
				if GLOBALVOLSLIDE_ON
					dd S1_C17
				else
					dd S1_r
				endif
				dd S1_r
				dd S1_r
				dd S1_r
				if SETENVELOPEPOS_ON
					dd S1_C21
				else
					dd S1_r
				endif
				dd S1_r
				dd S1_r
				dd S1_r
				dd S1_C25
			else
				if SETENVELOPEPOS_ON
					if SETSPEED_ON
						dd S1_C15
					else
						dd S1_r
					endif
					if SETGLOBALVOLUME_ON
						dd S1_C16
					else
						dd S1_r
					endif
					if GLOBALVOLSLIDE_ON
						dd S1_C17
					else
						dd S1_r
					endif
					dd S1_r
					dd S1_r
					dd S1_r
					dd S1_C21
				else
					if GLOBALVOLSLIDE_ON
						if SETSPEED_ON
							dd S1_C15
						else
							dd S1_r
						endif
						if SETGLOBALVOLUME_ON
							dd S1_C16
						else
							dd S1_r
						endif
						dd S1_C17
					else
						if SETGLOBALVOLUME_ON
							if SETSPEED_ON
								dd S1_C15
							else
								dd S1_r
							endif
							dd S1_C16
						else
							if SETSPEED_ON
								dd S1_C15
							endif
						endif
					endif
				endif
			endif
		endif
	endif
endif
if PORTAUP_ON
S1_C1:
	test dl,dl
	je donote_xm_portaup_end
	mov [esi+FMUSIC_CHANNEL.portaup],dl
donote_xm_portaup_end:
	ret
endif
if PORTADOWN_ON
S1_C2:
	test dl,dl
	je donote_xm_portadown_end
	mov [esi+FMUSIC_CHANNEL.portadown],dl
donote_xm_portadown_end:
	ret
endif
if PORTATO_ON
S1_C3:
	test dl,dl
	je donote_xm_portato_end
	mov [esi+FMUSIC_CHANNEL.portaspeed],dl
donote_xm_portato_end:
if PORTATOVOLSLIDE_ON
	jmp donote_xm_portavolsl_end
else
	mov eax,[esi+FMUSIC_CHANNEL.period]
	and BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],NOT_FMUSIC_FREQ
	mov [esi+FMUSIC_CHANNEL.portatarget],eax
	ret
endif
endif
if PORTATOVOLSLIDE_ON
S1_C5:
	test dl,dl
	je donote_xm_portavolsl_end
	mov [esi+FMUSIC_CHANNEL.volslide],dl
donote_xm_portavolsl_end:
	mov eax,[esi+FMUSIC_CHANNEL.period]
	and BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],NOT_FMUSIC_FREQ
	mov [esi+FMUSIC_CHANNEL.portatarget],eax
	ret
endif
if VIBRATO_ON
S1_C4:
	mov al,[ebp+donote_paramx]
	test al,al
	je donote_vib_x_ok
	mov [esi+FMUSIC_CHANNEL.vibspeed],al
donote_vib_x_ok:
	test bl,bl
	je donote_vib_y_ok
	mov BYTE PTR [esi+FMUSIC_CHANNEL.vibdepth],bl
donote_vib_y_ok:
if VIBRATOVOLSLIDE_ON
	jmp donote_xm_vibvolsl_end
else
	; Vibrato:cptr = ESI
	call Vibrato
	ret
endif
endif
if VIBRATOVOLSLIDE_ON
S1_C6:
	test dl,dl
	je donote_xm_vibvolsl_end
	mov [esi+FMUSIC_CHANNEL.volslide],dl
donote_xm_vibvolsl_end:
	; Vibrato:cptr = ESI
	call Vibrato
	ret
endif
if TREMOLO_ON
S1_C7:
	mov al,[ebp+donote_paramx]
	test al,al
	jz donote_trem_x_ok
	mov [esi+FMUSIC_CHANNEL.tremolospeed],al
donote_trem_x_ok:
	test bl,bl
	je donote_trem_y_ok
	mov [esi+FMUSIC_CHANNEL.tremolodepth],bl
donote_trem_y_ok:
	ret
endif
if SETPANPOSITION_ON
S1_C8:
	movzx eax,dl
	mov [esi+FMUSIC_CHANNEL.pan],eax
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_PAN
	ret
endif
if SETSAMPLEOFFSET_ON
S1_C9:
	xor eax,eax
	mov ah,dl
	test eax,eax
	jz donote_soffset_ok
	mov [esi+FMUSIC_CHANNEL.sampleoffset],eax
donote_soffset_ok:
	mov ecx,[ebp+donote_sptr]
	mov edx,[ecx+FSOUND_SAMPLE.loopstart]
	add edx,[ecx+FSOUND_SAMPLE.looplen]
	mov eax,[esi+FMUSIC_CHANNEL.sampleoffset]
	cmp eax,edx
	jb donote_set_offset
	and BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],NOT_FMUSIC_TRIGGER
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_STOP
	ret
donote_set_offset:
	mov ecx,[esi+FMUSIC_CHANNEL.cptr]
	mov [ecx+FSOUND_CHANNEL.fsampleoffset],eax
	ret
endif
if VOLUMESLIDE_ON
S1_C10:
	test dl,dl
	je donote_volslide_ok
	mov [esi+FMUSIC_CHANNEL.volslide],dl
donote_volslide_ok:
	ret
endif
if PATTERNJUMP_ON
S1_C11:
	movzx ecx,dl
	mov eax,[ebp+donote_mod]
	and DWORD PTR [eax+FMUSIC_MODULE.nextrow],0
	mov [eax+FMUSIC_MODULE.nextorder],ecx
if PATTERNBREAK_ON
	inc DWORD PTR [ebp+donote_jumpflag]
donote_set_nextord:
endif
	movzx ecx,WORD PTR [eax+FMUSIC_MODULE.numorders]
	cmp [eax+FMUSIC_MODULE.nextorder],ecx
	jl donote_nextorder_ok
	and DWORD PTR [eax+FMUSIC_MODULE.nextorder],0
donote_nextorder_ok:
	ret
endif
if SETVOLUME_ON
S1_C12:
	movzx eax,dl
	mov [esi+FMUSIC_CHANNEL.volume],eax
	ret
endif
if PATTERNBREAK_ON
S1_C13:
	movzx eax,BYTE PTR [ebp+donote_paramx]
	movzx ecx,bl
	lea eax,[eax+eax*4]
	lea ecx,[ecx+eax*2] ; paramx*10+paramy
	mov eax,[ebp+donote_mod]
	mov [eax+FMUSIC_MODULE.nextrow],ecx
if PATTERNJUMP_ON
	cmp DWORD PTR [ebp+donote_jumpflag],0
	jne donote_nextorder_ok
endif
	mov ecx,[eax+FMUSIC_MODULE.order]
	inc ecx
	mov [eax+FMUSIC_MODULE.nextorder],ecx
if PATTERNJUMP_ON
	jmp donote_set_nextord
else
	movzx ecx,WORD PTR [eax+FMUSIC_MODULE.numorders]
	cmp [eax+FMUSIC_MODULE.nextorder],ecx
	jl donote_jump_pat:
	and DWORD PTR [eax+FMUSIC_MODULE.nextorder],0
donote_jump_pat:
	ret
endif
endif
if SETSPEED_ON
S1_C15:
	cmp dl,20h
	movzx ecx,dl
	jae donote_setbpm
	mov eax,[ebp+donote_mod]
	mov [eax+FMUSIC_MODULE.speed],ecx
	ret
donote_setbpm:
	; SetBPM:bpm = ECX
	call SetBPM
	ret
endif
if SETGLOBALVOLUME_ON
S1_C16:
	movzx eax,dl
	cmp eax,64
	mov ecx,[ebp+donote_mod]
	jle donote_setglobalvol
	push 64
	pop eax
donote_setglobalvol:
	mov [ecx+FMUSIC_MODULE.globalvolume],eax
	ret
endif
if GLOBALVOLSLIDE_ON
S1_C17:
	test dl,dl
	je donote_globalvsl_ok
	mov ecx,[ebp+donote_mod]
	mov [ecx+FMUSIC_MODULE.globalvsl],dl
donote_globalvsl_ok:
	ret
endif
if SETENVELOPEPOS_ON
S1_C21:
	test BYTE PTR [ecx+FMUSIC_INSTRUMENT.VOLtype],1
	jne donote_envelope_on
	ret
donote_envelope_on:
	mov ebx,ecx
	add ecx,FMUSIC_INSTRUMENT.VOLPoints+4
	movzx edx,dl
	; Search and reinterpolate new envelope position
	xor eax,eax
	cmp dx,[ecx]
	jbe donote_env_endwhile
donote_envwhile:
	cmp al,[ebx+FMUSIC_INSTRUMENT.VOLnumpoints] ; if(currpos < iptr->VOLnumpoints) break
	jge donote_env_endwhile
	add ecx,4
	inc eax
	cmp dx,[ecx] ; if(current->eparam > iptr->VOLPoints[(currpos+1)<<1]) break
	ja donote_envwhile
donote_env_endwhile:
	mov [esi+FMUSIC_CHANNEL.envvolpos],eax
	; if it is at the last position, abort the envelope and continue last volume
	movzx ecx,BYTE PTR [ebx+FMUSIC_INSTRUMENT.VOLnumpoints]
	lea edx,[ecx-1]
	cmp eax,edx
	jl donote_env_continue
	movzx eax,WORD PTR [ebx+ecx*4+FMUSIC_INSTRUMENT.VOLPoints-2]
	inc BYTE PTR [esi+FMUSIC_CHANNEL.envvolstopped]
	mov [esi+FMUSIC_CHANNEL.envvol],eax ; cptr->envvol <= iptr->VOLPoints[((iptr->VOLnumpoints-1)<<1)+1]
	ret
donote_env_continue:
	mov BYTE PTR [esi+FMUSIC_CHANNEL.envvolstopped],0
	movzx ecx,BYTE PTR [edi+FMUSIC_NOTE.eparam]
	mov [esi+FMUSIC_CHANNEL.envvoltick],ecx
	movzx ecx,WORD PTR [ebx+eax*4+FMUSIC_INSTRUMENT.VOLPoints]   ; get tick at this point
	mov [ebp+donote_currtick],ecx
	movzx ecx,WORD PTR [ebx+eax*4+FMUSIC_INSTRUMENT.VOLPoints+2] ; get tick at next point
	lea edx,[eax+1]
	movzx eax,WORD PTR [ebx+edx*4+FMUSIC_INSTRUMENT.VOLPoints]   ; get VOL at this point
	shl ecx,16
	; interpolate 2 points to find delta step
	sub eax,[ebp+donote_currtick]
	jz donote_no_tickdiff
	movzx edx,WORD PTR [ebx+edx*4+FMUSIC_INSTRUMENT.VOLPoints+2] ; get VOL at next point
	shl edx,16
	push ecx
	sub edx,ecx
	xchg eax,ecx
	xchg eax,edx
	cdq
	idiv ecx
	pop ecx
donote_no_tickdiff:
	mov [esi+FMUSIC_CHANNEL.envvoldelta],eax
	mov eax,[esi+FMUSIC_CHANNEL.envvoltick]
	sub eax,[ebp+donote_currtick]
	imul eax,DWORD PTR [esi+FMUSIC_CHANNEL.envvoldelta]
	add ecx,eax
	mov [esi+FMUSIC_CHANNEL.envvolfrac],ecx
	sar ecx,16
	mov [esi+FMUSIC_CHANNEL.envvol],ecx
	inc DWORD PTR [esi+FMUSIC_CHANNEL.envvolpos]
	ret
endif
if PANSLIDE_ON
S1_C25:
	test dl,dl
	jz donote_panslide_ok
	mov [esi+FMUSIC_CHANNEL.panslide],dl
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_PAN
donote_panslide_ok:
	ret
endif
if MULTIRETRIG_ON
S1_C27:
	test dl,dl
	jz donote_multiretrig_ok
	mov al,[ebp+donote_paramx]
	mov [esi+FMUSIC_CHANNEL.retrigy],bl
	mov [esi+FMUSIC_CHANNEL.retrigx],al
donote_multiretrig_ok:
	ret
endif
if TREMOR_ON
S1_C29:
	test dl,dl
	jz donote_do_tremor
	mov al,[ebp+donote_paramx]
	mov [esi+FMUSIC_CHANNEL.tremoroff],bl
	mov [esi+FMUSIC_CHANNEL.tremoron],al
donote_do_tremor:
	; Tremos:cptr = ESI
	call Tremor
	ret
endif
if EXTRAFINEPORTA_ON
S1_C33:
	cmp BYTE PTR [ebp+donote_paramx],1
	jne donote_paramx_n1
	test bl,bl
	jz donote_paramy_z1
	mov [esi+FMUSIC_CHANNEL.xtraportaup],bl
donote_paramy_z1:
	movzx eax,BYTE PTR [esi+FMUSIC_CHANNEL.xtraportaup]
	sub [esi+FMUSIC_CHANNEL.freq],eax
donote_paramx_n1:
	cmp BYTE PTR [ebp+donote_paramx],2
	jne donote_paramx_n2
	test bl,bl
	jz donote_paramy_z2
	mov [esi+FMUSIC_CHANNEL.xtraportadown],bl
donote_paramy_z2:
	movzx eax,BYTE PTR [esi+FMUSIC_CHANNEL.xtraportadown]
	add [esi+FMUSIC_CHANNEL.freq],eax
donote_paramx_n2:
endif
S2_r:
	ret
S1_C14:
	movzx eax,BYTE PTR [ebp+donote_paramx]
	dec eax ; skip FMUSIC_XM_SETFILTER
if FINEPORTAUP_ON
else
	dec eax
endif
if PATTERNDELAY_ON
	cmp al,13
else
	if NOTEDELAY_ON
		cmp al,12
	else
		if FINEVOLUMESLIDE_ON
			cmp al,10
		else
			if SETPANPOSITION16_ON
				cmp al,7
			else
				if SETTREMOLOWAVE_ON
					cmp al,6
				else
					if PATTERNLOOP_ON
						cmp al,5
					else
						cmp al,4
					endif
				endif
			endif
		endif
	endif
endif
if FINEPORTAUP_ON
	ja S2_r
else
	jae S2_r
endif
donote_do_special:
	jmp DWORD PTR [eax*4+S2_TBL]
S2_TBL:
if FINEPORTAUP_ON
	dd S2_C1
endif
if FINEPORTADOWN_ON
	dd S2_C2
else
	dd S2_r
endif
	dd S2_r ; skip FMUSIC_XM_SETGLISSANDO
if SETVIBRATOWAVE_ON
	dd S2_C4
else
	dd S2_r
endif
if SETFINETUNE_ON
	dd S2_C5
else
	dd S2_r
endif
if PATTERNDELAY_ON
	if PATTERNLOOP_ON
		dd S2_C6
	else
		dd S2_r
	endif
	if SETTREMOLOWAVE_ON
		dd S2_C7
	else
		dd S2_r
	endif
	if SETPANPOSITION16_ON
		dd S2_C8
	else
		dd S2_r
	endif
	dd S2_r ; skip FMUSIC_XM_RETRIG
	if FINEVOLUMESLIDE_ON
		dd S2_C10
		dd S2_C11
	else
		dd S2_r
		dd S2_r
	endif
	dd S2_r ; skip FMUSIC_XM_NOTECUT
	if NOTEDELAY_ON
		dd S2_C13
	else
		dd S2_r
	endif
	dd S2_C14
else
	if NOTEDELAY_ON
		if PATTERNLOOP_ON
			dd S2_C6
		else
			dd S2_r
		endif
		if SETTREMOLOWAVE_ON
			dd S2_C7
		else
			dd S2_r
		endif
		if SETPANPOSITION16_ON
			dd S2_C8
		else
			dd S2_r
		endif
		dd S2_r ; skip FMUSIC_XM_RETRIG
		if FINEVOLUMESLIDE_ON
			dd S2_C10
			dd S2_C11
		else
			dd S2_r
			dd S2_r
		endif
		dd S2_r
		dd S2_C13
	else
		if FINEVOLUMESLIDE_ON
			if PATTERNLOOP_ON
				dd S2_C6
			else
				dd S2_r
			endif
			if SETTREMOLOWAVE_ON
				dd S2_C7
			else
				dd S2_r
			endif
			if SETPANPOSITION16_ON
				dd S2_C8
			else
				dd S2_r
			endif
			dd S2_r
			dd S2_C10
			dd S2_C11
		else
			if SETPANPOSITION16_ON
				if PATTERNLOOP_ON
					dd S2_C6
				else
					dd S2_r
				endif
				if SETTREMOLOWAVE_ON
					dd S2_C7
				else
					dd S2_r
				endif
				dd S2_C8
			else
				if SETTREMOLOWAVE_ON
					if PATTERNLOOP_ON
						dd S2_C6
					else
						dd S2_r
					endif
					dd S2_C7
				else
					if PATTERNLOOP_ON
						dd S2_C6
					endif
				endif
			endif
		endif
	endif
endif
if FINEPORTAUP_ON
S2_C1:
	test bl,bl
	je donote_finepup_ok
	mov [esi+FMUSIC_CHANNEL.fineportaup],bl
donote_finepup_ok:
	movzx eax,BYTE PTR [esi+FMUSIC_CHANNEL.fineportaup]
	shl eax,2
	sub [esi+FMUSIC_CHANNEL.freq],eax
	ret
endif
if FINEPORTADOWN_ON
S2_C2:
	test bl,bl
	je donote_finepdown_ok
	mov [esi+FMUSIC_CHANNEL.fineportadown],bl
donote_finepdown_ok:
	movzx eax,BYTE PTR [esi+FMUSIC_CHANNEL.fineportadown]
	shl eax,2
	add [esi+FMUSIC_CHANNEL.freq],eax
	ret
endif
if SETVIBRATOWAVE_ON
S2_C4:
	and BYTE PTR [esi+FMUSIC_CHANNEL.wavecontrol],0F0h
	or BYTE PTR [esi+FMUSIC_CHANNEL.wavecontrol],bl
	ret
endif
if SETFINETUNE_ON
S2_C5:
	mov eax,[ebp+donote_sptr]
	mov [eax+FSOUND_SAMPLE.finetune],bl
	ret
endif
if PATTERNLOOP_ON
S2_C6:
	test bl,bl
	jne donote_not_paramy
	mov eax,[ebp+donote_mod]
	mov eax,[eax+FMUSIC_MODULE.row]
	mov [esi+FMUSIC_CHANNEL.patlooprow],eax
	ret
donote_not_paramy:
	mov ecx,[esi+FMUSIC_CHANNEL.patloopno]
	test ecx,ecx
	jnz donote_patloopno_ok
	movzx ecx,bl
	inc ecx
donote_patloopno_ok:
	dec ecx
	mov [esi+FMUSIC_CHANNEL.patloopno],ecx
	jz donote_patloopno_end
	mov eax,[esi+FMUSIC_CHANNEL.patlooprow]
	mov ecx,[ebp+donote_mod]
	mov [ecx+FMUSIC_MODULE.nextrow],eax
donote_patloopno_end:
	ret
endif
if SETTREMOLOWAVE_ON
S2_C7:
	and BYTE PTR [esi+FMUSIC_CHANNEL.wavecontrol],0Fh
	shl bl,4
	or BYTE PTR [esi+FMUSIC_CHANNEL.wavecontrol],bl
	ret
endif
if SETPANPOSITION16_ON
S2_C8:
	movzx eax,bl
	shl eax,4
	mov [esi+FMUSIC_CHANNEL.pan],eax
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_PAN
	ret
endif
if FINEVOLUMESLIDE_ON
S2_C10:
	neg bl
S2_C11:
	test bl,bl
	jz donote_finevols_ok
	mov [esi+FMUSIC_CHANNEL.finevslup],bl
donote_finevols_ok:
	movsx eax,BYTE PTR [esi+FMUSIC_CHANNEL.finevslup]
	sub [esi+FMUSIC_CHANNEL.volume],eax
	ret
endif
if NOTEDELAY_ON
S2_C13:
	mov eax,[ebp+donote_currtick]
	mov [esi+FMUSIC_CHANNEL.volume],eax
	mov eax,[ebp+donote_oldfreq]
	mov [esi+FMUSIC_CHANNEL.freq],eax
	mov eax,[ebp+donote_oldpan]
	mov [esi+FMUSIC_CHANNEL.pan],eax
	mov BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],0
	ret
endif
if PATTERNDELAY_ON
S2_C14:
	movzx eax,bl
	mov ecx,[ebp+donote_mod]
	imul DWORD PTR [ecx+FMUSIC_MODULE.speed]
	mov [ecx+FMUSIC_MODULE.patterndelay],eax
	ret
endif

DoEffs:
; mod = ESI
doeff_mod     equ -4
doeff_current equ -8
	; Point our note pointer to the correct pattern buffer, and to the
	; correct offset in this buffer indicated by row and number of channels
	mov eax,[esi+FMUSIC_MODULE.order]
	push ebp
	mov bl,[eax+esi+FMUSIC_MODULE.orderlist]
	mov eax,[esi+FMUSIC_MODULE.row]
	mul DWORD PTR [esi+FMUSIC_MODULE.numchannels]
	lea eax,[eax+eax*4] ; x SIZE FMUSIC_NOTE
	mov ebp,esp
	add eax,[ecx+ebx*FMUSIC_PATTERN_size+FMUSIC_PATTERN.data] ; mod->pattern[mod->orderlist[mod->order]].data+(mod->row*mod->numchannels)
	push esi
	push eax
	mov eax,[esi+FMUSIC_MODULE.numchannels]
	shl eax,7 ; x FMUSIC_CHANNEL_size
	push esi
	push ebx
	mov esi,OFFSET uFMOD_Ch
	add eax,esi
doeff_for_channels:
	push eax
	mov edx,[ebp+doeff_mod]
	movzx edi,BYTE PTR [esi+FMUSIC_CHANNEL.inst]
	imul edi,FMUSIC_INSTRUMENT_size
	add edi,[edx+FMUSIC_MODULE.instrument]
	movzx eax,BYTE PTR [esi] ; note
	cdq
	mov al,[eax+edi+FMUSIC_INSTRUMENT.keymap]
	cmp al,16
	jae doeff_set_sptr
	mov edx,[edi+eax*4+FMUSIC_INSTRUMENT.sample]
doeff_set_sptr:
	test edx,edx
	jnz doeff_valid_sptr
	mov edx,OFFSET DummySamp
doeff_valid_sptr:
if INSTRUMENTVIBRATO_ON
	push edi ; iptr
endif
	push edx ; sptr
	xor ebx,ebx
	mov [esi+FMUSIC_CHANNEL.voldelta],ebx
	mov [esi+FMUSIC_CHANNEL.freqdelta],ebx
	mov [esi+FMUSIC_CHANNEL.notectrl],bl
	; PROCESS ENVELOPES
if VOLUMEENVELOPE_ON
	test BYTE PTR [edi+FMUSIC_INSTRUMENT.VOLtype],1
	je doeff_no_voltype
	push FMUSIC_VOLUME
	push edi
	; Envelope:cptr = ESI
	call Envelope
doeff_no_voltype:
endif
if PANENVELOPE_ON
	test BYTE PTR [edi+FMUSIC_INSTRUMENT.PANtype],1
	je doeff_no_pantype
	push FMUSIC_PAN
	push edi
	; Envelope:cptr = ESI
	call Envelope
doeff_no_pantype:
endif
	; PROCESS VOLUME FADEOUT
	cmp [esi+FMUSIC_CHANNEL.keyoff],bl
	je doeff_fadevol_ok
	movzx eax,WORD PTR [edi+FMUSIC_INSTRUMENT.VOLfade]
	sub [esi+FMUSIC_CHANNEL.fadeoutvol],eax
	jns doeff_fadevol_ns
	mov [esi+FMUSIC_CHANNEL.fadeoutvol],ebx
doeff_fadevol_ns:
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_VOLUME
doeff_fadevol_ok:
if VOLUMEBYTE_ON
	mov eax,[ebp+doeff_current]
	movzx eax,BYTE PTR [eax+FMUSIC_NOTE.uvolume]
	mov ecx,eax
	shr eax,4
	and cl,0Fh
	sub al,6
	jz doeff_case_vol6
	dec eax
	jz doeff_case_vol7
if VIBRATO_ON
	sub al,4
	jz doeff_case_volB
	dec eax
	dec eax
else
	sub al,6
endif
	jz doeff_case_volD
	dec eax
	jz doeff_case_volE
if PORTATO_ON
	dec eax
	jnz doeff_volbyte_end
	; Portamento:cptr = ESI
	call Portamento
endif
	jmp doeff_volbyte_end
doeff_case_volD:
	neg ecx
doeff_case_volE:
	add [esi+FMUSIC_CHANNEL.pan],ecx
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_PAN
	jmp doeff_volbyte_end
if VIBRATO_ON
doeff_case_volB:
	mov [esi+FMUSIC_CHANNEL.vibdepth],cl
	; Vibrato:cptr = ESI
	call Vibrato
	mov al,[esi+FMUSIC_CHANNEL.vibpos]
	add al,[esi+FMUSIC_CHANNEL.vibspeed]
	cmp al,31
	jle doeff_set_vibpos
	sub al,64
doeff_set_vibpos:
	mov [esi+FMUSIC_CHANNEL.vibpos],al
	jmp doeff_volbyte_end
endif
doeff_case_vol6:
	neg ecx
doeff_case_vol7:
	add [esi+FMUSIC_CHANNEL.volume],ecx
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_VOLUME
doeff_volbyte_end:
endif ; VOLUMEBYTE_ON
	mov edx,[ebp+doeff_current]
	mov bl,[edx+FMUSIC_NOTE.eparam]
	mov cl,bl
	and bl,0Fh ; grab the effect parameter y
	movzx eax,BYTE PTR [edx+FMUSIC_NOTE.effect]
	shr cl,4   ; grab the effect parameter x
if ARPEGGIO_ON
else
	dec eax
endif
if TREMOR_ON
	cmp al,29
else
	if MULTIRETRIG_ON
		cmp al,27
	else
		if PANSLIDE_ON
			cmp al,25
		else
			if GLOBALVOLSLIDE_ON
				cmp al,17
			else
				if RETRIG_ON
					cmp al,14
				else
					if NOTECUT_ON
						cmp al,14
					else
						if NOTEDELAY_ON
							cmp al,14
						else
							cmp al,10
						endif
					endif
				endif
			endif
		endif
	endif
endif
if ARPEGGIO_ON
	ja doeff_s3_brk
else
	jae doeff_s3_brk
endif
	call DWORD PTR [eax*4+S3_TBL]
doeff_s3_brk:
	; cptr in ESI
	call DoFlags
	sub esi,-(FMUSIC_CHANNEL_size)
	pop eax
	add DWORD PTR [ebp+doeff_current],FMUSIC_NOTE_size
	cmp esi,eax
	jl doeff_for_channels
	pop ebx
	pop esi
doeff_R:
	leave
S3_r:
	ret
S3_TBL:
if ARPEGGIO_ON
	dd S3_C0
endif
if PORTAUP_ON
	dd S3_C1
else
	dd S3_r
endif
if PORTADOWN_ON
	dd S3_C2
else
	dd S3_r
endif
if PORTATO_ON
	; Portamento:cptr = ESI
	dd Portamento
else
	dd S3_r
endif
if VIBRATO_ON
	dd S3_C4
else
	dd S3_r
endif
if PORTATOVOLSLIDE_ON
	dd S3_C5
else
	dd S3_r
endif
if VIBRATOVOLSLIDE_ON
	dd S3_C6
else
	dd S3_r
endif
if TREMOLO_ON
	; Tremolo:cptr = ESI
	dd Tremolo
else
	dd S3_r
endif
	dd S3_r ; skip FMUSIC_XM_SETPANPOSITION
	dd S3_r ; skip FMUSIC_XM_SETSAMPLEOFFSET
if VOLUMESLIDE_ON
	dd S3_C10
else
	dd S3_r
endif
if TREMOR_ON
	dd S3_r ; skip FMUSIC_XM_PATTERNJUMP
	dd S3_r ; slip FMUSIC_XM_SETVOLUME
	dd S3_r ; skip FMUSIC_XM_PATTERNBREAK
	dd S3_C14
	dd S3_r ; skip FMUSIC_XM_SETSPEED
	dd S3_r ; skip FMUSIC_XM_SETGLOBALVOLUME
	if GLOBALVOLSLIDE_ON
		dd S3_C17
	else
		dd S3_r
	endif
	dd S3_r ; unassigned effect ordinal [18]
	dd S3_r ; unassigned effect ordinal [19]
	dd S3_r ; skip FMUSIC_XM_KEYOFF
	dd S3_r ; skip FMUSIC_XM_SETENVELOPEPOS
	dd S3_r ; unassigned effect ordinal [22]
	dd S3_r ; unassigned effect ordinal [23]
	dd S3_r ; unassigned effect ordinal [24]
	if PANSLIDE_ON
		dd S3_C25
	else
		dd S3_r
	endif
	dd S3_r ; unassigned effect ordinal [26]
	if MULTIRETRIG_ON
		dd S3_C27
	else
		dd S3_r
	endif
	dd S3_r ; unassigned effect ordinal [28]
	; case FMUSIC_XM_TREMOR
	; Tremos:cptr = ESI
	dd Tremor
else
	if MULTIRETRIG_ON
		dd S3_r
		dd S3_r
		dd S3_r
		dd S3_C14
		dd S3_r
		dd S3_r
		if GLOBALVOLSLIDE_ON
			dd S3_C17
		else
			dd S3_r
		endif
		dd S3_r
		dd S3_r
		dd S3_r
		dd S3_r
		dd S3_r
		dd S3_r
		dd S3_r
		if PANSLIDE_ON
			dd S3_C25
		else
			dd S3_r
		endif
		dd S3_r
		dd S3_C27
	else
		if PANSLIDE_ON
			dd S3_r
			dd S3_r
			dd S3_r
			dd S3_C14
			dd S3_r
			dd S3_r
			if GLOBALVOLSLIDE_ON
				dd S3_C17
			else
				dd S3_r
			endif
			dd S3_r
			dd S3_r
			dd S3_r
			dd S3_r
			dd S3_r
			dd S3_r
			dd S3_r
			dd S3_C25
		else
			if GLOBALVOLSLIDE_ON
				dd S3_r
				dd S3_r
				dd S3_r
				dd S3_C14
				dd S3_r
				dd S3_r
				dd S3_C17
			else
				if RETRIG_ON
					dd S3_r
					dd S3_r
					dd S3_r
					dd S3_C14
				else
					if NOTECUT_ON
						dd S3_r
						dd S3_r
						dd S3_r
						dd S3_C14
					else
						if NOTEDELAY_ON
							dd S3_r
							dd S3_r
							dd S3_r
							dd S3_C14
						endif
					endif
				endif
			endif
		endif

	endif
endif
if VIBRATOVOLSLIDE_ON
S3_C6:
	; Vibrato:cptr = ESI
	call Vibrato
	mov al,[esi+FMUSIC_CHANNEL.vibpos]
	add al,[esi+FMUSIC_CHANNEL.vibspeed]
	cmp al,31
	jle doeff_vibvolslide_ok
	sub al,64
doeff_vibvolslide_ok:
	mov [esi+FMUSIC_CHANNEL.vibpos],al
if VOLUMESLIDE_ON
	if PORTATOVOLSLIDE_ON
		jmp S3_C10
	endif
else
	movzx eax,BYTE PTR [esi+FMUSIC_CHANNEL.volslide]
	mov ecx,eax
	and eax,0Fh
	shr ecx,4
	jz doeff_volslide_ok
	; Slide up takes precedence over down
	xchg eax,ecx
	neg eax
doeff_volslide_ok:
	sub [esi+FMUSIC_CHANNEL.volume],eax
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_VOLUME
	ret
endif
endif
if PORTATOVOLSLIDE_ON
S3_C5:
	; Portamento:cptr = ESI
	call Portamento
if VOLUMESLIDE_ON
else
	movzx eax,BYTE PTR [esi+FMUSIC_CHANNEL.volslide]
	mov ecx,eax
	and eax,0Fh
	shr ecx,4
	jz doeff_volslide_ok
	; Slide up takes precedence over down
	xchg eax,ecx
	neg eax
doeff_volslide_ok:
	sub [esi+FMUSIC_CHANNEL.volume],eax
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_VOLUME
	ret
endif
endif
if VOLUMESLIDE_ON
S3_C10:
	movzx eax,BYTE PTR [esi+FMUSIC_CHANNEL.volslide]
	mov ecx,eax
	and al,0Fh
	shr ecx,4
	jz doeff_volslide_ok
	; Slide up takes precedence over down
	xchg eax,ecx
	neg eax
doeff_volslide_ok:
	sub [esi+FMUSIC_CHANNEL.volume],eax
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_VOLUME
	ret
endif
if VIBRATO_ON
S3_C4:
	; Vibrato:cptr = ESI
	call Vibrato
	mov al,[esi+FMUSIC_CHANNEL.vibpos]
	add al,[esi+FMUSIC_CHANNEL.vibspeed]
	cmp al,31
	jle doeff_vibrato_ok
	sub al,64
doeff_vibrato_ok:
	mov [esi+FMUSIC_CHANNEL.vibpos],al
	ret
endif
if PORTADOWN_ON
S3_C2:
	movzx eax,BYTE PTR [esi+FMUSIC_CHANNEL.portadown]
	shl eax,2
	add [esi+FMUSIC_CHANNEL.freq],eax
doeff_portadown_ok:
	and DWORD PTR [esi+FMUSIC_CHANNEL.freqdelta],0
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_FREQ
	ret
endif
if PORTAUP_ON
S3_C1:
	movzx eax,BYTE PTR [esi+FMUSIC_CHANNEL.portaup]
	shl eax,2
	sub [esi+FMUSIC_CHANNEL.freq],eax
	cmp DWORD PTR [esi+FMUSIC_CHANNEL.freq],56
if PORTADOWN_ON
	jge doeff_portadown_ok
else
	jge doeff_portaup_ok
endif
	push 56
	pop DWORD PTR [esi+FMUSIC_CHANNEL.freq]
if PORTADOWN_ON
	jmp doeff_portadown_ok
else
doeff_portaup_ok:
	and DWORD PTR [esi+FMUSIC_CHANNEL.freqdelta],0
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_FREQ
	ret
endif
endif
if ARPEGGIO_ON
S3_C0:
	cmp BYTE PTR [edx+FMUSIC_NOTE.eparam],0
	jbe doeff_arpeggio_ok
	mov eax,[ebp+doeff_mod]
	mov eax,[eax+FMUSIC_MODULE.tick]
	push 3
	cdq
	pop edi
	idiv edi
	dec edx
	jz doeff_arpeg_c1
	dec edx
	jne doeffs_enable_freq
	mov eax,[ebp+doeff_mod]
	test BYTE PTR [eax+FMUSIC_MODULE.flags],1
	mov cl,bl
	je doeffs_flagsn1
doeffs_arpeggio_freqd:
	movzx eax,cl
	shl eax,6
	mov [esi+FMUSIC_CHANNEL.freqdelta],eax
	jmp doeffs_enable_freq
doeff_arpeg_c1:
	mov eax,[ebp+doeff_mod]
	test BYTE PTR [eax+FMUSIC_MODULE.flags],1
	jne doeffs_arpeggio_freqd
doeffs_flagsn1:
if AMIGAPERIODS_ON
	movzx ecx,cl
	movzx eax,BYTE PTR [esi+FMUSIC_CHANNEL.realnote]
	mov edx,[esp+4] ; sptr
	mov edi,eax
	push edx
	push edx
	add eax,ecx ; note = EAX
	call AmigaPeriod
	xchg eax,edi
	call AmigaPeriod
	sub edi,eax
	mov [esi+FMUSIC_CHANNEL.freqdelta],edi
endif
doeffs_enable_freq:
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_FREQ
doeff_arpeggio_ok:
	ret
endif
if MULTIRETRIG_ON
S3_C27:
	movzx ecx,BYTE PTR [esi+FMUSIC_CHANNEL.retrigy]
	test ecx,ecx
	jz doeff_multiretrig_ok
	mov eax,[ebp+doeff_mod]
	mov eax,[eax+FMUSIC_MODULE.tick]
	cdq
	idiv ecx
	test edx,edx
	jnz doeff_multiretrig_ok
	movzx ecx,BYTE PTR [esi+FMUSIC_CHANNEL.retrigx]
	and cl,0Fh
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_TRIGGER
	dec ecx
	mov eax,[esi+FMUSIC_CHANNEL.volume]
	js doeff_multiretrig_ok
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_VOLUME
	call DWORD PTR [ecx*4+S4_TBL]
	mov [esi+FMUSIC_CHANNEL.volume],eax
doeff_multiretrig_ok:
	ret
S4_C2:
	dec eax
S4_C1:
	dec eax
	ret
S4_C5:
	sub eax,8
S4_C4:
	sub eax,4
S4_C3:
	sub eax,4
	ret
S4_C6:
	shl eax,1
	push 3
	cdq
	pop ecx
	idiv ecx
	ret
S4_C14:
	lea eax,[eax+eax*2]
S4_C7:
	sar eax,1
	ret
S4_C10:
	inc eax
S4_C9:
	inc eax
	ret
S4_C13:
	add eax,8
S4_C12:
	add eax,4
S4_C11:
	add eax,4
	ret
S4_C15:
	shl eax,1
S4_r:
	ret
S4_TBL:
	dd S4_C1
	dd S4_C2
	dd S4_C3
	dd S4_C4
	dd S4_C5
	dd S4_C6
	dd S4_C7
	dd S4_r
	dd S4_C9
	dd S4_C10
	dd S4_C11
	dd S4_C12
	dd S4_C13
	dd S4_C14
	dd S4_C15
endif
if PANSLIDE_ON
S3_C25:
	movzx eax,BYTE PTR [esi+FMUSIC_CHANNEL.panslide]
	mov ecx,eax
	and al,0Fh
	shr ecx,4
	; Slide right takes precedence over left
	jz doeff_pan_slide_left
	cdq
	add [esi+FMUSIC_CHANNEL.pan],ecx
	dec dl ; edx <= 255
	cmp [esi+FMUSIC_CHANNEL.pan],edx
	jle doeff_panslide_ok
	mov [esi+FMUSIC_CHANNEL.pan],edx
	jmp doeff_panslide_ok
doeff_pan_slide_left:
	sub [esi+FMUSIC_CHANNEL.pan],eax
	jns doeff_panslide_ok
	mov [esi+FMUSIC_CHANNEL.pan],ecx
doeff_panslide_ok:
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_PAN
	ret
endif
if GLOBALVOLSLIDE_ON
S3_C17:
	mov edx,[ebp+doeff_mod]
	movzx eax,BYTE PTR [edx+FMUSIC_MODULE.globalvsl]
	mov ecx,eax
	and al,0Fh
	shr ecx,4
	; Slide up takes precedence over down
	jz doeff_gvsl_slide_down
	add [edx+FMUSIC_MODULE.globalvolume],ecx
	cmp DWORD PTR [edx+FMUSIC_MODULE.globalvolume],64
	jle doeff_gvsl_ok
	push 64
	pop DWORD PTR [edx+FMUSIC_MODULE.globalvolume]
	ret
doeff_gvsl_slide_down:
	sub [edx+FMUSIC_MODULE.globalvolume],eax
	jns doeff_gvsl_ok
	and DWORD PTR [edx+FMUSIC_MODULE.globalvolume],0
doeff_gvsl_ok:
	ret
endif
S3_C14:
if RETRIG_ON
	cmp cl,FMUSIC_XM_RETRIG
	je doeff_do_retrig
endif
if NOTECUT_ON
	sub cl,FMUSIC_XM_NOTECUT
	jz doeff_do_notecut
endif
if NOTEDELAY_ON
	if NOTECUT_ON
		dec cl
	else
		cmp cl,FMUSIC_XM_NOTEDELAY
	endif
	jne doeff_special_r
	mov ecx,[ebp+doeff_mod]
	movzx eax,bl
	cmp [ecx+FMUSIC_MODULE.tick],eax
	jne doeff_notectrl_z
	; PROCESS INSTRUMENT NUMBER
	mov eax,[esp+4] ; sptr
	; zcptr:cptr = ESI
	; zcptr:sptr = EAX
	call zcptr
	mov eax,[esi+FMUSIC_CHANNEL.period]
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_FREQ_OR_TRIGGER
	mov [esi+FMUSIC_CHANNEL.freq],eax
if VOLUMEBYTE_ON
	mov eax,[ebp+doeff_current]
	movzx edx,BYTE PTR [eax+FMUSIC_NOTE.uvolume]
	; VolByte:volume = EDX
	; VolByte:cptr   = ESI
	call VolByte
endif
	ret
doeff_notectrl_z:
	mov BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],ah
doeff_special_r:
endif
	ret
if NOTECUT_ON
doeff_do_notecut:
	mov ecx,[ebp+doeff_mod]
	movzx eax,bl
	cmp [ecx+FMUSIC_MODULE.tick],eax
	jne doeff_notecut_ok
	and DWORD PTR [esi+FMUSIC_CHANNEL.volume],0
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_VOLUME
doeff_notecut_ok:
	ret
endif
if RETRIG_ON
doeff_do_retrig:
	test bl,bl
	jz doeff_retrig_ok
	mov eax,[ebp+doeff_mod]
	mov eax,[eax+FMUSIC_MODULE.tick]
	movzx ecx,bl
	cdq
	idiv ecx
	test edx,edx
	jne doeff_retrig_ok
	or BYTE PTR [esi+FMUSIC_CHANNEL.notectrl],FMUSIC_VOL_OR_FREQ_OR_TR
doeff_retrig_ok:
	ret
endif

if INSTRUMENTVIBRATO_ON
f0_024  dd 2.4543688e-2 ; 2*3.141592/256.0f
f64     dd 6.4e1        ; 64.0f
endif

f7      dd 7.4214768e-9
f4608_0 dd 4.608e3
f0_0013 dd 1.302083375e-3

if AMIGAPERIODS_ON
f625_27 dd 6.2527106e2
else
f8363_0 dd 8.3630004275e3
endif

; DESCRIPTION: Process sample flags
DoFlags:
ivib_iptr  equ 12
flags_sptr equ 8
	push ebp
	mov ebp,esp
	mov ebx,esi ; cptr
	push esi
	push edi
if INSTRUMENTVIBRATO_ON
	; THIS GETS ADDED TO PREVIOUS FREQDELTAS
	mov esi,[ebp+ivib_iptr]
	movzx edx,BYTE PTR [esi+FMUSIC_INSTRUMENT.VIBtype]
	xor eax,eax
	mov al,128
	lea edi,[ebx+FMUSIC_CHANNEL.ivibpos]
	dec edx ; switch(iptr->VIBtype)
	js ivib_case_0
	jz ivib_case_1
	dec edx
	jz ivib_case_2
	; case 3 / default
	push -128 ; -128 == 384 on a 1byte scope
	pop edx
	sub edx,[edi]
trim:
	movzx edx,dl
	sub eax,edx
	sar eax,1 ; delta = (128-((384-cptr->ivibpos)&0xFF))>>1 (case 3)
	          ; delta = (128-((cptr->ivibpos+128)&0xFF))>>1 (case 2)
	jmp ivib_end_switch
ivib_case_2:
	mov edx,[edi]
	sub edx,eax
	jmp trim
ivib_case_1:
	cmp [edi],eax
	setnl dl
	dec edx
	and edx,eax
	add edx,-64
	xchg eax,edx ; delta = +/- 64
	jmp ivib_end_switch
ivib_case_0:
	fild DWORD PTR [edi]
	fmul DWORD PTR [f0_024]
	fsin
	fmul DWORD PTR [f64]
	fistp DWORD PTR [ebp+ivib_iptr]
	mov eax,[ebp+ivib_iptr] ; delta = sin(cptr->ivibpos*2*3.141592/256.0f)*64.0f
ivib_end_switch:
	movzx edx,BYTE PTR [esi+FMUSIC_INSTRUMENT.iVIBdepth]
	imul edx,eax ; delta *= iptr->iVIBdepth
	movzx eax,BYTE PTR [esi+FMUSIC_INSTRUMENT.VIBsweep]
	test eax,eax
	jz sweep_ok
	push edi
	xchg eax,edi
	mov eax,[ebx+FMUSIC_CHANNEL.ivibsweeppos]
	imul edx
	idiv edi
	xchg eax,edx ; delta *= cptr->ivibsweeppos/iptr->VIBsweep
	xchg eax,edi ; iptr->VIBsweep
	pop edi
sweep_ok:
	sar edx,6
	add [ebx+FMUSIC_CHANNEL.freqdelta],edx ; cptr->freqdelta += delta>>6
	mov edx,[ebx+FMUSIC_CHANNEL.ivibsweeppos]
	inc edx
	cmp edx,eax
	jle sweeppos_ok
	xchg eax,edx
sweeppos_ok:
	mov [ebx+FMUSIC_CHANNEL.ivibsweeppos],edx
	or BYTE PTR [ebx+FMUSIC_CHANNEL.notectrl],FMUSIC_FREQ
	movzx eax,BYTE PTR [esi+FMUSIC_INSTRUMENT.VIBrate]
	cdq
	add [edi],eax ; cptr->ivibpos += iptr->VIBrate
	mov dh,1 ; edx <- 256
	mov eax,[edi]
	sub eax,edx
	jl ivibpos_ok
	mov [edi],eax
ivibpos_ok:
endif ; INSTRUMENTVIBRATO_ON
	mov esi,[ebx+FMUSIC_CHANNEL.cptr]
	test BYTE PTR [ebx+FMUSIC_CHANNEL.notectrl],FMUSIC_TRIGGER
	jz no_trig
	; TRIGGER: allocate a channel
	lodsd
	imul eax,FSOUND_CHANNEL_size
	mov edx,eax
	add eax,OFFSET Channels
	cmp DWORD PTR [eax+FSOUND_CHANNEL.fsptr],0
	jz no_swap
	; Swap between channels to avoid sounds cutting each other off and causing a click
	mov ecx,32*FSOUND_CHANNEL_size
	lea esi,[eax+FSOUND_CHANNEL.actualvolume]
	cmp edx,ecx
	jl index_ok
	neg ecx
index_ok:
	add ecx,eax
	; Copy the whole channel except it's index and trailing data
	push (FSOUND_CHANNEL_size-28)/4
	lea edi,[ecx+FSOUND_CHANNEL.actualvolume]
	mov edx,ecx
	pop ecx
	rep movsd
	; This should cause the old channel to ramp out nicely
	mov [eax+FSOUND_CHANNEL.actualvolume],ecx
	mov [eax+FSOUND_CHANNEL.leftvolume],ecx
	mov [eax+FSOUND_CHANNEL.rightvolume],ecx
	xchg eax,edx
	mov [ebx+FMUSIC_CHANNEL.cptr],eax
no_swap:
	lea edi,[eax+FSOUND_CHANNEL.fsptr]
	xchg eax,esi
	mov eax,[ebp+flags_sptr]
	stosd ; fsptr
	; START THE SOUND!
	xor eax,eax
	mov edx,[esi+FSOUND_CHANNEL.fsampleoffset]
	stosd ; mixposlo
	stosd ; speeddir
	stosd ; ramp_leftvolume
	stosd ; ramp_rightvolume
	stosd ; ramp_count
	mov [esi+FSOUND_CHANNEL.fsampleoffset],eax
	mov [esi+FSOUND_CHANNEL.mixpos],edx
no_trig:
	xor ecx,ecx
	dec cl ; ecx <- 255
	mov edx,[ebx+FMUSIC_CHANNEL.volume]
	test edx,edx
	jns volume_ns
	xor edx,edx
volume_ns:
	cmp edx,64
	jle volume_le64
	push 64
	pop edx
volume_le64:
	mov [ebx+FMUSIC_CHANNEL.volume],edx
	test BYTE PTR [ebx+FMUSIC_CHANNEL.notectrl],FMUSIC_VOLUME
	jz no_volume
	add edx,[ebx+FMUSIC_CHANNEL.voldelta]
	mov [ebp+flags_sptr],edx
	fild DWORD PTR [ebp+flags_sptr]
	fimul DWORD PTR [ebx+FMUSIC_CHANNEL.envvol]
	fimul DWORD PTR [ebx+FMUSIC_CHANNEL.fadeoutvol]
	fimul DWORD PTR [_mod+FMUSIC_MODULE.globalvolume]
if FTOL
	mov eax,OFFSET cw
	fmul DWORD PTR [f7]
	fldcw [eax]
	fistp DWORD PTR [ebp+flags_sptr]
	fldcw [eax+2]
else
	fmul DWORD PTR [f7]
	fistp DWORD PTR [ebp+flags_sptr]
endif
	mov edi,[ebp+flags_sptr]
	mov eax,[esi+FSOUND_CHANNEL.actualpan]
	imul edi
	mov [esi+FSOUND_CHANNEL.actualvolume],edi
	idiv ecx
	mov [esi+FSOUND_CHANNEL.leftvolume],eax ; leftvolume <- volume*actualpan/255
	mov eax,ecx
	sub eax,[esi+FSOUND_CHANNEL.actualpan]
	imul edi
	idiv ecx
	mov [esi+FSOUND_CHANNEL.rightvolume],eax
no_volume:
	test BYTE PTR [ebx+FMUSIC_CHANNEL.notectrl],FMUSIC_PAN
	jz no_pan
	mov edi,128
	mov eax,[ebx+FMUSIC_CHANNEL.pan]
	mov [ebp+flags_sptr],eax
	sub eax,edi
	cdq
	xor eax,edx
	sub eax,edx
	sub edi,eax
	mov eax,[ebx+FMUSIC_CHANNEL.envpan]
	sar edi,5
	sub eax,32
	imul edi
	add eax,[ebp+flags_sptr]
	jns pan_ns
	xor eax,eax ; if(pan < 0) pan = 0
pan_ns:
	cmp eax,ecx
	mov edi,[esi+FSOUND_CHANNEL.actualvolume]
	mov edx,ecx
	xchg eax,edi
	jae pan_ae255
	imul edi
	idiv ecx
	mov edx,edi
pan_ae255:
	xchg eax,edx
	mov [esi+FSOUND_CHANNEL.actualpan],eax
	mov [esi+FSOUND_CHANNEL.leftvolume],edx
	not al ; 255 - pan
	imul DWORD PTR [esi+FSOUND_CHANNEL.actualvolume]
	idiv ecx
	mov [esi+FSOUND_CHANNEL.rightvolume],eax
no_pan:
if SETSAMPLEOFFSET_ON
	test BYTE PTR [ebx+FMUSIC_CHANNEL.notectrl],FMUSIC_STOP
	jz no_stop
	xor eax,eax
	mov [esi+FSOUND_CHANNEL.fsampleoffset],eax
	mov [esi+FSOUND_CHANNEL.mixpos],eax
	mov [esi+FSOUND_CHANNEL.mixposlo],eax
no_stop:
endif
	test BYTE PTR [ebx+FMUSIC_CHANNEL.notectrl],FMUSIC_FREQ
	jz no_freq
	mov ecx,[ebx+FMUSIC_CHANNEL.freq]
	add ecx,[ebx+FMUSIC_CHANNEL.freqdelta]
	jz no_freq ; only if freq+freqdelta != 0
	test BYTE PTR [_mod+FMUSIC_MODULE.flags],1
	jz modflags_n1
	fild DWORD PTR [ebx+FMUSIC_CHANNEL.freq]
	fsubr DWORD PTR [f4608_0]
	fiadd DWORD PTR [ebx+FMUSIC_CHANNEL.freqdelta]
	fmul DWORD PTR [f0_0013]
if AMIGAPERIODS_ON
	call exp2_2
	fmul DWORD PTR [f625_27]
else
	; Inline exp2_2
	fld st0
	frndint
	fsub st1,st0
	fxch st1
	f2xm1
	fld1
	faddp st1,st0
	fscale
	fstp st1
	fmul DWORD PTR [f8363_0]
endif
if FTOL
	mov eax,OFFSET cw
	fldcw [eax]
	fistp DWORD PTR [ebp+flags_sptr]
	fldcw [eax+2]
else
	fistp DWORD PTR [ebp+flags_sptr]
endif
	mov eax,[ebp+flags_sptr] ; freq = 625.271028*exp2((4608-freq+freqdelta)/64)
	jmp modflags_1
modflags_n1:
	mov eax,14317056
	cdq
	idiv ecx ; freq = 14317056/(freq+freqdelta)
modflags_1:
	; Work out low fractional part of the speed
	mov ebx,44100 ; mixing rate in hz
	cmp eax,100
	jge freq_ok
	push 100
	pop eax
freq_ok:
	cdq
	div ebx
	mov [esi+FSOUND_CHANNEL.speedhi],eax
	div ebx ; divide remainder by rate to get lower 32bit fraction
	mov [esi+FSOUND_CHANNEL.speedlo],eax
no_freq:
	pop edi
	pop esi
	pop ebp
if INSTRUMENTVIBRATO_ON
	ret 8
else
	ret 4
endif

; DESCRIPTION: Loads an XM stream into memory. Returns non-zero on success.
LoadXM:
; EAX - 0
; ESI - _mod
; EDI - _mod
loadxm_fnumpat     equ -4
loadxm_count1      equ -8
loadxm_numpat      equ -12
loadxm_count2      equ -16
loadxm_skip        equ -20
loadxm_s0loopmode  equ -38
loadxm_s0bytes     equ -42
loadxm_s0looplen   equ -48
loadxm_s0loopstart equ -52
loadxm_sample_2    equ -56
loadxm_pat_size    equ -63
loadxm_pat         equ -68
loadxm_tmp27       equ -93
loadxm_tmp         equ -120
	push ebp
	mov ebp,esp
	add esp,-120
	push ebx
	push esi
	mov [ebp+loadxm_numpat],eax
	mov [ebp+loadxm_count1],eax
	push eax
if INFO_API_ON
	; Save track title
	push 20 ; a title has max. 20 chars
	lea edx,[esi+17]
	mov ebx,OFFSET szTtl
	pop ecx
loadxm_ttl:
	mov al,[edx]
	inc edx
	cmp al,20h ; copy only printable chars
	jl loadxm_ttl_ok
	mov [ebx],al
	inc ebx
loadxm_ttl_ok:
	dec ecx
	jnz loadxm_ttl
	mov [ebx],cl ; store additional null char
endif
	mov eax,[esi+FMUSIC_MODULE.mixer_samplespertick]
	; Seek to pattern data
	movzx ebx,WORD PTR [esi+FMUSIC_MODULE.numpatternsmem]
	add eax,60
	mov [ebp+loadxm_fnumpat],ebx
	; uFMOD_fseek:pos <= EAX
	call [uFMOD_fseek]
	; Count number of patterns
	movzx ecx,WORD PTR [esi+FMUSIC_MODULE.numorders]
	xor eax,eax
	cdq
	dec ecx
	dec dl ; edx <= 255
	cmp ecx,edx
	ja loadxm_R
loadxm_for_pats:
	mov dl,[esi+FMUSIC_MODULE.orderlist]
	cmp dl,[ebp+loadxm_numpat]
	jbe loadxm_for_continue
	mov [ebp+loadxm_numpat],dl
loadxm_for_continue:
	inc esi
	dec ecx
	jns loadxm_for_pats
	mov eax,[ebp+loadxm_numpat]
	inc eax
	mov esi,edi
	; Allocate the pattern array (whatever is bigger: loadxm_fnumpat or loadxm_numpat)
	cmp eax,ebx
	ja loadxm_pats_ok2
	xchg eax,ebx
loadxm_pats_ok2:
	movzx ecx,BYTE PTR [esi+FMUSIC_MODULE.numinsts]
	imul ecx,FMUSIC_INSTRUMENT_size
	mov [esi+FMUSIC_MODULE.numpatternsmem],ax
	lea edi,[ecx+eax*FMUSIC_PATTERN_size]
	mov eax,edi
	sub edi,ecx
	; alloc.numbytes = EAX
	call alloc
	test eax,eax
	mov [esi],eax ; FMUSIC_MODULE.pattern
	jz loadxm_R
	; Allocate instrument array
	add edi,eax
	mov [esi+FMUSIC_MODULE.instrument],edi
	xchg eax,edi
	mov eax,[ebp+loadxm_fnumpat]
	test eax,eax
	jz loadxm_R
	; Unpack and read patterns
loadxm_load_pats:
	lea eax,[ebp+loadxm_pat]
	push 9
	; uFMOD_fread:buf <= EAX
	call [uFMOD_fread]
	movzx ecx,BYTE PTR [esi+FMUSIC_MODULE.numchannels_xm]
	cmp ecx,32
	jle loadxm_numchan_ok
	xor ecx,ecx
loadxm_numchan_ok:
	mov eax,[ebp+loadxm_pat_size] ; length of pattern & packed pattern size
	mov [esi+FMUSIC_MODULE.numchannels],ecx
	mov [edi],eax
	; Allocate memory for pattern buffer
	movzx eax,ax
	imul ecx
	test eax,eax
	jz loadxm_R
	mov [ebp+loadxm_count2],eax
	lea eax,[eax+eax*4] ; x SIZE FMUSIC_NOTE
	; alloc.numbytes = EAX
	call alloc
	test eax,eax
	mov [edi+FMUSIC_PATTERN.data],eax
	jz loadxm_R
	cmp WORD PTR [edi+FMUSIC_PATTERN.patternsize],0
	jz loadxm_ldpats_continue
	mov [ebp+loadxm_tmp],eax
	inc eax
	xchg eax,ebx
loadxm_for_rowsxchan:
	push esi
	mov esi,[uFMOD_fread]
	lea eax,[ebp+loadxm_skip]
	push 1
	; uFMOD_fread:buf <= EAX
	call esi ; uFMOD_fread
	mov al,[ebp+loadxm_skip]
	test al,80h
	jz loadxm_noskip
	and eax,1
	jz loadxm_nonote
	push eax
	mov eax,[ebp+loadxm_tmp] ; &nptr->note
	; uFMOD_fread:buf <= EAX
	call esi ; uFMOD_fread
loadxm_nonote:
	test BYTE PTR [ebp+loadxm_skip],2
	jz loadxm_nonumber
	push 1
	mov eax,ebx ; &nptr->number
	; uFMOD_fread:buf <= EAX
	call esi ; uFMOD_fread
loadxm_nonumber:
	test BYTE PTR [ebp+loadxm_skip],4
	jz loadxm_novolume
	push 1
	lea eax,[ebx+1] ; &nptr->volume
	; uFMOD_fread:buf <= EAX
	call esi ; uFMOD_fread
loadxm_novolume:
	test BYTE PTR [ebp+loadxm_skip],8
	jz loadxm_noeffect
	push 1
	lea eax,[ebx+2] ; &nptr->effect
	; uFMOD_fread:buf <= EAX
	call esi ; uFMOD_fread
loadxm_noeffect:
	test BYTE PTR [ebp+loadxm_skip],16
	jz loadxm_isnote97
	push 1
	lea eax,[ebx+3] ; &nptr->eparam
	jmp loadxm_skip_read
loadxm_noskip:
	test al,al
	jz loadxm_skip_z
	mov ecx,[ebp+loadxm_tmp]
	mov [ecx],al
loadxm_skip_z:
	push 4
	mov eax,ebx
loadxm_skip_read:
	; uFMOD_fread:buf <= EAX
	call esi ; uFMOD_fread
loadxm_isnote97:
	mov eax,[ebp+loadxm_tmp]
	pop esi
	inc eax
	mov dl,BYTE PTR [esi+FMUSIC_MODULE.numinsts]
	add ebx,5
	cmp [eax],dl
	jbe loadxm_number_ok
	mov BYTE PTR [eax],0
loadxm_number_ok:
	add eax,4
	dec DWORD PTR [ebp+loadxm_count2]
	mov [ebp+loadxm_tmp],eax
	jnz loadxm_for_rowsxchan
loadxm_ldpats_continue:
	inc DWORD PTR [ebp+loadxm_count1]
	mov eax,[ebp+loadxm_fnumpat]
	add edi,8
	cmp eax,[ebp+loadxm_count1]
	ja loadxm_load_pats
	; allocate and clean out any extra patterns
	mov ecx,[ebp+loadxm_numpat]
	cmp ecx,eax
	jb loadxm_extrapats_ok
	mov ebx,[esi] ; FMUSIC_MODULE.pattern
	mov eax,[esi+FMUSIC_MODULE.numchannels]
	push esi
	lea esi,[ebx+ecx*FMUSIC_PATTERN_size]
	lea edi,[eax+eax*4]
	shl edi,6 ; numchannels*64*SIZE FMUSIC_NOTE
loadxm_for_extrapats:
	dec DWORD PTR [ebp+loadxm_numpat]
	push 64
	pop DWORD PTR [esi] ; pptr->rows <= 64
	; Allocate memory for pattern buffer
	mov eax,edi
	; alloc.numbytes = EAX
	call alloc
	test eax,eax
	mov [esi+FMUSIC_PATTERN.data],eax
	jz loadxm_R
	sub esi,FMUSIC_PATTERN_size
	mov eax,[ebp+loadxm_fnumpat]
	cmp [ebp+loadxm_numpat],eax
	jae loadxm_for_extrapats
	pop esi
loadxm_extrapats_ok:
	; Load instrument information
	mov al,BYTE PTR [esi+FMUSIC_MODULE.numinsts]
	test al,al
	jz loadxm_ret1
	mov [ebp+loadxm_count1],al
	mov ebx,[esi+FMUSIC_MODULE.instrument]
loadxm_for_instrs:
	lea eax,[ebp+loadxm_tmp]
	mov BYTE PTR [ebp+loadxm_tmp27],0
	push 33
	; uFMOD_fread:buf <= EAX
	call [uFMOD_fread] ; instrument size & name
	mov esi,[ebp+loadxm_tmp] ; firstsampleoffset = tmp[0]
	movzx edx,BYTE PTR [ebp+loadxm_tmp27]
	sub esi,33
	xor eax,eax
	cmp edx,16
	ja loadxm_R ; if(numsamples > 16) goto error
	mov [ebx],edx
	test edx,edx
	jz loadxm_inst_ok
	lea eax,[ebx+FMUSIC_INSTRUMENT.keymap]
	push 208
	; uFMOD_fread:buf <= EAX
	call [uFMOD_fread]
	sub esi,208
loadxm_inst_ok:
	push 1 ; SEEK_CUR
	xchg eax,esi
	; uFMOD_fseek:pos <= EAX
	call [uFMOD_fseek]
	lea edx,[ebx+FMUSIC_INSTRUMENT.VOLfade]
	xor eax,eax
	mov cx,[edx]
	shl ecx,1
	cmp BYTE PTR [edx+FMUSIC_INSTRUMENT.VOLnumpoints-FMUSIC_INSTRUMENT.VOLfade],2
	mov [edx],cx ; iptr->VOLfade *= 2
	jnb ladxm_voltype_ok
	mov BYTE PTR [edx+FMUSIC_INSTRUMENT.VOLtype-FMUSIC_INSTRUMENT.VOLfade],al
ladxm_voltype_ok:
	cmp BYTE PTR [edx+FMUSIC_INSTRUMENT.PANnumpoints-FMUSIC_INSTRUMENT.VOLfade],2
	jnb loadxm_PANtype_ok
	mov BYTE PTR [edx+FMUSIC_INSTRUMENT.PANtype-FMUSIC_INSTRUMENT.VOLfade],al
loadxm_PANtype_ok:
	cmp [ebx],eax
	je loadx_for_loadsamp_end
	mov [ebp+loadxm_numpat],eax
	lea eax,[ebx+FMUSIC_INSTRUMENT.sample]
	mov [ebp+loadxm_fnumpat],eax
loadxm_for_samp:
	lea eax,[ebp+loadxm_sample_2]
	push 40
	; uFMOD_fread:buf <= EAX
	call [uFMOD_fread]
	mov esi,[ebp+loadxm_s0loopstart]
	mov al,[ebp+loadxm_s0bytes]
	mov ecx,eax
	shr eax,4 ; sample[0].bytes >>= 4
	and al,1  ; [b 4] : 8/16 bit sample data
	mov [ebp+loadxm_s0bytes],al
	jz loadxm_s0bytes_ok
	shr DWORD PTR [ebp+loadxm_s0looplen],1 ; sample[0].looplen >>= 1
	shr esi,1 ; sample[0].loopstart >>= 1
	shr DWORD PTR [ebp+loadxm_sample_2],1 ; sample[0].length >>= 1
loadxm_s0bytes_ok:
	and ecx,3 ; [b 0-1] : loop type
	jz loadxm_reset_sample
	cmp DWORD PTR [ebp+loadxm_s0looplen],0
	jnz loadxm_s0loop_ok
loadxm_reset_sample:
	; If loop length is 0, the sample is NOT looping.
	; So, ensure the looping flag is cleared and reset loop bounds to default.
	mov eax,[ebp+loadxm_sample_2]
	xor esi,esi
	xor ecx,ecx
	mov [ebp+loadxm_s0looplen],eax ; sample[0].looplen = sample[0].length
loadxm_s0loop_ok:
	mov [ebp+loadxm_s0loopstart],esi
	mov [ebp+loadxm_s0loopmode],cl
	mov eax,[ebp+loadxm_sample_2]
	lea eax,[eax+eax+26] ; sample[0].length*2+SIZE FSOUND_SAMPLE+4
	; alloc.numbytes = EAX
	call alloc
	mov ecx,[ebp+loadxm_fnumpat]
	test eax,eax
	mov [ecx],eax
	jz loadxm_R
	; memcpy(iptr->sample[count2],sample,sizeof(FSOUND_SAMPLE))
	inc DWORD PTR [ebp+loadxm_numpat]
	add DWORD PTR [ebp+loadxm_fnumpat],4
	push 5
	xchg eax,edi
	mov eax,[ebp+loadxm_numpat]
	pop ecx
	cmp eax,[ebx]
	lea esi,[ebp+loadxm_sample_2]
	rep movsd
	jb loadxm_for_samp
	; Load sample data
	mov [ebp+loadxm_numpat],ecx
	lea eax,[ebx+FMUSIC_INSTRUMENT.sample]
	mov [ebp+loadxm_fnumpat],eax
loadx_for_loadsamp:
	mov eax,[ebp+loadxm_fnumpat]
	mov esi,[eax]
	mov edx,[esi]
	mov ch,[esi+FSOUND_SAMPLE.Reserved]
	mov cl,[esi+FSOUND_SAMPLE.bytes]
	xor eax,eax
	add esi,FSOUND_SAMPLE.buff
if ADPCM_ON
	cmp ch,0ADh ; ModPlug 4-bit ADPCM
	jne loadxm_regular_samp
	inc edx
	mov edi,esi
	shr edx,1
	push ebx
	push edx
	lea edx,[edx+edx*2]
	add edi,edx ; ptr <= buff+compressed_length*3
	; Read in the compression table
	lea ebx,[ebp+loadxm_sample_2]
	push 16
	mov eax,ebx
	; uFMOD_fread:buf <= EAX
	call [uFMOD_fread]
	; Read in the sample data
	mov eax,edi
	; uFMOD_fread:buf <= EAX
	call [uFMOD_fread]
	; Decompress sample data
	mov edx,esi
	xor ecx,ecx ; delta
loadxm_unpack_loop:
	cmp edx,edi
	jge loadxm_unpack_ok
	mov al,[edi]
	mov ah,al
	and al,0Fh
	xlatb
	shr ah,4
	inc edi
	add ch,al
	mov al,ah
	xlatb
	add al,ch
	shl eax,24
	or ecx,eax
	mov [edx],ecx
	shr ecx,16 ; ch <- delta
	add edx,4
	jmp loadxm_unpack_loop
loadxm_unpack_ok:
	pop ebx
	jmp loadxm_chk_loop_bidi
loadxm_regular_samp:
endif
	shl edx,cl
	push edx ; sptr->length << sptr->bytes
	mov eax,esi
	; uFMOD_fread:buf <= EAX
	call [uFMOD_fread]
	cmp BYTE PTR [esi+FSOUND_SAMPLE.bytes-FSOUND_SAMPLE.buff],0
	mov ecx,DWORD PTR [esi+FSOUND_SAMPLE._length-FSOUND_SAMPLE.buff]
	jne loadxm_16bit_ok
	lea edi,[ecx+esi] ; buff <= sptr->buff+sptr->length
	lea eax,[edi+ecx] ; ptr <= buff+sptr->length
	cdq
	; Promote to 16 bits
loadxm_to16bits:
	dec eax
	dec edi
	dec eax
	mov dh,[edi]
	mov [eax],dx
	cmp eax,edi
	ja loadxm_to16bits
loadxm_16bit_ok:
	mov eax,esi
	; Do delta conversion
	cdq
loadxm_do_delta_conv:
	add dx,[eax]
	mov [eax],dx
	inc eax
	inc eax
	dec ecx
	jg loadxm_do_delta_conv
	js loadxm_loops_ok
loadxm_chk_loop_bidi:
	mov eax,DWORD PTR [esi+FSOUND_SAMPLE.looplen-FSOUND_SAMPLE.buff]
	mov ecx,DWORD PTR [esi+FSOUND_SAMPLE.loopstart-FSOUND_SAMPLE.buff]
	add eax,ecx
	cmp BYTE PTR [esi+FSOUND_SAMPLE.loopmode-FSOUND_SAMPLE.buff],2 ; FSOUND_LOOP_BIDI
	lea eax,[esi+eax*2]
	jnz loadxm_chk_loop_normal
	mov cx,[eax-2]
	jmp loadxm_fix_loop
loadxm_chk_loop_normal:
	cmp BYTE PTR [esi+FSOUND_SAMPLE.loopmode-FSOUND_SAMPLE.buff],1 ; FSOUND_LOOP_NORMAL
	jnz loadxm_loops_ok
	mov cx,WORD PTR [esi+ecx*2]
loadxm_fix_loop:
	mov [eax],cx
loadxm_loops_ok:
	inc DWORD PTR [ebp+loadxm_numpat]
	mov eax,[ebp+loadxm_numpat]
	add DWORD PTR [ebp+loadxm_fnumpat],4
	cmp eax,[ebx]
	jb loadx_for_loadsamp
loadx_for_loadsamp_end:
	add ebx,FMUSIC_INSTRUMENT_size
	dec BYTE PTR [ebp+loadxm_count1]
	jnz loadxm_for_instrs
loadxm_ret1:
	inc eax
loadxm_R:
	pop esi
	pop ebx
	leave
	ret