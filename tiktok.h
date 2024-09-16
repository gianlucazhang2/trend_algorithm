#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <regex>

class Tiktok {
public:
//constructor
	Tiktok(std::string& jsonStr) {
		int pos = jsonStr.find("\"id\":");
        id = extractValue(jsonStr, pos+5);

        pos = jsonStr.find("\"text\":");
        text = extractValue(jsonStr, pos+7);

        pos = jsonStr.find("\"webVideoUrl\":");
        webVideoUrl = extractValue(jsonStr, pos+14);

        pos = jsonStr.find("\"coverUrl\":");
        coverUrl = extractValue(jsonStr, pos+11);

        pos = jsonStr.find("\"musicAuthor\":");
        musicAuthor = extractValue(jsonStr, pos+14);

        pos = jsonStr.find("\"musicName\":");
        musicName = extractValue(jsonStr, pos+12);

        pos = jsonStr.find("\"musicId\":");
        musicId = extractValue(jsonStr, pos+10);

        pos = jsonStr.find("\"playCount\":");
        playCount = extractCount(jsonStr, pos+12);
	}
	
	void displayInfo() {
        std::cout << "ID: " << id << std::endl;
        std::cout << "Text: " << text << std::endl;
        std::cout << "Web Video URL: " << webVideoUrl << std::endl;
        std::cout << "Cover URL: " << coverUrl << std::endl;
        std::cout << "Music Author: " << musicAuthor << std::endl;
        std::cout << "Music Name: " << musicName << std::endl;
        std::cout << "Music ID: " << musicId << std::endl;
        std::cout << "Play Count: " << playCount << std::endl;
    }

//accessors
	std::string getID() {return id;}
	std::string getText() {return text;}
	std::string getWeb() {return webVideoUrl;}
	std::string getCover() {return coverUrl;}
	std::string getMusicAuthor() {return musicAuthor;}
	std::string getMusicName() {return musicName;}
	std::string getMusicID() {return musicId;}
	long unsigned int getPlayCount() const {return playCount;}

//get hashtags from the text
//returns a vector of strings with all the hashtags
	std::vector<std::string> getHashTags() {
		std::vector<std::string> hashTags; 
		std::regex hashtagRegex("#([\\w\\u0080-\\uFFFF]+)"); 
		std::sregex_iterator hashtagIterator(text.begin(), text.end(), hashtagRegex);
		std::sregex_iterator endIterator;

		// iterate over the matches and extract the hashtags
		while (hashtagIterator != endIterator) {
			std::smatch match = *hashtagIterator;
			std::string hashtag = match.str(1);  
			hashTags.push_back(hashtag); 

			++hashtagIterator;
		}
		return hashTags; 
	}
	
private: 
//variables
	std::string id;
    std::string text;
    std::string webVideoUrl;
    std::string coverUrl;
    std::string musicAuthor;
    std::string musicName;
    std::string musicId;
    long unsigned int playCount;
//parse out value method
	std::string extractValue(const std::string& str, int pos) {
        size_t startQuote = str.find('"', pos);
        size_t endQuote = str.find('"', startQuote + 1);
        pos = endQuote;
		return str.substr(startQuote + 1, endQuote - startQuote - 1);
    }
//parse out view count
	int extractCount(const std::string& str, int pos) {
		size_t start = str.find(' ', pos); 
		size_t end = str.find(',', pos); 
		return std::stoi(str.substr(start+1, end - start - 1)); 
	}
};
	
	