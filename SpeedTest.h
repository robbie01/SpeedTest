//
// Created by Francesco Laurita on 5/29/16.
//

#ifndef SPEEDTEST_SPEEDTEST_H
#define SPEEDTEST_SPEEDTEST_H

#include <map>
#include <curl/curl.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <libxml/xmlreader.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>


static const float EARTH_RADIUS_KM = 6371.0;

static const std::string IP_INFO_API_URL = "http://speedtest.ookla.com/api/ipaddress.php";
static const std::string SERVER_LIST_URL = "http://www.speedtest.net/speedtest-servers-static.php";
static const std::string USER_AGENT = "Mozilla/5.0 SpeedTest++";

typedef struct ip_info_t {
    std::string ip_address;
    std::string isp;
    float lat;
    float lon;
} IPInfo;

typedef struct server_info_t {
    std::string url;
    std::string name;
    std::string country;
    std::string country_code;
    std::string host;
    std::string sponsor;
    int   id;
    float lat;
    float lon;
    float distance;

} ServerInfo;

typedef struct test_config_t {
    long start_size;
    long max_size;
    long incr_size;
    long buff_size;
    long min_test_time_ms;
    int  concurrency;
} TestConfig;

class SpeedTestClient;
typedef bool (SpeedTestClient::*opFn)(const long size, const long chunk_size, long &millisec);
class SpeedTest {
public:
    SpeedTest();
    ~SpeedTest();
    CURLcode httpGet(const std::string& url, std::stringstream& os, CURL *handler = nullptr, long timeout = 30);
    static std::map<std::string, std::string> parseQueryString(const std::string& query);
    static std::vector<std::string> splitString(const std::string& instr, const char separator);
    bool ipInfo(IPInfo *info);
    const std::vector<ServerInfo>& serverList();
    const ServerInfo bestServer(const int sample_size = 5);
    const double &latency();
    const float downloadSpeed(const ServerInfo& server, const TestConfig& config);
    const float uploadSpeed(const ServerInfo& server, const TestConfig& config);
private:
    static size_t writeFunc(void* buf, size_t size, size_t nmemb, void* userp);
    static ServerInfo processServerXMLNode(xmlTextReaderPtr reader);
    float execute(const ServerInfo &server, const TestConfig &config, const opFn &fnc);
    template <typename T>
        static T deg2rad(T n);
    template <typename T>
        static T harversine(std::pair<T, T> n1, std::pair<T, T> n2);

    IPInfo mIpInfo;
    std::vector<ServerInfo> mServerList;
    double mLatency;

};


#endif //SPEEDTEST_SPEEDTEST_H
