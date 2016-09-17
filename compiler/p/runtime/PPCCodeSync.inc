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
 *******************************************************************************/

#include <stdint.h>
#include <string.h>

extern "C" void *getCodeSync();

#if defined(TR_HOST_POWER)
#if defined(__ibmxl__)
#define dcbf(x)  __dcbf(x)
#define sync()  __sync()
#define isync() __isync()
#elif (__IBMC__ || __IBMCPP__)
void dcbf(unsigned char *);
void icbi(unsigned char *);
void sync();
void isync();
#pragma mc_func dcbf  {"7c0018ac"}
#pragma mc_func icbi  {"7c001fac"}
#pragma mc_func sync  {"7c0004ac"}
#pragma mc_func isync {"4c00012c"}
#pragma reg_killed_by dcbf
#pragma reg_killed_by icbi
#pragma reg_killed_by sync
#pragma reg_killed_by isync
#endif
#endif

uint32_t getPPCCacheLineSize();

#if defined(TR_HOST_POWER)
extern "C" void jitPPCDataCacheBlockZero(void *);
#endif

//Returns the L1 cache line size on PPC, which is computed dynamically for the JIT.
uint32_t getPPCCacheLineSize()
   {
#if defined(TR_HOST_POWER)
   static int32_t ppcCacheLineSize = -1;

   if (ppcCacheLineSize < 0)
      {
      int32_t  count;
      int32_t  i;
      char buf[1024];

      memset(buf, 255, 1024);
      jitPPCDataCacheBlockZero((void *) &buf[512]);

      for (i = 0, count = 0; i < 1024; i++)
         if (buf[i] == 0)
            count++;
      ppcCacheLineSize = count;
      }

   return (uint32_t) ppcCacheLineSize;
#else
return 32; //For AOT compile, a constant default is returned.
#endif
}

// make sure the code gets from the data cache to the instruction cache
void ppcCodeSync(unsigned char *codeStart, unsigned int codeSize)
   {
#if defined(TR_HOST_POWER)
	int      cacheLineSize = getPPCCacheLineSize();
	unsigned char  *addr;
	unsigned char  *limit;

        limit = (unsigned char *)(((unsigned long)codeStart + codeSize + (cacheLineSize - 1))
						/ cacheLineSize * cacheLineSize);

	// for each cache line, do a data cache block flush
	for(addr = codeStart ; addr < limit; addr += cacheLineSize)
	   {
		#if defined(__IBMC__) || defined(__IBMCPP__) || defined(__ibmxl__)
		dcbf(addr);
		#elif defined(LINUX)
		__asm__(
			"dcbf 0,%0"
			: /* no outputs */
			: "r" (addr) );
		#endif
	   }

#if defined(__IBMC__) || defined(__IBMCPP__) || defined(__ibmxl__)
	sync();
#elif defined(LINUX)
	__asm__("sync");
#endif

	limit = (unsigned char *)(((unsigned long)codeStart + codeSize + (cacheLineSize - 1))
						/ cacheLineSize * cacheLineSize);

	// for each cache line  do an icache block invalidate
	for(addr = codeStart ; addr < limit; addr += cacheLineSize)
	   {

		#if defined(__IBMC__) || defined(__IBMCPP__)
		icbi(addr);
		#elif defined(LINUX)
		__asm__(
			"icbi 0,%0"
			: /* no outputs */
			: "r" (addr) );
		#endif
      }

#if defined(__IBMC__) || defined(__IBMCPP__) || defined(__ibmxl__)
	sync();
	isync();
#elif defined(LINUX)
	__asm__("sync");
	__asm__("isync");
#endif
#endif /* TR_HOST_POWER */
   }

void *getCodeSync()
   {
   return (void *) ppcCodeSync;
   }
