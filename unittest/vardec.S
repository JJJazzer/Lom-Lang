/*
 * file name: vardec.S
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Sat 23 Mar 2019 03:28:43 PM CST
 */


.section .data 
tmp_int:
	.int 	1
tmp_string:
	.asciz  "Hello"

.section .text
.globl main
main:
	
