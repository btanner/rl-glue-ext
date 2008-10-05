/* 
* Copyright (C) 2007, Andrew Butcher

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

#include <assert.h> /* assert  */
#include <unistd.h> /* sleep   */
#include <string.h> /* strlen */ /* I'm sorry about using strlen. */
#include <stdio.h>  /* fprintf */
#include <stdlib.h> /* calloc, getenv, exit */

#include <ctype.h> /* isdigit */
#include <netdb.h> /* gethostbyname */
#include <arpa/inet.h> /* inet_ntoa */

#include <rlglue/Environment_common.h>
#include <rlglue/network/RL_network.h>


static const char* kUnknownMessage = "Unknown Message: %s\n";

static action_t theAction                 = {0};
static state_key_t theStateKey            = {0};
static random_seed_key_t theRandomSeedKey = {0};
static rlBuffer theBuffer               = {0};
static message_t theInMessage = 0;
static unsigned int theInMessageCapacity = 0;

static void onEnvInit(int theConnection) {
  task_specification_t theTaskSpec = 0;
  unsigned int theTaskSpecLength = 0;
  unsigned int offset = 0;

  /* Read the data in the buffer (data from server) */
  /* No data to read .... */

  /* Call RL method on the recv'd data */
  theTaskSpec = env_init();
  if (theTaskSpec != NULL) {
    theTaskSpecLength = strlen(theTaskSpec);
  }

  /* Prepare the buffer for sending data back to the server */
  rlBufferClear(&theBuffer);
  offset = rlBufferWrite(&theBuffer, offset, &theTaskSpecLength, 1, sizeof(int));
  if (theTaskSpecLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, theTaskSpec, theTaskSpecLength, sizeof(char));
  }
}

static void onEnvStart(int theConnection) {
	observation_t theObservation = {0};
  	unsigned int offset = 0;

	theObservation=env_start();
	__RL_CHECK_STRUCT(&theObservation)

  rlBufferClear(&theBuffer);
  offset = rlCopyADTToBuffer(&theObservation, &theBuffer, offset);
}

static void onEnvStep(int theConnection) {
	reward_observation_t ro = {0};
	unsigned int offset = 0;

  offset = rlCopyBufferToADT(&theBuffer, offset, &theAction);
  __RL_CHECK_STRUCT(&theAction);

  ro = env_step(theAction);	
  __RL_CHECK_STRUCT(&ro.o)


  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlBufferWrite(&theBuffer, offset, &ro.terminal, 1, sizeof(terminal_t));
  offset = rlBufferWrite(&theBuffer, offset, &ro.r, 1, sizeof(reward_t));
  offset = rlCopyADTToBuffer(&ro.o, &theBuffer, offset);
}

static void onEnvCleanup(int theConnection) {
  env_cleanup();
  rlBufferClear(&theBuffer);

  free(theAction.intArray);
  free(theAction.doubleArray);
  free(theRandomSeedKey.intArray);
  free(theRandomSeedKey.doubleArray);
  free(theStateKey.intArray);
  free(theStateKey.doubleArray);
  free(theInMessage);

  theAction.intArray           = 0;
  theAction.doubleArray        = 0;
  theRandomSeedKey.intArray    = 0;
  theRandomSeedKey.doubleArray = 0;
  theStateKey.intArray         = 0;
  theStateKey.doubleArray      = 0;

  theAction.numInts = 0;
  theAction.numDoubles = 0;
  theRandomSeedKey.numInts = 0;
  theRandomSeedKey.numDoubles = 0;
  theStateKey.numInts = 0;
  theStateKey.numDoubles = 0;

  theInMessage = 0;
  theInMessageCapacity = 0;
}

static void onEnvSetState(int theConnection) {
  unsigned int offset = 0;

  offset = rlCopyBufferToADT(&theBuffer, offset, &theStateKey);
  env_set_state(theStateKey);

  rlBufferClear(&theBuffer);
}

static void onEnvSetRandomSeed(int theConnection) {
  unsigned int offset = 0;

  offset = rlCopyBufferToADT(&theBuffer, offset, &theRandomSeedKey);  
  env_set_random_seed(theRandomSeedKey);

  rlBufferClear(&theBuffer);
}

static void onEnvGetState(int theConnection) {
	state_key_t key = {0};
  unsigned int offset = 0;

  key=	env_get_state();


		__RL_CHECK_STRUCT(&key);
		rlBufferClear(&theBuffer);
  offset = rlCopyADTToBuffer(&key, &theBuffer, offset);
}

static void onEnvGetRandomSeed(int theConnection) {
	random_seed_key_t key = {0};
  unsigned int offset = 0;

key=env_get_random_seed();
	__RL_CHECK_STRUCT(&key);


  rlBufferClear(&theBuffer);
  rlCopyADTToBuffer(&key, &theBuffer, offset);
}

static void onEnvMessage(int theConnection) {
  unsigned int inMessageLength = 0;
  unsigned int outMessageLength = 0;
  message_t inMessage = 0;
  message_t outMessage = 0;
  unsigned int offset = 0;

  offset = 0;
  offset = rlBufferRead(&theBuffer, offset, &inMessageLength, 1, sizeof(int));
  if (inMessageLength >= theInMessageCapacity) {
    inMessage = (message_t)calloc(inMessageLength+1, sizeof(char));
    free(theInMessage);

    theInMessage = inMessage;
    theInMessageCapacity = inMessageLength;
  }

  if (inMessageLength > 0) {
    offset = rlBufferRead(&theBuffer, offset, theInMessage, inMessageLength, sizeof(char));
  }
/*Make sure to null terminate the string */
   theInMessage[inMessageLength]='\0';

  outMessage = env_message(theInMessage);

  if (outMessage != NULL) {
   outMessageLength = strlen(outMessage);
  }

  
  /* we want to start sending, so we're going to reset the offset to 0 so we write the the beginning of the buffer */
  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlBufferWrite(&theBuffer, offset, &outMessageLength, 1, sizeof(int));
  if (outMessageLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, outMessage, outMessageLength, sizeof(char));
  }
}

static void runEnvironmentEventLoop(int theConnection) {
  int envState = 0;

  do { 
    rlBufferClear(&theBuffer);
    rlRecvBufferData(theConnection, &theBuffer, &envState);

    switch(envState) {
    case kEnvInit:
      onEnvInit(theConnection);
      break;

    case kEnvStart:
      onEnvStart(theConnection);
      break;

    case kEnvStep:
      onEnvStep(theConnection);
      break;

    case kEnvCleanup:
      onEnvCleanup(theConnection);
      break;

    case kEnvSetState:
      onEnvSetState(theConnection);
      break;

    case kEnvSetRandomSeed:
      onEnvSetRandomSeed(theConnection);
      break;

    case kEnvGetState:
      onEnvGetState(theConnection);
      break;

    case kEnvGetRandomSeed:
      onEnvGetRandomSeed(theConnection);
      break;

    case kEnvMessage:
      onEnvMessage(theConnection);
      break;

    case kRLTerm:
      break;

    default:
      fprintf(stderr, kUnknownMessage, envState);
      exit(0);
      break;
    };

    rlSendBufferData(theConnection, &theBuffer, envState);
  } while (envState != kRLTerm);
}

int main(int argc, char** argv) {
  int theConnection = 0;

  const char *usage = "The following environment variables are used by the environment to control its function:\n"
    "RLGLUE_HOST  : If set the environment will use this ip or hostname to connect to rather than %s\n"
	"RLGLUE_PORT  : If set the environment will use this port to connect on rather than %d\n"; 
	 
  struct hostent *host_ent;

  char* host = kLocalHost;
  short port = kDefaultPort;

  char* envptr = 0;

  if (argc > 1) {
    fprintf(stderr, usage, kLocalHost, kDefaultPort);
    exit(1);
  }

  host = getenv("RLGLUE_HOST");
  if (host == 0) {
    host = kLocalHost;
  }

  envptr = getenv("RLGLUE_PORT");  
  if (envptr != 0) {
    port = strtol(envptr, 0, 10);
    if (port == 0) {
      port = kDefaultPort;
    }
  }


  if (isalpha(host[0])) {
    host_ent = gethostbyname(host); 
    host = inet_ntoa(*(struct in_addr*)host_ent->h_addr);
  }

  fprintf(stdout, "RL-Glue C Environment Codec Version %s, Build %s\n\tConnecting to host=%s on port=%d...\n", VERSION,__rlglue_get_svn_version(),host, port);
	fflush(stdout);


  /* Allocate what should be plenty of space for the buffer - it will dynamically resize if it is too small */
  rlBufferCreate(&theBuffer, 4096);
  
    theConnection = rlWaitForConnection(host, port, kRetryTimeout);
	fprintf(stdout, "\tRL-Glue C Environment Codec :: Connected\n");
    rlBufferClear(&theBuffer);
    rlSendBufferData(theConnection, &theBuffer, kEnvironmentConnection);
    runEnvironmentEventLoop(theConnection);
    rlClose(theConnection);

  rlBufferDestroy(&theBuffer);

  return 0;
}
