/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upload.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbardeau <fbardeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 17:40:56 by fbardeau          #+#    #+#             */
/*   Updated: 2024/03/19 17:29:59 by fbardeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

 #include "../includes/WebServer.hpp"
 #include "../includes/ConfigParse.hpp"
 #include <cstdlib>

string extractBoundary(const std::string& request)
{
    size_t pos = request.find("Content-Type: multipart/form-data; boundary=");
    
    if (pos == string::npos) 
        return "";
    pos += 44;
    size_t end = request.find("\r\n", pos);
    if (end == std::string::npos) 
        return "";
    std::string boundary = request.substr(pos, end - pos);
    
    return boundary;
}

vector<string> splitRequestByBoundary(const string& request, const string& boundary)
{
    vector<string> parts;
    string delimiter = "--" + boundary + "\r\n";
    size_t pos = 0, start = 0;
    
    while ((pos = request.find(delimiter, start)) != string::npos)
    {
        size_t end = request.find("\r\n--" + boundary + "--", pos + delimiter.length());
        if (end == string::npos)
            break;
        string part = request.substr(pos + delimiter.length(), end - pos - delimiter.length());
        parts.push_back(part);
        start = end + delimiter.length();
    }
    return parts;
}

bool isFilePart(const string& part)
{
    return part.find("Content-Disposition: form-data; name=\"filename\";") != string::npos;
}

std::string extractFilename(const std::string& part)
{
    size_t pos = part.find("filename=\"");
    if (pos == std::string::npos) return "";
    
    pos += 10;
    size_t end = part.find("\"", pos);
    if (end == std::string::npos) return "";
    
    return part.substr(pos, end - pos);
}

std::string extractFileContent(const std::string& part)
{
    size_t pos = part.find("\r\n\r\n");
    if (pos == std::string::npos) return "";
    
    return part.substr(pos + 4);
}

string handleFileUpload(const std::string& request) 
{
    string boundary = extractBoundary(request);
    string baseDir = "./html/upload/";
    vector<string> parts = splitRequestByBoundary(request, boundary);
    bool fileSaved = false;
    
    for (size_t i = 0; i < parts.size(); ++i) {
        if (isFilePart(parts[i]))
        {
            std::string filename = extractFilename(parts[i]);
            std::string fileContent = extractFileContent(parts[i]);
            string fullPath = baseDir + filename;
            std::ofstream file(fullPath.c_str(), std::ios::binary);
            
            if (file.is_open()) 
            {
                file << fileContent;
                file.close();
                fileSaved = true;
            }
            else {
                cerr << "Error: Unable to open file for writing." << endl;
            }
            break;
        }
    }
    string resourceLocation = "./html/upload/";
    std::ostringstream responseStream;
    responseStream << "HTTP/1.1 " << (fileSaved ? "201 Created\r\n" : "500 Internal Server Error\r\n")
                   << "Content-Type: text/plain\r\n";

    std::string responseBody = "Resource created successfully";
    responseStream << "Content-Length: " << responseBody.length() << "\r\n"
                   << (fileSaved ? ("Location: " + resourceLocation + "\r\n") : "")
                   << "\r\n"
                   << responseBody;
            
    cout << "\nREQUEST:\n" << responseStream.str() << endl;

    return responseStream.str();
}