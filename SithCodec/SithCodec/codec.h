/**
 *	@file codec.h
 *	@brief Library of functions to encode and decode audio formats for
 *		   <em>Star Wars: Knights of the Old Republic</em>.
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

#ifndef SITHCODEC_CODEC_H
#define SITHCODEC_CODEC_H

#include <cstddef>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "fileheaders.h"

 /**
  *	%SithCodec project namespace.
  */
namespace SithCodec {
	/**
	 *	@brief Enumerator of KOTOR audio formats.
	 */
	enum class AudioFormat {
		None,
		SFX,
		VO,
	};

	/**
		@brief Object containing a file path and an optional error message associated
			   with the file operation.
	 */
	struct FileOperation {
		std::filesystem::path path;
		std::optional<std::string> error;
	};

	constexpr const char* mp3 = ".mp3";
	constexpr const char* wav = ".wav";

	constexpr const char* indentLevel1 = "  ";
	constexpr const char* indentLevel2 = "    ";

	/**
	 *	@brief Loads list of file operations from a given path.
	 *
	 *	@param path path to a file containing a list of paths, or a folder
	 *
	 *	@return vector of FileOperaiton objects without error messages
	 */
	std::vector<FileOperation> loadOperations(const std::filesystem::path& path);

	/**
	 *	@brief Loads list of file operations from a folder.
	 *
	 *	@param path path of folder
	 *
	 *	@return vector of FileOperaiton objects without error messages
	 */
	std::vector<FileOperation> loadOperationsFromFolder(const std::filesystem::path& path);

	/**
	 *	@brief Loads list of file operations from a file.
	 *
	 *	@param path path to a file containing a list of paths
	 *
	 *	@return vector of FileOperaiton objects without error messages
	 */
	std::vector<FileOperation> loadOperationsFromFile(const std::filesystem::path& path);

	/**
	 *	@brief Encodes a given file in a given format.
	 *
	 *	@param inputPath  path of the input file
	 *	@param format	  format of output audio file
	 *	@param outputPath optional path of the final output file
	 *					  (default argument will use inputPath and potentially
	 *					  overwrite)
	 *
	 *	@throws runtime_error
	 */
	void encode(const std::filesystem::path& inputPath, AudioFormat format, const std::filesystem::path& outputPath = "");

	/**
	 *	@brief Encodes all of the files included in a given list of files.
	 *
	 *	@param inputPath  path of the list that contains the names of the files
	 *					  that we want to encode
	 *	@param format	  format of output audio file
	 *	@param outputPath optional path of the final output file
	 *					  (default argument will use inputPath and potentially
	 *					  overwrite)
	 *
	 *	@return vector of file paths with optional error messages if something went wrong
	 *
	 *	@throws runtime_error
	 */
	std::vector<FileOperation> encodeAll(const std::filesystem::path& inputPath, AudioFormat format, const std::filesystem::path& outputPath = "");

	/**
	 *	@brief Decodes a given file.
	 *
	 *	@param inputPath  path of the input file
	 *	@param outputPath optional path of the final output file
	 *					  (default argument will use inputPath and potentially
	 *					  overwrite)
	 *
	 *	@throws runtime_error
	 */
	void decode(const std::filesystem::path& inputPath, const std::filesystem::path& outputPath = "");

	/**
	 *	@brief Decodes all of the files included in a given list of files.
	 *
	 *	@param inputPath path of the list that contains the names of the files
	 *					 that we want to encode
	 *	@param outputPath optional path of the final output file
	 *					  (default argument will use inputPath and potentially
	 *					  overwrite)
	 *
	 *	@return vector of file paths with optional error messages if something went wrong
	 *
	 *	@throws runtime_error
	 */
	std::vector<FileOperation> decodeAll(const std::filesystem::path& inputPath, const std::filesystem::path& outputPath = "");

	/**
	 *	@brief Prints individual bytes of a header as hex numbers.
	 *	@details Used to create headers in Header.h.
	 *
	 *	@param path	  path of input file
	 *	@param output output stream
	 */
	void printHeaderSource(const std::filesystem::path& inputPath, std::ostream& output = std::cout);

	/**
	 *	@brief Prints the format of every file in a given directory.
	 *
	 *	@param inputPath  path of directory containing audio files
	 *	@param outputPath path of output file, or empty string for cout
	 *
	 *	@throws runtime_error
	 */
	void printFormats(const std::filesystem::path& inputPath, std::ostream& output = std::cout);

	/**
	 *	@brief Gets the bytes of an audio format's header.
	 *
	 *	@param format audio format
	 *
	 *	@return bytes
	 */
	constexpr const char* getHeader(AudioFormat format);

	/**
	 *	@brief Gets the size of an audio format's header.
	 *
	 *	@param format audio format
	 *
	 *	@return number of bytes
	 */
	constexpr std::streamsize sizeOfHeader(AudioFormat format);
	
	/**
	 *	@brief Skips a number of bytes equal to the audio format header size.
	 *	
	 *	@param input  input stream
	 *	@param format audio format
	 */
	void skipHeader(std::istream& input, AudioFormat format);

	/**
	 *	@brief Determines the audio format of a given file.]
	 *
	 *	@param input input stream
	 *
	 *	@return audio format
	 *
	 *	@throws runtime_error
	 */
	AudioFormat formatOf(std::istream& input);

	/**
	 *	@brief Converts an audio format to a human-readable string.
	 *
	 *	@param format audio format
	 *
	 *	@return string
	 */
	std::string toString(AudioFormat format);

	/**
		@brief Generates a random string.

		@param chars  characters to choose from
		@param length length of output string

		@return string
	 */
	std::string getRandomString(std::string_view chars, std::string::size_type length);

	/**
		@brief Generates a new, unique path in the user's temp folder.

		@return temporary file path
	 */
	std::filesystem::path getTempPath();

	/**
	 *	@brief Removes a directory from a path, leaving only the relative part.
	 *
	 *	@param path path
	 *	@param path directory to remove
	 *
	 *	@note if directory is totally unrelated to path, returns filename only
	 *
	 *	@return relative portion of path
	 */
	std::filesystem::path getRelativePath(const std::filesystem::path& path, const std::filesystem::path& directory);

	/**
	 *	@brief Gets the appropraite file extension for encoding.
	 *
	 *	@param format audio format
	 *
	 *	@return file extension
	 */
	constexpr const char* getEncodeExtension(AudioFormat format);

	/**
	 *	@brief Gets the appropraite file extension for decoding.
	 *
	 *	@param format audio format
	 *
	 *	@return file extension
	 */
	constexpr const char* getDecodeExtension(AudioFormat format);

	/**
	 *	@brief Error message for failure to open a file.
	 *
	 *	@param path path that raised error
	 *
	 *	@return string
	 */
	std::string openErrorMsg(const std::filesystem::path& path);

	/**
	 *	@brief End of file error message.
	 *
	 *	@param path path that raised error
	 *
	 *	@return string
	 */
	std::string eofErrorMsg(const std::filesystem::path& path);

	/**
	 *	@brief Error message for failure to write a file.
	 *
	 *	@param path path that raised error
	 *
	 *	@return string
	 */
	std::string writeErrorMsg(const std::filesystem::path& path);

	/**
	 *	@brief Error message for failure to delete a file.
	 *
	 *	@param path path that raised error
	 *
	 *	@return string
	 */
	std::string deleteErrorMsg(const std::filesystem::path& path);

	constexpr const char* formatErrorMsg = "invalid audio format";

	constexpr const char* failMsg = "failed!";
	constexpr const char* successMsg = "done!";

	constexpr const char* getHeader(AudioFormat format) {
		switch( format ) {
		case AudioFormat::SFX:
			return Header::sfx;
		case AudioFormat::VO:
			return Header::vo;
		default:
			return nullptr;
		}
	}

	constexpr std::streamsize sizeOfHeader(AudioFormat format) {
		switch( format ) {
		case AudioFormat::SFX:
			return Header::sfxSize;
		case AudioFormat::VO:
			return Header::voSize;
		default:
			return Header::maxSize;
		}
	}

	constexpr const char* getEncodeExtension(AudioFormat format) {
		return wav;
	}

	constexpr const char* getDecodeExtension(AudioFormat format) {
		return format == AudioFormat::VO ? mp3 : wav;
	}
}

#endif