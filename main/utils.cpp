#include "../header/utils.h"

std::string formatTanggal(time_t timestamp) {
    char buffer[30];
    struct tm* timeinfo = localtime(&timestamp);
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
    return std::string(buffer);
}