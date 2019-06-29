STEP equ 1
section .text
			   global          _start




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


_start:

				xor r9, r9		;count
again:
				xor rax, rax
				mov rsi, buf
				mov rdx, 1024
				syscall

				lea             rsi, [buf + rax]
                neg             rax
                jz              exit
                jg              readfail
				
				
do:
				movzx           edx, byte [rsi + rax]
				
				cmp             dl, 9
				je end_loop
				cmp             dl, 10
				je end_loop
				cmp             dl, 11
				je end_loop
				cmp             dl, 12
				je end_loop
				cmp             dl, 13
				je end_loop
				cmp             dl, 32
				je end_loop
				
				jmp end_loop_
end_loop:		
				inc r9
end_loop_:
				inc rax
				jnz do
				
				
				mov rax, r9
				call print_number
				
				jmp again
				
				
				
readfail:		ud2
exit: 			mov rax, 60
				xor rdi, rdi
				syscall
				
section         .rodata
msg:            db              0x0a
msg_size:       equ             $ - msg

section         .bss
buf:			resb 1024
rbuf:			resb 1024