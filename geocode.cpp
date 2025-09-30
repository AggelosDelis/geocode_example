#include <iostream>
#include <fstream>
#include <curl/curl.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    std::ofstream *out = static_cast<std::ofstream*>(stream);
    out->write((char*)ptr, size * nmemb);
    return size * nmemb;
}

int main() {
    std::string api_key = "Your_API_Key";  // replace with your key
    std::string lat = "35.308097";  // X coordinate
    std::string lng = "25.083084";  // Y coordinate

    std::string url = "https://maps.googleapis.com/maps/api/staticmap?center=" +
                      lat + "," + lng +
                      "&zoom=15&size=600x400&markers=color:red|" +    // Size is 600 x 400.
                      lat + "," + lng +
                      "&key=" + api_key;

    CURL *curl = curl_easy_init();
    if (curl) {
        std::ofstream file("map.png", std::ios::binary);  // save as PNG
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        file.close();

        if (res == CURLE_OK)
            std::cout << "Map found and saved as png.\n";
        else
            std::cout << "Error finding map: " << curl_easy_strerror(res) << "\n";
    }
    return 0;
}

