; vim: set smartindent et ts=8 sw=8 sts=8 fdm=marker syntax=64tass:
;
; Simple Koala show program, requires 64tass to build

        BITMAP = $2000
        VIDRAM = $0c00
        COLRAM = $1000

        ; Needs work to allow showing other files
        ; KOALA_FILE = "../focus-3d.koa"


        * = $0801

        .word (+), 2020
        .null $9e, format("%d", init)
+       .word 0


init
        lda #0
        sta $d020
        lda #$3b
        sta $d011
        lda #$38
        sta $d018
        lda #$18
        sta $d016
        lda #3
        sta $dd00
        ldx #0
-       lda COLRAM,x
        sta $d800,x
        lda COLRAM + $0100,x
        sta $d900,x
        lda COLRAM + $0200,x
        sta $da00,x
        lda COLRAM + $02e8,x
        sta $dae8,x
        inx
        bne -
        lda COLRAM + 1000
        sta $d021
        jmp *

        * = BITMAP
        .binary format("%s", KOALA_FILE), 2, 8000

        * = VIDRAM
        .binary format("%s", KOALA_FILE), 2 + 8000, 1000

        * = COLRAM
        ; add 1 more byte to grab the background color
        .binary KOALA_FILE, 2 + 8000 + 1000, 1000 + 1


