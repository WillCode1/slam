#include <ros/ros.h>
#include "slam_interfaces/SlamMode.h"

ros::Publisher map_topic_pub;

std::string execCommand(const std::string &command)
{
    std::array<char, 128> buffer;
    std::string result;

    ROS_INFO("exec \"%s\"", command.c_str());
    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}

std::string kill_progress_name = "";

bool slam_callback(slam_interfaces::SlamModeRequest &request, slam_interfaces::SlamModeResponse &response)
{
    ROS_INFO("The slam request received. map name = %s, action = %s.", request.map_name.c_str(), request.action.c_str());

    if (request.action.compare("mapping") == 0)
    {
        ros::param::set("official/map_path", request.map_name);
        kill_progress_name = "fastlio_sam_ros1";
        execCommand("gnome-terminal -- bash -c \"roslaunch fast_lio_sam run.launch\"");
    }
    else if (request.action.compare("stitch") == 0)
    {
        ros::param::set("official/prior_map_path", request.map1_name);
        ros::param::set("official/stitch_map_path", request.map2_name);
        ros::param::set("official/result_map_path", request.map_name);
        kill_progress_name = "map_stitch";
        execCommand("gnome-terminal -- bash -c \"roslaunch fast_lio_sam map_stitch.launch\"");
    }
    else if (request.action.compare("localization") == 0)
    {
        ros::param::set("official/map_path", request.map_name);
        kill_progress_name = "fastlio_localization_ros1";
        execCommand("gnome-terminal -- bash -c \"roslaunch fastlio_localization run_service.launch\"");
    }
    else if (kill_progress_name.compare("") == 0)
    {
        response.result = request.action + "_recv";
        return true;
    }
    else if (request.action.compare("finish") == 0)
    {
        execCommand("gnome-terminal -- bash -c \"killall -2 " + kill_progress_name + "\"");
        // execCommand("gnome-terminal -- bash -c \"killall -2 gnome-terminal-\"");
    }
    else if (request.action.compare("cancel") == 0)
    {
        execCommand("gnome-terminal -- bash -c \"killall -2 " + kill_progress_name + "\"");
    }

    response.result = request.action + "_recv";
    return true;
}

int main(int argc, char **argv)
{
    ros::init(argc,argv,"slam_server");
    ros::NodeHandle nh;
    ros::ServiceServer server = nh.advertiseService("slam_service", slam_callback);
    ROS_INFO("Slam service ready!");

    ros::spin();
    return 0;
}
