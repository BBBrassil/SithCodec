/**
 *	@file SithCodec.h
 *	@brief Library of functions to encode and decode audio formats for
 *		   <em>Star Wars: Knights of the Old Republic</em>.
 *	@author Brendan Brassil
 *	@date 2020-05-15 created
 *	@date 2020-05-17 last modified
 *
 *	@par Changelog
 *	@parblock
 *		2020-05-15
 *		- Wrote AudioFormat enumerator, headersMatch(), encodeAll(), decdeAl(),
 *		error messages.
 *		- Started formatOf(), encode(), decode().
 *	
 *		2020-05-16
 *		- Added printHeaderSourceCode().
 *		- Moved header globals to Header.h.
 *		- Updated formatOf() to check real headers from Header.h.
 *		- Added formatToString().
 *		- Finished writing encode(), decode() for now.
 *		- Updated encodeAll(), decodeAll() to support directories.
 *	
 *		2020-05-17
 *		- Updated encodeAll(), decodeAll() to verify that the output path is an
 *		  existing folder.
 *		- Debugged encode(), decode() file system stuff.
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

#ifndef SITHCODEC_H
#define SITHCODEC_H

#include "FileHeaders.h"
#include <cstddef>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/**
 *	@brief temporary file extension.
 */
#define TEMP ".tmp"

/**
 *	@brief MP3 file extension.
 */
#define MP3 ".mp3"

/**
 *	@brief WAV file extension.
 */
#define WAV ".wav"

/**
 *	@brief Error message for failure to open a file.
 */
#define OPEN_ERROR(path) "Failed to open \"" + path + "\"."

/**
 *	@brief End of file error message.
 */
#define EOF_ERROR(path) "Reached end of \"" + path + "\" before data could be read."

/**
 *	@brief Error message for failure to write a file.
 */
#define WRITE_ERROR(path) "Failed to write \"" + path + "\"."

/**
 *	@brief Error message for failure to delete a file.
 */
#define DELETE_ERROR(path) "Failed to delete \"" + path + "\"."

/**
 *	@brief Error message for failed operations.
 */
#define FAIL_ERROR "FAILED"

/**
 *	Project namespace.
 */
namespace SithCodec {
	namespace {
		/**
		 *	@brief Enumerator of KOTOR audio formats.
		 */
		enum class AudioFormat : unsigned char {
			NONE,
			SFX,
			VO
		};

		/**
		 *	@brief Converts an audio format to a human-readable string.
		 *	@return string
		 */
		std::string formatToString(AudioFormat format) {
			switch( format ) {
			case AudioFormat::SFX:
				return "SFX";
			case AudioFormat::VO:
				return "VO";
			}
			return "NONE";
		}

		/**
		 *	@brief Decodes a given file.
		 *
		 *	@param inStr path of the input file
		 *	@param outStr optional path of the final output file;
		 *				  default argument will use the original filename and
		 *				  potentially overwrite the file
		 *
		 *	@throws runtime_error
		 */
		void decode(const std::string& inStr, const std::string& outStr = "");

		/**
		 *	@brief Decodes all of the files included in a given list of files.
		 *
		 *	@param inStr path of the list that contains the names of the files
		 *				 that we want to encode
		 *
		 *	@return vector of file names and whether the operation performed on them
		 *          was successful
		 *
		 *	@throws runtime_error
		 */
		std::vector<std::pair<std::string, bool>> decodeAll(
			const std::string& inStr, const std::string& outStr = "");

		/**
		 *	@brief Determines the audio format of a given file.]
		 *
		 *	@param inStr path of the file
		 *
		 *	@return audio format
		 *
		 *	@throws runtime_error
		 */
		AudioFormat formatOf(const std::string& inStr);

		/**
		 *	@brief Gets the size of an audio format's header.
		 *
		 *	@param format audio format
		 *
		 *	@return number of bytes
		 */
		std::size_t sizeOfHeader(AudioFormat format);

		/**
		 *	@brief Checks if two given headers are for the same audio format.
		 *
		 *	@param headerA pointer to first header's data
		 *	@param headerB pointer to second header's data
		 *	@param size number of characters in the header
		 */
		bool headersMatch(const char* headerA, const char* headerB, std::size_t size);

		/**
		 *	@brief Encodes a given file in a given format.
		 *
		 *	@param inStr path of the input file
		 *	@param format type of output audio file
		 *	@param outStr optional path of the final output file;
		 *				  default argument will use the original filename and
		 *				  potentially overwrite the file
		 *
		 *	@throws runtime_error
		 */
		void encode(const std::string& inStr, AudioFormat format,
			const std::string& outStr = "");

		/**
		 *	@brief Encodes all of the files included in a given list of files.
		 *
		 *	@param inStr path of the list that contains the names of the files
		 *				 that we want to encode
		 *	@param format type of output audio file
		 *
		 *	@return vector of file names and whether the operation performed on them
		 *          was successful
		 *
		 *	@throws runtime_error
		 */
		std::vector<std::pair<std::string, bool>> encodeAll(
			const std::string& inStr, AudioFormat format,
			const std::string& outStr = "");

		/**
		 *	@brief Prints individual bytes of a header as hex numbers.
		 *	@details Used to create headers in Header.h.
		 *
		 *	@param inStr file path
		 *	@param os output stream
		 */
		void printHeaderSource(const std::string& inStr, std::ostream& os = std::cout);

		/**
		 *	@brief Prints the format of every file in a given directory.
		 *
		 *	@param inStr path of directory containing audio files
		 *	@param outStr path of output file, or empty string for cout
		 *
		 *	@throws runtime_error
		 */
		void printFormats(const std::string& inStr, const std::string& outStr = "");

		AudioFormat formatOf(const std::string& inStr) {
			AudioFormat format;
			char header[Header::MAX_SIZE];
			std::fstream file;
			std::size_t index;

			file.open(inStr, std::ios_base::in, std::ios_base::binary);

			if( !file )
				throw std::runtime_error(OPEN_ERROR(inStr));

			index = 0;
			while( file && index < Header::MAX_SIZE ) {
				file.read(header + index, 1);
				++index;
			}

			// SFX
			if( index >= Header::SFX_SIZE && headersMatch(header, Header::SFX, Header::SFX_SIZE) )
				format = AudioFormat::SFX;
			// VO
			else if( index >= Header::VO_SIZE && headersMatch(header, Header::VO, Header::VO_SIZE) )
				format = AudioFormat::VO;
			// No match & EOF
			else if( !file ) {
				file.close();
				throw std::runtime_error(EOF_ERROR(inStr));
			}
			// No match, unknown header
			else format = AudioFormat::NONE;

			file.close();

			return format;
		}

		std::size_t sizeOfHeader(AudioFormat format) {
			switch( format ) {
			case AudioFormat::SFX:
				return Header::SFX_SIZE;
			case AudioFormat::VO:
				return Header::VO_SIZE;
			}
			return Header::MAX_SIZE;
		}

		bool headersMatch(const char* headerA, const char* headerB, std::size_t size) {
			for( std::size_t i = 0; i < size; ++i )
				if( headerA[i] != headerB[i] )
					return false;
			return true;
		}

		void decode(const std::string& inStr, const std::string& outStr) {
			std::filesystem::path inPath(inStr), outPath(outStr);
			std::string fileExt;
			std::fstream input, output;
			char ch;
			AudioFormat format;

			// Determine audio format
			try {
				format = formatOf(inStr);
			}
			catch( ... ) {
				throw;
			}
			// Do nothing for files without a KOTOR audio header
			if( format == AudioFormat::NONE )
				return;

			// Open input file
			input.open(inPath, std::ios_base::in | std::ios_base::binary);
			if( !input )
				throw std::runtime_error(OPEN_ERROR(inPath.string()));

			// Determine file extension
			if( (outStr == "" || std::filesystem::is_directory(outPath))
				&& format == AudioFormat::VO )
				fileExt = MP3;
			else if( outStr != "" && inPath != outStr )
				fileExt = "";
			else
				fileExt = TEMP;
			// Determine file path
			if( outStr == "" )
				outPath = inPath;
			else if( std::filesystem::is_directory(outPath) )
				outPath /= inPath.filename();
			// Replace extension if necessary
			if( fileExt != "" )
				outPath.replace_extension(fileExt);

			// Open output file
			output.open(outPath, std::ios_base::out | std::ios_base::binary);
			if( !output )
				throw std::runtime_error(WRITE_ERROR(outPath.string()));

			// Skip header
			switch( format ) {
			case AudioFormat::SFX:
				input.seekg(Header::SFX_SIZE);
				break;
			case AudioFormat::VO:
				input.seekg(Header::VO_SIZE);
				break;
			}

			// Write file
			while( input ) {
				input.read(&ch, 1);
				output.write(&ch, 1);
			}

			// Done with files
			input.close();
			output.close();

			// Rename output file if necessary
			if( fileExt == TEMP ) {
				try {
					std::filesystem::remove(inPath);
				}
				catch( ... ) {
					throw std::runtime_error(DELETE_ERROR(inPath.string()));
				}
				try {
					std::filesystem::rename(outPath, inPath);
				}
				catch( ... ) {
					throw std::runtime_error(DELETE_ERROR(outPath.string()));
				}
			}
		}

		std::vector<std::pair<std::string, bool>> decodeAll(const std::string& inStr,
			const std::string& outStr) {
			std::vector<std::pair<std::string, bool>> operations;
			std::fstream file;
			std::string str;

			// Check if input path exists
			if( !std::filesystem::exists(inStr) )
				throw std::runtime_error(OPEN_ERROR(inStr));
			// Check if output path is an existing folder
			if( outStr != "" &&
				(!std::filesystem::exists(outStr) ||
					!std::filesystem::is_directory(outStr)) )
				throw std::runtime_error(OPEN_ERROR(outStr));

			// Input path is a folder
			if( std::filesystem::is_directory(inStr) ) {
				for( const auto& p : std::filesystem::recursive_directory_iterator(inStr) ) {
					if( !std::filesystem::is_directory(p) )
						operations.emplace_back(p.path().string(), false);
				}
			}
			// Input path is a list of files
			else {
				file.open(inStr, std::ios_base::in);

				while( std::getline(file, str) )
					operations.emplace_back(str, false);

				file.close();
			}

			// Decode all files
			for( std::size_t i = 0; i < operations.size(); ++i ) {
				try {
					decode(operations[i].first, outStr);
					operations[i].second = true;
				}
				catch( ... ) {} // failed operations already set to false, do nothing
			}

			return operations;
		}

		void encode(const std::string& inStr, AudioFormat format,
			const std::string& outStr) {
			std::filesystem::path inPath(inStr), outPath(outStr);
			std::string fileExt, filePath;
			std::fstream input, output;
			std::size_t headerSize;
			const char* header;
			char ch;

			// Open input file
			input.open(inPath, std::ios_base::in | std::ios_base::binary);
			if( !input )
				throw std::runtime_error(OPEN_ERROR(inPath.string()));

			// Determine file extension
			if( (outStr == "" || std::filesystem::is_directory(outPath))
				&& format == AudioFormat::VO )
				fileExt = WAV;
			else if( outStr != "" && inPath != outStr )
				fileExt = "";
			else
				fileExt = TEMP;
			// Determine file path
			if( outStr == "" )
				outPath = inPath;
			else if( std::filesystem::is_directory(outPath) )
				outPath /= inPath.filename();
			// Replace extension if necessary
			if( fileExt != "" )
				outPath.replace_extension(fileExt);

			// Open output file
			output.open(outPath, std::ios_base::out | std::ios_base::binary);
			if( !output )
				throw std::runtime_error(WRITE_ERROR(outPath.string()));

			// Determine header info
			switch( format ) {
			case AudioFormat::SFX:
				header = Header::SFX;
				headerSize = Header::SFX_SIZE;
				break;
			case AudioFormat::VO:
				header = Header::VO;
				headerSize = Header::VO_SIZE;
				break;
			default:
				header = nullptr;
				headerSize = 0;
				break;
			}

			// Write file
			for( std::size_t i = 0; i < headerSize; ++i )
				output.write(header + i, 1);
			while( input ) {
				input.read(&ch, 1);
				output.write(&ch, 1);
			}

			// Done with files
			input.close();
			output.close();

			// Rename output file if necessary
			if( fileExt == TEMP ) {
				try {
					std::filesystem::remove(inPath);
				}
				catch( ... ) {
					throw std::runtime_error(DELETE_ERROR(inPath.string()));
				}
				try {
					std::filesystem::rename(outPath, inPath);
				}
				catch( ... ) {
					throw std::runtime_error(DELETE_ERROR(outPath.string()));
				}
			}
		}

		std::vector<std::pair<std::string, bool>> encodeAll(
			const std::string& inStr, AudioFormat format, const std::string& outStr) {
			std::vector<std::pair<std::string, bool>> operations;
			std::fstream file;
			std::string str;

			// Check if input path exists
			if( !std::filesystem::exists(inStr) )
				throw std::runtime_error(OPEN_ERROR(inStr));
			// Check if output path is an existing folder
			if( outStr != "" &&
				(!std::filesystem::exists(outStr) ||
					!std::filesystem::is_directory(outStr)) )
				throw std::runtime_error(OPEN_ERROR(outStr));

			// Input path is a folder
			if( std::filesystem::is_directory(inStr) ) {
				for( const auto& p : std::filesystem::recursive_directory_iterator(inStr) ) {
					if( !std::filesystem::is_directory(p) )
						operations.emplace_back(p.path().string(), false);
				}
			}
			// Input path is a list of files
			else {
				file.open(inStr, std::ios_base::in);

				while( std::getline(file, str) )
					operations.emplace_back(str, false);

				file.close();
			}

			// Encode all files
			for( std::size_t i = 0; i < operations.size(); ++i ) {
				try {
					encode(operations[i].first, format, outStr);
					operations[i].second = true;
				}
				catch( ... ) {} // failed operations already set to false, do nothing
			}

			return operations;
		}

		void printHeaderSource(const std::string& inStr, std::ostream& os) {
			try {
				std::stringstream ss;
				AudioFormat format = formatOf(inStr);
				std::fstream file;
				char ch;

				if( format == AudioFormat::NONE )
					os << "None\n";
				else {
					file.open(inStr, std::ios_base::in | std::ios_base::binary);

					if( !file )
						os << OPEN_ERROR(inStr) << '\n';

					ss << std::hex;
					for( std::size_t i = 0; i < sizeOfHeader(format); ++i ) {
						file.read(&ch, 1);
						ss << "(char)0x" << +ch << ",\n";
						os << ss.str();
						ss.str("");
					}
				}
			}
			catch( const std::exception & ex ) {
				os << ex.what() << '\n';
			}
		}

		void printFormats(const std::string& inStr, const std::string& outStr) {
			std::stringstream ss;
			std::filesystem::path inPath = inStr == "" ?
				std::filesystem::current_path() : inStr;
			std::fstream file;

			// Check if input path exists
			if( !std::filesystem::exists(inPath) )
				throw std::runtime_error(OPEN_ERROR(inPath.string()));

			// Print files & formats to buffer
			ss << inPath.string() << '\n';
			for( const auto& e : std::filesystem::recursive_directory_iterator(inPath) ) {
				if( std::filesystem::is_directory(e) )
					ss << "  " << e.path().string() << '\n';
				else {
					try {
						ss << "      " << e.path().filename().string() << ' ';
						ss << formatToString(formatOf(e.path().string())) << '\n';
					}
					catch( ... ) {
						ss << FAIL_ERROR << '\n';
					}
				}
			}

			// Output is file path
			if( outStr != "" ) {
				file.open(outStr, std::ios_base::out);

				if( !file )
					throw std::runtime_error(WRITE_ERROR(outStr));

				file << ss.str();
			}
			// Default output to console
			else
				std::cout << ss.str();
		}
	}
}

#undef TEMP
#undef MP3
#undef WAV
#undef OPEN_ERROR
#undef EOF_ERROR
#undef WRITE_ERROR
#undef DELETE_ERROR

#endif