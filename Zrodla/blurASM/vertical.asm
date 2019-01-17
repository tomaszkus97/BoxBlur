; void MyProc
;(byte * source, = RCX 
;byte * target = RDX
;int width, = r8
 ;int height); = r9
.data
align 16
RCPDIVIDER real4 0.0, 0.0, 0.0, 0.0
TARGET DQ 0.0 
SOURCE DQ 0.0
OFFSET_COUNTER DQ 0.0
OFFSET_FINISH DQ 0.0
TMP DQ 0.0
BLUR_SIZE DQ 15
BORDER DQ 0.0

.code

vertical proc
	mov TARGET, rdx; save the rdx value - target array pointer
	mov SOURCE, rcx; save the rcx value - source array pointer
	 mov r10, 0 ;array offset pointer
	 mov r11, 0; average array offset pointer
	 mov r12w, 3; r12 - neighbourhood counter
	 mov r13, r8; width to pixel counter
	 mov rax, r9; height to accumulator
	 mul r13; making r13 pixel counter
	 mov r13, rax;
	mov r14, 0; iterartion counter
	mov r15, 16; size of data in xmm in bytes
	
	 mov rax, BLUR_SIZE
	 cvtsi2ss xmm0,eax
	 shufps xmm0, xmm0, 0 ;move to all 4 dwords
	 rcpps xmm0, xmm0 ;recip values in xmm0
	 movaps [RCPDIVIDER], xmm0 ;move to the memory

	 ;computing offset counter
mov rax, BLUR_SIZE
mov rcx, 2
div rcx;
mov TMP, rax;
xor r11, r11; zero offset counter
sub r11, TMP; offset counter = -(BLUR_SIZE/2)
mov OFFSET_COUNTER, r11; move to the memory

;calculate border
xor rdx, rdx
	 mov rax, BLUR_SIZE
	 mov rcx, 2
	 div rcx
	 mov BORDER, rax

;calculate offset finish
 mov rax, BORDER
 inc rax
 mov OFFSET_FINISH, rax

pixelLoop:
;loop by all pixels in bitmap (BGR0)	 
 pxor xmm0,xmm0  ; zero the xmm0 register -> holding average value

 ;check if we are at the border of image 

 ;xor rdx, rdx; clear the "modulo" register
 ;mov rax, r14; move divident
 ;mov rcx, r8; move divisor
 ;div rcx; dividing, rax = row, rdx = column

 ;mov rcx, rax

 ;sub rcx, BORDER
 ;cmp rcx, 0
 ;jl nextPixelRW
 ;mov rcx, r9
 ;sub rcx, BORDER
 ;cmp rax, rcx; if row > width - BORDER 
 ;jge nextPixelRW
   



mov r12, OFFSET_COUNTER





;loop computing the neigbour average of BRG values	
avgloop:

	;compute offset 
mov r15, r10; neigbourhood offset = array offset
mov r11,r12
mov rax, 16
mul r11;
mul r8;
mov r11, rax
add r15, r11; add offset sum to neigbourhood offset
	
	
	 
	pxor xmm1,xmm1 ; zero the xmm1 register -> tmp sum
	mov rcx, SOURCE
	addps xmm1, xmmword ptr[rcx+r15] ; read 16 bytes of data to tmp sum
	mulps xmm1,RCPDIVIDER ;mulityply tmp sum of  by 1/3	
	addps xmm0, xmm1; add value of tmpsum to average sum
	
	inc r12
	cmp r12, OFFSET_FINISH
	jne avgloop

nextPixel:
mov rdx, TARGET
movapd xmmword ptr[rdx+r10],xmm0 ; move result to the result array
add r10, 16 ;move 16 bytes in array
;mov r11, 0; ; zero the r11 register
mov r12w, 3; move 3 to neigbourhood counter
inc r14; increment iterartion counter
dec r13; decrement pixel counter
jz finish
jmp pixelLoop

nextPixelRW:
pxor xmm1,xmm1 ; zero the xmm1 register -> tmp sum
mov rcx, SOURCE

addps xmm1, xmmword ptr[rcx+r10] ; read 16 bytes of data to tmp sum
mov rdx, TARGET
movapd xmmword ptr[rdx+r10],xmm1 ; move result to the result array


add r10, 16 ;move 16 bytes in array
;mov r11, 0; ; zero the r11 register
mov r12w, 3; move 3 to neigbourhood counter
inc r14; increment iterartion counter
dec r13; decrement pixel counter

jnz pixelLoop ; if not zero zero

finish:
	ret	
	
	
vertical endp

end
