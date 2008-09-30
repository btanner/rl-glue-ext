# 
# Copyright (C) 2007, Mark Lee
# 
#http://rl-glue-ext.googlecode.com/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import random
import sys
from rlglue.environment.Environment import Environment
from rlglue.environment import EnvironmentLoader as EnvironmentLoader
from rlglue.types import Observation
from rlglue.types import Action
from rlglue.types import Reward_observation
from rlglue.types import State_key
from rlglue.types import Random_seed_key

class test_message_environment(Environment):

	def env_init(self):  
		return ""

	def env_start(self):
		return Observation()
	
	def env_step(self,action):
		return Reward_observation()

	def env_cleanup(self):
		pass
	
	def env_set_state(self, stateKey):
		pass
	
	def env_set_random_seed(self, randomSeedKey):
		pass
	
	def env_get_state(self):
		return State_key()
	
	def env_get_random_seed(self):
		return Random_seed_key()
	
	def env_message(self,inMessage):
		if inMessage==None:
			return "null"

		if inMessage=="":
			return "empty"

		if inMessage=="null":
			return None

		if inMessage=="empty":
			return ""
		
		return inMessage;	

if __name__=="__main__":
	EnvironmentLoader.loadEnvironment(test_message_environment())