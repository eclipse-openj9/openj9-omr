/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2000, 2017
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 *******************************************************************************/

#ifndef X86OPS_INLINES_INCL
#define X86OPS_INLINES_INCL

template <typename TBuffer> inline typename TBuffer::cursor_t TR_X86OpCode::OpCode_t::encode(typename TBuffer::cursor_t cursor, uint8_t rexbits) const
   {
   if (isX87())
      {
      TBuffer buffer(cursor);
      buffer.append(opcode);
      // Heuristics for X87 second byte opcode
      // It could be eliminated if GCC/MSVC fully support initializer list
      buffer.append((uint8_t)((modrm_opcode << 5) | (modrm_form << 3) | immediate_size));
      return buffer;
      }
   TBuffer buffer(cursor);
   // Prefixes
   TR::Instruction::REX rex(rexbits);
   rex.W = rex_w;
   switch (prefixes)
      {
      case PREFIX___:
         break;
      case PREFIX_66:
         buffer.append('\x66');
         break;
      case PREFIX_F2:
         buffer.append('\xf2');
         break;
      case PREFIX_F3:
         buffer.append('\xf3');
         break;
      default:
         break;
      }
   // REX
   if (rex.value() || rexbits)
      {
      buffer.append(rex);
      }
   // OpCode escape
   switch (escape)
      {
      case ESCAPE_____:
         break;
      case ESCAPE_0F__:
         buffer.append('\x0f');
         break;
      case ESCAPE_0F38:
         buffer.append('\x0f');
         buffer.append('\x38');
         break;
      case ESCAPE_0F3A:
         buffer.append('\x0f');
         buffer.append('\x3a');
         break;
      default:
         break;
      }
   // OpCode
   buffer.append(opcode);
   // ModRM
   if (modrm_form)
      {
      buffer.append(TR::Instruction::ModRM(modrm_opcode));
      }
   return buffer;
   }

inline void TR_X86OpCode::CheckAndFinishGroup07(TR_X86OpCodes op, uint8_t* cursor)
   {
   if(_binaries[op].isGroup07())
      {
      auto pModRM = (TR::Instruction::ModRM*)(cursor-1);
      switch(op)
         {
         case XEND:
            pModRM->rm = 0x05; // 0b101
            break;
         default:
            TR_ASSERT(false, "INVALID OPCODE.");
            break;
         }
      }
   }

#endif
