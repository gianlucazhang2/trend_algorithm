#include "tiktok.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <set>

class Hashtag { //hashtag object (string and number of tiktoks it appears in)
public: 
	Hashtag(std::string hash, int c, long unsigned int t) {
		hashtag = hash; 
		count = c;
		total_count = t; 
	}
	
	int getCount() {return count;}
	long unsigned int getTotalCount() {return total_count;}
	std::string getHashtag() {return hashtag;}
	
private:
	std::string hashtag;
	int count; 
	long unsigned int total_count; 
};

class Sound { //sound object (id, name, author, number of tiktoks)
public: 
	Sound(std::string id, std::string name, std::string author, long unsigned int t) {
		sound_id = id; 
		sound_name = name; 
		sound_author = author;  
		total_count = t; 
	}
	long unsigned int getTotalCount() {return total_count;}
	std::string getID() {return sound_id;}
	std::string getName() {return sound_name;}
	std::string getAuthor() {return sound_author;}
	
private:
	std::string sound_id, sound_name, sound_author; 
	long unsigned int total_count; 
};



struct compareHashtags { //compare hashtags based on count
	bool operator()(Hashtag& h1, Hashtag& h2) {
		if (h1.getCount() == h2.getCount()) {
			if (h1.getTotalCount() == h2.getTotalCount()) {
				return (h1.getHashtag() > h2.getHashtag()); 
			}
			return (h1.getTotalCount() < h2.getTotalCount()); 
		}
		return (h1.getCount() < h2.getCount()); 
	}
};

struct compareSounds { //compare sounds based on count
	bool operator()(Sound& s1, Sound& s2) {
		if (s1.getTotalCount() == s2.getTotalCount()) {
			return (s1.getID() > s2.getID()); 
		}
		return (s1.getTotalCount() < s2.getTotalCount()); 
	}
};

bool compareTiktoks(Tiktok& t1, Tiktok& t2) {//sort tiktoks in the vector map by view count
	return (t1.getPlayCount() > t2.getPlayCount());
}

int main(int argc, char* argv[]) {
	if (argc != 4) {return 1; }
	//command arguments
	std::string fileName = argv[1]; 
	std::string outputName = argv[2]; 
	std::string type = argv[3]; //hashtag or sound
	
	std::ofstream out(outputName);
	
	//map <hashtag, vector of tiktoks>
	std::map<std::string, std::vector<Tiktok>> hashtagMap; 
	//map <songID, vector of tiktoks>
	std::map<std::string, std::vector<Tiktok>> musicMap; 
	
    std::ifstream jsonFile(fileName); 
	std::string line; 
	while (std::getline(jsonFile, line)) {
		Tiktok tiktok(line); 
		if (type == "hashtag") { //hashtag case 
			for (std::string h: tiktok.getHashTags()) {
				std::string hashtag = "#" + h; 
				hashtagMap[hashtag].push_back(tiktok);
			}
		}
		if (type == "sound") {
			std::string music_id = tiktok.getMusicID(); 
			musicMap[music_id].push_back(tiktok); 
		}
	}
	
	//priority queue for hashtags
	std::priority_queue<Hashtag, std::vector<Hashtag>, compareHashtags> hq; 
	
	//priority queue for sounds
	std::priority_queue<Sound, std::vector<Sound>, compareSounds> sq; 
	
	if (type == "hashtag") {
		std::map<std::string, std::vector<Tiktok>>::iterator it; 
		for (it = hashtagMap.begin(); it != hashtagMap.end(); ++it) {
			std::sort(it->second.begin(), it->second.end(), compareTiktoks); 
			long unsigned int total = 0;
			for (Tiktok t: it->second) {
				total += t.getPlayCount(); 
			} 
			Hashtag hashtag(it->first, it->second.size(), total); 
			hq.push(hashtag); 
		}
		//print out trending hashtags
		int i = 0;
		out << "trending hashtags:\n" << std::endl; 
		while (i != 10) {
			Hashtag hash = hq.top(); 
			std::string h = hash.getHashtag(); 
			out << "========================" << std::endl;
			out << h << std::endl; 
			out << "used " << hash.getCount() << " times" << std::endl; 
			out << hash.getTotalCount() << " views\n" << std::endl; 
			for (long unsigned int j = 0; j < 3; j++) {
				if (j >= hashtagMap[h].size()) {break;}
				out << "cover url: " << hashtagMap[h][j].getCover() << std::endl; 
				out << "web video url: " << hashtagMap[h][j].getWeb() << std::endl; 
			}
			//use throw try catch exception to detect when we reach the end of the list
			try {
				if (i == 9) {
					throw std::string("end of list"); 
				}
				out << "========================" << std::endl; 
			}
			catch (std::string str) {
				out << "========================"; 
			}
			hq.pop();
			i++; 
		}
	}
	
	
	if (type == "sound") {
		std::map<std::string, std::vector<Tiktok>>::iterator it; 
		for (it = musicMap.begin(); it != musicMap.end(); ++it) {
			std::sort(it->second.begin(), it->second.end(), compareTiktoks); 
			long unsigned int total = 0; 
			for (Tiktok t: it->second) {
				total += t.getPlayCount(); 
			}
			Sound sound(it->first, it->second[0].getMusicName(), it->second[0].getMusicAuthor(), total); 
			sq.push(sound); 
		}
		//print out trending sounds
		int i = 0;
		out << "trending sounds:\n" << std::endl;  
		while (i != 10) {
			Sound sound = sq.top();
			std::string id = sound.getID(); 
			out << "========================" << std::endl; 
			out << sound.getName() << std::endl; 
			out << sound.getTotalCount() << " views" << std::endl; 
			out << sound.getAuthor() << std::endl; 
			out << "music id: " << sound.getID() << "\n" << std::endl; 
			for (long unsigned int j = 0; j < 3; j++) {
				if (j >= musicMap[id].size()) {break;}
				out << "cover url: " << musicMap[id][j].getCover() << std::endl; 
				out << "web video url: " << musicMap[id][j].getWeb() << std::endl; 
			}
			//use throw try catch exception to detect when we reach the end of the list
			try {
				if (i == 9) {
					throw std::string("end of list"); 
				}
				out << "========================" << std::endl; 
			}
			catch (std::string str) {
				out << "========================"; 
			}
			sq.pop(); 
			i++; 
		}
	}
	
	

    return 0;
}