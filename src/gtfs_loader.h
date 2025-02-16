#include <string>
#include <unordered_map>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

// Structure to hold stop information
struct StopInfo {
  std::string stop_id;
  std::string stop_name;
  std::string stop_desc;
};

// Structure to hold our sequence mapping
struct SequenceMapping {
  // Maps stop_sequence to stop info
  std::map<uint32_t, StopInfo> sequences;
};

// Main lookup table type
using TripStopMap = std::unordered_map<std::string, SequenceMapping>;

class GTFSLoader {
private:
  // Helper to split a CSV line
  std::vector<std::string> split_csv_line(const std::string& line) {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string item;

    while (std::getline(ss, item, ',')) {
      // Remove quotes if they exist
      if (item.size() >= 2 && item.front() == '"' && item.back() == '"') {
        item = item.substr(1, item.size() - 2);
      }
      result.push_back(item);
    }
    return result;
  }

  // Helper to find column index by name
  int find_column_index(const std::vector<std::string>& headers, const std::string& column_name) {
    for (size_t i = 0; i < headers.size(); i++) {
      if (headers[i] == column_name) return i;
    }
    return -1;
  }

  std::unordered_set<std::string> get_ion_trip_ids(const std::string& gtfs_dir) {
    std::unordered_set<std::string> ion_trips;
    std::ifstream trips_file(gtfs_dir + "/trips.txt");

    if (!trips_file.is_open()) {
      throw std::runtime_error("Could not open trips.txt");
    }

    std::string line;
    // Read header
    std::getline(trips_file, line);
    auto headers = split_csv_line(line);

    int route_id_idx = find_column_index(headers, "route_id");
    int trip_id_idx = find_column_index(headers, "trip_id");

    // Read trips
    while (std::getline(trips_file, line)) {
      auto fields = split_csv_line(line);
      if (fields.size() > std::max(route_id_idx, trip_id_idx)) {
        if (fields[route_id_idx] == "301") {  // ION light rail route
          ion_trips.insert(fields[trip_id_idx]);
        }
      }
    }

    std::cout << "Found " << ion_trips.size() << " ION trips\n";
    return ion_trips;
  }

  std::unordered_set<std::string> get_ion_stop_ids(
    const std::string& gtfs_dir, 
    const std::unordered_set<std::string>& ion_trips) {

    std::unordered_set<std::string> ion_stops;
    std::ifstream times_file(gtfs_dir + "/stop_times.txt");

    if (!times_file.is_open()) {
      throw std::runtime_error("Could not open stop_times.txt");
    }

    std::string line;
    // Read header
    std::getline(times_file, line);
    auto headers = split_csv_line(line);

    int trip_id_idx = find_column_index(headers, "trip_id");
    int stop_id_idx = find_column_index(headers, "stop_id");

    // Read stop times
    while (std::getline(times_file, line)) {
      auto fields = split_csv_line(line);
      if (fields.size() > std::max(trip_id_idx, stop_id_idx)) {
        if (ion_trips.count(fields[trip_id_idx]) > 0) {
          ion_stops.insert(fields[stop_id_idx]);
        }
      }
    }

    std::cout << "Found " << ion_stops.size() << " ION stops\n";
    return ion_stops;
  }

public:
  TripStopMap load_gtfs_data(const std::string& gtfs_dir) {
    auto ion_trips = get_ion_trip_ids(gtfs_dir);
    auto ion_stops = get_ion_stop_ids(gtfs_dir, ion_trips);

    TripStopMap lookup_table;
    std::unordered_map<std::string, StopInfo> stops_map;

    // First load stops.txt to get stop information
    std::ifstream stops_file(gtfs_dir + "/stops.txt");
    if (!stops_file.is_open()) {
      throw std::runtime_error("Could not open stops.txt");
    }

    std::string line;
    std::getline(stops_file, line);
    auto stop_headers = split_csv_line(line);

    int stop_id_idx = find_column_index(stop_headers, "stop_id");
    int stop_name_idx = find_column_index(stop_headers, "stop_name");
    int stop_desc_idx = find_column_index(stop_headers, "stop_desc");

    // Read stops
    while (std::getline(stops_file, line)) {
      auto fields = split_csv_line(line);
      if (fields.size() > std::max({stop_id_idx, stop_name_idx, stop_desc_idx})) {
        if (ion_stops.count(fields[stop_id_idx]) <= 0) continue;

        StopInfo info;
        info.stop_id = fields[stop_id_idx];
        info.stop_name = fields[stop_name_idx];
        info.stop_desc = stop_desc_idx >= 0 ? fields[stop_desc_idx] : "";
        stops_map[info.stop_id] = info;
      }
    }

    // Now load stop_times.txt to create the sequence mapping
    std::ifstream times_file(gtfs_dir + "/stop_times.txt");
    if (!times_file.is_open()) {
      throw std::runtime_error("Could not open stop_times.txt");
    }

    std::getline(times_file, line);
    auto time_headers = split_csv_line(line);

    int trip_id_idx = find_column_index(time_headers, "trip_id");
    int stop_sequence_idx = find_column_index(time_headers, "stop_sequence");
    stop_id_idx = find_column_index(time_headers, "stop_id");

    while (std::getline(times_file, line)) {
      auto fields = split_csv_line(line);
      if (fields.size() > std::max({trip_id_idx, stop_id_idx, stop_sequence_idx})) {
        std::string trip_id = fields[trip_id_idx];
        std::string stop_id = fields[stop_id_idx];
        uint32_t sequence = std::stoul(fields[stop_sequence_idx]);

        auto stop_it = stops_map.find(stop_id);
        if (stop_it != stops_map.end()) {
          lookup_table[trip_id].sequences[sequence] = stop_it->second;
        }
      }
    }

    return lookup_table;
  }

  // Helper function to get stop info from sequence
  static StopInfo* get_stop_info(TripStopMap& lookup_table, 
                                 const std::string& trip_id, 
                                 uint32_t stop_sequence) {
    auto trip_it = lookup_table.find(trip_id);
    if (trip_it == lookup_table.end()) return nullptr;

    auto& sequences = trip_it->second.sequences;
    auto seq_it = sequences.find(stop_sequence);
    if (seq_it == sequences.end()) return nullptr;

    return &seq_it->second;
  }

  static void print_head(const TripStopMap& lookup_table, size_t n = 5) {
    std::cout << "\n=== Lookup Table Head (max " << n << " trips) ===\n";

    size_t trips_shown = 0;
    for (const auto& [trip_id, mapping] : lookup_table) {
      if (trips_shown >= n) break;

      std::cout << "\nTrip ID: " << trip_id << "\n";
      std::cout << "  Sequences:\n";

      // Print first 5 sequences for this trip
      size_t sequences_shown = 0;
      for (const auto& [seq, stop] : mapping.sequences) {
        if (sequences_shown >= 5) {
          std::cout << "  ... and " << 
            (mapping.sequences.size() - sequences_shown) << 
            " more sequences\n";
          break;
        }

        std::cout << "    " << seq << ": " << 
          stop.stop_name << " (ID: " << stop.stop_id << ")\n";
        sequences_shown++;
      }

      trips_shown++;
    }

    if (lookup_table.size() > n) {
      std::cout << "\n... and " << (lookup_table.size() - n) << 
        " more trips\n";
    }

    std::cout << "\nTotal trips in lookup table: " << 
      lookup_table.size() << "\n";
    std::cout << "==========================================\n\n";
  }
};

