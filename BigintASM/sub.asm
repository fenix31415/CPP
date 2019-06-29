                section         .text

                global          _start
_start:
                sub             rsp, 2 * 128 * 8		;add space
                lea             rdi, [rsp + 128 * 8]
                mov             rcx, 128				;length
                call            read_long
                mov             rdi, rsp
                call            read_long
                lea             rsi, [rsp + 128 * 8]
				
                call            sub_long_long

                call            write_long

                mov             al, 0x0a
                call            write_char

                jmp             exit
				
				
; mul two long numbers
;    rdi -- address #1 (long number)
;    rsi -- address #2 (long number)
;    rcx -- length of long numbers in qwords
; result:
;    mul -> rdi
mul_long_long:
                push            rdi
                push            rsi
                push            rcx
                push            r9
				push			rbx
				
				clc
				xor				r12, r12				;index
				mov				r13, rcx
				sub             rsp, 2 * 128 * 8		;add another space
				lea             r9, [rsp + 128 * 8]		;r9 = ans
				lea             r8, [rsp]				;r8 = tmp
				mov				r10, rdi				;first
				mov				r11, rsi				;second
				mov				rdi, r9
				call			set_zero				;ans = 0
				mov				rdi, r8
				call			set_zero				;tmp = 0
				
				
.loop:
				
				
				mov             rbx, [r11]
				
				
                lea             r11, [r11 + 8]
				
				
				push rax
				mov				rax,[r11]
			;	call			print_number
				pop rax
				
				mov				rdi, r10
				call			mul_long_short_copy		;ans in r8
				
				
				
				mov				rdi, r9
				mov				rsi, r8
				call			add_long_long_off		;curans in ans
				
				inc r12
				
				dec				r13
				
				
				
				
				cmp				r13, 0
				jne             .loop
				
				
				mov				rsi, r9
				mov				rdi, r10
				call			copy_long
				
				lea             rsp, [rsp + 2 * 128 * 8]		;delete 2 longs
				
				pop				rbx
                pop             r9
                pop             rcx
                pop             rsi
                pop             rdi
                ret
		
; print rax
print_number:
				push			rbp
				push			rdi
				push			rax
				push			rbx
				push			rdx
				push			rsi
				
				
                mov             rbp, rsp		;remember rsp
                mov             rdi, rsp		;rdi - msg
                sub             rsp, 24

                dec             rdi
                mov             byte [rdi], 10	;add \n

                or              rax, rax
                jz              .write_zero		;0

                mov             rbx, 10
.loop:
                xor             rdx, rdx		;ax=00ax/bx
                div             rbx				;dx=00ax%bx

                add             rdx, '0'
                dec             rdi
                mov             byte [rdi], dl	;\n...(cur)

                or              rax, rax
                jnz             .loop
                jmp             .print

.write_zero:
                dec             rdi
                mov             byte [rdi], '0'

.print:
                mov             rax, 1
                mov             rsi, rdi
                mov             rdx, rbp
                sub             rdx, rdi	;rdx = len
                mov             rdi, 1
                syscall			;rax - syswrite(1), rdi - in(0)/out(1), rsi - msg, rdx - length

                mov             rsp, rbp
				
				
				pop			rsi
				pop			rdx
				pop			rbx
				pop			rax
				pop			rdi
				pop			rbp
				
                ret
		
; copy two long numbers
;    rdi -- address #1 (long number)
;    rsi -- address #2 (long number)
;    rcx -- length of long numbers in qwords
; result:
;    rsi -> rdi
copy_long:
                push            rdi
                push            rsi
                push            rcx
.loop:
                mov             rax, [rsi]
                lea             rsi, [rsi + 8]
                mov             [rdi], rax
                lea             rdi, [rdi + 8]
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rsi
                pop             rdi
                ret
				
; sub two long numbers
;    rdi -- address of summand #1 (long number)
;    rsi -- address of summand #2 (long number)
;    rcx -- length of long numbers in qwords
; result:
;    sub -> rdi
sub_long_long:
                push            rdi
                push            rsi
                push            rcx

                clc
.loop:
                mov             rax, [rsi]
                lea             rsi, [rsi + 8]
                sbb             [rdi], rax
                lea             rdi, [rdi + 8]
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rsi
                pop             rdi
                ret
				
; adds two long number with offset of second
;    rdi -- address of summand #1 (long number)
;    rsi -- address of summand #2 (long number)
;    rcx -- length of long numbers in qwords
;    r12 -- offset
; result:
;    sum is written to rdi
add_long_long_off:
                push            rdi
                push            rsi
                push            rcx
				push			r10
				push			r11
                push            rax
				

				sub				rcx, r12
				lea				r10, [rcx]		;r10 -- counts steps
				lea				rsi, [rsi]
				
                clc
				
.loop:
                mov             rax, [rsi]
			;	call print_number
                lea             rsi, [rsi + 8]
                adc             [rdi + 8 * r12], rax
                lea             rdi, [rdi + 8]
				
				
                dec             r10
                jnz             .loop

                pop             rax
				pop				r11
				pop				r10
                pop             rcx
                pop             rsi
                pop             rdi
				
				
				
                ret
				
				
; adds two long number
;    rdi -- address of summand #1 (long number)
;    rsi -- address of summand #2 (long number)
;    rcx -- length of long numbers in qwords
; result:
;    sum is written to rdi
add_long_long:
                push            rdi
                push            rsi
                push            rcx

                clc
.loop:
                mov             rax, [rsi]
                lea             rsi, [rsi + 8]
                adc             [rdi], rax
                lea             rdi, [rdi + 8]
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rsi
                pop             rdi
                ret
				
; read long number from stdin
;    rdi -- location for output (long number)
;    rcx -- length of long number in qwords
read_long:
                push            rcx
                push            rdi

                call            set_zero
.loop:
                call            read_char
                or              rax, rax
                js              exit
                cmp             rax, 0x0a
                je              .done
                cmp             rax, '0'
                jb              .invalid_char
                cmp             rax, '9'
                ja              .invalid_char

                sub             rax, '0'
                mov             rbx, 10
                call            mul_long_short
                call            add_long_short
                jmp             .loop

.done:
                pop             rdi
                pop             rcx
                ret

.invalid_char:
                mov             rsi, invalid_char_msg
                mov             rdx, invalid_char_msg_size
                call            print_string
                call            write_char
                mov             al, 0x0a
                call            write_char

.skip_loop:
                call            read_char
                or              rax, rax
                js              exit
                cmp             rax, 0x0a
                je              exit
                jmp             .skip_loop
				
		
; adds 64-bit number to long number
;    rdi -- address of summand #1 (long number)
;    rax -- summand #2 (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    sum is written to rdi
add_long_short:
                push            rdi
                push            rcx
                push            rdx

                xor             rdx,rdx
.loop:
                add             [rdi], rax
                adc             rdx, 0
                mov             rax, rdx
                xor             rdx, rdx
                add             rdi, 8
                dec             rcx
                jnz             .loop

                pop             rdx
                pop             rcx
                pop             rdi
                ret

; multiplies long number by a short
;    rdi -- address of multiplier #1 (long number)
;    rbx -- multiplier #2 (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    product is written to r8
mul_long_short_copy:
                push            rax
                push            rdi
                push            rcx
				push			rsi
				push			rbx
				push			rdx
				push			r8
				
                xor             rsi, rsi
				
.loop:
                mov             rax, [rdi]
                mul             rbx
                add             rax, rsi
                adc             rdx, 0
                mov             [r8], rax
                lea             r8, [r8 + 8]
				lea 			rdi, [rdi + 8]
                mov             rsi, rdx
				
				
				
                dec             rcx
                jnz             .loop
				
				pop				r8
				pop				rdx
				pop				rbx
				pop				rsi
                pop             rcx
                pop             rdi
                pop             rax
                ret

; multiplies long number by a short
;    rdi -- address of multiplier #1 (long number)
;    rbx -- multiplier #2 (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    product is written to rdi
mul_long_short:
                push            rax
                push            rdi
                push            rcx

                xor             rsi, rsi
.loop:
                mov             rax, [rdi]
                mul             rbx
                add             rax, rsi
                adc             rdx, 0
                mov             [rdi], rax
                add             rdi, 8
                mov             rsi, rdx
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rdi
                pop             rax
                ret
		
		
; write long number to stdout
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
write_long:
                push            rax
                push            rcx
                push            rbp
                push            rsi
				push			rbx
				push			rdx

                mov             rax, 20
                mul             rcx
                mov             rbp, rsp
                sub             rsp, rax

                mov             rsi, rbp

.loop:
                mov             rbx, 10
                call            div_long_short
                add             rdx, '0'
                dec             rsi
                mov             [rsi], dl
                call            is_zero
                jnz             .loop

                mov             rdx, rbp
                sub             rdx, rsi
                call            print_string

                mov             rsp, rbp
				
				pop 			rdx
				pop 			rbx
                pop             rsi
                pop             rbp
                pop             rcx
                pop             rax
                ret
				
; checks if a long number is a zero
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
; result:
;    ZF=1 if zero
is_zero:
                push            rax
                push            rdi
                push            rcx

                xor             rax, rax
                rep scasq

                pop             rcx
                pop             rdi
                pop             rax
                ret
				
				
				
; divides long number by a short
;    rdi -- address of dividend (long number)
;    rbx -- divisor (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    quotient is written to rdi
;    rdx -- remainder
div_long_short:
                push            rdi
                push            rax
                push            rcx

                lea             rdi, [rdi + 8 * rcx - 8]
                xor             rdx, rdx

.loop:
                mov             rax, [rdi]
                div             rbx
                mov             [rdi], rax
                sub             rdi, 8
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rax
                pop             rdi
                ret
				
				
; assigns a zero to long number
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
set_zero:
                push            rax
                push            rdi
                push            rcx

                xor             rax, rax
                rep stosq

                pop             rcx
                pop             rdi
                pop             rax
                ret
				
; read one char from stdin
; result:
;    rax == -1 if error occurs
;    rax \in [0; 255] if OK
read_char:
                push            rcx
                push            rdi

                sub             rsp, 1
                xor             rax, rax
                xor             rdi, rdi
                mov             rsi, rsp
                mov             rdx, 1
                syscall

                cmp             rax, 1
                jne             .error
                xor             rax, rax
                mov             al, [rsp]
                add             rsp, 1

                pop             rdi
                pop             rcx
                ret
.error:
                mov             rax, -1
                add             rsp, 1
                pop             rdi
                pop             rcx
                ret
				
				
exit_succ:
				push rax
				push rdi
				push rsi
				push rdx
				
				mov 			rax, 1
				mov             rdi, 1
                mov             rsi, sucMsg
                mov             rdx, sucMsg_size
				syscall
				
				pop rdx
				pop rsi
				pop rdi
				pop rax
				
				ret
exit:
                mov             rax, 60
                xor             rdi, rdi
                syscall


; write one char to stdout, errors are ignored
;    al -- char
write_char:
				push			rax
				push			rdi
				push			rdx
				push			rsi


                sub             rsp, 1
                mov             [rsp], al

                mov             rax, 1
                mov             rdi, 1
                mov             rsi, rsp
                mov             rdx, 1
                syscall
                add             rsp, 1
				
				
				pop			rsi
				pop			rdx
				pop			rdi
				pop			rax
				
                ret

; print string to stdout
;    rsi -- string
;    rdx -- size
print_string:
                push            rax

                mov             rax, 1
                mov             rdi, 1
                syscall

                pop             rax
                ret
				
				section .rodata
sucMsg: db "Finished.",10
sucMsg_size: equ             $ - sucMsg

invalid_char_msg:
                db              "Invalid character: "
invalid_char_msg_size: equ             $ - invalid_char_msg