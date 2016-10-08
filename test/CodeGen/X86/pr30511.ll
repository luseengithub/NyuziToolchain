; RUN: llc < %s -mtriple=x86_64-pc-linux-gnu | FileCheck %s

target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i64 @PR30511(<2 x double> %a) {
; CHECK:       # BB#0:
; CHECK-NEXT:    addpd {{.*}}(%rip), %xmm0
; CHECK-NEXT:    pshufd {{.*#+}} xmm0 = xmm0[0,2,2,3]
; CHECK-NEXT:    cvtdq2pd %xmm0, %xmm0
; CHECK-NEXT:    mulpd {{.*}}(%rip), %xmm0
; CHECK-NEXT:    movd %xmm0, %rax
; CHECK-NEXT:    retq
  %1 = fadd <2 x double> %a, <double 0x4338000000000000, double 0x4338000000000000>
  %2 = bitcast <2 x double> %1 to <2 x i64>
  %3 = trunc <2 x i64> %2 to <2 x i32>
  %4 = sitofp <2 x i32> %3 to <2 x double>
  %5 = fmul <2 x double> %4, <double 0x3E76800000000000, double 0x3E76800000000000>
  %6 = extractelement <2 x double> %5, i32 0
  %7 = bitcast double %6 to i64
  ret i64 %7
}

