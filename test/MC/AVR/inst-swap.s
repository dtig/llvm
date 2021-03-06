; RUN: llvm-mc -triple avr-none -show-encoding < %s | FileCheck %s


foo:

  swap r31
  swap r25
  swap r5
  swap r0

; CHECK: swap r31                ; encoding: [0xf2,0x95]
; CHECK: swap r25                ; encoding: [0x92,0x95]
; CHECK: swap r5                 ; encoding: [0x52,0x94]
; CHECK: swap r0                 ; encoding: [0x02,0x94]
