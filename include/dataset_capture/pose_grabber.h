#include "abb_librws/rws_interface.h"
#include "abb_librws/rws_rapid.h"
#include <vector>
#include <string>



class Pose_grabber : public abb::rws::RWSInterface
{

  public:

    Pose_grabber(const std::string& ip_address);

    // Connect to YuMi
    bool init();

    // Grabs pose and delivers it as a (x, y, z, q1, q2, q3, q4)
    std::vector<double> grab_pose(const std::string& mechunit);

    //default destructor
    ~Pose_grabber(){};

 
};