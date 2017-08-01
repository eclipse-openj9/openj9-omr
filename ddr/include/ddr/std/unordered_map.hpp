/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2013, 2017
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

#if !defined(DDR_UNORDERED_MAP)
#define DDR_UNORDERED_MAP

#include "ddr/config.hpp"

#if defined(OMR_HAVE_CXX11)
#include <unordered_map>

#elif defined(OMR_HAVE_TR1)
#include <unordered_map>
namespace std {
using std::tr1::hash;
using std::tr1::unordered_map;
} /* namespace std */

#else
#error "Need std::unordered_map defined in TR1 or C++11."
#endif /* OMR_HAVE_TR1 */

#endif /* DDR_UNORDERED_MAP */
