readarray:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$readarray.size, %eax
	subq	%rax, %rsp
	movl	$0, %eax
	movl	%eax, -4(%rbp)
.L0:
	movl	-4(%rbp), %eax
	cmpl	n, %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L1
	movslq	-4(%rbp),%rax
	imull	$4, %rax
	movq	a, %rdi
	addl	%rax, %rdi
	leaq	.L2, %rax
	movq	%rdi, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	scanf
	movl	-4(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -4(%rbp)
	jmp	.L0
.L1:

readarray.exit:
	movq	%rbp, %rsp
	popq	%rbp
	ret

	.set	readarray.size, 16
	.globl	readarray

writearray:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$writearray.size, %eax
	subq	%rax, %rsp
	movl	$0, %eax
	movl	%eax, -4(%rbp)
.L3:
	movl	-4(%rbp), %eax
	cmpl	n, %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L4
	movslq	-4(%rbp),%rax
	imull	$4, %rax
	movq	a, %rdi
	addl	%rax, %rdi
	movl	(