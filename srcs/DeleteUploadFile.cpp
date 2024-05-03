/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteUploadFile.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbardeau <fbardeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 14:50:15 by fbardeau          #+#    #+#             */
/*   Updated: 2024/03/20 14:55:00 by fbardeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/WebServer.hpp"


bool isFile2(const string& path) 
{
    struct stat path_stat;
    stat(path.c_str(), &path_stat);
    return S_ISREG(path_stat.st_mode);
}

string deleteAllFilesInDirectory(const string& directoryPath) 
{
    DIR* dir = opendir(directoryPath.c_str());
    struct dirent* entry;
    ostringstream responseStream;

    if (dir == NULL) {
        cerr << "Could not open directory: " << directoryPath << std::endl;
        string errorBody = "Error opening directory.";
        responseStream << "HTTP/1.1 500 Internal Server Error\r\n"
                       << "Content-Type: text/plain\r\n"
                       << "Content-Length: " << errorBody.length() << "\r\n\r\n"
                       << errorBody;
        return responseStream.str();
    }

    bool errorOccurred = false;
    while ((entry = readdir(dir)) != NULL) {
        std::string entryName = entry->d_name;

        if (entryName == "." || entryName == "..") {
            continue;
        }

        std::string fullPath = directoryPath + "/" + entryName;
        if (isFile2(fullPath) && unlink(fullPath.c_str()) == -1) {
            cerr << "Failed to delete file: " << fullPath << std::endl;
            errorOccurred = true;
        }
    }
    closedir(dir);

    if (errorOccurred) {
        std::string errorBody = "Error deleting some files.";
        responseStream << "HTTP/1.1 500 Internal Server Error\r\n"
                       << "Content-Type: text/plain\r\n"
                       << "Content-Length: " << errorBody.length() << "\r\n\r\n"
                       << errorBody;
    } else {
        responseStream  << "HTTP/1.1 204 No Content\r\n"
                        << "Content-Type: text/plain\r\n"
                        << "Content-length: 26\r\n"
                        << "\r\n";
    }

    return responseStream.str();
}


