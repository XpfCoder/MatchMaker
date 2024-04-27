#ifndef __PXLASSERT_H_8k2ko__
#define __PXLASSERT_H_8k2ko__


#include "../SafeHelper.h"
#include "../../MatchMaker/Utility.h"


#ifndef lightAssert
#	ifdef _DEBUG
		#define lightAssert(exp)		(int)( (exp) || ((PAF_ASSERT(exp),0)
#	else
		#define lightAssert(exp)		((int)1)
#	endif
#endif

//不管是否release，都要判断
#undef sysAssert

#ifdef _DEBUG
	#define sysAssert(exp)		(int)( (exp) || (PAF_ASSERT(exp) ), 0)

	#define strongAssert(exp)	(int)( (exp) || (PAF_ASSERT(exp) , 0) )

	#define alwaysAssert(exp)	(int)( (exp) || (PAF_ASSERT(exp) , 0) )
#else
	#define sysAssert(exp)		(int)( (exp) || (PAF_ASSERT(exp) , 0) )

	#define strongAssert(exp)	(int)( (exp) || (PAF_ASSERT(exp) , 0) )

	#define alwaysAssert(exp)	(int)( (exp) || (PAF_ASSERT(exp) , 0) )
#endif

#endif
