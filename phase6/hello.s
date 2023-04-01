main:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$main.size, %eax
	subq	%rax, %rsp
	leaq	.L0, %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf

main.exit:
	movq	%rbp, %rsp
	popq	%rbp
	ret

	.set	main.size, 0
	.globl	main

	.data	
.L0:	.asciz	"hello world
"
