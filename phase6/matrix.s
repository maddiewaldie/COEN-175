allocate:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$allocate.size, %eax
	subq	%rax, %rsp
	movl	%edi, -4(%rbp)
	movslq	-4(%rbp),%rax
	imull	$8, %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	malloc
	movq	%rax, -16(%rbp)
	movl	$0, %eax
	movl	%eax, -8(%rbp)
.L0:
	movl	-8(%rbp), %eax
	cmpl	-4(%rbp), %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L1
	movslq	-4(%rbp),%rax
	imull	$4, %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	malloc
	movslq	-8(%rbp),%rdi
	imull	$8, %rdi
	movq	-16(%rbp), %rsi
	addl	%rdi, %rsi
	movq	%rax, (%rsi)
	movl	-8(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -8(%rbp)
	jmp	.L0
.L1:
	movq	-16(%rbp), %rax
	jmp	allocate.exit

allocate.exit:
	movq	%rbp, %rsp
	popq	%rbp
	ret

	.set	allocate.size, 16
	.globl	allocate

initialize:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$initialize.size, %eax
	subq	%rax, %rsp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	$0, %eax
	movl	%eax, -16(%rbp)
.L2:
	movl	-16(%rbp), %eax
	cmpl	-12(%rbp), %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L3
	movl	$0, %eax
	movl	%eax, -20(%rbp)
.L4:
	movl	-20(%rbp), %eax
	cmpl	-12(%rbp), %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L5
	movl	-16(%rbp), %eax
	addl	-20(%rbp), %eax
	movslq	-16(%rbp),%rdi
	imull	$8, %rdi
	movq	-8(%rbp), %rsi
	addl	%rdi, %rsi
	movq	(