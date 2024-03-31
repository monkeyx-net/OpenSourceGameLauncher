// Read file
std::string read_file(std::string filename);

//write config file
void writeIPConfigToFile(const std::string& filename, int svr[], int ply1[], int ply2[], int ply3[], int ply4[], int size);

//Functions for saving IPs
std::string intArrayToIPAddress(int arr[], int size);
void save_ips_to_file(const int* ips[], size_t size, const std::string& filename);


void string_to_int_array(const std::string& ip_string, int int_array[4]);
// Write File Function

// Run system command and capture output
std::string exec( const char* cmd );

//Check file exits
std::string lfile_exists (std::string fname);

//Split string
std::vector<std::string> split (const std::string &s, char delim);


// Read config file
std::map<std::string, std::string> readKeyValuePairsFromFile(const std::string& filename);


// need to mention a version string.
const char *argp_program_version = "Portmaster Game Options 0.0.12";

// documentation string that will be displayed in the help section.
static char doc[] = "This is an onging development. Please feedback suggestions and changes via the github page. https://github.com/monkeyx-net/Imgui4Portmaster";

// email address for bug reporting.
const char *argp_program_bug_address = "tim@monkeyx.net";

// argument list for doc. This will be displayed on --help
static char args_doc[] = "-w Screen width, -p players etc";

// cli argument availble options.
static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Produce verbose output"},
    {"width", 'w', "Width", 0,"Screen Width"},
    {"height", 'h', "Height", 0,"Screen Height"},
    {"scale", 's', "Scale", 0,"Screen Scale"},
    {"players", 'p', "Players", 0,"Number of players 0 - 4. 0 show server only.\nCreates escape codes 120 to use server ip\nPlayers 1-4 are 121-124"},
    {"title", 't', "Game", 0,"Game Title"},
    {"user", 'u', "User", 0,"User Name"},
    {"other", 'o', "Other", 0,"Other text"},

    {0}
};

// define a struct to hold the arguments.
struct arguments{
    int  verbose;
    char *args[1];
    char *width;
    char *height;
    char *scale;
    char *players;
    char *title;
    char *user;
    char *other;
};






