#ifndef WAVDECODER_H
#define WAVDECODER_H

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>

namespace Decoder
{

	struct WavHeader
	{
		uint16_t channels;
		uint32_t samplesPerSec;
		uint32_t avgBytesPerSec;
		uint16_t blockAlign;
		uint16_t bitsPerSample;
		uint32_t bytesPerSample;
	};

	// Decodes WAVE files and decodes to 16-bit signed PCM data.
	class WavDecoder
	{
	public:


		WavDecoder(std::string wavFile);
		~WavDecoder();

		const WavHeader& GetHeader() const;
		bool ReadComplete() const;


		// Read one sample. 
		// Output: reference 16-bit resolution PCM samples, valid up to the next call to readSample.
		const std::vector<int16_t>& ReadSample();

	private:
		const int PCMAudioFormat = 0X01;
		const unsigned long long WAVHeaderSize = 44;

		void decodeHeader();

		WavHeader header;
		std::ifstream inFile;
		std::vector<int16_t> wavDataBuffer;		
	};


} // namespace Decoder

#endif // WAVDECODER_H

