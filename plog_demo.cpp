#include "bwi_kr_execution/ExecutePlanAction.h"
#include <actionlib/client/simple_action_client.h>
#include <ros/ros.h>

#include <string>
#include <fstream>

typedef actionlib::SimpleActionClient<bwi_kr_execution::ExecutePlanAction> Client;

using namespace std;

int main(int argc, char**argv) {
  ros::init(argc, argv, "plog_demo");
  ros::NodeHandle n;

  ROS_INFO("I'M RUNNING!!!");

  Client client("/action_executor/execute_plan", true);
  ROS_INFO("ABOUT TO WAIT");
  client.waitForServer();

  ROS_INFO("DONE WAITING");

  bwi_kr_execution::ExecutePlanGoal goal;
  bwi_kr_execution::AspRule rule;
  bwi_kr_execution::AspFluent fluent;
  
  string door;
  fluent.name = "not facing";
    
  system("./plog.sh"); //should be synchronous
    
  bool found_door = false;
  ifstream best_doors("answer.txt");
    
  while(getline(best_doors, door)) {
    if (door.find("max_door") == 0) {
      int start = door.find("=");
      door = door.substr(start, door.find(" ") - start);
      found_door = true;
      ROS_INFO_STREAM("FOUND DOOR " << door);
      break;
    }
  }
    
  if (found_door == false) {
    ROS_INFO("FAILED TO FIND A DOOR");
    return 0;
  }
    
  fluent.variables.push_back(door);
    
  rule.body.push_back(fluent);
  goal.aspGoal.push_back(rule);
    
  ROS_INFO("sending goal");
  client.sendGoalAndWait(goal);
    
  if (client.getState() == actionlib::SimpleClientGoalState::ABORTED) {
    ROS_INFO("Aborted");
  } else if (client.getState() == actionlib::SimpleClientGoalState::PREEMPTED) {
    ROS_INFO("Preempted");
  }
    
  else if (client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
    ROS_INFO("Succeeded!");
  } else
    ROS_INFO("Terminated");
    
  return 0;
}
