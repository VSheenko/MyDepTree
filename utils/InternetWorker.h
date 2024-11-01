#ifndef MYDEPTREE_INTERNETWORKER_H
#define MYDEPTREE_INTERNETWORKER_H

#include <stdio.h>
#include <curl/curl.h>
#include <string>
#include <chrono>
#include <deque>
#include <fstream>
#include "Indicators.h"

#define TIME_TO_DOWNLOAD_ERROR 5
#define MAX_SPEED_SAMPLES 10

size_t write_data(void* contents, size_t size, size_t nmemb, void* userp) {
    std::ofstream* ofs = static_cast<std::ofstream*>(userp);
    size_t totalSize = size * nmemb;
    ofs->write(static_cast<char*>(contents), totalSize);
    return totalSize;
}

// Функция для обработки заголовков и извлечения Content-Length
size_t header_callback(char* buffer, size_t size, size_t nitems, size_t* content_length) {
    std::string header(buffer, size * nitems);
    std::string prefix = "Content-Length: ";

    if (header.find(prefix) != std::string::npos) {
        *content_length = std::stoul(header.substr(prefix.size()));
    }

    return nitems * size;
}

double GetMovingAverage (std::deque<double>& history, double new_elem) {
    while (history.size() > MAX_SPEED_SAMPLES)
        history.pop_front();
    history.push_back(new_elem);

    double sum = 0.0;
    for (auto& i : history)
        sum += i;

    return sum / history.size();
}

int progress_callback(void* clientp,
                      curl_off_t dltotal, // Общий размер файла для загрузки
                      curl_off_t dlnow,   // Размер уже загруженных данных
                      curl_off_t ultotal,
                      curl_off_t ulnow) {

    static double last_load= 0.0;
    static auto start_time = std::chrono::high_resolution_clock::now();
    static auto small_speed_time = std::chrono::high_resolution_clock::now();
    static std::deque<double> speed_history;

    Indicators* indicator = static_cast<Indicators*>(clientp);

    if (dltotal <= 0) {
        return 0;
    }

    double loaded = static_cast<double>(dlnow);
    double progress = (loaded / dltotal);

    auto cur_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_time = cur_time - start_time;

    if (progress != 1 && duration_time.count() < 0.2)
        return 0;

    double speed = ((loaded - last_load)/ duration_time.count());
    speed = GetMovingAverage(speed_history, speed);

    if (progress != 1 && speed < 3) {
        if (std::chrono::duration<double>(cur_time - small_speed_time).count() > TIME_TO_DOWNLOAD_ERROR) {
            indicator->Update(-1);
            return 1;
        }

        return 0;
    }



    indicator->Update(progress, speed);

    start_time = std::chrono::high_resolution_clock::now();
    small_speed_time  = start_time;
    last_load = loaded;
    return 0;
}

namespace InternetWorker {
    int DownloadFile (const std::string& url, const std::string& outfile_path, bool progressbar_flag = true) {
        CURL *curl;
        curl = curl_easy_init();

        if (!curl)
            return 1;

        std::ofstream ofs(outfile_path, std::ios::binary);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ofs);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        std::cout << url.substr(url.rfind('/') + 1)  << ": ";
        Indicators* indicator;
        if (progressbar_flag) {
            indicator = new Indicators(true);
            curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
            curl_easy_setopt(curl, CURLOPT_XFERINFODATA, indicator);
        }

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (indicator){
            delete indicator;
            indicator = nullptr;
        }

        ofs.close();

        if (res != CURLE_OK)
            return -1;

        return 0;
    }

    size_t GetFileSize (const std::string& url) {
        CURL* curl = curl_easy_init();
        size_t content_length = 0;

        if (!curl)
            return 1;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L); // Только заголовки
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &content_length);

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            content_length = -1;

        curl_easy_cleanup(curl);
        return content_length;
    }
}


#endif //MYDEPTREE_INTERNETWORKER_H
