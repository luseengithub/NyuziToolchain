#include "VectorProcTargetInfo.h"

#include "lld/Core/LLVM.h"

#include "llvm/ADT/StringSwitch.h"
#include "llvm/Support/ErrorOr.h"

using namespace lld;

#define LLD_CASE(name) .Case(#name, llvm::ELF::name)

ErrorOr<Reference::Kind> elf::VectorProcTargetInfo::relocKindFromString(
    StringRef str) const {
  int32_t ret = llvm::StringSwitch<int32_t>(str)
    LLD_CASE(R_VECTORPROC_BRANCH)
    LLD_CASE(R_VECTORPROC_ABS32)
    .Default(-1);

  if (ret == -1)
    return make_error_code(yaml_reader_error::illegal_value);
  return ret;
}

#undef LLD_CASE

#define LLD_CASE(name) case llvm::ELF::name: return std::string(#name);

ErrorOr<std::string> 
elf::VectorProcTargetInfo::stringFromRelocKind(Reference::Kind kind) const {
  switch (kind) {
  LLD_CASE(R_VECTORPROC_BRANCH)
  LLD_CASE(R_VECTORPROC_ABS32)
  }

  return make_error_code(yaml_reader_error::illegal_value);
}
