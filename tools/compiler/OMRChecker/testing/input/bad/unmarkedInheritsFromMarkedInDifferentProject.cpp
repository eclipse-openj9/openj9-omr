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
 * Description: An annotated extensible class is direct parent of
 *    of a class that is not annotated, which is not allowed as
 *    it breaks the extensible class.
 */

#define OMR_EXTENSIBLE __attribute__((annotate("OMR_Extensible")))

namespace OMR { class OMR_EXTENSIBLE Class1Marked {}; }
namespace IBM { class Class1Unmarked : public OMR::Class1Marked {}; }

