# RUN: llvm-mc -arch=nyuzi -show-encoding %s | FileCheck %s

foo:  b target0    # CHECK: encoding: [0bAAA00000,A,A,0b1111011A]
  # CHECK: fixup A - offset: 0, value: target0, kind: fixup_Nyuzi_PCRel_Branch

  bnz s1, target1  # CHECK: encoding: [0bAAA00001,A,A,0b1111010A]
  # CHECK: #   fixup A - offset: 0, value: target1, kind: fixup_Nyuzi_PCRel_Branch

  bz s2, target2  # CHECK: encoding: [0bAAA00010,A,A,0b1111001A]
  # CHECK: #   fixup A - offset: 0, value: target2, kind: fixup_Nyuzi_PCRel_Branch

  call target5  # CHECK: encoding: [0bAAA00000,A,A,0b1111100A]
  # CHECK: #   fixup A - offset: 0, value: target5, kind: fixup_Nyuzi_PCRel_Branch

  b s2  # CHECK: encoding: [0xe2,0x03,0xf0,0xc0]

  call s3 # CHECK: encoding: [0x63,0x00,0x00,0xfc]

  eret  # CHECK: encoding: [0x00,0x00,0x00,0xfe]

target0: nop
target1: nop
target2: nop
target3: nop
target4: nop
target5: nop
