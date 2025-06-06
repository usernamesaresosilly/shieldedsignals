#include "message_generator.h"

#include <random>
#include <cstdio>
#include <cstdlib>
#include <numeric>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <string>
#include <iomanip>

constexpr int MAX_IDS = 1000, MIN_MSG = 1, MAX_MSG = 1;

constexpr struct {
    double lat = 36.214972;
    double lon = -86.612972;
    double alt = 174.0;
} JACKSON_GRAVE;

constexpr double SPD = 13.4, HDG = 92.3, ACC = 0.5;
constexpr int TIME = 171364;

std::string generate_messages(int n) {
    if (n < MIN_MSG || n > MAX_IDS) {
        return "Error: Must be between " + std::to_string(MIN_MSG) +
               " and " + std::to_string(MAX_IDS) + " (got " + std::to_string(n) + ").\n";
    }

    static std::mt19937 rng(5489u);
    std::uniform_int_distribution<int> dt(-5, 5);
    std::uniform_real_distribution<double>
        dlat(-0.0005, 0.0005), dlon(-0.0005, 0.0005),
        dalt(-0.5, 0.5), dspd(-1.0, 1.0), dhdg(-15.0, 15.0);

    int ids[MAX_IDS];
    std::iota(ids, ids + MAX_IDS, 0);

    std::ostringstream out;

    for (int i = 0; i < n; ++i) {
        int j = i + (rng() % (MAX_IDS - i));
        std::swap(ids[i], ids[j]);

        double heading = HDG + dhdg(rng);
        if (heading < 0.0) heading += 360.0;
        else if (heading >= 360.0) heading -= 360.0;

        out << "CAM,StationID=" << ids[i]
            << ",Time=" << (TIME + dt(rng))
            << ",Lat=" << std::fixed << std::setprecision(6) << (JACKSON_GRAVE.lat + dlat(rng))
            << ",Lon=" << std::fixed << std::setprecision(6) << (JACKSON_GRAVE.lon + dlon(rng))
            << ",Alt=" << std::fixed << std::setprecision(1) << (JACKSON_GRAVE.alt + dalt(rng))
            << ",Speed=" << (SPD + dspd(rng))
            << ",Heading=" << heading
            << ",Acc=" << ACC << "\n";
    }

    return out.str();
}
