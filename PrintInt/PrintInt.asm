section .text
			   global          _start

_start:
                mov             rax, 123
				
				call print_number
				
                mov             rax, 314159265358979323
				
				call print_number

                mov             rax, 60		;return
                xor             rdi, rdi
                syscall

print_number:
                mov             rbp, rsp		;remember rsp
                mov             rdi, rsp		;rdi - msg
                sub             rsp, 24

                dec             rdi
                mov             byte [rdi], 10	;add \n

                or              rax, rax
                jz              write_zero		;0

                mov             ebx, 10
loop:
                xor             edx, edx		;ax=00ax/bx
                div             rbx				;dx=00ax%bx

                add             edx, '0'
                dec             rdi
                mov             byte [rdi], dl	;\n...(cur)

                or              rax, rax
                jnz             loop
                jmp             print

write_zero:
                dec             rdi
                mov             byte [rdi], '0'

print:
                mov             rax, 1
                mov             rsi, rdi
                mov             rdx, rbp
                sub             rdx, rdi	;rdx = len
                mov             rdi, 1
                syscall			;rax - syswrite(1), rdi - in(0)/out(1), rsi - msg, rdx - length

                mov             rsp, rbp
                ret