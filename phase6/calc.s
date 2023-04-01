lexan:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$lexan.size, %eax
	subq	%rax, %rsp
	movl	c, %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L0
	movl	$0, %eax
	call	getchar
	movl	%eax, c
.L0:
.L2:
	movl	c, %edi
	movl	$0, %eax
	call	isspace
	cmpl	$0, %eax
	je	.L4
	movslq	c,%rax
	cmpq	$10, %rax
	setne	%al
	movzbl	%al, %rax
	cmpl	$0, %eax
	je	.L4
	jmp	.L5
.L4:
	movl	$0, %eax
.L5:
	cmpl	$0, %eax
	je	.L3
	movl	$0, %eax
	call	getchar
	movl	%eax, c
	jmp	.L2
.L3:
	movl	c, %edi
	movl	$0, %eax
	call	isdigit
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L6
	movl	c, %eax
	movl	%eax, -8(%rbp)
	movl	$0, %eax
	movl	%eax, c
	movl	-8(%rbp), %eax
	jmp	lexan.exit
.L6:
	movl	$0, %eax
	movl	%eax, -4(%rbp)
.L8:
	movl	c, %edi
	movl	$0, %eax
	call	isdigit
	cmpl	$0, %eax
	je	.L9
	movl	-4(%rbp), %eax
	imull	$10, %eax
	addl	c, %eax
	subl	$48, %eax
	movl	%eax, -4(%rbp)
	movl	c, %edi
	movl	$0, %eax
	call	getchar
	movl	%eax, c
	jmp	.L8
.L9:
	movl	-4(%rbp), %eax
	movl	%eax, lexval
	movl	NUM, %eax
	jmp	lexan.exit

lexan.exit:
	movq	%rbp, %rsp
	popq	%rbp
	ret

	.set	lexan.size, 16
	.globl	lexan

match:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$match.size, %eax
	subq	%rax, %rsp
	movl	%edi, -4(%rbp)
	movl	lookahead, %eax
	cmpl	-4(%rbp), %eax
	setne	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L10
	leaq	.L12, %rax
	movl	lookahead, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	movl	$1, %edi
	movl	$0, %eax
	call	exit
.L10:
	call	lexan
	movl	%eax, lookahead

match.exit:
	movq	%rbp, %rsp
	popq	%rbp
	ret

	.set	match.size, 16
	.globl	match

factor:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$factor.size, %eax
	subq	%rax, %rsp
	movslq	lookahead,%rax
	cmpq	$40, %rax
	sete	%al
	movzbl	%al, %rax
	cmpl	$0, %eax
	je	.L13
	movq	$40, %rax
	movl	%eax, %edi
	call	match
	movl	$0, %eax
	call	expr
	movl	%eax, -4(%rbp)
	movq	$41, %rax
	movl	%eax, %edi
	call	match
	movl	-4(%rbp), %eax
	jmp	factor.exit
.L13:
	movl	lexval, %eax
	movl	%eax, -4(%rbp)
	movl	NUM, %edi
	call	match
	movl	-4(%rbp), %eax
	jmp	factor.exit

factor.exit:
	movq	%rbp, %rsp
	popq	%rbp
	ret

	.set	factor.size, 16
	.globl	factor

term:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$term.size, %eax
	subq	%rax, %rsp
	call	factor
	movl	%eax, -4(%rbp)
.L15:
	movslq	lookahead,%rax
	cmpq	$42, %rax
	sete	%al
	movzbl	%al, %rax
	cmpl	$0, %eax
	jne	.L17
	movslq	lookahead,%rax
	cmpq	$47, %rax
	sete	%al
	movzbl	%al, %rax
	cmpl	$0, %eax
	jne	.L17
	jmp	.L18
.L17:
	movl	$1, %eax
.L18:
	cmpl	$0, %eax
	je	.L16
	movslq	lookahead,%rax
	cmpq	$42, %rax
	sete	%al
	movzbl	%al, %rax
	cmpl	$0, %eax
	je	.L19
	movq	$42, %rax
	movl	%eax, %edi
	call	match
	call	factor
	movl	-4(%rbp), %edi
	imull	%eax, %edi
	movl	%edi, -4(%rbp)
	jmp	.L20
.L19:
	movq	$47, %rax
	movl	%eax, %edi
	call	match
	call	factor
	movl	%eax, -8(%rbp)
	movl	-4(%rbp), %eax
	movl	-8(%rbp), %ecx
	cltd	
	idivl	%ecx
	movl	%eax, -4(%rbp)
.L20:
	jmp	.L15
.L16:
	movl	-4(%rbp), %eax
	jmp	term.exit

term.exit:
	movq	%rbp, %rsp
	popq	%rbp
	ret

	.set	term.size, 16
	.globl	term

expr:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$expr.size, %eax
	subq	%rax, %rsp
	call	term
	movl	%eax, -4(%rbp)
.L21:
	movslq	lookahead,%rax
	cmpq	$43, %rax
	sete	%al
	movzbl	%al, %rax
	cmpl	$0, %eax
	jne	.L23
	movslq	lookahead,%rax
	cmpq	$45, %rax
	sete	%al
	movzbl	%al, %rax
	cmpl	$0, %eax
	jne	.L23
	jmp	.L24
.L23:
	movl	$1, %eax
.L24:
	cmpl	$0, %eax
	je	.L22
	movslq	lookahead,%rax
	cmpq	$43, %rax
	sete	%al
	movzbl	%al, %rax
	cmpl	$0, %eax
	je	.L25
	movq	$43, %rax
	movl	%eax, %edi
	call	match
	call	term
	movl	-4(%rbp), %edi
	addl	%eax, %edi
	movl	%edi, -4(%rbp)
	jmp	.L26
.L25:
	movq	$45, %rax
	movl	%eax, %edi
	call	match
	call	term
	movl	-4(%rbp), %edi
	subl	%eax, %edi
	movl	%edi, -4(%rbp)
.L26:
	jmp	.L21
.L22:
	movl	-4(%rbp), %eax
	jmp	expr.exit

expr.exit:
	movq	%rbp, %rsp
	popq	%rbp
	ret

	.set	expr.size, 16
	.globl	expr

main:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$main.size, %eax
	subq	%rax, %rsp
	movl	$256, %eax
	movl	%eax, NUM
	call	lexan
	movl	%eax, lookahead
.L27:
	movl	$1, %eax
	negl	%eax
	movl	lookahead, %edi
	cmpl	%eax, %edi
	setne	%dil
	movzbl	%dil, %edi
	cmpl	$0, %edi
	je	.L28
	movl	%eax, -8(%rbp)
	call	expr
	movl	%eax, -4(%rbp)
	leaq	.L29, %rax
	movl	-4(%rbp), %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
.L30:
	movslq	lookahead,%rax
	cmpq	$10, %rax
	sete	%al
	movzbl	%al, %rax
	cmpl	$0, %eax
	je	.L31
	movq	$10, %rax
	movl	%eax, %edi
	call	match
	jmp	.L30
.L31:
	jmp	.L27
.L28:

main.exit:
	movq	%rbp, %rsp
	popq	%rbp
	ret

	.set	main.size, 16
	.globl	main

	.comm	NUM, 4
	.comm	lookahead, 4
	.comm	c, 4
	.comm	lexval, 4
	.data	
.L29:	.asciz	"%d
"
.L12:	.asciz	"syntax error at %d
"
