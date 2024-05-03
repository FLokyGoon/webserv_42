/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbardeau <fbardeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 12:54:12 by fbardeau          #+#    #+#             */
/*   Updated: 2024/03/21 15:30:25 by fbardeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/WebServer.hpp"

const int MAX_BUFFER_SIZE = 1024;

static void handle_alarm(int sig) 
{
    std::cerr << "Alarm signal (" << sig << ") received.\n";
}

std::string WebServer::executeCGI(const std::string& scriptPath, const std::string& request, int clientSocket) {
    int pipefd[2];
    if (pipe(pipefd) == -1) 
    {
        std::cerr << "Error creating pipe: " << strerror(errno) << std::endl;
        return sendInternalServerError(clientSocket);
    }

    pid_t pid = fork();
    if (pid == -1) 
    {
        std::cerr << "Error forking process: " << strerror(errno) << std::endl;
        close(pipefd[0]);
        close(pipefd[1]);
        return sendInternalServerError(clientSocket);
    }

    if (pid == 0) 
    { 
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        
        string queryString = "name=fbardeau";
        string queryStringEnv = "QUERY_STRING=" + queryString;
        
        char* env[] = {
            
            const_cast<char*>(queryStringEnv.c_str()),
            NULL
        };

        signal(SIGALRM, handle_alarm);
        alarm(5);
        execle(scriptPath.c_str(), scriptPath.c_str(), NULL, env);
        std::cerr << "Error executing CGI script: " << strerror(errno) << std::endl;
        close(pipefd[1]);
        _exit(EXIT_FAILURE);
    } 
    else 
    { 
        close(pipefd[1]); 

        char buffer[MAX_BUFFER_SIZE];
        ssize_t bytesRead = read(pipefd[0], buffer, sizeof(buffer));
        close(pipefd[0]);

        if (bytesRead == -1) 
        {
            std::cerr << "Error reading from pipe: " << strerror(errno) << std::endl;
            return sendInternalServerError(clientSocket);
        }
        else
            cout << "read all right" << endl;

        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) 
        {
            return std::string(buffer, bytesRead);
        } 
        else 
        {
            std::cerr << "CGI script failed to execute" << std::endl;
            return sendInternalServerError(clientSocket);
        }
    }
}