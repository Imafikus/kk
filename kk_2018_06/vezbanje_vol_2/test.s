	.text
	.file	"my module"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movl	$.L__unnamed_1, %edi
	movl	$14, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$.L__unnamed_1, %edi
	movl	$232, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$.L__unnamed_1, %edi
	movl	$128, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$.L__unnamed_1, %edi
	movl	$64, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$-1, 12(%rsp)
	movl	$508, 16(%rsp)                  # imm = 0x1FC
	movl	$-1, 20(%rsp)
	movl	$.L__unnamed_1, %edi
	movl	$2556, %esi                     # imm = 0x9FC
	xorl	%eax, %eax
	callq	printf@PLT
	xorl	%eax, %eax
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	.L__unnamed_1,@object           # @0
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_1:
	.asciz	"%u\n"
	.size	.L__unnamed_1, 4

	.type	.L__unnamed_2,@object           # @1
.L__unnamed_2:
	.asciz	"0x%x\n"
	.size	.L__unnamed_2, 6

	.section	".note.GNU-stack","",@progbits
