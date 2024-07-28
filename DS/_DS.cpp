#include "Header.h"
#include "emoji.h"
#include <windows.h>
#include <cwchar>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <stack>
#include "cryptlib.h"
#include "sha.h"
#include "hex.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/access.hpp>
#include "boost/serialization/deque.hpp"
#include "boost/serialization/stack.hpp"
#include <boost/serialization/queue.hpp>
#include <boost/optional.hpp>
#include<boost/unordered_map.hpp>
#include <unordered_map>
#include <algorithm>
#include <thread>
#include <chrono>
using namespace std;

string Song::getName()
{
    return name;
}
float Song::getDuration()
{
    return duration;
}
float Song::getEnergy()
{
    return energy;
}
float Song::getKey()
{
    return key;
}
float Song::getLoudness()
{
    return loudness;
}
float Song::getSpeechiness()
{
    return speechiness;
}
float Song::getAcousticness()
{
    return acousticness;
}
float Song::getInstrumentalness()
{
    return instrumentalness;
}
float Song::getLiveness()
{
    return liveness;
}
float Song::getTempo()
{
    return tempo;
}
void Song::setName(string n)
{
    name = n;
}
void Song::setDuration(float dur)
{
    duration = dur;
}
void Song::setEnergy(float en)
{
    energy = en;
}
void Song::setKey(float k)
{
    key = k;
}
void Song::setLoudness(float loud)
{
    loudness = loud;
}
void Song::setSpeechiness(float spch)
{
    speechiness = spch;
}
void Song::incStreams() {
    streams++;
}
void Song::setAcousticness(float ac)
{
    acousticness = ac;
}
void Song::setInstrumentalness(float instr)
{
    instrumentalness = instr;
}
void Song::setLiveness(float live)
{
    liveness = live;
}
void Song::setTempo(float tmp)
{
    tempo = tmp;
}

void Song::displayInfo()
{
    cout << "Name: " << name << endl;
    cout << "Duration: " << duration << endl;
    cout << "Energy: " << energy << endl;
    cout << "Key: " << key << endl;
    cout << "Loudness: " << loudness << endl;
    cout << "Speechiness: " << speechiness << endl;
    cout << "Acousticness: " << acousticness << endl;
    cout << "Instrumentalness: " << instrumentalness << endl;
    cout << "Liveness: " << liveness << endl;
    cout << "Tempo: " << tempo << endl;
}

Song::Song() {

}

SongNode* Playlist::getNodeAtPosition(int pos)
{
    SongNode* current = head;
    for (int i = 0; i < pos && current != NULL; ++i) {
        current = current->next;
    }
    return current;
}

int Playlist::getCount() {
    int count = 0;
    SongNode* current = head;
    while (current != NULL) {
        ++count;
        current = current->next;
    }
    return count;
}
Playlist::Playlist() {
    head = NULL;
    name = " ";
}
void Playlist::addSong(Song* song)
{
    SongNode* newSong = new SongNode(song);
    if (head == NULL)
    {
        head = newSong;
    }
    else
    {
        SongNode* temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newSong;
    }
    cout << song->getName() << " has been added to " << name << "." << endl;
}

void Playlist::removeSong(const string& songName) {
    SongNode* current = head;
    SongNode* prev = NULL;

    while (current != NULL && current->song->getName() != songName) {
        prev = current;
        current = current->next;
    }

    if (current != NULL) {
        if (prev != NULL) {
            prev->next = current->next;
        }
        else {
            head = current->next;
        }
        delete current;
        cout << songName << " has been removed from " << name << "." << endl;
    }
    else {
        cout << songName << " is not in " << name << "." << endl;
    }
}
void Playlist::displayPlaylist()
{
    cout << "--------------------------------------------------" << endl;
    cout << "Playlist: " << name << endl;
    cout << "--------------------------------------------------" << endl;
    SongNode* current = head;
    string temp;
    while (current != NULL)
    {
        temp = current->song->getName();
        cout << temp << endl;
        current = current->next;
    }
    cout << endl;
}
void Playlist::shufflePlaylist() {
    srand(time(0));
    int count = getCount();
    for (int i = count - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        SongNode* songI = getNodeAtPosition(i);
        SongNode* songJ = getNodeAtPosition(j);
        swap(songI->song, songJ->song);
    }
    cout << name << " has been shuffled." << endl;
}
Playlist& Playlist::operator=(const Playlist& other) {
    if (this != &other) {
        while (head != NULL) {
            SongNode* temp = head;
            head = head->next;
            delete temp;
        }
        this->name = other.name;
        SongNode* otherCurrent = other.head;
        SongNode* current = NULL;
        while (otherCurrent != NULL) {
            Song* newSong = new Song(*otherCurrent->song);
            SongNode* newSongNode = new SongNode(newSong);

            if (current == NULL) {
                head = newSongNode;
            }
            else {
                current->next = newSongNode;
            }

            current = newSongNode;
            otherCurrent = otherCurrent->next;
        }
    }
    return *this;
}

Playlist::~Playlist() {
    while (head != NULL) {
        SongNode* temp = head;
        head = head->next;
        delete temp->song;
        delete temp;
    }
}
SongNode* Playlist::merge(SongNode* left, SongNode* right) {
    if (left == NULL) {
        return right;
    }
    if (right == NULL) {
        return left;
    }

    if (left->song->getName() < right->song->getName()) {
        left->next = merge(left->next, right);
        return left;
    }
    else {
        right->next = merge(left, right->next);
        return right;
    }
}
SongNode* Playlist::mergeSort(SongNode* head) {
    if (head == NULL || head->next == NULL)
    {
        return head;
    }
    SongNode* slow = head;
    SongNode* fast = head->next;
    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    SongNode* left = head;
    SongNode* right = slow->next;
    slow->next = NULL;
    left = mergeSort(left);
    right = mergeSort(right);
    return merge(left, right);
}
void Playlist::sortPlaylist()
{
    head = mergeSort(head);
    cout << name << " has been sorted alphabetically." << endl;
}

size_t UserHashTable::HashFunction(string key) {
    return hash < string>{}(key) % TABLE_SIZE;
}
string UserHashTable::hashpassword(const string& password) {
    using namespace CryptoPP;
    SHA1 hash;
    byte digest[SHA1::DIGESTSIZE];
    hash.CalculateDigest(digest, (const byte*)password.c_str(), password.length());
    HexEncoder encoder;
    string hashedPassword;
    encoder.Attach(new StringSink(hashedPassword));
    encoder.Put(digest, sizeof(digest));
    encoder.MessageEnd();
    return hashedPassword;
}

void UserHashTable::InsertUser(User& user) {
    size_t index = HashFunction(user.username);
    user.password = hashpassword(user.password);
    HashTable[index].emplace_back(user);
}

void UserHashTable::LoadFromFile() {
    {
        ifstream ifs("user_data.txt");
        boost::archive::text_iarchive iarchive(ifs);

        try {
            while (true) {
                User loadedUser;
                iarchive >> loadedUser;
                InsertUser(loadedUser);
            }
        }
        catch (const boost::archive::archive_exception& e) {
            if (e.code == boost::archive::archive_exception::input_stream_error) {
                cout << "End of file reached." << endl;
            }
            else {
                cerr << "Error during deserialization: " << e.what() << endl;
            }
        }
    }


}
boost::optional<User> UserHashTable::ValidateLogin(const string& username, const string& password) {
    size_t index = HashFunction(username);
    for (const auto& user : HashTable[index]) {
        if (user.username == username) {
            string hashEnteredPassword = hashpassword(password);
            cout << "Stored Hash: " << user.password << endl;
            cout << "Entered Hash: " << hashEnteredPassword << endl;
            if (user.password == hashEnteredPassword) {
                return boost::make_optional(user);
            }
        }
    }
    return boost::none;
}

void UserMenu::ProfileMenu(User*& u1) {
    Logo();
    cout << "1. View Profile" << endl;
    cout << "2. Logout" << endl;

    int choice;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
    case 1:
        u1->displayProfile();
        break;

    case 2:
        cout << "Logging out..." << endl;
        u1 = nullptr;
        break;

    default:
        cout << "Invalid choice. Please enter a valid option." << endl;
    }
}

void UserMenu::MainMenu(User*& u1, int value = 0) {
    using namespace std::chrono;
    using namespace std::this_thread;
    Logo();
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (value == 0) {
        SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
        cout << "Hello! ";
        sleep_until(system_clock::now() + 1s);
        cout << "Where would you like to start....." << endl;
        sleep_until(system_clock::now() + 0.5s);
    }
    else if (value == 1)
    {
        SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "Welcome Back!" << endl;
        sleep_until(system_clock::now() + 1s);
    }
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
    cout << "Press ";
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_BLUE);
    cout << "Enter To ";
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
    cout << "Continue ";
    while (1) {
        if (GetAsyncKeyState(VK_RETURN)) {
            LoginPrompt();
            break;
        }
    }
    _asm {
        call Clrscr
    }
    int choice;
    
        cout << "Main Menu:" << endl;
        cout << "1. Profile Menu" << endl;
        cout << "2.Playlist Menu" << endl << "3.Recommendations" << endl;
        cin >> choice; 
    
    switch (choice) {
    case 1: {
        ProfileMenu(u1);
        break;
    }
    case 2:
    {
        //Playlist myPlaylist("MyPlaylist"); 
        //string p;
        cout << "Would you like to:" << endl;
        cout << "1. Display Playlist" << endl;
        cout << "2. Add song" << endl;
        cout << "3. Remove song" << endl;
        cout << "4. Shuffle Playlist" << endl;
        cout << "Enter your choice: ";
        cin >> choice2;

        PlaylistChoice(choice2, p, myPlaylist);
        break;
    }
    case 3:
    {  
        cout << "Reccommending...";
        break;
    }
    }
}

void PlaylistChoice(int choice, string& playlistName, Playlist& playlist) {
    switch (choice) {
    case 1:
        playlist.displayPlaylist();
        break;

    case 2: {
        string songName;
        cout << "Enter the name of the song to add: ";
        cin >> songName;
        playlist.addSong(new Song(songName));
        break;
    }

    case 3: {
        string songName;
        cout << "Enter the name of the song to remove: ";
        cin >> songName;
        playlist.removeSong(songName);
        break;
    }

    case 4:{
        playlist.shufflePlaylist();
        break;
    }
    default:{
        cout << "Invalid choice. Please enter a valid option." << endl;
    }
}

void UserMenu::Welcome() {

    Logo();
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cursorpos;
    cursorpos.X = 45;
    cursorpos.Y = 6;
    SetConsoleCursorPosition(consoleHandle, cursorpos);
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
    cout << "Press ";
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_BLUE);
    cout << "TAB ";
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
    cout << "To SignUp!";
    cursorpos.X = 45;
    cursorpos.Y = 7;

    SetConsoleCursorPosition(consoleHandle, cursorpos);
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
    cout << "Press ";
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_BLUE);
    cout << "Enter";
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
    cout << " To Login!";

    while (1) {
        if (GetAsyncKeyState(VK_RETURN)) {
            LoginPrompt();
            break;
        }
        if (GetAsyncKeyState(VK_TAB)) {
            SignUpPrompt();
            break;
        }
    }
}

void UserMenu::Logo() {
    SetConsoleOutputCP(CP_UTF8);
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(consoleHandle, &consoleMode);
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << "\t\t\t\t\t\tMuSicHub" << endl;
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
    cout << "\t\t\t\tRecommending Songs Through User Behaviour" << endl;
    COORD cursorpos;
    cursorpos.X = 85;
    cursorpos.Y = 2;
    SetConsoleCursorPosition(consoleHandle, cursorpos);

    float dist;
    for (int i = 0; i <= 2 * 3; i++) {
        for (int j = 0; j <= 2 * 3; j++) {
            dist = sqrt((i - 3) * (i - 3) +
                (j - 3) * (j - 3));
            if (dist > 3 - 0.5 && dist < 3 + 0.5)
                cout << emojicpp::emojize(":musical_note:");
            else
                cout << " ";
        }

        cursorpos.Y++;
        SetConsoleCursorPosition(consoleHandle, cursorpos);
    }
}

void UserMenu::LoginPrompt() {
    system("cls");
    Logo();
    string username; char password[100];
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode;
    COORD coord;
    coord.X = 50;
    coord.Y = 2;
    SetConsoleCursorPosition(consoleHandle, coord);
    GetConsoleMode(consoleHandle, &consoleMode);
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
    cout << "Enter username : " << endl;
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
    SetConsoleCursorPosition(consoleHandle, coord);
    GetConsoleMode(consoleHandle, &consoleMode);
    coord.X = 68;
    coord.Y = 2;
    SetConsoleCursorPosition(consoleHandle, coord);
    cin >> username;
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_INTENSITY);
    coord.X = 50;
    coord.Y = 3;
    SetConsoleCursorPosition(consoleHandle, coord);
    cout << "Enter password : " << endl;
    coord.X = 68;
    coord.Y = 3;
    SetConsoleCursorPosition(consoleHandle, coord);
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
    int k = 1;
    while (password[k - 1] != '\r')
    {
        password[k] = _getch();
        if (password[k - 1] != '\r')
        {
            cout << "*";
        }
        k++;
    }
    UserHashTable uhs;
    uhs.LoadFromFile();
    string pass_a = password;
    if (uhs.ValidateLogin(username, pass_a)) {
        cout << "Login Successful!" << endl;
    }
    else {
        cout << "Login Unsuccessful!" << endl;
    }
}

User::User() {

}
void User::displayProfile() {
    HANDLE conhandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cord;
    cord.X = 2;
    cord.Y = 3;
    SetConsoleCursorPosition(conhandle, cord);
    cout << "Your Playlists : : ";
    PrintStack(SavedPlaylists);
}
User::User(string username, string password) {
    this->username = username;
    this->password = password;
}
void UserMenu::SignUpPrompt() {
    system("cls");
    Logo();
    string username; char password[100];
    HANDLE conhandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cord;
    cord.X = 40;
    cord.Y = 3;
    SetConsoleCursorPosition(conhandle, cord);
    cout << "Enter Username : ";
    cord.Y++;
    SetConsoleCursorPosition(conhandle, cord);
    cin >> username;
    cord.Y++;
    SetConsoleCursorPosition(conhandle, cord);
    cout << "Enter Password : ";
    cord.Y++;
    SetConsoleCursorPosition(conhandle, cord);
    int k = 1;
    while (password[k - 1] != '\r')
    {
        password[k] = _getch();
        if (password[k - 1] != '\r')
        {
            cout << "*";
        }
        k++;
    }
    {
        ofstream ofs("user_data.txt");
        boost::archive::text_oarchive oarchive(ofs);

        User user1(username, password);
        oarchive << user1;

        cout << "User Signed-Up successfully." << endl;
    }
}

int main() {
    UserMenu m1;
    m1.Welcome();
    return 0;
}