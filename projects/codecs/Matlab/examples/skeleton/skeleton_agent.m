%  Copyright 2008 Brian Tanner
%  http://rl-glue-ext.googlecode.com/
%  brian@tannerpages.com
%  http://brian.tannerpages.com
%  
%   Licensed under the Apache License, Version 2.0 (the "License");
%  you may not use this file except in compliance with the License.
%   You may obtain a copy of the License at
%  
%       http://www.apache.org/licenses/LICENSE-2.0
%  
%   Unless required by applicable law or agreed to in writing, software
%   distributed under the License is distributed on an "AS IS" BASIS,
%   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
%   See the License for the specific language governing permissions and
%   limitations under the License.
%  
%   $Revision$
%   $Date$
%   $Author$
%  $HeadURL$
%
function theAgent=skeleton_agent()
    theAgent.agent_init=@skeleton_agent_init;
    theAgent.agent_start=@skeleton_agent_start;
    theAgent.agent_step=@skeleton_agent_step;
    theAgent.agent_end=@skeleton_agent_end;
    theAgent.agent_cleanup=@skeleton_agent_cleanup;
    theAgent.agent_message=@skeleton_agent_message;

	fprintf(1,'Sample Agent.  Remember that currently the Matlab codec can only run\n');
	fprintf(1,'ONE agent, environment, and experiment per Matlab instance.  To connect this agen\n');
	fprintf(1,'to anything you will need to run them in a separate Matlab instance or using a different codec.\n');
	fprintf(1,'If running them all in one Matlab instance is very important to you, please go to the \n');
	fprintf(1,'following URL and click the star to show your interest:\n');
	fprintf(1,'http://code.google.com/p/rl-glue-ext/issues/detail?id=57\n\n');
end

function skeleton_agent_init(taskSpec)

end    

function theAction=skeleton_agent_start(theObservation)
%This is a persistent struct we will use to store things
%that we want to keep around
    global skeleton_agent_struct;

    theAction = org.rlcommunity.rlglue.codec.types.Action();
	theAction.intArray=[round(rand(1))];

	%Make copies (using Java methods) of the observation and action
	%Store in our persistent struct
	skeleton_agent_struct.lastAction=theAction.duplicate();
	skeleton_agent_struct.lastObservation=theObservation.duplicate();	
end

function theAction=skeleton_agent_step(theReward, theObservation)
	%This is a persistent struct we will use to store things
	%that we want to keep around
	    global skeleton_agent_struct;

	    theAction = org.rlcommunity.rlglue.codec.types.Action();
		theAction.intArray=[round(rand(1))];

		%Make copies (using Java methods) of the observation and action
		%Store in our persistent struct
		skeleton_agent_struct.lastAction=theAction.duplicate();
		skeleton_agent_struct.lastObservation=theObservation.duplicate();
end

function skeleton_agent_end(theReward)
end

function returnMessage=skeleton_agent_message(theMessageJavaObject)
%Java strings are objects, and we want a Matlab string
    inMessage=char(theMessageJavaObject);

	if strcmp(inMessage,'what is your name?')==1
		returnMessage='my name is skeleton_agent, Matlab edition!';
    else
		returnMessage='I don\''t know how to respond to your message';
	end
end

function skeleton_agent_cleanup()
    global skeleton_agent_struct;
end