.data
stack:	.space	 2000

.text
main:
	li $t7, 0
	li $t8, 1
	li $t9, 0
	la $t0, stack
	sw $0, ($t0)
	sw $0, 4($t0)
	sw $0, 8($t0)
	addi $t8, $t8, 1
ins0:
	j ins23
	li $t8, 23
	addi $t8, $t8, 1
ins1:
	j ins2
	li $t8, 2
	addi $t8, $t8, 1
ins2:
	addi $t7, $t7, 3
	addi $t8, $t8, 1
ins3:
	li $t4, 1
	jal base
	li $t1, 5
	add $t1, $t1, $t4
	la $t0, stack
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t2, ($t0)
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	sw $t2, ($t0)
	addi $t7, $t7, 1
	addi $t8, $t8, 1
ins4:
	li $t2, 0
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	sw $t2, ($t0)
	addi $t7, $t7, 1
	addi $t8, $t8, 1
ins5:
	addi $t7, $t7, -1
	la $t0, stack
	addi $t1, $t7, -1
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t1, 4($t0)
	lw $t2, ($t0)
	bne $t1, $t2, bneget5
	li $t1, 0
	j bneEnd5
bneget5:
	li $t1, 1
bneEnd5:
	sw $t1, ($t0)
	addi $t8, $t8, 1
ins6:
	addi $t7, $t7, -1
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t1, ($t0)
	bne $t1, $0, nojump6
	j ins22
	li $t8, 22
nojump6:
	addi $t8, $t8, 1
ins7:
	li $t4, 1
	jal base
	li $t1, 3
	add $t1, $t1, $t4
	la $t0, stack
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t2, ($t0)
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	sw $t2, ($t0)
	addi $t7, $t7, 1
	addi $t8, $t8, 1
ins8:
	li $t4, 1
	jal base
	li $t1, 4
	add $t1, $t1, $t4
	la $t0, stack
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t2, ($t0)
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	sw $t2, ($t0)
	addi $t7, $t7, 1
	addi $t8, $t8, 1
ins9:
	addi $t7, $t7, -1
	la $t0, stack
	addi $t1, $t7, -1
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t1, 4($t0)
	lw $t2, ($t0)
	div $t2, $t1
	mflo $t2
	sw $t2, ($t0)
	addi $t8, $t8, 1
ins10:
	addi $t7, $t7, -1
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $s0, ($t0)
	li $t4, 1
	jal base
	li $t1, 6
	add $t1, $t1, $t4
	sll $t1, $t1, 2
	la $t0, stack
	add $t0, $t0, $t1
	sw $s0, ($t0)
	addi $t8, $t8, 1
ins11:
	li $t4, 1
	jal base
	li $t1, 3
	add $t1, $t1, $t4
	la $t0, stack
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t2, ($t0)
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	sw $t2, ($t0)
	addi $t7, $t7, 1
	addi $t8, $t8, 1
ins12:
	li $t4, 1
	jal base
	li $t1, 6
	add $t1, $t1, $t4
	la $t0, stack
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t2, ($t0)
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	sw $t2, ($t0)
	addi $t7, $t7, 1
	addi $t8, $t8, 1
ins13:
	li $t4, 1
	jal base
	li $t1, 4
	add $t1, $t1, $t4
	la $t0, stack
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t2, ($t0)
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	sw $t2, ($t0)
	addi $t7, $t7, 1
	addi $t8, $t8, 1
ins14:
	addi $t7, $t7, -1
	la $t0, stack
	addi $t1, $t7, -1
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t1, 4($t0)
	lw $t2, ($t0)
	mul $t2, $t2, $t1
	sw $t2, ($t0)
	addi $t8, $t8, 1
ins15:
	addi $t7, $t7, -1
	la $t0, stack
	addi $t1, $t7, -1
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t1, 4($t0)
	lw $t2, ($t0)
	sub $t2, $t2, $t1
	sw $t2, ($t0)
	addi $t8, $t8, 1
ins16:
	addi $t7, $t7, -1
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $s0, ($t0)
	li $t4, 1
	jal base
	li $t1, 5
	add $t1, $t1, $t4
	sll $t1, $t1, 2
	la $t0, stack
	add $t0, $t0, $t1
	sw $s0, ($t0)
	addi $t8, $t8, 1
ins17:
	li $t4, 1
	jal base
	li $t1, 4
	add $t1, $t1, $t4
	la $t0, stack
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t2, ($t0)
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	sw $t2, ($t0)
	addi $t7, $t7, 1
	addi $t8, $t8, 1
ins18:
	addi $t7, $t7, -1
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $s0, ($t0)
	li $t4, 1
	jal base
	li $t1, 3
	add $t1, $t1, $t4
	sll $t1, $t1, 2
	la $t0, stack
	add $t0, $t0, $t1
	sw $s0, ($t0)
	addi $t8, $t8, 1
ins19:
	li $t4, 1
	jal base
	li $t1, 5
	add $t1, $t1, $t4
	la $t0, stack
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t2, ($t0)
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	sw $t2, ($t0)
	addi $t7, $t7, 1
	addi $t8, $t8, 1
ins20:
	addi $t7, $t7, -1
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $s0, ($t0)
	li $t4, 1
	jal base
	li $t1, 4
	add $t1, $t1, $t4
	sll $t1, $t1, 2
	la $t0, stack
	add $t0, $t0, $t1
	sw $s0, ($t0)
	addi $t8, $t8, 1
ins21:
	j ins3
	li $t8, 3
	addi $t8, $t8, 1
ins22:
	add $t7, $t9, $0
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t8, 8($t0)
	lw $t9, 4($t0)
	beq $t8, $0, mainEnd
	jr $s4
	addi $t8, $t8, 1
ins23:
	addi $t7, $t7, 7
	addi $t8, $t8, 1
ins24:
	li $v0, 11
	addi $a0, $zero, 63
	syscall
	li $v0, 5
	syscall
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	sw $v0, ($t0)
	addi $t7, $t7, 1
	addi $t8, $t8, 1
ins25:
	addi $t7, $t7, -1
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $s0, ($t0)
	li $t4, 0
	jal base
	li $t1, 3
	add $t1, $t1, $t4
	sll $t1, $t1, 2
	la $t0, stack
	add $t0, $t0, $t1
	sw $s0, ($t0)
	addi $t8, $t8, 1
ins26:
	li $v0, 11
	addi $a0, $zero, 63
	syscall
	li $v0, 5
	syscall
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	sw $v0, ($t0)
	addi $t7, $t7, 1
	addi $t8, $t8, 1
ins27:
	addi $t7, $t7, -1
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $s0, ($t0)
	li $t4, 0
	jal base
	li $t1, 4
	add $t1, $t1, $t4
	sll $t1, $t1, 2
	la $t0, stack
	add $t0, $t0, $t1
	sw $s0, ($t0)
	addi $t8, $t8, 1
ins28:
	li $t4, 0
	jal base
	li $t1, 3
	add $t1, $t1, $t4
	la $t0, stack
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t2, ($t0)
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	sw $t2, ($t0)
	addi $t7, $t7, 1
	addi $t8, $t8, 1
ins29:
	li $t4, 0
	jal base
	li $t1, 4
	add $t1, $t1, $t4
	la $t0, stack
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t2, ($t0)
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	sw $t2, ($t0)
	addi $t7, $t7, 1
	addi $t8, $t8, 1
ins30:
	addi $t7, $t7, -1
	la $t0, stack
	addi $t1, $t7, -1
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t1, 4($t0)
	lw $t2, ($t0)
	slt $t1, $t2, $t1
	sw $t1, ($t0)
	addi $t8, $t8, 1
ins31:
	addi $t7, $t7, -1
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t1, ($t0)
	bne $t1, $0, nojump31
	j ins38
	li $t8, 38
nojump31:
	addi $t8, $t8, 1
ins32:
	li $t4, 0
	jal base
	li $t1, 3
	add $t1, $t1, $t4
	la $t0, stack
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t2, ($t0)
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	sw $t2, ($t0)
	addi $t7, $t7, 1
	addi $t8, $t8, 1
ins33:
	addi $t7, $t7, -1
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $s0, ($t0)
	li $t4, 0
	jal base
	li $t1, 5
	add $t1, $t1, $t4
	sll $t1, $t1, 2
	la $t0, stack
	add $t0, $t0, $t1
	sw $s0, ($t0)
	addi $t8, $t8, 1
ins34:
	li $t4, 0
	jal base
	li $t1, 4
	add $t1, $t1, $t4
	la $t0, stack
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t2, ($t0)
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	sw $t2, ($t0)
	addi $t7, $t7, 1
	addi $t8, $t8, 1
ins35:
	addi $t7, $t7, -1
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $s0, ($t0)
	li $t4, 0
	jal base
	li $t1, 3
	add $t1, $t1, $t4
	sll $t1, $t1, 2
	la $t0, stack
	add $t0, $t0, $t1
	sw $s0, ($t0)
	addi $t8, $t8, 1
ins36:
	li $t4, 0
	jal base
	li $t1, 5
	add $t1, $t1, $t4
	la $t0, stack
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t2, ($t0)
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	sw $t2, ($t0)
	addi $t7, $t7, 1
	addi $t8, $t8, 1
ins37:
	addi $t7, $t7, -1
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $s0, ($t0)
	li $t4, 0
	jal base
	li $t1, 4
	add $t1, $t1, $t4
	sll $t1, $t1, 2
	la $t0, stack
	add $t0, $t0, $t1
	sw $s0, ($t0)
	addi $t8, $t8, 1
ins38:
	li $t2, 1
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	sw $t2, ($t0)
	addi $t7, $t7, 1
	addi $t8, $t8, 1
ins39:
	addi $t7, $t7, -1
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $s0, ($t0)
	li $t4, 0
	jal base
	li $t1, 5
	add $t1, $t1, $t4
	sll $t1, $t1, 2
	la $t0, stack
	add $t0, $t0, $t1
	sw $s0, ($t0)
	addi $t8, $t8, 1
ins40:
	li $t4, 0
	jal base
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	sw $t4, ($t0)
	sw $t9, 4($t0)
	sw $t8, 8($t0)
	move $t9, $t7
	li $t8, 2
	la $s1, ins2
	jalr $s4, $s1
	addi $t8, $t8, 1
ins41:
	li $t4, 0
	jal base
	li $t1, 3
	add $t1, $t1, $t4
	la $t0, stack
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t2, ($t0)
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	sw $t2, ($t0)
	addi $t7, $t7, 1
	addi $t8, $t8, 1
ins42:
	li $v0, 1
	la $t0, stack
	addi $t1, $t7, -1
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $a0, ($t0)
	syscall
	addi $t7, $t7, -1
	addi $t8, $t8, 1
ins43:
	li $v0, 11
	addi $a0, $zero, 10
	syscall
	addi $t8, $t8, 1
ins44:
	add $t7, $t9, $0
	la $t0, stack
	move $t1, $t7
	sll $t1, $t1, 2
	add $t0, $t0, $t1
	lw $t8, 8($t0)
	lw $t9, 4($t0)
	beq $t8, $0, mainEnd
	jr $s4
base:
	move $t1, $t9
	la $t0, stack
baseWhile:
	bgt $t4, $0, baseGo
	j baseEnd
baseGo:
	sll $t2, $t1, 2
	add $t0, $t0, $t2
	lw $t1, ($t0)
	addi $t4, $t4, -1
	j baseWhile
baseEnd:
	move $t4, $t1
	jr $31
mainEnd:
	li $v0, 10
	syscall
