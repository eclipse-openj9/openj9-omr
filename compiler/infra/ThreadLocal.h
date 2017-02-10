/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2000, 2016
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
 ******************************************************************************/

#ifndef THREADLOCAL_INCL
#define THREADLOCAL_INCL

#if defined(SUPPORTS_THREAD_LOCAL)

#if defined(WINDOWS) || defined(LINUX) || defined(OSX) || defined(AIXPPC)
 #if defined(WINDOWS)
  #include "windows.h"
  #define tlsDeclare(type, variable) extern DWORD variable
  #define tlsDefine(type, variable) DWORD variable = TLS_OUT_OF_INDEXES
  #define tlsAlloc(variable) if ((variable) == TLS_OUT_OF_INDEXES) { (variable) = TlsAlloc();}
  #define tlsFree(variable)  (TlsFree(variable))
  #define tlsSet(variable, value) TlsSetValue((variable), value)
  #define tlsGet(variable, type) ((type)TlsGetValue(variable))
 #else /* if defined(LINUX) || defined(AIXPPC) */
  #define tlsDeclare(type, variable) extern __thread type variable
  #define tlsDefine(type, variable) __thread type variable = NULL
  #define tlsAlloc(variable) // not required on win, linux or AIX
  #define tlsFree(variable)  // not required on win, linux or AIX
  #define tlsSet(variable, value) variable = value
  #define tlsGet(variable, type) (variable)
 #endif
#else /* !(defined(WINDOWS) || defined(LINUX) || defined(OSX) || defined(AIXPPC)) */  /* mainly defined(J9ZOS390) */
 #include <pthread.h>

 #define tlsDeclare(type, variable) extern pthread_key_t variable
 #define tlsDefine(type, variable) pthread_key_t variable
 #define tlsAlloc(variable) pthread_key_create(&variable, NULL)   // allocates a new key, which will have initial value NULL for all threads
 #define tlsFree(variable) pthread_key_delete(variable)
 #define tlsSet(variable, value) pthread_setspecific(variable, value)
 #if defined(J9ZOS390)
  #define tlsGet(variable, type) ((type) pthread_getspecific_d8_np(variable))
 #else
  #define tlsGet(variable, type) ((type) pthread_getspecific(variable))
 #endif
#endif

#else /* !defined(SUPPORTS_THREAD_LOCAL) */
 #define tlsDeclare(type, variable) extern type variable
 #define tlsDefine(type, variable) type variable = NULL
 #define tlsAlloc(variable)
 #define tlsFree(variable)
 #define tlsSet(variable, value) variable = value
 #define tlsGet(variable, type) (variable)
#endif

#endif /* THREADLOCAL_INCL */
