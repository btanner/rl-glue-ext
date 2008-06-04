package org.rlcommunity.rlglue.ext.codecs.matlab;

/* Agent Interface 
void agent_init(const Task_specification task_spec);
Action agent_start(Observation o);
Action agent_step(Reward r, Observation o);
void agent_end(Reward r);  
void agent_cleanup();
void agent_freeze();
Message agent_message(const Message message);
 */
import java.util.logging.Level;
import java.util.logging.Logger;
import rlglue.agent.Agent;
import rlglue.types.Action;
import rlglue.types.Observation;

/**
 *
 * @author mradkie
 */

/*
 *  MatlabControl.eval(String command)
MatlabControl.feval(String command, Object[] args)
Object[] MatlabControl.blockingFeval(String command, Object[] args)
 * */
public class MatlabAgentCodec implements Agent {

    MatlabControl mc;// = new MatlabControl();
    //main for testing stuff.
    String agent_initFunc;
    String agent_startFunc;
    String agent_stepFunc;
    String agent_endFunc;
    String agent_freezeFunc;
    String agent_cleanupFunc;
    String agent_messageFunc;
    
    static private int initNum = 0;
    static private int startNum = 1;
    static private int stepNum = 2;
    static private int endNum = 3;
    static private int freezeNum = 4;
    static private int cleanupNum = 5;
    static private int messageNum = 6;

    public static void main(String[] args) {

    }

    public void test() {
        mc.setEchoEval(true);
        mc.testBlockingFeval("mtest", null);

    }

    public MatlabAgentCodec(String[] matLabAgentCode) {
        if (mc == null) {
            mc = new MatlabControl();
        }

        agent_initFunc = matLabAgentCode[initNum];
        agent_startFunc = matLabAgentCode[startNum];
        agent_stepFunc = matLabAgentCode[stepNum];
        agent_endFunc = matLabAgentCode[endNum];
        agent_freezeFunc = matLabAgentCode[freezeNum];
        agent_cleanupFunc = matLabAgentCode[cleanupNum];
        agent_messageFunc = matLabAgentCode[messageNum];
    }

    /**
     * This will be called from the Glue
     * @param task_spec
     */
    public void agent_init(String task_spec) {
        mc.testFeval(agent_initFunc, new Object[]{task_spec});
    }

    /**
     * Called from the Glue. agent_start takes in an observation as a parameter,
     * then calls the matlab agents agent_start function with the observation.
     * Agent_start returns an action, which is then passed back to the Glue.
     * 
     * @param obs
     * @return
     */
    public Action agent_start(Observation obs) {
        Action returnObj = new Action();
        try {
            Object[] args = new Object[2];
            args[0] = obs.intArray;
            args[1] = obs.doubleArray;
            returnObj = (Action)mc.blockingFeval(agent_startFunc, args);
        } catch (InterruptedException ex) {
            Logger.getLogger(MatlabAgentCodec.class.getName()).log(Level.SEVERE, null, ex);
        }

        return returnObj;
    }

    public Action agent_step(double reward, Observation obs) {
        Action returnObj = new Action();
        try {
            Object[] args = new Object[3];
            args[0] = obs.intArray;
            args[1] = obs.doubleArray;
            args[2] = reward;
            returnObj = (Action)mc.blockingFeval(agent_stepFunc, args);
        } catch (InterruptedException ex) {
            Logger.getLogger(MatlabAgentCodec.class.getName()).log(Level.SEVERE, null, ex);
        }

        return returnObj;
    }

    public void agent_end(double reward) {
        mc.testFeval(agent_endFunc, new Object[]{reward});
    }

    public void agent_cleanup() {
        mc.testEval(agent_cleanupFunc);
    }

    public void agent_freeze() {
        mc.eval(agent_freezeFunc);
    }

    public String agent_message(String message) {
        String returnMessage = "";
        try {
            Object[] args = new Object[1];
            args[0] = message;
            returnMessage = (String)mc.blockingFeval(agent_messageFunc, args);
        } catch (InterruptedException ex) {
            Logger.getLogger(MatlabAgentCodec.class.getName()).log(Level.SEVERE, null, ex);
        }

        return returnMessage;
    }
}