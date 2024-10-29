#ifndef MYDEPTREE_INTERNETWORKER_H
#define MYDEPTREE_INTERNETWORKER_H

#include <stdio.h>
#include <curl/curl.h>
#include <string>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
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

namespace InternetWorker {
    int DownloadFile (const std::string& url, const std::string& outfile_path) {
        CURL *curl;
        FILE *fp;

        curl = curl_easy_init();
        if (!curl)
            return 1;

        fp = fopen(outfile_path.c_str(),"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);

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
