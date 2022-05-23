/**
 *	@file main.cpp
 *	@brief Command-line interface for %SithCodec.
 *
 *	@copyright GNU General Public License
 *	@parblock
 *		Copyright (C) 2022 Brendan Brassil
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

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "codec.h"

namespace fs = std::filesystem;
using namespace std;
using namespace SithCodec;

/**
 *	@brief Enumerator describing menu action results
 */
enum class Result {
	Success,
	Failure,
	BadInput,
	Quit
};

/**
 *	@brief Displays the main menu screen.
 *
 *	@param output output stream
 *
 *	@return output
 */
void menu(istream& input = cin, ostream& output = cout);

/**
 *	@brief Displays the welcome screen.
 *
 *	@param output output stream
 *
 *	@return output
 */
ostream& welcome(ostream& output = cout);

/**
 *	@brief Displays the help screen.
 *
 *	@param output output stream
 *
 *	@return output
 */
ostream& help(ostream& output = cout);

/**
 *	@brief Displays commands screen.
 *
 *	@param output output stream
 *
 *	@return output
 */
ostream& commands(ostream& output = cout);

/**
 *	@brief Displays examples screen.
 *
 *	@param output output stream
 *
 *	@return output
 */
ostream& examples(ostream& output = cout);

/**
 *	@brief Executes a command based on the input arguments.
 *
 *	@param argc command-line argument count
 *	@param argv command-line argument vector
 *
 *	@return vector of arguments
 */
vector<string> parseArgs(int argc, const char* argv[]);

/**
 *	@brief Extracts arguments from a string.
 *
 *	@param str string containing arguments
 *
 *	@return vector of arguments
 */
vector<string> parseArgs(const string& str);

/**
 *	@brief Executes a command based on the input arguments.
 *
 *	@param args arguments
 *	@param log  output stream for logging
 *
 *	@return result of execution
 */
Result executeArgs(vector<string>& args, ostream& log = cout);

/**
 *	@brief Decodes an audio file.
 *
 *	@param inputPath  input file path
 *	@param format     audio format
 *	@param outputPath output file path
 *	@param log        output stream for logging
 */
void runEncode(const fs::path& inputPath, SithCodec::AudioFormat format, const fs::path& outputPath = "", ostream& log = cout);

/**
 *	@brief Encodes all audio files.
 *
 *	@param inputPath  path to list of audio files, or directory containing audio files
 *	@param format     audio format
 *	@param outputPath output file path
 *	@param log        output stream for logging
 */
void runEncodeAll(const fs::path& inputPath, SithCodec::AudioFormat format, const fs::path& outputPath = "", ostream& log = cout);

/**
 *	@brief Decodes an audio file.
 *
 *	@param inputPath  input file path
 *	@param outputPath output file path
 *	@param log        output stream for logging
 */
void runDecode(const fs::path& inputPath, const fs::path& outputPath = "", ostream& log = cout);

/**
 *	@brief Decodes all audio files.
 *
 *	@param inputPath  path to list of audio files, or directory containing audio files
 *	@param outputPath output directory
 *	@param log        output stream for logging
 */
void runDecodeAll(const fs::path& inputPath, const fs::path& outputPath = "", ostream& log = cout);

/**
 *	@brief Generates a list of all files in a directory and the corresponding
 *		   audio formats.
 *
 *	@param inputPath  directory to search
 *	@param outputPath output path of file, or empty string for cout
 *	@param log        output stream for logging
 *
 *	@warning If the current path is used, the executable will appear in the list.
 */
void runList(const fs::path& inputPath, const fs::path& outputPath = "", ostream& log = cout);

/**
 *	@brief Prints the status of a file operation.
 *
 *	@param op  FileOperation
 *	@param log output stream for logging
 */
void printLog(const FileOperation& op, ostream& log = cout);

/**
 *	@brief Converts a string to all lowercase.
 *
 *	@param str string to convert
 *
 *	@return lowercase string
 */
string toLowercase(string str);

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
AudioFormat toAudioFormat(const string& str);


int main(int argc, const char* argv[]) {
	vector<string> args = parseArgs(argc, argv);

	cout.sync_with_stdio(false);

	if( args.size() > 1 )
		executeArgs(args);
	else
		menu();

	return 0;
}

void menu(istream& input, ostream& output) {
	bool quit = false;

	output << welcome;

	do {
		output << ">";

		string str;

		getline(input, str);

		auto args = parseArgs(str);
		auto result = executeArgs(args, output);

		switch( result ) {
		case Result::Success:
			break;
		case Result::Failure:
			output << "Finished with errors.\n";
			break;
		case Result::BadInput:
			output << "Invalid input. You can enter -h or --help to get help.\n";
			break;
		case Result::Quit:
			quit = true;
			break;
		}
	} while( !quit );
}

ostream& welcome(ostream& output) {
	return output
		<< "-------------------------------------------------------------------------------\n"
		<< "SithCodec                                                                      \n"
		<< "                                                                               \n"
		<< "Converts audio formats for Star Wars: Knights of the Old Republic &            \n"
		<< "Star Wars: Knights of the Old Republic II - The Sith Lords                     \n"
		<< help;
}

ostream& help(ostream& output) {
	return output
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

ostream& commands(ostream& output) {
	return output
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

ostream& examples(ostream& output) {
	return output
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

vector<string> parseArgs(int argc, const char* argv[]) {
	vector<string> args(argc);

	for( size_t i = 0; i < argc; ++i )
		args[i] = argv[i];

	return args;
}

vector<string> parseArgs(const string& str) {
	vector<string> args;
	string arg;
	string::size_type length = str.length(), start = 0, end = 0;

	args.emplace_back(""); // to match command line arg count
	do {
		start = str.find('-', start);

		if( start == string::npos || start == length - 1 ) {
			arg = "";
		}
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

Result executeArgs(vector<string>& args, ostream& log) {
	string::size_type argc = args.size(), pos;
	string option, inputStr, outputStr, format, arg;

	for( size_t i = 1; i < argc; ++i ) {
		arg = toLowercase(args[i]);
		// Quit - immediate success
		if( arg == "-q" || arg == "--quit" )
			return Result::Quit;
		// Help screen - immediate success
		if( arg == "-h" || arg == "--help" ) {
			log << help;
			return Result::Success;
		}
		// Commands screen - immediate success
		if( arg == "-c" || arg == "--commands" ) {
			log << commands;
			return Result::Success;
		}
		// Examples screen - immediate success
		if( arg == "-x" || arg == "--examples" ) {
			log << examples;
			return Result::Success;
		}
		// Other options need to loop to get all their arguments
		// Decode
		if( arg == "-d" || arg == "--decode" ) {
			if( option != "" )
				return Result::BadInput;
			if( i == argc - 1 )
				return Result::BadInput;
			option = "d";
		}
		//Encode
		else if( arg == "-e" || arg == "--encode" ) {
			if( option != "" )
				return Result::BadInput;
			if( i == argc - 1 )
				return Result::BadInput;
			option = "e";
		}
		// List
		else if( arg == "-l" || arg == "--list" ) {
			if( option != "" )
				return Result::BadInput;
			option = "l";
		}
		// Decode/encode upgraded to decode all/encode all
		else if( arg == "-a" || arg == "--all" ) {
			if( option == "d" )
				option = "da";
			else if( option == "e" )
				option = "ea";
			else
				return Result::BadInput;
		}
		// Input path (can only be set once)
		else if( (pos = arg.find("-i")) == 0 ||
			arg.find("--in") == 0 ) {
			pos = pos != string::npos ? 3 : 5;
			if( inputStr != ""
				|| arg.length() < pos + 1
				|| arg[pos - 1] != '=' )
				return Result::BadInput;
			inputStr = args[i].substr(pos, arg.length() - pos);
		}
		// Output path (can only be set once)
		else if( (pos = arg.find("-o")) == 0 ||
			arg.find("--out") == 0 ) {
			pos = pos != string::npos ? 3 : 6;
			if( outputStr != ""
				|| arg.length() < pos + 1
				|| arg[pos - 1] != '=' )
				return Result::BadInput;
			outputStr = args[i].substr(pos, arg.length() - pos);
		}
		// Audio format (can only be set once)
		else if( arg == "-f" || arg == "--format" ) {
			if( ++i == argc )
				return Result::BadInput;
			if( format != "" )
				return Result::BadInput;
			format = args[i];
		}
		// Other errors
		else
			return Result::BadInput;
	}

	try {
		if( option == "d" )
			runDecode(inputStr, outputStr, log);
		else if( option == "da" )
			runDecodeAll(inputStr, outputStr, log);
		else if( option == "e" )
			runEncode(inputStr, toAudioFormat(format), outputStr, log);
		else if( option == "ea" )
			runEncodeAll(inputStr, toAudioFormat(format), outputStr, log);
		else if( option == "l" )
			runList(inputStr, outputStr, log);
		return Result::Success;
	}
	catch( const exception& ex ) {
		log << ex.what() << '\n';
		return Result::Failure;
	}
}

void runEncode(const fs::path& inputPath, AudioFormat format, const fs::path& outputPath, ostream& log) {
	if( format == AudioFormat::None ) {
		log << formatErrorMsg << '\n';
		return;
	}

	try {
		encode(inputPath, format, outputPath);
	}
	catch( const exception& ex ) {
		log << ex.what() << '\n';
	}
}

void runEncodeAll(const fs::path& inputPath, AudioFormat format, const fs::path& outputPath, ostream& log) {
	if( format == AudioFormat::None ) {
		log << formatErrorMsg << '\n';
		return;
	}

	try {
		const auto operations = encodeAll(inputPath, format, outputPath);

		for( const auto& op : operations )
			printLog(op, log);
	}
	catch( const exception& ex ) {
		log << indentLevel2 << ex.what() << '\n';
	}
}

void runDecode(const fs::path& inputPath, const fs::path& outputPath, ostream& log) {
	try {
		decode(inputPath, outputPath);
	}
	catch( const exception& ex ) {
		log << ex.what() << '\n';
	}
}

void runDecodeAll(const fs::path& inputPath, const fs::path& outputPath, ostream& log) {
	try {
		const auto operations = decodeAll(inputPath, outputPath);

		for( const auto& op : operations )
			printLog(op, log);
	}
	catch( const exception& ex ) {
		log << indentLevel2 << ex.what() << '\n';
	}
}

void runList(const fs::path& inputPath, const fs::path& outputPath, ostream& log) {
	try {
		if( outputPath == "" ) {
			printFormats(inputPath);
		}
		else {
			ofstream file(outputPath);

			if( !file )
				log << writeErrorMsg(outputPath);
			else
				printFormats(inputPath, file);
		}
	}
	catch( const exception& ex ) {
		log << indentLevel2 << ex.what() << '\n';
	}
}

void printLog(const FileOperation& op, ostream& log) {
	log << indentLevel1 << op.path.string() << ' ';
	if( op.error ) {
		log
			<< failMsg << '\n'
			<< indentLevel2 << op.error.value() << '\n';
	}
	else {
		log << successMsg << '\n';
	}
}

string toLowercase(string str) {
	for( string::size_type i = 0; i < str.size(); ++i )
		str[i] = tolower(static_cast<unsigned char>(str[i]));
	return str;
}

AudioFormat toAudioFormat(const string& str) {
	string lowercase = toLowercase(str);

	if( lowercase == "-m" || lowercase == "--music" ||
		lowercase == "-v" || lowercase == "--vo" )
		return AudioFormat::VO;
	else if( lowercase == "-s" || lowercase == "--sfx" )
		return AudioFormat::SFX;
	else
		return AudioFormat::None;
}