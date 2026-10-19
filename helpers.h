#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cctype>
using namespace std;

const int MAX = 50;

// =====================================================
// SHARED HELPER FUNCTIONS
// =====================================================

// Reads a full line (spaces allowed)
string readText(string message)
{
    string text;
    cout << message;
    getline(cin, text);
    return text;
}

// Reads a number. Returns -1 if the user typed something that is not a number
int readNumber(string message)
{
    string text = readText(message);
    if (text == "" || !isdigit(text[0]))
        return -1;
    return atoi(text.c_str());
}

// Keeps asking until the number is between low and high
int readRange(string message, int low, int high)
{
    int number = readNumber(message);
    while (number < low || number > high)
    {
        cout << "Please enter a number between " << low << " and " << high << ".\n";
        number = readNumber(message);
    }
    return number;
}

string toText(int number)
{
    stringstream ss;
    ss << number;
    return ss.str();
}

// Replaces special HTML characters so user text cannot break the page
string makeSafe(string text)
{
    string result = "";
    int length = text.length();
    for (int i = 0; i < length; i++)
    {
        if (text[i] == '&') result += "&amp;";
        else if (text[i] == '<') result += "&lt;";
        else if (text[i] == '>') result += "&gt;";
        else if (text[i] == '"') result += "&quot;";
        else result += text[i];
    }
    return result;
}

// Example: ("certificate_", "Ali Khan") -> "certificate_ali_khan.html"
string makeFileName(string prefix, string name)
{
    string result = prefix;
    int length = name.length();
    for (int i = 0; i < length; i++)
    {
        if (isalnum(name[i])) result += tolower(name[i]);
        else result += '_';
    }
    return result + ".html";
}

// =====================================================
// SHARED HTML / CSS GENERATOR
// =====================================================

// Opens the file and writes the top part of the page
bool createPage(ofstream& file, string fileName, string title, string css)
{
    file.open(fileName.c_str());
    if (!file)
    {
        cout << "Could not create " << fileName << "\n";
        return false;
    }
    file << "<!DOCTYPE html>\n<html>\n<head>\n<meta charset='UTF-8'>\n";
    file << "<title>" << makeSafe(title) << "</title>\n<style>\n";
    file << "body { font-family: Georgia, serif; background: #f0f0f0; padding: 20px; }\n";
    file << ".page { max-width: 800px; margin: 20px auto; background: white; padding: 40px; }\n";
    file << "img.photo { width: 110px; height: 110px; object-fit: cover; border-radius: 50%; }\n";
    file << "img.logo { max-height: 80px; }\n";
    file << "img.signature { max-height: 60px; }\n";
    file << "img.project { max-width: 100%; max-height: 200px; }\n";
    file << "@media print { body { background: white; padding: 0; } .page { margin: 0; } }\n";
    file << css << "</style>\n</head>\n<body>\n<div class='page'>\n";
    return true;
}

// Writes the bottom part of the page and closes the file
void finishPage(ofstream& file, string fileName)
{
    file << "</div>\n</body>\n</html>\n";
    file.close();
    cout << "Created " << fileName << "\n";
}

// Adds an optional image (does nothing if no path was given)
void writeImage(ofstream& file, string path, string cssClass)
{
    if (path != "")
        file << "<img class='" << cssClass << "' src='" << makeSafe(path) << "'>\n";
}

#endif