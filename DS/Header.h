#pragma once
#include <iostream>
#include <windows.h>
#include <unordered_map>
#include <string>
#include <stack>
#include <queue>
#include <fstream>
#include <list>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/access.hpp>
#include "boost/serialization/deque.hpp"
#include "boost/serialization/stack.hpp"
#include <boost/serialization/queue.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/optional.hpp>
#include <unordered_map>
#include<boost/serialization/unordered_map.hpp>
#include <boost/serialization/serialization.hpp>
#include <vector>

using namespace std;


class Song {
private:
    string name;
    float duration;
    float energy;
    float key;
    float loudness;
    float speechiness;
    float acousticness;
    float instrumentalness;
    float liveness;
    float tempo;
    int streams;

public:
    Song();
    Song(const string& n) : name(n), duration(0), energy(0), key(0), loudness(0), speechiness(0), acousticness(0), instrumentalness(0), liveness(0), tempo(0) {}
    void setName(string n);
    void setArtist(const string& a);
    void setDuration(float dur);
    void setEnergy(float en);
    void setKey(float k);
    void setLoudness(float loud);
    void setSpeechiness(float spch);
    void setAcousticness(float ac);
    void setInstrumentalness(float instr);
    void setLiveness(float live);
    void setTempo(float tmp);

    void incStreams();
    string getName();
    string getArtist();
    float getDuration();
    float getEnergy();
    float getKey();
    float getLoudness();
    float getSpeechiness();
    float getAcousticness();
    float getInstrumentalness();
    float getLiveness();
    float getTempo();
    struct HashFunction {
        size_t operator()(const Song& song) const {
            return hash<string>()(song.name);
        }
    };
    struct EqualityComparison {
        bool operator()(const Song& lhs, const Song& rhs) const {
            if (lhs.name == rhs.name) {
                return true;
            }
            return false;
        }
    };

    void displayInfo();

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& name;
        ar& duration;
        ar& energy;
        ar& key;
        ar& loudness;
        ar& speechiness;
        ar& acousticness;
        ar& instrumentalness;
        ar& liveness;
        ar& tempo;
    }
};

class SongNode {
public:
    Song* song;
    SongNode* next;

    SongNode() : song(nullptr), next(nullptr) {}
    SongNode(Song* songPtr) : song(songPtr), next(nullptr) {}

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& song;
        ar& next;
    }
};

class Playlist
{
private:
    SongNode* head;
    SongNode* getNodeAtPosition(int pos);
    int getCount();
public:
    string name;
    Playlist();
    Playlist(const string& playlistName) : name(playlistName), head(nullptr) {}
    void addSong(Song* song);
    void removeSong(const string& songName);
    void displayPlaylist();
    void shufflePlaylist();
    Playlist& operator=(const Playlist& other);
    ~Playlist();
    SongNode* merge(SongNode* left, SongNode* right);
    SongNode* mergeSort(SongNode* head);
    void sortPlaylist();
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& name;
        ar& head;
    }
};

void PrintStack(stack<Playlist*> p1) {
    cout << "Playlist Names in the Stack:" << endl;
    while (!p1.empty()) {
        cout << p1.top()->name << endl;
        p1.pop();
    }
}


class User {
public:
    string username;
    string password;
    stack<Playlist*> SavedPlaylists;
    stack<Song*> ListeningHistory;
    stack<Song*> LikedSongs;
    unordered_map<Song, int, Song::HashFunction, Song::EqualityComparison> MostListenedToSongs;

    User(string usern, string passw);
    User();
    void displayProfile();

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& username;
        ar& password;
        ar& SavedPlaylists;
        ar& ListeningHistory;
        ar& LikedSongs;
        ar& MostListenedToSongs;
    }
};

class UserHashTable {
    vector<list<User>> HashTable;
    static const size_t TABLE_SIZE = 100;
    size_t HashFunction(string key);

public:
    void LoadFromFile();
    UserHashTable() : HashTable(TABLE_SIZE) {};
    void InsertUser(User& user);
    boost::optional<User> ValidateLogin(const string& username, const string& password);

private:
    string hashpassword(const string& password);
};
class SongDB {
    //THIS IS WHERE AVL TREES COME
};
class UserMenu {
public:
    void SignUpPrompt();
    void LoginPrompt();
    void Welcome();
    void MainMenu(User*& u1, int value = 0);
    void ProfileMenu(User*& u1);
    void Logo();
};