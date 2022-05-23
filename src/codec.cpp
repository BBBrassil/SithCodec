/**
 *	@file codec.pp
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

#include "codec.h"

#include <exception>
#include <fstream>
#include <iomanip>
#include <random>

namespace SithCodec {
	namespace fs = std::filesystem;
	using namespace std;

	vector<FileOperation> loadOperations(const fs::path& path) {
		return is_directory(path)
			? loadOperationsFromFolder(path)
			: loadOperationsFromFile(path);
	}

	vector<FileOperation> loadOperationsFromFolder(const fs::path& path) {
		vector<FileOperation> operations;

		for( const auto& entry : fs::recursive_directory_iterator(path) )
			if( !is_directory(entry) )
				operations.push_back({ entry.path(), nullopt });

		return operations;
	}

	vector<FileOperation> loadOperationsFromFile(const fs::path& path) {
		ifstream file(path);

		if( !file )
			throw runtime_error(openErrorMsg(path));

		vector<FileOperation> operations;
		string str;

		while( getline(file, str) )
			operations.push_back({ str, nullopt });

		return operations;
	}

	void encode(const fs::path& inputPath, AudioFormat format, const fs::path& outputPath) {
		ifstream input(inputPath, ios::binary);

		if( !input )
			throw runtime_error(openErrorMsg(inputPath));

		const auto tempPath = getTempPath();
		ofstream output(tempPath, ios::binary);

		if( !output )
			throw runtime_error(writeErrorMsg(tempPath));

		const char* header = getHeader(format);
		const auto headerSize = sizeOfHeader(format);

		output.write(header, headerSize);
		copy(istreambuf_iterator(input), {}, ostreambuf_iterator(output));
		input.close();
		output.close();

		if( exists(outputPath) ) {
			error_code error;

			remove(outputPath, error);

			if( error )
				throw runtime_error(deleteErrorMsg(outputPath));
		}
		else if( !outputPath.parent_path().empty() ) {
			create_directories(outputPath.parent_path());
		}

		rename(tempPath, fs::path(outputPath).replace_extension(getEncodeExtension(format)));
	}

	vector<FileOperation> encodeAll(const fs::path& inputPath, AudioFormat format, const fs::path& outputPath) {
		if( !exists(inputPath) )
			throw std::runtime_error(openErrorMsg(inputPath));

		const auto& outputDirectory = outputPath == "" ? inputPath : outputPath;

		if( !exists(outputDirectory) && !empty(outputDirectory) )
			create_directories(outputDirectory);

		if( !is_directory(outputDirectory) )
			throw runtime_error(openErrorMsg(outputPath));

		auto operations = loadOperations(inputPath);

		for( auto& op : operations ) {
			try {
				encode(op.path, format, outputDirectory / getRelativePath(op.path, inputPath));
			}
			catch( const exception& ex ) {
				op.error = ex.what();
			}
		}

		return operations;
	}

	void decode(const fs::path& inputPath, const fs::path& outputPath) {
		ifstream input(inputPath, ios::binary);

		if( !input )
			throw runtime_error(openErrorMsg(inputPath));

		auto tempPath = getTempPath();
		ofstream output(tempPath, ios::binary);

		if( !output )
			throw runtime_error(writeErrorMsg(tempPath));

		const auto format = formatOf(input);

		skipHeader(input, format);
		copy(istreambuf_iterator(input), {}, ostreambuf_iterator(output));
		input.close();
		output.close();

		if( exists(outputPath) ) {
			error_code error;

			remove(outputPath, error);

			if( error )
				throw runtime_error(deleteErrorMsg(outputPath));
		}
		else if( !outputPath.parent_path().empty() ) {
			create_directories(outputPath.parent_path());
		}

		rename(tempPath, fs::path(outputPath).replace_extension(getDecodeExtension(format)));
	}

	vector<FileOperation> decodeAll(const fs::path& inputPath, const fs::path& outputPath) {
		if( !exists(inputPath) )
			throw runtime_error(openErrorMsg(inputPath));

		const auto& outputDirectory = outputPath == "" ? inputPath : outputPath;

		if( !exists(outputDirectory) && !empty(outputDirectory) )
			create_directories(outputDirectory);

		if( !is_directory(outputDirectory) )
			throw runtime_error(openErrorMsg(outputPath));

		auto operations = loadOperations(inputPath);

		for( auto& op : operations ) {
			try {
				decode(op.path, outputDirectory / getRelativePath(op.path, inputPath));
			}
			catch( exception& ex ) {
				op.error = ex.what();
			}
		}

		return operations;
	}
	void printHeaderSource(const fs::path& inputPath, ostream& output) {
		try {
			ifstream file(inputPath, ios::binary);

			if( !file )
				output << openErrorMsg(inputPath) << '\n';

			AudioFormat format = formatOf(file);

			if( format == AudioFormat::None ) {
				output << toString(format);
			}
			else {
				auto flags = output.flags();
				char ch;

				output << hex;
				for( streamsize i = 0; i < sizeOfHeader(format); ++i ) {
					file.read(&ch, sizeof(char));
					output << "(char)0x" << +ch << ",\n";
				}
				output.flags(flags);
			}
		}
		catch( const exception& ex ) {
			output << ex.what() << '\n';
		}
	}

	void printFormats(const fs::path& inputPath, ostream& output) {
		const auto inputDirectory = inputPath == "" ? fs::current_path() : inputPath;

		if( !exists(inputDirectory) || !is_directory(inputDirectory) )
			throw runtime_error(openErrorMsg(inputDirectory));

		output << inputDirectory.string() << '\n';
		for( const auto& entry : fs::recursive_directory_iterator(inputDirectory) ) {
			if( is_directory(entry.path()) ) {
				output << indentLevel1 << entry.path().string() << '\n';
			}
			else {
				ifstream file(entry.path());

				output << indentLevel2 << entry.path().filename().string() << ' ' << (file ? toString(formatOf(file)) : failMsg) << '\n';
			}
		}
	}
	
	void skipHeader(istream& input, AudioFormat format) {
		switch( format ) {
		case AudioFormat::SFX:
			input.seekg(Header::sfxSize);
			break;
		case AudioFormat::VO:
			input.seekg(Header::voSize);
			break;
		}
	}

	AudioFormat formatOf(istream& file) {
		const auto pos = file.tellg();
		char header[Header::maxSize];

		file.seekg(0, ios::beg);
		file.read(header, Header::maxSize);
		file.seekg(pos, ios::beg);

		if( Header::sfxSize && equal(header, header + Header::sfxSize, Header::sfx) )
			return AudioFormat::SFX;
		else if( Header::voSize && equal(header, header + Header::voSize, Header::vo) )
			return AudioFormat::VO;
		else
			return AudioFormat::None;
	}

	string toString(AudioFormat format) {
		switch( format ) {
		case AudioFormat::SFX:
			return "SFX";
		case AudioFormat::VO:
			return "VO";
		default:
			return "None";
		}
	}

	string getRandomString(string_view chars, string::size_type length) {
		static auto rand = mt19937(random_device()());
		auto dist = uniform_int_distribution(static_cast<string_view::size_type>(0), chars.length() - 1);
		string str;

		str.reserve(length);
		for( string::size_type i = 0; i < length; ++i )
			str += chars[dist(rand)];

		return str;
	}

	fs::path getTempPath() {
		const char* chars =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"01234567890";
		const string::size_type length = 16;
		fs::path tempPath;

		do
			tempPath = fs::temp_directory_path() / getRandomString(chars, length);
		while( exists(tempPath) );

		return tempPath;
	}

	fs::path getRelativePath(const fs::path& path, const fs::path& directory) {
		auto relativePath = relative(path, directory);

		return relativePath.empty() ? path.filename() : relativePath;
	}

	string openErrorMsg(const fs::path& path) {
		return "Failed to open \"" + path.string() + "\".";
	}

	string eofErrorMsg(const fs::path& path) {
		return "Reached end of \"" + path.string() + "\" before data could be read.";
	}

	string writeErrorMsg(const fs::path& path) {
		return "Failed to write \"" + path.string() + "\".";
	}

	string deleteErrorMsg(const fs::path& path) {
		return "Failed to delete \"" + path.string() + "\".";
	}
}