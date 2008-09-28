/* 
* Copyright (C) 2008, Brian Tanner

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
* 
*  $Revision$
*  $Date$
*  $Author$
*  $HeadURL$
* 
*/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h> /* free, calloc */
#include <string.h> /* memcpy */
#include <rlglue/utils/C/RLStruct_util.h>

/**	
*	Sept 8 2008, Brian Tanner is creating replace function
*	This one takes a src and dst, and puts all the data from the src into the dst
*	Freeing and reallocating if necessary
**/
void replaceRLStruct(const rl_abstract_type_t *src, rl_abstract_type_t *dst){
	if(dst->numInts!=src->numInts){
		if(dst->numInts>0 && dst->intArray!=0){
			free(dst->intArray);
		}
		dst->numInts=src->numInts;
		dst->intArray=(int *)calloc(dst->numInts,sizeof(int));
	}
	if(src->numInts>0){
		memcpy(dst->intArray, src->intArray,dst->numInts);
	}

	if(dst->numDoubles!=src->numDoubles){
		if(dst->numDoubles>0 && dst->doubleArray!=0){
			free(dst->doubleArray);
		}
		dst->numDoubles=src->numDoubles;
		dst->doubleArray=(double *)calloc(dst->numDoubles,sizeof(double));
	}
	if(src->numDoubles>0){
		memcpy(dst->doubleArray, src->doubleArray,dst->numDoubles);
	}

	if(dst->numChars!=src->numChars){
		if(dst->numChars>0 && dst->charArray!=0){
			free(dst->charArray);
		}
		dst->numChars=src->numChars;
		dst->charArray=(char *)calloc(dst->numChars,sizeof(char));
	}
	if(src->numChars>0){
		memcpy(dst->charArray, src->charArray,dst->numChars);
	}	
}

/**
Created by Brian Tanner on Sept 27, 2008.
I thought this might be handy for people
*/
void clearRLStruct(rl_abstract_type_t *dst){
	if(dst->intArray!=0){
		free(dst->intArray);
	}
	dst->intArray=0;

	if(dst->doubleArray!=0){
		free(dst->doubleArray);
	}
	dst->doubleArray=0;
	if(dst->charArray!=0){
		free(dst->charArray);
	}
	dst->charArray=0;

	dst->numInts=0;
	dst->numDoubles=0;
	dst->numChars=0;
	
}

void freeRLStructPointer(rl_abstract_type_t *dst){
	if(dst!=0){
		clearRLStruct(dst);
		free(dst);
	}
}

void allocateRLStruct(rl_abstract_type_t *dst, const unsigned int numInts, const unsigned int numDoubles, const unsigned int numChars){
	if(dst!=0){
		clearRLStruct(dst);
	}
	dst->numInts=numInts;
	dst->numDoubles=numDoubles;
	dst->numChars=numChars;
	
	if(dst->numInts!=0)
		dst->intArray=(int *)calloc(dst->numInts,sizeof(int));

	if(dst->numDoubles!=0)
		dst->doubleArray=(double *)calloc(dst->numDoubles,sizeof(double));

	if(dst->numChars!=0)
		dst->charArray=(char *)calloc(dst->numChars,sizeof(char));
}

rl_abstract_type_t *allocateRLStructPointer(const unsigned int numInts, const unsigned int numDoubles, const unsigned int numChars){
	rl_abstract_type_t *dst=(rl_abstract_type_t *)calloc(1,sizeof(rl_abstract_type_t));
	allocateRLStruct(dst,numInts,numDoubles,numChars);
	return dst;
}
