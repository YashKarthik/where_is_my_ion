#include <iostream>
#include <string>
#include <curl/curl.h>
#include "gtfs-realtime.pb.h"
#include "gtfs_loader.h"

const std::string ION_ROUTE_ID = "301";
const std::string GRT_REALTIME_VEHICLE_URL = "https://webapps.regionofwaterloo.ca/api/grt-routes/api/vehiclepositions";

// Callback function to write received data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
  userp->append((char*)contents, size * nmemb);
  return size * nmemb;
}

int main() {
  CURL* curl = curl_easy_init();
  std::string buffer;

  GTFSLoader loader;
  auto lookup_table = loader.load_gtfs_data("../GRT GTFS/");
  GTFSLoader::print_head(lookup_table);

  if(curl) {
    // Set URL for vehicle positions - use c_str() to get C-style string
    curl_easy_setopt(curl, CURLOPT_URL, GRT_REALTIME_VEHICLE_URL.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

    // Perform request
    CURLcode res = curl_easy_perform(curl);

    if(res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    } else {
      transit_realtime::FeedMessage feed;
      if (feed.ParseFromString(buffer)) {
        // Iterate through entities
        for (const transit_realtime::FeedEntity& entity : feed.entity()) {

          if (entity.has_vehicle() && entity.vehicle().has_trip() && 
            entity.vehicle().trip().route_id() == ION_ROUTE_ID) {
            const auto& vehicle = entity.vehicle();

            if (vehicle.has_position()) {

              std::cout << "ION Train ID: " << entity.id() << std::endl;
              std::cout << "Trip ID:" << vehicle.trip().trip_id() << std::endl;

              std::cout << "Position: " << vehicle.position().latitude() 
                << ", " << vehicle.position().longitude() << std::endl;


              if (vehicle.has_current_stop_sequence()) {
                std::cout << "Current Stop Sequence: " 
                  << vehicle.current_stop_sequence() << std::endl;
              }

              StopInfo* stop = GTFSLoader::get_stop_info(lookup_table, vehicle.trip().trip_id(), vehicle.current_stop_sequence());
              if (stop) {
                std::cout << "Found stop: " << stop->stop_name << std::endl;
              } else {
                std::cout << "Stop not found" << std::endl;
              }

            }

            std::cout << "-----------------------------------\n" << std::endl;
          }
        }
      } else {
        std::cerr << "Failed to parse protobuf message" << '\n';
      }
    }

    curl_easy_cleanup(curl);
  }

  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}
