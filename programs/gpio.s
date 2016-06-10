ldr r0, =0x20200000
mov r1, #1
lsl r1, #18
str r1, [r0, #4]

mov r1, #1
lsl r1, #16
str r1, [r0, #0x1c]
mov r2, #0

endless:

cmp r2, #0
bne else
str r1, [r0, #0x28]
mov r2, #1
b wait
else:
str r1, [r0, #0x1c]
mov r2, #0

wait:
mov r4, #0x00c00000

loop:
sub r4, r4, #1
cmp r4, #1
bge loop

b endless
