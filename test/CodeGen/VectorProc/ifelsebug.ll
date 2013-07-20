; RUN: llc %s -o - | FileCheck %s
; XFAIL:

target triple = "vectorproc"

define i32 @_Z3fibi(i32 %n) {
entry:
  %cmp = icmp slt i32 %n, 2
  br i1 %cmp, label %return, label %if.else

; CHECK: btrue {{s[0-9]+}}, [[TRUELABEL:[A-Z0-9a-z_]+]]

if.else:                                          ; preds = %entry
  %sub = add nsw i32 %n, -1
  %call = tail call i32 @_Z3fibi(i32 %sub)
  %sub1 = add nsw i32 %n, -2
  %call2 = tail call i32 @_Z3fibi(i32 %sub1)
  %add = add nsw i32 %call2, %call

; CHECK:	call _Z3fibi
; CHECK:	goto [[EXITLABEL:[A-Z0-9a-z_]+]]

  ret i32 %add

; CHECK: [[TRUELABEL]]:                          
; CHECK: [[EXITLABEL]]:                          

return:                                           ; preds = %entry
  ret i32 %n
}	