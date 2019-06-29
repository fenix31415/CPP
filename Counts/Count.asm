STEP equ 1
section .text
			   global          _start




print_number:

	push rdx
	push rbx
	push r11
	push r10
	push r12
	push r13
	push r14



				xor dx, dx
				mov bx, 10
				xor r11, r11
				mov r10, buf
				
				
loop:
				div bx
				
				mov word [r10], dx
				add word [r10], 48
				xor dx, dx
				add r10, STEP
				add r11, 1
				
				cmp ax, 0
				je reverse
				
				jmp loop
				
reverse:
				mov r12, rbuf
				xor r13, r13
				sub r10, STEP
r_loop:
				cmp r11, 0
				je print
				
				mov r14, [r10]
				mov [r12], r14
				
				sub r10, STEP
				sub r11, 1
				add r12, STEP
				add r13, 1
				jmp r_loop
print:
				mov rax, r13
				mov r13, STEP
				mul r13
				
				mov rdx, rax
				mov rax, 1
				mov rdi, 1
				mov rsi, rbuf
				syscall
				
				
				pop r14
				pop r13
				pop r12
				pop r10
				pop r11
				pop rbx
				pop rdx
				
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
				cmp             dl, 48
				
				jne end_loop
				inc r9
end_loop:		
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