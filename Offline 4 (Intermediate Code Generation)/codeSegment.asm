.CODE
	f PROC
		PUSH BP
		MOV BP, SP
		PUSH 10		;Line 2: Save 10
		POP AX		; Load 10
		MOV [BP+10], AX		; Line 2: a = 10
		PUSH AX		; Save declared value of a
		POP AX		; Line 2: Expression evaluated
	L0: 
		PUSH 10		;Line 3: Save 10
		POP AX		; Load 10
		MOV [BP+8], AX		; Line 3: b = 10
		PUSH AX		; Save declared value of b
		POP AX		; Line 3: Expression evaluated
	L1: 
		PUSH [BP + 10]		;Line 4: Save a
		PUSH [BP + 8]		;Line 4: Save b
		POP BX		; Line 4: Load a
		POP AX		; Line 4: Load b
		ADD AX, BX		; Line 4: Add a and b
		PUSH AX		;Save result
		; Line 4: return a + b
		POP AX
		MOV [BP+4], AX
		ADD SP, 0		;Line 4: Removing all the local variables from the stack
		POP BP
		RET
	L2: 
		; Line: 5 In case no return type
		POP BP
		RET
	f ENDP
	main PROC
		MOV AX, @DATA
		MOV DS, AX		; data segment loaded
		MOV BP, SP
		SUB SP, 2		;Line8 : variable i declared
	L4: 
		PUSH 7		;Line 9: Save 7
		POP AX		; Load 7
		MOV [BP-2], AX		; Line 9: i = 7
		PUSH AX		; Save declared value of i
		POP AX		; Line 9: Expression evaluated
	L5: 
		PUSH 0		;Line 13: Save 0
		; Line 13: return 0
		POP AX
		MOV [BP+4], AX
		ADD SP, 0		;Line 13: Removing all the local variables from the stack
		POP BP
	L6: 

	L3:
		MOV AH, 4CH
		INT 21H
		main ENDP

	PRINT_NEWLINE PROC
        ; PRINTS A NEW LINE WITH CARRIAGE RETURN
        PUSH AX
        PUSH DX
        MOV AH, 2
        MOV DL, 0Dh
        INT 21h
        MOV DL, 0Ah
        INT 21h
        POP DX
        POP AX
        RET
    PRINT_NEWLINE ENDP

	PRINT_NUM_FROM_BX PROC
    	PUSH CX  
    	; push to stack to 
	    ; check the end of the number  
	    MOV AX, 'X'
	    PUSH AX
    
	    CMP BX, 0  
	    JE ZERO_NUM
	    JNL NON_NEGATIVE 
    
	    NEG BX
	    ; print - for negative number
	    MOV DL, '-'
	    MOV AH, 2
	    INT 21H
	    JMP NON_NEGATIVE  
    
	    ZERO_NUM:
		        MOV DX, 0
		        PUSH DX
		        JMP POP_PRINT_LOOP
    
	    NON_NEGATIVE:
    
		    MOV CX, 10 
    
		    MOV AX, BX
	    PRINT_LOOP:
	        ; if AX == 0
	        CMP AX, 0
	        JE END_PRINT_LOOP
	        ; else
	        MOV DX, 0 ; DX:AX = 0000:AX
        
	        ; AX = AX / 10 ; store reminder in DX 
	        DIV CX
    
	        PUSH DX
        
	        JMP PRINT_LOOP

	    END_PRINT_LOOP:
    
    
    
	    POP_PRINT_LOOP:
	        POP DX
	        ; loop ending condition
	        ; if DX == 'X'
	        CMP DX, 'X'
	        JE END_POP_PRINT_LOOP
        
	        ; if DX == '-'
	        CMP DX, '-'
	        JE PRINT_TO_CONSOLE
        
	        ; convert toascii
	        ADD DX, 30H       
	        ; print the digit
	        PRINT_TO_CONSOLE:
		        MOV AH, 2
		        INT 21H
        
		        JMP POP_PRINT_LOOP
    
	    END_POP_PRINT_LOOP: 
			CALL PRINT_NEWLINE
		    POP CX
		    RET
	PRINT_NUM_FROM_BX ENDP
END MAIN
