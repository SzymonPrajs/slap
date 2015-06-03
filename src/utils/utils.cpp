/*SLAP - Supernova Lightcurve Analysis Package
 *
 *Copyright (C) 2014  Szymon Prajs
 *
 *This program is free software; you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation; either version 2 of the License, or
 *(at your option) any later version.
 *
 *This program is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.
 *
 *You should have received a copy of the GNU General Public License along
 *with this program; if not, write to the Free Software Foundation, Inc.,
 *51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. 

 Contact author: S.Prajs@soton.ac.uk
 */

#include "utils.h"


void split(const string &s, char delim, vector<string> &elems) {
    elems.clear();
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }
}


vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}


void dirlist(const string &path, vector<string> &files) {
    files.clear();
    string temp;
    string ignore(".");
    DIR *dir;
    struct dirent *file;

    dir = opendir(path.c_str());
    if (dir != NULL) {
        while ((file = readdir(dir)) != NULL) {
            temp = string(file->d_name);

            if (temp.at(0) != ignore.at(0)) {
                files.push_back(temp);   
            }
        }
        closedir (dir);
    } else {
        cout << "ERROR: Cannot open directory " << path << endl;
    }
}


vector<string> dirlist(const string &path) {
    vector<string> files;
    dirlist(path, files);
    return files;
}


bool compareStrings(string i, string j) {
    return (i==j);
}

void copyFile(string in, string out) {
    ifstream source(in, ios::binary);
    ofstream dest(out, ios::binary);

    istreambuf_iterator<char> begin_source(source);
    istreambuf_iterator<char> end_source;
    ostreambuf_iterator<char> begin_dest(dest); 
    copy(begin_source, end_source, begin_dest);

    source.close();
    dest.close();
}