#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <regex>
#include <map>

class NewsAggregator {
    static const int newspapers_amount = 1;
    std::map<std::string, std::string> newspapers;

    void write_draft_data(const std::string file, const std::string paper) {
        FILE *fp = fopen(file.c_str(), "w");
        CURL *curl = curl_easy_init();

        curl_easy_setopt(curl, CURLOPT_URL, newspapers[paper].c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        CURLcode result = curl_easy_perform(curl);

        if (result != CURLE_OK) {
            std::cout << "ERROR:" << std::endl;
            std::cout << curl_easy_strerror(result) << std::endl;
        }

        fclose(fp);
    }

    static void write_clear_data(const std::string file, const std::string key, const std::string paper) {
        std::fstream draft;
        std::fstream result;

        draft.open(file, std::ios::in);

        std::string time = get_date();
        std::string new_file = key + "-" + paper + "-" + time + ".html";

        std::string path = get_path_console();

        result.open(path + new_file, std::ios::app);

        result << "<h1>Pages from " + paper +  " which consist keyword: " + key + "</h1>";

        if (draft.is_open() && result.is_open()) {
            std::string line;

            // Scroll lines until the HTML header information will be skipped (there is no <description> in the header)
            while (std::getline(draft, line) && !line.find("<item>"));

            // Find out length of a tag, e.g. for <title> length is 7 because it has 7 characters
            int tag_title_length = 7;
            int tag_description_length = 13;

            std::string title;
            std::string description;

            while (std::getline(draft, line)) {
                line = clean_space_before(line);

                if (line.substr(0, tag_title_length) == "<title>") {
                    title = get_text(line, tag_title_length);
                }

                if (line.substr(0, tag_description_length) == "<description>") {
                    description = get_text(line, tag_description_length);

                    if (int(description.find(key)) != -1 || int(title.find(key)) != -1) {
                        description = clean_text(description);

                        result << "<h2>" << title << "</h2>" << "\n";
                        result << description << "\n";
                        result << "<br><br><br>" << "\n";
                    }
                }
            }

            draft.close();
            result.close();
        }
    }

    // Function cleans the space before HTML code ("   <item>" -> "<item>")
    static std::string clean_space_before(std::string line) {
        int i = 0;

        while (line[i] == ' ') {
            i += 1;
        }

        line = line.substr(i);

        return line;
    }

    // It gets text without changing XML code
    static std::string get_text(std::string line, int tag_length) {
        return line.substr(tag_length, line.length() - 2 * tag_length - 2);
    }

    // It cleans XML code, particularly it changes brackets to use it in HTML code
    static std::string clean_text(std::string line) {
        line = std::regex_replace(line, std::regex("&lt;"), "<");
        line = std::regex_replace(line, std::regex("&gt;"), ">");

        return line;
    }

    static std::string get_date() {
        time_t now = time(nullptr);
        tm *local_time = localtime(&now);

        std::string year = std::to_string(1900 + local_time->tm_year);
        std::string month = std::to_string(1 + local_time->tm_mon);
        std::string day = std::to_string(local_time->tm_mday);
        std::string hour = std::to_string(local_time->tm_hour);
        std::string minute = std::to_string(local_time->tm_min);

        std::string date = hour + "-" + minute + "-" + month + "-" + day + "-" + year;

        return date;
    }

    static std::string get_path_console() {
        std::string path;

        std::cout << "Write the path to store articles (with slash on the end): ";
        std::cin >> path;
        std::cout << std::endl;

        return path;
    }

    void is_data_correct(std::string paper) {
        if (newspapers.count(paper) == 0) {
            throw std::invalid_argument("There is no such paper!");
        }
    }

    static void successful_compiled_console() {
        std::cout << "The search has been done. File is ready" << std::endl;
    }

public:
    NewsAggregator() {
        newspapers["TheGuardian"] = "https://www.theguardian.com/uk/rss";
    }

    void get_articles_html(const std::string key, const std::string paper) {
        std::string file = key + "-" + paper + ".txt";

        is_data_correct(paper);

        write_draft_data(file, paper);

        write_clear_data(file, key, paper);

        successful_compiled_console();

        remove(file.c_str());
    }

    void get_newspapers_console() {
        int k = 1;

        for (std::map<std::string, std::string>::iterator it = newspapers.begin(); it != newspapers.end(); it++ ) {
            std::cout << k << ". " << it->first << std::endl;
            k += 1;
        }
    }
};
