.text


.globl main
main:
pushq %rbp
movq %rsp, %rbp
subq $24, %rsp
movq	%rdi, -16(%rbp)
movq	$1, -8(%rbp)
.L3:
movq	-8(%rbp), %rax
movq	$10, %rcx
cmpq	%rcx, %rax
jl .L5
jmp .L8
.L5:
movq	-8(%rbp), %r10
movq	$0, %rax
movq	$printfmt, %rdi
movq	%r10, %rsi
call printf
movq	-8(%rbp), %rbx
movq	$1, %rcx
addq	%rcx, %rbx
movq	%rbx, -8(%rbp)
jmp .L3
.L8:
leave
ret
movq $60, %rax
xorq %rbx, %rbx
syscall
printfmt: 
.string "%d\n"
