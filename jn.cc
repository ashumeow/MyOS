/**
 * jn.cc(experimental version 0.0.0.1)
 * 
 * Copyright 2013 MatriX <matrix@MatriX-Labs>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 * */

#include <iostream> // for std::cout
#include <fstream>  // for filestreams
#include <sstream>  // strings input/output buffer/manipulator
/**
 * 
 * name: version_help
 * @param: show_hepp_version
 * @return: void
 * @function: display help information
 * 
 * Comparison between char* and std::strings result in undefined behavior
 * even after C/C++-style casting it. I had to make an unnecessary copy to the char*
 * To test it yourself, comment(that is, //) the line where show_help_version was declared,
 * change show_help_version to show_hepp_version, reason for the weird name "hepp"
 *
 **/
std::string help = "Usage: jn [options] [STRINGs]...[FILEs]...[> FILE]\n" \
"Concatenate FILE(s) and(or) STRING(s), from standard input(or piped input) to standard output/files.\n" \
"OPTIONS\n" \
"  -n, --number             number all output lines \n" \
"  -h                       show help\n" \
"      --help     display this help and exit\n" \
"  -v  --version  output version information and exit\n" \
"\n" \
"With no FILE, shows this help and exit.\n" \
"\n" \
"Examples:\n" \
"  jn -n file.txt file2.txt > file3.txt     Output file1 and file2 contents into new file3 with each line numbered.\n" \
"  jn Pussy Cat Pussy Cat > children.txt    Output the strings into file childern.txt.\n" \
"\n" \
"Report \'jn\' bugs to ogunyinkajoshua@yahoo.com\n";



void version_help(char *show_help_version)
{
    
    if((std::string)show_help_version == "-v" || (std::string)show_help_version =="--version")
        std::cout << "jn: experimental version 0.0.0.1" <<std::endl;
    else if ((std::string)show_help_version == "-h" || (std::string)show_help_version == "--help")
        std::cout << help << std::endl;
}

int main(int argc, char* argv[], char *envp[])
{
    // no arguments? Show help & exit
    if(argc ==1){
        version_help((char*)"-h");
        return 0;
    }
    // otherwise do this part
    else
    {
        bool number_lines = false;
        int n = 1;
        // a string input/output buffer
        std::stringstream os;
        for(int i =1; i < argc; i++)
        {
            if((std::string)argv[i] == "-n" || (std::string)argv[i]=="--number")
                number_lines = true;
            
            // a string used to check if the filename/strings supplied contains "write-to-file" operator >
            std::string output_to_file = argv[i];
            version_help(argv[i]);
            
            if(i > 2 && i == (argc-1) && (*(output_to_file.cbegin()) == '>' || *(output_to_file.crbegin()) == '>'))
            {
                // FILE *manipulator for writing
                std::ofstream output_file;
                
                // Note the space: I check if the filename is of format (">%s", filename) or ("> %s", filename)
                if(output_to_file.length() > 2)
                    output_file.open(argv[i+1], std::ios::out);
                else
                {
                    // If the former is the case, strip off the "write-to-file" operator >
                    char *filename = (char *)(std::string(output_to_file.cbegin()+1,output_to_file.cend()).c_str());
                    output_file.open(filename, std::ios::out);
                }
                // the content of the output buffer is written to file, thereby emptying the output buffer
                output_file << os.str();
                output_file.close();
            }
            else
            {
                // try opening the filename specified in the argument variables, if it exists -- read from it
                std::ifstream in(argv[i]);
                if(in.is_open())
                {
                    std::string each_line;
                    while(in)
                    {
                        std::string temp_number{};
                        //if number_lines is set, cast a temporary ostringstream ref to a pointer and grab the str()
                        if(number_lines)
                           temp_number = static_cast<std::ostringstream *>(&(std::ostringstream()<<n))->str() + " ";
                        std::getline(in, each_line);  //read each lines, seekg points to the next line
                        os << temp_number<<each_line << std::endl;
                        n++;
                    }
                    in.close();
                }
                else
                // otherwise, write to the ouput buffer if argument isn't any of these
                    os << (((std::string)argv[i] == "-h"    || (std::string)argv[i] == "--help"
                             || (std::string)argv[i] == "-v"|| (std::string)argv[i] == "--version"
                             || (std::string)argv[i] == "-n"|| (std::string)argv[i] == "--number" ? "": argv[i]))
                        << "";
            }
        }
        std::cout << os.str() << std::endl;
    }
    return 0;
}
