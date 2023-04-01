main:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$main.size, %eax
	subq	%rax, %rsp
	movq	-8(%rbp), %rax
	addl	$0, %rax
	movq	(