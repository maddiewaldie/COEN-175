#Function:
#Prologue:
main:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$4, %rsp
#Assignment:
	movl	$10, -4(%rbp)
#Function Call:
	movl	-4(%rbp), %edi
	call	init_array
#Function Call:
	movl	-4(%rbp), %edi
	call	print_array
#Epilogue:
	movq	%rbp, %rsp
	popq	%rbp
	ret
	.globl	main
#Generating Globals:
	.comm	a, 40
