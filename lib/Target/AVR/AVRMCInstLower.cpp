//===-- AVRMCInstLower.cpp - Convert AVR MachineInstr to an MCInst --------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains code to lower AVR MachineInstrs to their corresponding
// MCInst records.
//
//===----------------------------------------------------------------------===//

#include "AVRMCInstLower.h"

#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/IR/Mangler.h"

#include "AVRInstrInfo.h"
#include "MCTargetDesc/AVRMCExpr.h"

namespace llvm {

MCOperand AVRMCInstLower::
LowerSymbolOperand(const MachineOperand &MO, MCSymbol *Sym) const
{
  // FIXME: We would like an efficient form for this, so we don't have to do a
  // lot of extra uniquing.
  unsigned char TF = MO.getTargetFlags();
  const MCExpr *Expr = MCSymbolRefExpr::create(Sym, Ctx);

  if (TF & AVRII::MO_NEG)
  {
    Expr = MCUnaryExpr::createMinus(Expr, Ctx);
  }

  if (!MO.isJTI() && MO.getOffset())
  {
    Expr = MCBinaryExpr::createAdd(Expr, MCConstantExpr::create(MO.getOffset(),
                                                                Ctx), Ctx);
  }

  if (TF & AVRII::MO_LO)
  {
    Expr = AVRMCExpr::create(AVRMCExpr::VK_AVR_LO8, Expr, Ctx);
  }
  else if (TF & AVRII::MO_HI)
  {
    Expr = AVRMCExpr::create(AVRMCExpr::VK_AVR_HI8,Expr, Ctx);
  }
  else if (TF != 0)
  {
    llvm_unreachable("Unknown target flag on symbol operand");
  }

  return MCOperand::createExpr(Expr);
}

void AVRMCInstLower::Lower(const MachineInstr *MI, MCInst &OutMI) const
{
  OutMI.setOpcode(MI->getOpcode());

  for (unsigned i = 0, e = MI->getNumOperands(); i != e; ++i)
  {
    MCOperand MCOp;
    const MachineOperand &MO = MI->getOperand(i);

    switch (MO.getType())
    {
    default:
      MI->dump();
      llvm_unreachable("unknown operand type");
    case MachineOperand::MO_Register:
      // Ignore all implicit register operands.
      if (MO.isImplicit()) continue;
      MCOp = MCOperand::createReg(MO.getReg());
      break;
    case MachineOperand::MO_Immediate:
      MCOp = MCOperand::createImm(MO.getImm());
      break;
    case MachineOperand::MO_GlobalAddress:
      // TODO: Cleanup 3.4
      MCOp = LowerSymbolOperand(MO, Printer.getSymbol(MO.getGlobal()));
      //const GlobalValue *GV = MO.getGlobal();
      
      //MCOp = LowerSymbolOperand(MO, GetSymbolFromOperand(MO));
      break;
    case MachineOperand::MO_ExternalSymbol:
      MCOp = LowerSymbolOperand(MO,
               Printer.GetExternalSymbolSymbol(MO.getSymbolName()));
      break;
    case MachineOperand::MO_MachineBasicBlock:
      MCOp = MCOperand::createExpr(MCSymbolRefExpr::create(
        MO.getMBB()->getSymbol(), Ctx));
      break;
    case MachineOperand::MO_RegisterMask:
      continue;
    case MachineOperand::MO_BlockAddress:
      MCOp = LowerSymbolOperand(MO,
               Printer.GetBlockAddressSymbol(MO.getBlockAddress()));
      break;
      //:FIXME: readd this when needed
      /*
      case MachineOperand::MO_JumpTableIndex:
        MCOp = LowerSymbolOperand(MO, AsmPrinter.GetJTISymbol(MO.getIndex()));
        break;
      case MachineOperand::MO_ConstantPoolIndex:
        MCOp = LowerSymbolOperand(MO, AsmPrinter.GetCPISymbol(MO.getIndex()));
        break;*/
    }

    OutMI.addOperand(MCOp);
  }
}

} // end of namespace llvm
