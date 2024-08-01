#include <rclcpp/rclcpp.hpp>
#include "slam_interfaces/srv/slam_mode.hpp"

std::shared_ptr<rclcpp::Node> node;

std::string execCommand(const std::string &command)
{
    std::array<char, 128> buffer;
    std::string result;

    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "exec \"%s\"", command.c_str());
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

void slam_callback(const slam_interfaces::srv::SlamMode::Request::SharedPtr request,
                   slam_interfaces::srv::SlamMode::Response::SharedPtr response)
{
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "The slam request received. map name = %s, action = %s.", request->map_name.c_str(), request->action.c_str());

    if (request->action.compare("mapping") == 0)
    {
        kill_progress_name = "fastlio_sam_ros1";
        execCommand("gnome-terminal -- bash -c \"ros2 launch fast_lio_sam run_launch.py\"");
    }
    else if (request->action.compare("stitch") == 0)
    {
        // node->set_parameter(rclcpp::Parameter("prior_map_path", request->map1_name));
        // node->set_parameter(rclcpp::Parameter("stitch_map_path", request->map2_name));
        // node->set_parameter(rclcpp::Parameter("result_map_path", request->map_name));
        kill_progress_name = "map_stitch";
        // execCommand("gnome-terminal -- bash -c \"ros2 launch fast_lio_sam map_stitch.py\"");
    }
    else if (request->action.compare("localization") == 0)
    {
        node->set_parameter(rclcpp::Parameter("map_path", request->map_name));
        kill_progress_name = "fastlio_localization_ros1";
        execCommand("gnome-terminal -- bash -c \"ros2 launch fastlio_localization run_launch.py\"");
    }
    else if (kill_progress_name.compare("") == 0)
    {
        response->result = request->action + "_recv";
    }
    else if (request->action.compare("finish") == 0)
    {
        execCommand("gnome-terminal -- bash -c \"killall -2 " + kill_progress_name + "\"");
        // execCommand("gnome-terminal -- bash -c \"killall -2 gnome-terminal-\"");
    }
    else if (request->action.compare("cancel") == 0)
    {
        execCommand("gnome-terminal -- bash -c \"killall -2 " + kill_progress_name + "\"");
    }

    response->result = request->action + "_recv";
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    node = rclcpp::Node::make_shared("slam_server");

    auto service = node->create_service<slam_interfaces::srv::SlamMode>("slam_service", &slam_callback);
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Slam service ready!");

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
