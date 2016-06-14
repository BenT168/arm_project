ldr r0, =0x20200000
mov r1, #1
lsl r1, #18
str r1, [r0, #4]

mov r2, #1
lsl r2, #16

endless:
str r2, [r0, #0x1c]
mov r1, #0x3F0000

wait1:
sub r1, r1, #1
cmp r1, #0xFF
bne wait1

str r2, [r0, #0x28]

mov r1, #0x3F0000

wait2:
sub r1, r1,#1
cmp r1, #0xFF
bne wait2

b endless

andeq r0,r0,r0
