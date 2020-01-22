#include "dataset_capture/pose_grabber.h"



Pose_grabber::Pose_grabber(const std::string& ip_address)
:
abb::rws::RWSInterface(ip_address)
{
}


bool
Pose_grabber::init()
{
  // Auto mode check. Check if robot controller is in Auto mode. 
  if(!this->isAutoMode().isTrue() )
  {
    return false;
  }

  return true;
}


std::vector<double>
Pose_grabber::grab_pose(const std::string& mechunit)
{   

    /*
        RobTarget:
                - pos
                - orient
                - robconf
                - extax
    */


    abb::rws::RobTarget yumi;
    this->getMechanicalUnitRobTarget(mechunit, &yumi);


    std::vector<double> pose;
    //pose.resize(7);
    
    pose.push_back(std::stod(yumi.pos.x.constructString()));
    pose.push_back(std::stod(yumi.pos.y.constructString()));
    pose.push_back(std::stod(yumi.pos.z.constructString()));
    pose.push_back(std::stod(yumi.orient.q1.constructString()));
    pose.push_back(std::stod(yumi.orient.q2.constructString()));
    pose.push_back(std::stod(yumi.orient.q3.constructString()));
    pose.push_back(std::stod(yumi.orient.q4.constructString()));
    
    return pose;
  
}



