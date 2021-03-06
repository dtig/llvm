//===-- AVRMCAsmInfo.h - AVR asm properties ---------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the AVRMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_AVR_ASM_INFO_H
# define LLVM_AVR_ASM_INFO_H

# include "AVRConfig.h"

# include "llvm/MC/MCAsmInfo.h"

namespace llvm {

class StringRef;
class Triple;

class AVRMCAsmInfo : public MCAsmInfo {
public:
  explicit AVRMCAsmInfo(const Triple &TT);
};

} // end namespace llvm

#endif // LLVM_AVR_ASM_INFO_H
