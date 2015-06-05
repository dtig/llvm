//===- AVRInstPrinter.h - Convert AVR MCInst to assembly syntax -*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class prints an AVR MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#ifndef __INCLUDE_AVRINSTPRINTER_H__
#define __INCLUDE_AVRINSTPRINTER_H__

#include "llvm/MC/MCInstPrinter.h"

namespace llvm
{
/*!
 * Prints AVR instructions to a textual stream.
 */
class AVRInstPrinter : public MCInstPrinter
{
public:
  AVRInstPrinter(const MCAsmInfo &MAI, const MCInstrInfo &MII,
                 const MCRegisterInfo &MRI) :
    MCInstPrinter(MAI, MII, MRI) {}

  static const char * getPrettyRegisterName(unsigned RegNo, MCRegisterInfo const& MRI);

public: // MCInstPrinter

  void printInst(const MCInst *MI,
                 raw_ostream &O,
                 StringRef Annot,
                 const MCSubtargetInfo &STI) override;

private:
  static const char *getRegisterName(unsigned RegNo);
  void printOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O);
  void print_pcrel_imm(const MCInst *MI, unsigned OpNo, raw_ostream &O);

  // Autogenerated by tblgen.
  void printInstruction(const MCInst *MI, raw_ostream &O);
  bool printAliasInstr(const MCInst *MI, raw_ostream &O);
  void printCustomAliasOperand(const MCInst *MI, unsigned OpIdx,
                               unsigned PrintMethodIdx,
                               raw_ostream &O);
};

} // end namespace llvm

#endif //__INCLUDE_AVRINSTPRINTER_H__
