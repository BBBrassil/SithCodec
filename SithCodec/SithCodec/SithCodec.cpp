/**
 *	@file SithCodec.cpp
 *	@brief Runs %SithCodec application through the command line.
 *	@author Brendan Brassil
 *	@date 2020-05-16 created
 *	@date 2020-05-17 last modified
 *
 *	@par Changelog
 *	@parblock
 *		2020-05-16
 *		- menu(), help(), toLowercase(), formatFromInput(), getArgs(), executeArgs(),
 *		- runDecode(), runDecodeAll(), runEncode(), runEncodeAll(), runList().
 *		
 *		2020-05-17
 *		- Split some of help off into new welcome, commands, and examples screens.
 *		- Debugged getArgs(), executeArgs().
 *	@endparblock
 *	
 *	@copyright GNU General Public License
 *	@parblock
 *		Copyright (C) 2020 Brendan Brassil
 *
 *		This file is part of %SithCodec.
 *		
 *		%SithCodec is free software: you can redistribute it and/or modify
 *		it under the terms of the GNU General Public License as published by
 *		the Free Software Foundation, either version 3 of the License, or
 *		(at your option) any later version.
 *		
 *		%SithCodec is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *		
*		You should have received a copy of the GNU General Public License
 *		along with %SithCodec. If not, see <https://www.gnu.org/licenses/>.
 *	@endparblock
 */

#include "SithCodec.h"

#include <filesystem>

using namespace std;

/**
 *	@brief Error message for failed operations.
 */
#define FAIL_ERROR "FAILED"

/**
 *	@brief Enumerator describing menu action results
 */
enum class Result {
	SUCCESS,
	FAILURE,
	BAD_INPUT,
	QUIT
};

/**
 *	@brief Displays the main menu screen.
 */
void menu();

/**
 *	@brief Displays the welcome screen.
 */
void welcome();

/**
 *	@brief Displays the help screen.
 */
void help();

/**
 *	@brief Displays commands screen.
 */
void commands();

/**
 *	@brief Displays examples screen.
 */
void examples();

/**
 *	@brief Converts a string to all lowercase.
 *
 *	@param str string
 *
 *	@return lowercase string
 */
string toLowercase(const string& str);

/**
 *	@brief Determines audio format based on input command.
 *
 *	@param str input string
 *
 *	@return audio format
 *
 *	@pre string must be lowercase
 *	@throws runtime_error
 */
SithCodec::AudioFormat formatFromInput(const string& str);

/**
 *	@brief Executes a command based on the input arguments.
 *
 *	@param argc command-line argument count
 *	@param argv command-line argument vector
 *
 *	@return vector of arguments
 */
vector<string> getArgs(int argc, const char* argv[]);

/**
 *	@brief Extracts arguments from a string.
 *
 *	@param str string containing arguments
 *
 *	@return vector of arguments
 */
vector<string> getArgs(const string& str);

/**
 *	@brief Executes a command based on the input arguments.
 *
 *	@param args arguments
 *
 *	@return result of execution - SUCCESS, FAILURE, BAD_INPUT, or QUIT
 */
Result executeArgs(vector<string>& args);

/**
 *	@brief Decodes an audio file.
 *
 *	@param inStr input file path
 *	@param outStr output file path
 */
void runDecode(const string& inStr, const string& outStr = "");

/**
 *	@brief Decodes all audio files.
 *
 *	@param inStr path to list of audio files, or directory containing audio files
 *	@param outStr output file path
 */
void runDecodeAll(const string& inStr, const std::string& outStr = "");

/**
 *	@brief Decodes an audio file.
 *
 *	@param inStr file path
 *	@param format audio format
 *	@param outStr output file path
 */
void runEncode(const string& inStr, SithCodec::AudioFormat format, const string& outStr = "");

/**
 *	@brief Encodes all audio files.
 *
 *	@param inStr path to list of audio files, or directory containing audio files
 *	@param format audio format
 *	@param outStr output file path
 */
void runEncodeAll(const string& inStr, SithCodec::AudioFormat format, const std::string& outStr = "");

/**
 *	@brief Generates a list of all files in a directory and the corresponding
 *		   audio formats.
 *
 *	@param inStr directory to search
 *	@param outStr output path of file, or empty string for cout
 *
 *	@warning If the current path is used, the executable will appear in the list.
 */
void runList(const string& inStr, const string& outStr = "");

/**
 *	@brief Main
 *	
 *	@param argc command-line argument count
 *	@param argv command-line argument vector
 *	
 *	@return nothing
 */
int main(int argc, const char* argv[]) {
	vector<string> args = getArgs(argc, argv);

	if( args.size() > 1 )
		executeArgs(args);
	else
		menu();

	return 0;
}

void menu() {
	bool quit = false;
	string input;
	vector<string> args;
	Result result;

	welcome();

	while( !quit ) {
		cout << ">";

		getline(cin, input);
		args = getArgs(input);

		result = executeArgs(args);

		switch( result ) {
		case Result::SUCCESS:
			break;
		case Result::FAILURE:
			cout << "Finished with errors.\n";
			break;
		case Result::BAD_INPUT:
			cout << "Invalid input. You can enter -h or --help to get help.\n";
			break;
		case Result::QUIT:
			quit = true;
		}
	}
}

void welcome() {
	cout
		<< "-------------------------------------------------------------------------------\n"
		<< "SithCodec                                                                      \n"
		<< "                                                                               \n"
		<< "Converts audio formats for Star Wars: Knights of the Old Republic &            \n"
		<< "Star Wars: Knights of the Old Republic II - The Sith Lords                     \n";
	help();
}

void help() {
	cout
		<< "-------------------------------------------------------------------------------\n"
		<< "Help                                                                           \n"
		<< "                                                                               \n"
		<< "-d, --decode                decode audio                                       \n"
		<< "-e, --encode                encode audio                                       \n"
		<< "-f, --format                set output audio format                            \n"
		<< "-m, --music                 streammusic format                                 \n"
		<< "-s, --sfx                   streamsounds format                                \n"
		<< "-v, --vo                    streamwaves/streamvoice format                     \n"
		<< "-a, --all                   all files                                          \n"
		<< "-l, --list                  list files & formats                               \n"
		<< "-i, --in                    input path                                         \n"
		<< "-o, --out                   output path                                        \n"
		<< "-h, --help                  display this menu                                  \n"
		<< "-c, --commands              display list of commands                           \n"
		<< "-x, --examples              display example commands                           \n"
		<< "-q, --quit                  exit application                                   \n"
		<< "-------------------------------------------------------------------------------\n"
		<< "                                                                               \n";
}

void commands() {
	cout
		<< "                                                                               \n"
		<< "-------------------------------------------------------------------------------\n"
		<< "Commands                                                                       \n"
		<< "                                                                               \n"
		<< "-d -i=[input path]                                                             \n"
		<< "-d -i=[input path] -o=[output path]                                            \n"
		<< "-d -a                                                                          \n"
		<< "-d -a -i=[input path]                                                          \n"
		<< "-d -a -o=[output path]                                                         \n"
		<< "-d -a -i=[input path] -o=[output path]                                         \n"
		<< "-e -f -[format] -i=[input path]                                                \n"
		<< "-e -f -[format] -i=[input path] -o=[output path]                               \n"
		<< "-e -a -f -[format]                                                             \n"
		<< "-e -a -f -[format] -i=[input path]                                             \n"
		<< "-e -a -f -[format] -o=[output path]                                            \n"
		<< "-e -a -f -[format] -i=[input path] -o=[output path]                            \n"
		<< "-l                                                                             \n"
		<< "-l -i=[input path]                                                             \n"
		<< "-l -o=[output path]                                                            \n"
		<< "-l -i=[input path] -o=[output path]                                            \n"
		<< "-------------------------------------------------------------------------------\n"
		<< "                                                                               \n";
}

void examples() {
	cout
		<< "                                                                               \n"
		<< "-------------------------------------------------------------------------------\n"
		<< "Examples                                                                       \n"
		<< "                                                                               \n"
		<< "Encode all files in SFX format from the input path, to the output path:        \n"
		<< "-e --all -f --sfx- -i=in_folder -o=out_folder                                  \n"
		<< "                                                                               \n"
		<< "Decode a file, outputting to a new file:                                       \n"
		<< "-d -i=oldfile.old -o=newfile.new                                               \n"
		<< "                                                                               \n"
		<< "Decode a file without specifying output, possibly overwriting the original:    \n"
		<< "-d -i=file.wav                                                                 \n"
		<< "                                                                               \n"
		<< "List all files & formats in a given directory, printing to the console:        \n"
		<< "-l -i=my_folder                                                                \n"
		<< "                                                                               \n"
		<< "List all files & formats in the current directory, printing to a file:         \n"
		<< "-l -o=file.txt                                                                 \n"
		<< "-------------------------------------------------------------------------------\n"
		<< "                                                                               \n";
}

string toLowercase(const string& str) {
	stringstream ss;

	for( size_t i = 0; i < str.size(); ++i )
		ss << static_cast<char>(tolower(str[i]));

	return ss.str();
}

SithCodec::AudioFormat formatFromInput(const string& str) {
	std::string lowercase = toLowercase(str);

	if( lowercase == "-m" || lowercase == "--music" ||
		lowercase == "-v" || lowercase == "--vo" )
		return SithCodec::AudioFormat::VO;
	if( lowercase == "-s" || lowercase == "--sfx" )
		return SithCodec::AudioFormat::SFX;
	throw runtime_error("No such audio format.");
}

void runDecode(const string& inStr, const string& outStr) {
	try {
		SithCodec::decode(inStr, outStr);
	}
	catch( const exception & ex ) {
		cout << ex.what() << '\n';
	}
}

void runDecodeAll(const string& inStr, const std::string& outStr) {
	vector<pair<string, bool>> operations;

	try {
		operations = SithCodec::decodeAll(inStr, outStr);
		for( const auto& o : operations )
			cout << "  " << o.first << " " << (o.second ? "" : FAIL_ERROR) << '\n';
	}
	catch( const exception & ex ) {
		cout << ex.what() << '\n';
	}
}

void runEncode(const string& inStr, SithCodec::AudioFormat format, const string& outStr) {
	try {
		SithCodec::encode(inStr, format, outStr);
	}
	catch( const exception & ex ) {
		cout << ex.what() << '\n';
	}
}

void runEncodeAll(const string& inStr, SithCodec::AudioFormat format,
	const std::string& outStr) {
	vector<pair<string, bool>> operations;

	try {
		operations = SithCodec::encodeAll(inStr, format, outStr);
		for( const auto& o : operations )
			cout << "  " << o.first << " " << (o.second ? "" : FAIL_ERROR) << '\n';
	}
	catch( const exception & ex ) {
		cout << ex.what() << '\n';
	}
}

void runList(const string& inStr, const string& outStr) {
	try {
		SithCodec::printFormats(inStr, outStr);
	}
	catch( const exception & ex ) {
		cout << ex.what() << '\n';
	}
}

vector<string> getArgs(int argc, const char* argv[]) {
	vector<string> args(argc);

	for( size_t i = 0; i < argc; ++i )
		args[i] = argv[i];

	return args;
}

vector<string> getArgs(const string& str) {
	vector<string> args;
	string arg;
	size_t length = str.length(), start = 0, end = 0;

	args.emplace_back(""); // to match command line arg count
	do {
		start = str.find('-', start);

		if( start == string::npos || start == length - 1 )
			arg = "";
		else {
			end = str.find('-', start + (start < length - 1 ? 2 : 1));

			if( end == string::npos )
				end = length;
			else
				--end;

			arg = str.substr(start, end - start);
			args.emplace_back(arg);
		}
		start += arg.length();
	} while( arg != "" && end < length );

	return args;
}

Result executeArgs(vector<string>& args) {
	size_t argc = args.size(), pos;
	string option, inStr, outStr, format, arg;

	for( size_t i = 1; i < argc; ++i ) {
		arg = toLowercase(args[i]);
		// Quit - immediate success
		if( arg == "-q" || arg == "--quit" )
			return Result::QUIT;
		// Help screen - immediate success
		if( arg == "-h" || arg == "--help" ) {
			help();
			return Result::SUCCESS;
		}
		// Commands screen - immediate success
		if( arg == "-c" || arg == "--commands" ) {
			commands();
			return Result::SUCCESS;
		}
		// Examples screen - immediate success
		if( arg == "-x" || arg == "--examples" ) {
			examples();
			return Result::SUCCESS;
		}
		// Other options need to loop to get all their arguments
		// Decode
		if( arg == "-d" || arg == "--decode" ) {
			if( option != "" )
				return Result::BAD_INPUT;
			option = "d";
			if( i == argc - 1 )
				return Result::BAD_INPUT;
		}
		//Encode
		else if( arg == "-e" || arg == "--encode" ) {
			if( option != "" )
				return Result::BAD_INPUT;
			option = "e";
			if( i == argc - 1 )
				return Result::BAD_INPUT;
		}
		// List
		else if( arg == "-l" || arg == "--list" ) {
			if( option != "" )
				return Result::BAD_INPUT;
			option = "l";
		}
		// Decode/encode upgraded to decode all/encode all
		else if( arg == "-a" || arg == "--all" ) {
			if( option == "d" )
				option = "da";
			else if( option == "e" )
				option = "ea";
			else
				return Result::BAD_INPUT;
		}
		// Input path (can only be set once)
		else if( (pos = arg.find("-i")) == 0 ||
			arg.find("--in") == 0 ) {
			pos = pos != string::npos ? 3 : 5;
			if( inStr != "" ||
				arg.length() < pos + 1
				|| arg[pos - 1] != '=' )
				return Result::BAD_INPUT;
			inStr = args[i].substr(pos, arg.length() - pos);
		}
		// Output path (can only be set once)
		else if( (pos = arg.find("-o")) == 0 ||
			arg.find("--out") == 0 ) {
			pos = pos != string::npos ? 3 : 6;
			if( outStr != "" ||
				arg.length() < pos + 1
				|| arg[pos - 1] != '=' )
				return Result::BAD_INPUT;
			outStr = args[i].substr(pos, arg.length() - pos);
		}
		// Audio format (can only be set once)
		else if( arg == "-f" || arg == "--format" ) {
			if( ++i == argc )
				return Result::BAD_INPUT;
			if( format != "" )
				return Result::BAD_INPUT;
			format = args[i];
		}
		// Other errors
		else
			return Result::BAD_INPUT;
	}

	// Call functions
	try {
		if( option == "d" )
			runDecode(inStr, outStr);
		else if( option == "da" )
			runDecodeAll(inStr, outStr);
		else if( option == "e" )
			runEncode(inStr, formatFromInput(format), outStr);
		else if( option == "ea" )
			runEncodeAll(inStr, formatFromInput(format), outStr);
		else if( option == "l" )
			runList(inStr, outStr);
		return Result::SUCCESS;
	}
	catch( const exception& ex ) {
		cout << ex.what() << '\n';
	}
	return Result::FAILURE;
}

#undef FAIL_ERROR