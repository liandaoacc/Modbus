;***************************************************************************
; FILENAME: vector.asm
;
; DESCRIPTION:  init vector  
;***************************************************************************
      .title "vectors.asm"
      .ref   _c_int0
      .ref   _nothing
	  .ref   _tTIMERINT1
	  .ref   _tTIMERINT2
      .ref   _sUART
       
      .sect ".vectors"

reset   B	  _c_int0 	 	; 00
int1 	B     _nothing		; 02
int2    B	  _tTIMERINT1  	; 04
int3	B     _tTIMERINT2 	; 06
int4	B     _nothing	 	; 08
int5	B     _sUART		; 0A
int6	B     _nothing	 	; 0C
      .end

