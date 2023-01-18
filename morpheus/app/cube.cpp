#include <chrono>
#include <igris/getopt/cliopts.h>
#include <igris/time/systime.h>
#include <morpheus/defs.h>
#include <nos/inet/tcp_client.h>
#include <nos/print.h>
#include <nos/trent/json_print.h>
#include <nos/util/string.h>
#include <rabbit/mesh/primitives.h>
#include <thread>

int main(int argc, char **argv)
{
    igris::cliopts opts;
    opts.add_string("pose", 'o', "0,0,0,0,0,0");
    opts.add_string("id", 'i', "noid");
    opts.add_option("deg", 'd');
    opts.add_string("body", 'b', "box");
    opts.add_option("move", 'm');
    opts.parse(argc, argv);
    std::string offset_str = opts.get_string("pose");
    std::vector<std::string> offset_strs = nos::split(offset_str, ',');
    float offset_x = std::stof(offset_strs[0]);
    float offset_y = std::stof(offset_strs[1]);
    float offset_z = std::stof(offset_strs[2]);
    float offset_yaw = std::stof(offset_strs[3]);
    float offset_pitch = std::stof(offset_strs[4]);
    float offset_roll = std::stof(offset_strs[5]);

    bool deg = opts.get_bool("deg");
    if (deg)
    {
        offset_yaw = offset_yaw * M_PI / 180;
        offset_pitch = offset_pitch * M_PI / 180;
        offset_roll = offset_roll * M_PI / 180;
    }

    std::string body = opts.get_string("body");
    std::string id = opts.get_string("id");
    nos::fprintln("id: {}", id);

    rabbit::mesh<float> mesh;
    if (body == "box")
    {
        mesh = rabbit::box_mesh(10.f, 10.f, 10.f);
    }
    else if (body == "sphere")
    {
        mesh = rabbit::sphere_mesh(10.f, 10, 10);
    }

    auto mesh_trent = mesh.to_trent();

    auto trans =
        ralgo::pose3<float>::translation({offset_x, offset_y, offset_z});
    auto rot =
        ralgo::pose3<float>::euler({offset_yaw, offset_pitch, offset_roll});

    nos::trent json;
    json["cmd"] = "add_model";
    json["args"][0] = id;
    json["args"][1] = mesh_trent;
    json["args"][2] = (trans * rot).to_trent();

    auto client =
        nos::inet::tcp_client::dial("127.0.0.1", MORPHEUS_DEFAULT_PORT);

    auto json_str = nos::json::to_string(json);
    nos::print_to(client, json_str);

    if (opts.get_bool("move"))
    {
        auto start_time = igris::millis();
        while (true)
        {
            auto current_time = igris::millis() - start_time;
            auto pose = ralgo::pose3<float>::translation(
                            {offset_x,
                             offset_y,
                             offset_z + std::sin(current_time / 1000.f) * 10.f})
                            .to_trent();
            nos::fprintln("pose: {} curtime: {}",
                          nos::json::to_string(pose),
                          current_time);
            nos::trent json;
            json["cmd"] = "set_model_pose";
            json["args"][0] = id;
            json["args"][1] = pose;
            auto json_str = nos::json::to_string(json);
            nos::print_to(client, json_str);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}