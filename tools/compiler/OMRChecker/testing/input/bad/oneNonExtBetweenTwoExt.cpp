/*******************************************************************************
*
* (c) Copyright IBM Corp. 2016, 2016
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


/**
 * Description: An non-extensible class between two extensible classes,
 *    in an extensible class hierarchy, which is not allowed. 
 */

#define OMR_EXTENSIBLE __attribute__((annotate("OMR_Extensible")))

namespace OMR { class OMR_EXTENSIBLE Class1Ext {}; }
namespace TR  { class OMR_EXTENSIBLE Class1Ext : public OMR::Class1Ext {}; }

namespace OMR { class Class2NonExt : public TR::Class1Ext {}; }
namespace TR  { class Class2NonExt : public OMR::Class2NonExt {}; }

namespace OMR { class OMR_EXTENSIBLE Class3Ext : public TR::Class2NonExt {}; }
namespace TR  { class OMR_EXTENSIBLE Class3Ext : public OMR::Class3Ext {}; }
