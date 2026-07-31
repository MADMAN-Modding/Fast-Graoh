	.section	.text,"ax",@progbits
	.assume	ADL = 1
	.file	"llvm-link"
	.section	.text._main,"ax",@progbits
	.globl	_main                           ; -- Begin function main
	.type	_main,@function
_main:                                  ; @main
; %bb.0:
	ld	hl, -45
	call	__frameset
	or	a, a
	sbc	hl, hl
	ld	de, -3600421
	ld	(ix - 39), de
	ld	e, -65
	ld	(ix - 36), de
	lea	de, ix - 30
	ld	bc, 10
	ld	(ix - 45), de
	ld	(ix - 33), de
	.local	.LBB0_1
.LBB0_1:                                ; =>This Inner Loop Header: Depth=1
	ld	(ix - 42), hl
	or	a, a
	sbc	hl, bc
	jp	nc, .LBB0_3
; %bb.2:                                ;   in Loop: Header=BB0_1 Depth=1
	ld	hl, (ix - 36)
	push	hl
	ld	hl, (ix - 39)
	push	hl
	call	_cosf
	push	hl
	pop	bc
	ld	a, e
	pop	hl
	pop	hl
	ld	hl, -3670016
	ld	e, 66
	call	__fmul
	ld	hl, 2097152
	inc	e
	call	__fadd
	call	__ftol
	ld	hl, (ix - 33)
	ld	(hl), bc
	ld	hl, (ix - 36)
	push	hl
	ld	hl, (ix - 39)
	push	hl
	call	_sinf
	push	hl
	pop	bc
	ld	a, e
	pop	hl
	pop	hl
	ld	hl, -3670016
	ld	e, 66
	call	__fmul
	ld	hl, -1048576
	call	__fadd
	call	__ftol
	ld	iy, (ix - 33)
	ld	(iy + 3), bc
	ld	bc, (ix - 39)
	ld	hl, (ix - 36)
	ld	a, l
	ld	hl, 2152828
	ld	e, 64
	call	__fadd
	ld	(ix - 39), bc
	ld	bc, 10
	ld	l, a
	ld	(ix - 36), hl
	ld	de, 2
	ld	hl, (ix - 42)
	add	hl, de
	ld	iy, (ix - 33)
	lea	iy, iy + 6
	ld	(ix - 33), iy
	jp	.LBB0_1
	.local	.LBB0_3
.LBB0_3:
	call	_gfx_Begin
	or	a, a
	sbc	hl, hl
	push	hl
	call	_gfx_SetColor
	pop	hl
	ld	hl, 5
	push	hl
	ld	hl, (ix - 45)
	push	hl
	call	_gfx_Polygon
	pop	hl
	pop	hl
	.local	.LBB0_4
.LBB0_4:                                ; =>This Inner Loop Header: Depth=1
	call	_os_GetCSC
	or	a, a
	jr	z, .LBB0_4
; %bb.5:
	ld	hl, 229
	push	hl
	ld	hl, 120
	push	hl
	ld	hl, 160
	push	hl
	call	_gfx_FloodFill
	pop	hl
	pop	hl
	pop	hl
	.local	.LBB0_6
.LBB0_6:                                ; =>This Inner Loop Header: Depth=1
	call	_os_GetCSC
	or	a, a
	jr	z, .LBB0_6
; %bb.7:
	ld	hl, 9
	push	hl
	or	a, a
	sbc	hl, hl
	push	hl
	push	hl
	call	_gfx_FloodFill
	pop	hl
	pop	hl
	pop	hl
	.local	.LBB0_8
.LBB0_8:                                ; =>This Inner Loop Header: Depth=1
	call	_os_GetCSC
	or	a, a
	jr	z, .LBB0_8
; %bb.9:
	call	_gfx_End
	or	a, a
	sbc	hl, hl
	ld	sp, ix
	pop	ix
	ret
	.local	.Lfunc_end0
.Lfunc_end0:
	.size	_main, .Lfunc_end0-_main
                                        ; -- End function
	.ident	"clang version 19.1.0 (https://github.com/CE-Programming/llvm-project ef28e9c54cd1333a6091ab2ffbd315b465fc5090)"
	.section	".note.GNU-stack","",@progbits
	.extern	_cosf
	.extern	__Unwind_SjLj_Unregister
	.extern	__fmul
	.extern	_gfx_Polygon
	.extern	_gfx_FloodFill
	.extern	_llvm.sin.f32
	.extern	_llvm.stackrestore.p0
	.extern	_gfx_SetColor
	.extern	_os_GetCSC
	.extern	_gfx_End
	.extern	_llvm.eh.sjlj.setup.dispatch
	.extern	_llvm.cos.f32
	.extern	__frameset
	.extern	_llvm.stacksave.p0
	.extern	_llvm.eh.sjlj.functioncontext
	.extern	__ftol
	.extern	_llvm.fmuladd.f32
	.extern	__fadd
	.extern	_llvm.eh.sjlj.callsite
	.extern	_llvm.lifetime.end.p0
	.extern	_llvm.lifetime.start.p0
	.extern	_llvm.eh.sjlj.lsda
	.extern	_sinf
	.extern	_llvm.frameaddress.p0
	.extern	__Unwind_SjLj_Register
	.extern	_gfx_Begin
