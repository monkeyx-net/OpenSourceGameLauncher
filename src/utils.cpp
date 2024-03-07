#include <fstream>
#include <sstream>
#include <vector>



// Write File Function
std::string write_file(std::string filename, int player, int vecIp4[]) {
    //printf("Player = %d.%d.%d.%d", vecIp4[0],vecIp4[1],vecIp4[2],vecIp4[3]);
    std::ostringstream stream;
    for (int i = 0; i < 4; i++)
    {
        if (i) stream << '.';
        stream << vecIp4[i];
    }
    std::ofstream file(filename);
    std::string ip_string = stream.str();
    file << ip_string;
    file.close();
    //ips_load = ip_string;
    return ip_string;
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
std::string read_file(std::ifstream in_text )
{
    //std::ifstream in_text (in_text);
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
  return all_lines;
}

// SPlit function
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




