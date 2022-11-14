;
; Startup code for cc65 (C64 version)
; modified: Only startup needed, no teardown
;
; This must be the first file on the linker command line
;

        .export         __STARTUP__ : absolute = 1      ; Mark as startup

        .import         initlib
        .import         zerobss, callmain
        .import         BSOUT
        .import         __HIRAM_START__, __HIRAM_SIZE__   ; Linker generated

        .include        "zeropage.inc"

; ------------------------------------------------------------------------
; Startup code

.segment        "STARTUP"

; Switch off the BASIC ROM.

        lda     #$36
        sta     $01

; Save space by putting some of the start-up code in the ONCE segment,
; which can be re-used by the BSS segment, the heap and the C stack.

        jsr     init

; Clear the BSS data.

        jsr     zerobss

; Call main - will never return

        jmp     callmain

; ------------------------------------------------------------------------

.segment        "ONCE"

init:

; Set up the stack.

        lda     #<(__HIRAM_START__ + __HIRAM_SIZE__)
        ldx     #>(__HIRAM_START__ + __HIRAM_SIZE__)
        sta     sp
        stx     sp+1            ; Set argument stack ptr

; Switch to the second charset.

        lda     #14
        jsr     BSOUT

; Call the module constructors.

        jmp     initlib

