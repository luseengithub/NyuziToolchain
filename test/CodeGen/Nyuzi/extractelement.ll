; RUN: llc %s -o - | FileCheck %s
;
; Test vector extractelement instruction
;

target triple = "nyuzi-elf-none"

define i32 @testi(<16 x i32> %a, i32 %b) { ; CHECK-LABEL: testi:
  %elem = extractelement <16 x i32> %a, i32 %b

  ; CHECK: getlane s{{[0-9]+}}, v{{[0-9]+}}, s{{[0-9]+}}

  ret i32 %elem
}

define i32 @testiimm(<16 x i32> %a, i32 %b) { ; CHECK-LABEL: testiimm:
  %elem = extractelement <16 x i32> %a, i32 7

  ; CHECK: getlane s{{[0-9]+}}, v{{[0-9]+}}, 7

  ret i32 %elem
}

define float @testf(<16 x float> %a, i32 %b) { ; CHECK-LABEL: testf:
  %elem = extractelement <16 x float> %a, i32 %b

  ; CHECK: getlane s{{[0-9]+}}, v{{[0-9]+}}, s{{[0-9]+}}

  ret float %elem
}

define float @testfimm(<16 x float> %a, i32 %b) { ; CHECK-LABEL: testfimm:
  %elem = extractelement <16 x float> %a, i32 9

  ; CHECK: getlane s{{[0-9]+}}, v{{[0-9]+}}, 9

  ret float %elem
}
