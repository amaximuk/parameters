#include <iostream>
#include <memory>
#include <cmath>
#include <algorithm>
#include <cctype>
#include <string>
#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>

#ifdef WIN32
#include "windows.h"
#endif

#include "../git_commit_hash.h"
#include "parameters/formatter_code.h"
#include "parameters/formatter_wiki.h"
#include "parameters/formatter_html.h"
#include "parameters/yaml_parser.h"

#define TL(message) do { if (!is_batch) std::cout << message << std::endl; } while(0)
#define ELRF(message) do { if (!is_batch) std::cout << message << std::endl; return -1; } while(0)

char* GetCmdOption(char** begin, char** end, const std::string& option)
{
	char** itr = std::find(begin, end, option);
	if (itr != end && ++itr != end)
	{
		if (*itr[0] != '-')
			return *itr;
	}
	return 0;
}

bool CmdOptionExists(char** begin, char** end, const std::string& option)
{
	return std::find(begin, end, option) != end;
}

int main(int argc, char** argv)
{
#ifdef WIN32
	SetConsoleOutputCP(65001);
#endif

	if (argc < 2 || CmdOptionExists(argv, argv + argc, "-h"))
	{
		std::cout << "Git commit hash of parameters_compiler: " << GIT_COMMIT_HASH << std::endl;
		std::cout << "Usage: parameters_compiler config_file_name [options [parameters]]" << std::endl;
		std::cout << "Options:" << std::endl;
		std::cout << "    -h                    - Show help" << std::endl;
		std::cout << "    -b                    - Batch/bash mode" << std::endl;
		std::cout << "    -o [header_file_name] - Generate header [file name]" << std::endl;
		std::cout << "    -w [wiki_file_name]   - Generate wiki [file name]" << std::endl;
		std::cout << "    -t [html_file_name]   - Generate html [file name]" << std::endl;
		return -1;
	}

	std::string config_file_name = argv[1];

	std::string output_header_file_name = "";
	if (CmdOptionExists(argv, argv + argc, "-o"))
	{
		output_header_file_name = config_file_name + ".h";
		char* header_file_name = GetCmdOption(argv, argv + argc, "-o");
		if (header_file_name)
			output_header_file_name = header_file_name;
	}

	std::string output_wiki_file_name = "";
	if (CmdOptionExists(argv, argv + argc, "-w"))
	{
		output_wiki_file_name = config_file_name + ".txt";
		char* wiki_file_name = GetCmdOption(argv, argv + argc, "-w");
		if (wiki_file_name)
			output_wiki_file_name = wiki_file_name;
	}

	std::string output_html_file_name = "";
	if (CmdOptionExists(argv, argv + argc, "-t"))
	{
		output_html_file_name = config_file_name + ".html";
		char* html_file_name = GetCmdOption(argv, argv + argc, "-t");
		if (html_file_name)
			output_html_file_name = html_file_name;
	}

	bool is_batch = false;
	if (CmdOptionExists(argv, argv + argc, "-b"))
		is_batch = true;

	parameters::file_info file_info{};
	if (!parameters::yaml::parser::parse(config_file_name, is_batch, file_info))
		ELRF("File " << config_file_name << " parse failed");

	TL("File " << config_file_name << " parsed successfully");
	
	// Save header file
	if (output_header_file_name != "")
	{
		std::list<std::string> code;
		if (!parameters::formatter::code_formatter::format(file_info, is_batch, code))
			ELRF("Get code failed " << config_file_name);

		std::ofstream output_header_file(output_header_file_name);
		if (!output_header_file.is_open())
			ELRF("File " << output_header_file_name << " open failed");
		for (const auto& s : code)
			output_header_file << s << std::endl;
		output_header_file.close();
		TL("File " << output_header_file_name << " saved as header successfully");
	}

	// Save wiki file
	if (output_wiki_file_name != "")
	{
		std::list<std::string> wiki;
		if (!parameters::formatter::wiki_formatter::format(file_info, is_batch, wiki))
			ELRF("Get wiki failed " << config_file_name);

		std::ofstream output_wiki_file(output_wiki_file_name);
		if (!output_wiki_file.is_open())
			ELRF("File " << output_wiki_file_name << " open failed");
		for (const auto& s : wiki)
			output_wiki_file << s << std::endl;
		output_wiki_file.close();
		TL("File " << output_wiki_file_name << " saved as wiki successfully");
	}

	// Save html file
	if (output_html_file_name != "")
	{
		std::list<std::string> html;
		if (!parameters::formatter::html_formatter::format(file_info, is_batch, html))
			ELRF("Get html failed " << config_file_name);

		std::ofstream output_html_file(output_html_file_name);
		if (!output_html_file.is_open())
			ELRF("File " << output_html_file_name << " open failed");
		for (const auto& s : html)
			output_html_file << s << std::endl;
		output_html_file.close();
		TL("File " << output_html_file_name << " saved as html successfully");
	}
	
	return 0;
}