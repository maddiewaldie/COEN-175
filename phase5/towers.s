#Function:
#Prologue:
towers:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$0, %rsp
#Function Call:
	movl	16(%rbp), %ecx
	movl	20(%rbp), %edx
	movl	12(%rbp), %esi
	movl	8(%rbp), %edi
	call	call_towers
#Function Call:
	movl	16(%rbp), %esi
	movl	12(%rbp), %edi
	call	print_move
#Function Call:
	movl	12(%rbp), %ecx
	movl	16(%rbp), %edx
	movl	20(%rbp), %esi
	movl	8(%rbp), %edi
	call	call_towers
#Epilogue:
	movq	%rbp, %rsp
	popq	%rbp
	ret
	.globl	towers
#Function:
#Prologue:
main:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$4, %rsp
#Assignment:
	movl	$3, -4(%rbp)
#Function Call:
	movl	-4(%rbp), %edi
	call	print
#Function Call:
	movl	$3, %ecx
	movl	$2, %edx
	movl	$1, %esi
	movl	-4(%rbp), %edi
	call	towers
#Epilogue:
	movq	%rbp, %rsp
	popq	%rbp
	ret
	.globl	main
#Generating Globals:
