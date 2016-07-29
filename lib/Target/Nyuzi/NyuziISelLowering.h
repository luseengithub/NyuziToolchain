//===-- NyuziISelLowering.h - Nyuzi DAG Lowering Interface -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that Nyuzi uses to lower LLVM code into
// a selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_NYUZI_NYUZIISELLOWERING_H
#define LLVM_LIB_TARGET_NYUZI_NYUZIISELLOWERING_H

#include "Nyuzi.h"
#include "llvm/Target/TargetLowering.h"

namespace llvm {
class NyuziSubtarget;

namespace NyuziISD {
enum {
  FIRST_NUMBER = ISD::BUILTIN_OP_END,
  CALL,     // A call instruction.
  RET_FLAG, // Return with a flag operand.
  SPLAT,    // Copy scalar register into all lanes of a vector
  SEL_COND_RESULT,
  RECIPROCAL_EST,
  BR_JT,
  JT_WRAPPER,
};
}

class NyuziTargetLowering : public TargetLowering {
public:
  static const NyuziTargetLowering *create(const NyuziTargetMachine &TM,
                                           const NyuziSubtarget &STI);

  explicit NyuziTargetLowering(const TargetMachine &TM,
                               const NyuziSubtarget &STI);
  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;
  MachineBasicBlock *
  EmitInstrWithCustomInserter(MachineInstr &MI,
                              MachineBasicBlock *MBB) const override;
  const char *getTargetNodeName(unsigned Opcode) const override;
  ConstraintType getConstraintType(StringRef Constraint) const override;
  std::pair<unsigned, const TargetRegisterClass *>
  getRegForInlineAsmConstraint(const TargetRegisterInfo *TRI,
                               StringRef Constraint, MVT VT) const override;
  bool isOffsetFoldingLegal(const GlobalAddressSDNode *GA) const override;
  EVT getSetCCResultType(const DataLayout &, LLVMContext &Context,
                         EVT VT) const override;
  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals, const SDLoc &,
                      SelectionDAG &DAG) const override;
  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                               bool isVarArg,
                               const SmallVectorImpl<ISD::InputArg> &Ins,
                               const SDLoc &, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;
  SDValue LowerCall(TargetLowering::CallLoweringInfo &CLI,
                    SmallVectorImpl<SDValue> &InVals) const override;
  unsigned getJumpTableEncoding() const override;
  bool isShuffleMaskLegal(const SmallVectorImpl<int> &M, EVT VT) const override;
  bool isIntDivCheap(EVT VT, AttributeSet Attr) const override;
  bool shouldInsertFencesForAtomic(const Instruction *I) const override;

private:
  MachineBasicBlock *EmitSelectCC(MachineInstr &MI,
                                  MachineBasicBlock *BB) const;
  MachineBasicBlock *EmitAtomicBinary(MachineInstr &MI, MachineBasicBlock *BB,
                                      unsigned Opcode) const;
  MachineBasicBlock *EmitAtomicCmpSwap(MachineInstr &MI,
                                       MachineBasicBlock *BB) const;

  SDValue LowerBUILD_VECTOR(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerJumpTable(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerVECTOR_SHUFFLE(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerINSERT_VECTOR_ELT(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerSELECT_CC(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerConstantPool(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerConstant(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerFDIV(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerBR_JT(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerBRIND(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerSCALAR_TO_VECTOR(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerFNEG(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerEXTRACT_VECTOR_ELT(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerSETCC(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerCTLZ_ZERO_UNDEF(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerCTTZ_ZERO_UNDEF(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerUINT_TO_FP(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerFRAMEADDR(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerRETURNADDR(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerSIGN_EXTEND_INREG(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerBlockAddress(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerVASTART(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerFABS(SDValue Op, SelectionDAG &DAG) const;

  const NyuziSubtarget &Subtarget;
};
} // end namespace llvm

#endif // NYUZI_ISELLOWERING_H
