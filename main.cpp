#include "news_aggregator.h"

int main() {
    NewsAggregator test1;

    std::cout << "Hello! This is the program where you can find articles you are interested in from some news outlets"
              << std::endl;
    std::cout << std::endl;

    std::cout << "Available news outlets:" << std::endl;
    test1.get_newspapers_console();
    std::cout << std::endl;

    std::cout << "Please, follow the instruction" << std::endl;

    std::cout << "Write keyword of the articles: ";
    std::string keyword;
    std::cin >> keyword;

    std::cout << "Write a name of the news outlet: ";
    std::string newspaper;
    std::cin >> newspaper;

    test1.get_articles_html(keyword, newspaper);

    return 0;
}
