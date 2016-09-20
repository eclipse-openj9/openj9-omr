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
 * Description: Two extensible classes inherit from the same
 *    non-extensible class. This is an error because only one
 *    extensible class is allowed to inherit from a non-extensible
 *    class. However, a defect in OMRChecker causes it to think
 *    this is an infinitly recursive extensible class hierarchy,
 *    which is impossible. Hence, the error generated is that there
 *    are more than 50 extensible classes in the hierarchy.
 */

#define OMR_EXTENSIBLE __attribute__((annotate("OMR_Extensible")))

namespace OMR { class NonExtClass {}; }
namespace TR  { class NonExtClass : public OMR::NonExtClass {}; }

namespace OMR { class OMR_EXTENSIBLE FirstExtClass : public TR::NonExtClass {}; }
namespace OMR { class OMR_EXTENSIBLE SeconExtClass : public TR::NonExtClass {}; }
