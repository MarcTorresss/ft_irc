#include "ircserv.hpp"

void Server::addChannel(const std::string& channelName, const std::string& password) {
    (void) channelName;
    (void) password;
}

bool Server::validateChannelPassword(Client *cli, const std::string& channelName, const std::string& password) {
    
    for (size_t i = 0; i < _channels.size(); i++) {
        if (_channels[i].getName() == channelName) {
            // If the channel exists, validate the password
            if (_channels[i].getPass() == password) {
                std::cout << "Password is valid for channel: " << channelName << std::endl;
                _channels[i].addClient(cli);
                return true;
            } else {
                std::cout << "Invalid password for channel: " << channelName << std::endl;
                return false;
            }
        }
    }
    addChannel(channelName, password);
    return true;
}

std::vector<std::string> splitString(const std::string& input, char delimiter)
{
    std::istringstream stream(input);
    std::string token;
    std::vector<std::string> ret;

    while (std::getline(stream, token, delimiter))
        ret.push_back(token);
    
    return ret;
}

void Server::_handleJoin(Client *cli, std::string& params)
{
    //param format [#channel1,#channel2 key1,key2]
    std::vector<std::pair<std::string, std::string> > channelKeyPairs;    std::vector<std::string> channels, keys;
    std::istringstream iss(params);
    std::string command, channIn, keysIn;
    iss >> command;

    std::getline(iss, channIn, ' ');
    std::getline(iss, keysIn);

    if (channIn.empty() || keysIn.empty())
        throw std::invalid_argument("Invalid JOIN format, [#channel1,#channel2] [key1,key2]");
    
    channels = splitString(channIn, ',');
    keys = splitString(keysIn, ',');

    if (channels.size() != keys.size())
        throw std::invalid_argument("Number of channels does not match number of keys");
    
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (channels[i].find("#") != 0)
            throw std::invalid_argument("Invalid channel format. Channels must start with '#'.");
        channelKeyPairs.push_back(std::make_pair(channels[i], keys[i]));
        const std::string& channelName = channelKeyPairs[i].first;
        const std::string& channelKey = channelKeyPairs[i].second;
        validateChannelPassword(cli, channelName, channelKey);
    }

    //parse channel and pass
    //if channel exists
        //check password
            //join client
    //else addChannel
}