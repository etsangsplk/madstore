#ifndef MAD_RECORDS_H
#define MAD_RECORDS_H

#include <sys/stat.h>
#include <unistd.h>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <boost/interprocess/managed_mapped_file.hpp>
#include "../3rdparty/json.hpp"

using json = nlohmann::json;
namespace bi = boost::interprocess;

/**
 * Records persisted using memory mapped file
 */
template<typename Record,typename Metrics>
struct PersistentRecords {

  using MappedFileAllocator = bi::allocator<Record, bi::managed_mapped_file::segment_manager>;
  using RecordsVector = std::vector<Record, MappedFileAllocator>;

  const unsigned long FILE_SIZE = 2ul<<30; // 2GB
  const offset_t VOLUME_SIZE = FILE_SIZE/sizeof(Record) - 1024;

  std::vector<bi::managed_mapped_file*> segments;
  std::vector<RecordsVector*> volumes;

  PersistentRecords() {
    for (int i = 1; ; ++i) {
      std::ostringstream filebuf;
      filebuf<<"records"<<i<<".mad";
      std::string file = filebuf.str();
      if (access(file.c_str(), F_OK) == -1) {
        break;
      }
      auto segment = new bi::managed_mapped_file(bi::open_only, file.c_str());
      segments.push_back(segment);
      auto volume = segment->find_or_construct<RecordsVector>("records")(segment->get_segment_manager());
      volumes.push_back(volume);
      CLOG(INFO, "Store")
        <<"Opened volume "<<static_cast<void*>(volume)
        <<" ("<<file<<") with "<<volume->size()<<" records inside";
    }
  }
  
  ~PersistentRecords() {
    volumes.resize(0);
    for (auto segment : segments) {
      delete segment;
    }
    segments.resize(0);
  }

  /**
   * Create new memory mapped file, and create a new records vector based on it
   */
  void AddSegment() {
    std::ostringstream filebuf;
    filebuf<<"records"<<segments.size() + 1<<".mad";
    std::string file = filebuf.str();
    auto segment = new bi::managed_mapped_file(bi::create_only, file.c_str(), FILE_SIZE);
    segments.push_back(segment);
    auto volume = segment->find_or_construct<RecordsVector>("records")(segment->get_segment_manager());
    volume->reserve(VOLUME_SIZE);
    volumes.push_back(volume);
    CLOG(INFO, "Store")
      <<"Created volume "<<static_cast<void*>(volume)
      <<" ("<<file<<") for storing "<<VOLUME_SIZE<<" records";
  }

  /**
   * Returns all volumes that contain the given offset
   */
  std::vector<std::pair<offset_t,RecordsVector*>> GetVolumes(offset_t start_offset) {
    uint8_t volumes_num = volumes.size();
    std::vector<std::pair<offset_t,RecordsVector*>> result;
    result.reserve(volumes_num);
    for (int i = 0; i < volumes_num; ++i) {
      if (start_offset <= (i + 1) * VOLUME_SIZE) {
        result.push_back(std::pair<offset_t,RecordsVector*>(
              result.empty() ? start_offset - i * VOLUME_SIZE : 0, volumes[i]));
      } 
    }
    return result;
  }

  /**
   * Adds new record to the last volume and returns global offset
   */
  inline offset_t AddRecord(const Record& record) {
    if (volumes.empty() || volumes.back()->size() == volumes.back()->capacity()) {
      AddSegment();
    }
    volumes.back()->push_back(record);
    return (VOLUME_SIZE * (volumes.size() - 1)) + volumes.back()->size() - 1;
  }

  /**
   * Updates record using global offset
   */
  inline void UpdateRecord(offset_t offset, const Metrics& metrics) {
    (*volumes[offset / volumes.size()])[offset % volumes.size()].second += metrics;
  }

  void GetStats(json& stats) {
    offset_t records = 0;
    for (auto volume : volumes) {
      records += volume->size();
    }
    stats["records"] = std::to_string(records);
    stats["records_usage_mb"] = records * sizeof(Record) / (1024*1024);
  }

  void OptimizeMemUsage() {
  }
};

/**
 * Just in-memory records storage.
 * This class resembles PersistentRecords API so it will be easily replaceable using compile flag.
 */
template<typename Record,typename Metrics>
struct InMemoryRecords {

  std::vector<Record> records;

  std::vector<std::pair<offset_t,std::vector<Record>*>> GetVolumes(offset_t start_offset) {
    std::vector<std::pair<offset_t,std::vector<Record>*>> result;
    result.reserve(1);
    result.push_back(std::pair<offset_t,std::vector<Record>*>(start_offset, &records));
    return result;
  }

  inline offset_t AddRecord(const Record& record) {
    records.push_back(record);
    return records.size() - 1;
  }

  inline void UpdateRecord(offset_t offset, const Metrics& metrics) {
    records[offset].second += metrics;
  }

  void GetStats(json& stats) {
    stats["records"] = std::to_string(records.size());
    stats["records_usage_mb"] = records.size() * sizeof(Record) / (1024*1024);
  }

  void OptimizeMemUsage() {
    size_t prev_capacity = records.capacity();
    records.shrink_to_fit();

    CLOG(INFO, "Store")
      <<"Freed up "<<(prev_capacity - records.capacity()) * sizeof(Record) / (1024 * 1024)
      <<"MB of memory";
  }
};

#endif /* MAD_RECORDS_H */
