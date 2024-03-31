#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>

// Function to convert integer array to IP address string
std::string intArrayToIPAddress(int arr[], int size) {
    std::stringstream ss;
    for (int i = 0; i < size; ++i) {
        ss << arr[i];
        if (i < size - 1)
            ss << ".";
    }
    return ss.str();
}

// Function to write IP addresses to config file
void writeIPConfigToFile(const std::string& filename, int svr[], int ply1[], int ply2[], int ply3[], int ply4[], int size) {
    std::ofstream outputFile(filename);

    // Write server and clients IP addresses to the file
    if (outputFile.is_open()) {
        outputFile << "server1_ip=" << intArrayToIPAddress(svr, size) << std::endl;
        outputFile << "client1_ip=" << intArrayToIPAddress(ply1, size) << std::endl;
        outputFile << "client2_ip=" << intArrayToIPAddress(ply2, size) << std::endl;
        outputFile << "client3_ip=" << intArrayToIPAddress(ply3, size) << std::endl;
        outputFile << "client4_ip=" << intArrayToIPAddress(ply4, size) << std::endl;

        outputFile.close(); // Close the file
      //  std::cout << "Configuration file created successfully." << std::endl;
    } else {
        std::cerr << "Unable to open file!" << std::endl;
    }
}

void string_to_int_array(const std::string& ip_string, int int_array[3]) {
    std::stringstream ss(ip_string);
    char dot; // To store the dots
    int index = 0;
    while (ss >> int_array[index] >> dot && index < 4) {
        index++; // Move to the next element in the array
    }
}

// Run system command and capture output
std::string exec( const char* cmd ) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}
//Check file exits
std::string lfile_exists (std::string fname) 
{
    if (FILE *file = fopen(fname.c_str(), "r")) {
        fclose(file);
    }
    else{
        fname = "";
    }
    return fname;
}

// Read file
std::string read_file(std::string filename)
{
    std::ifstream in_text (filename);
    std::string line;
    std::string all_lines;
    if (in_text.is_open())
    {
        while ( getline (in_text,line) )
        {
            all_lines += line + "\n";
        }
    in_text.close();
  }  
  else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
  return all_lines;
}

// Split function
std::vector<std::string> split (const std::string &s, char delim)
 {
    std::vector<std::string> result;

    std::stringstream ss (s);
    std::string item;
    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

    // Read and parse config.txt
    std::map<std::string, std::string> readKeyValuePairsFromFile(const std::string& filename) {
    std::ifstream file(filename); // Open the file
    std::map<std::string, std::string> keyValueMap;

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) { // Read each line
            std::istringstream iss(line);
            std::string key, value;
            if (std::getline(iss, key, '=')) { // Split at '='
                if (std::getline(iss, value)) { // Extract text after '='
                    keyValueMap[key] = value;
                }
            }
        }
        file.close(); // Close the file
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    return keyValueMap;
}