//===-- HexagonSubtarget.h - Define Subtarget for the Hexagon ---*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the Hexagon specific subclass of TargetSubtarget.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_HEXAGON_HEXAGONSUBTARGET_H
#define LLVM_LIB_TARGET_HEXAGON_HEXAGONSUBTARGET_H

#include "HexagonFrameLowering.h"
#include "HexagonISelLowering.h"
#include "HexagonInstrInfo.h"
#include "HexagonSelectionDAGInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetSubtargetInfo.h"
#include <string>

#define GET_SUBTARGETINFO_HEADER
#include "HexagonGenSubtargetInfo.inc"

#define Hexagon_SMALL_DATA_THRESHOLD 8
#define Hexagon_SLOTS 4

namespace llvm {

class HexagonSubtarget : public HexagonGenSubtargetInfo {
  virtual void anchor();

  bool UseMemOps, UseHVXOps, UseHVXDblOps;
  bool ModeIEEERndNear;

public:
  enum HexagonArchEnum {
    V4, V5, V55, V60
  };

  HexagonArchEnum HexagonArchVersion;
  /// True if the target should use Back-Skip-Back scheduling. This is the
  /// default for V60.
  bool UseBSBScheduling;

private:
  std::string CPUString;
  HexagonInstrInfo InstrInfo;
  HexagonTargetLowering TLInfo;
  HexagonSelectionDAGInfo TSInfo;
  HexagonFrameLowering FrameLowering;
  InstrItineraryData InstrItins;
  void initializeEnvironment();

public:
  HexagonSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                   const TargetMachine &TM);

  /// getInstrItins - Return the instruction itineraries based on subtarget
  /// selection.
  const InstrItineraryData *getInstrItineraryData() const override {
    return &InstrItins;
  }
  const HexagonInstrInfo *getInstrInfo() const override { return &InstrInfo; }
  const HexagonRegisterInfo *getRegisterInfo() const override {
    return &InstrInfo.getRegisterInfo();
  }
  const HexagonTargetLowering *getTargetLowering() const override {
    return &TLInfo;
  }
  const HexagonFrameLowering *getFrameLowering() const override {
    return &FrameLowering;
  }
  const HexagonSelectionDAGInfo *getSelectionDAGInfo() const override {
    return &TSInfo;
  }

  HexagonSubtarget &initializeSubtargetDependencies(StringRef CPU,
                                                    StringRef FS);

  /// ParseSubtargetFeatures - Parses features string setting specified
  /// subtarget options.  Definition of function is auto generated by tblgen.
  void ParseSubtargetFeatures(StringRef CPU, StringRef FS);

  bool useMemOps() const { return UseMemOps; }
  bool hasV5TOps() const { return getHexagonArchVersion() >= V5; }
  bool hasV5TOpsOnly() const { return getHexagonArchVersion() == V5; }
  bool hasV55TOps() const { return getHexagonArchVersion() >= V55; }
  bool hasV55TOpsOnly() const { return getHexagonArchVersion() == V55; }
  bool hasV60TOps() const { return getHexagonArchVersion() >= V60; }
  bool hasV60TOpsOnly() const { return getHexagonArchVersion() == V60; }
  bool modeIEEERndNear() const { return ModeIEEERndNear; }
  bool useHVXOps() const { return UseHVXOps; }
  bool useHVXDblOps() const { return UseHVXOps && UseHVXDblOps; }
  bool useHVXSglOps() const { return UseHVXOps && !UseHVXDblOps; }

  bool useBSBScheduling() const { return UseBSBScheduling; }
  bool enableMachineScheduler() const override;
  // Always use the TargetLowering default scheduler.
  // FIXME: This will use the vliw scheduler which is probably just hurting
  // compiler time and will be removed eventually anyway.
  bool enableMachineSchedDefaultSched() const override { return false; }

  AntiDepBreakMode getAntiDepBreakMode() const override { return ANTIDEP_ALL; }
  bool enablePostRAScheduler() const override { return true; }

  bool enableSubRegLiveness() const override;

  const std::string &getCPUString () const { return CPUString; }

  // Threshold for small data section
  unsigned getSmallDataThreshold() const {
    return Hexagon_SMALL_DATA_THRESHOLD;
  }
  const HexagonArchEnum &getHexagonArchVersion() const {
    return HexagonArchVersion;
  }
};

} // end namespace llvm

#endif
