#include "WavDecoder.h"
#include "Utility.h"

#include <iterator> 
#include <string>
#include <array>
#include <iostream>



using namespace Decoder;
using namespace Utils;
using namespace std;


WavDecoder::WavDecoder(std::string wavFile)	
	: inFile(wavFile, ios::binary)
{	
	if (!inFile)
	{
		throw "Invalid File";
	}

	decodeHeader();

	wavDataBuffer.reserve(header.channels); 
}

WavDecoder::~WavDecoder()
{

}

std::string ReadString(std::istream& inFile, int size)
{
	std::vector<char> bytes(size);
	inFile.read(&bytes[0], size); 

	std::string str(std::begin(bytes), std::end(bytes));
	return str;
}

uint32_t ReadLittleEndian32(std::istream& inFile)
{	
	uint8_t bytes[4];
	inFile.read((char*)&bytes[0], sizeof(uint32_t)); 
	uint32_t value;
	
	if (Utility::IsBigEndian()) 
	{
		value = bytes[3] | (bytes[2] << 8) | (bytes[1] << 16) | (bytes[0] << 24);		
	}
	else
	{
		value = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
	}


	return value;
}

uint16_t ReadLittleEndian16(std::istream& inFile)
{
	char bytes[2];
	inFile.read(&bytes[0], sizeof(uint16_t));
	uint16_t value;

	if (Utility::IsBigEndian()) 
	{		
		value = bytes[1] | (bytes[0] << 8);
	}
	else
	{
		value = bytes[0] | (bytes[1] << 8);
	}


	return value;
}



void WavDecoder::decodeHeader()
{
	// http://soundfile.sapp.org/doc/WaveFormat/

	unsigned long long fileSize = Utility::GetStreamSize(inFile);
	if (fileSize <= WAVHeaderSize)
	{
		throw "Invalid file. Too small file";
	}
	
	
	if (ReadString(inFile, 4) != "RIFF")
	{
		throw "No RIFF found";
	}
	

	ReadLittleEndian32(inFile);  //Chunk size.


	if (ReadString(inFile, 4) != "WAVE")
	{
		throw "Not WAVE Type";
	}

	//Move till we reach fmt
	while (false == ReadComplete())
	{
		if (ReadString(inFile, 4) == "fmt ")
		{
			break;
		}
	}

	if (ReadComplete())
	{
		throw "No fmt found";
	}

		
	uint32_t formatChunkSize = ReadLittleEndian32(inFile);
	uint16_t audioFormat = ReadLittleEndian16(inFile);

	if (audioFormat != PCMAudioFormat)
	{
		throw "Not pcm format";
	}

	header.channels = ReadLittleEndian16(inFile);
	header.samplesPerSec = ReadLittleEndian32(inFile);;
	header.avgBytesPerSec = ReadLittleEndian32(inFile);
	header.blockAlign = ReadLittleEndian16(inFile);
	header.bitsPerSample = ReadLittleEndian16(inFile);;
	header.bytesPerSample = header.blockAlign / header.channels;


	//Move till we reach data sub chunk
	while (false == ReadComplete())
	{
		if (ReadString(inFile, 4) == "data")
		{
			break;
		}
	}

	if (ReadComplete())
	{
		throw "No data sub chunk";
	}

	uint32_t dataSize = ReadLittleEndian32(inFile);

	wavDataBuffer.resize(header.channels, 0);
}

const WavHeader& WavDecoder::GetHeader() const
{
	return header;
}



bool WavDecoder::ReadComplete() const
{ 
	return inFile.eof();
}


const std::vector<int16_t>& WavDecoder::ReadSample()
{		
	
	//Read one sample. Read bytes-> bytesPerSample * number of channel
	if (header.bitsPerSample == 16) 
	{				
		// if big endian: swap and store, else just read
		inFile.read(reinterpret_cast<char*>(&wavDataBuffer[0]), 2 * header.channels);
	
		if (Utility::IsBigEndian())
		{
			for (auto &sample : wavDataBuffer)
			{
				sample = ((sample & 0xFF) << 8) | ((sample & 0xFF00) >> 8);
			}			
		}		
	}
	else if (header.bitsPerSample == 8)
	{
		std::vector<unsigned char> sampleData(header.channels);
		inFile.read(reinterpret_cast<char*>(&sampleData[0]), 1 * header.channels);

		for (int i = 0; i < sampleData.size(); i++)
		{
			wavDataBuffer[i] = 257 * (sampleData[i])-32768; // [0,255] to [-32768,32767]
		}
		
	}
	else
	{
		throw "Not supported.";
	}


	return wavDataBuffer;
}

