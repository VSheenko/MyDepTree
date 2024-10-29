#ifndef MYDEPTREE_INTERNETWORKER_H
#define MYDEPTREE_INTERNETWORKER_H

#include <stdio.h>
#include <curl/curl.h>
#include <string>

namespace InternetWorker {
    size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
        size_t written = fwrite(ptr, size, nmemb, stream);
        return written;
    }

    int Download (const std::string& url, const std::string& outfile_path) {
        CURL *curl;
        FILE *fp;
        CURLcode res;
        curl = curl_easy_init();
        if (curl) {
            fp = fopen(outfile_path.c_str(),"wb");
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            res = curl_easy_perform(curl);
            /* always cleanup */
            curl_easy_cleanup(curl);
            fclose(fp);
        }
        return 0;
    }
}

#endif //MYDEPTREE_INTERNETWORKER_H
