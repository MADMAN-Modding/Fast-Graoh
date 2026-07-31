	.assume adl=1

	.section .header.icon

	.local ___icon
___icon_jump:
	jp	__start
	.db	$02

	.global ___description
___description:
	.db	"CE C Toolchain Demo", 0
	.extern __start
