#ifndef MP3ENCODER_H
#define MP3ENCODER_H

#include <string>
#include <memory>
#include <fstream>

#include "lame.h"
#include "WavDecoder.h"
#include "WavToMp3EncodeSettings.h"
          

namespace Encoder {

	// ==== Forward declaration ===	


	class Mp3Encoder 
	{

	public:

		Mp3Encoder(Settings::WavToMp3EncodeSettings settings);
		~Mp3Encoder();
		

		// Encode PCM data from wavDecoder to the output file
		void Encode(std::shared_ptr<Decoder::WavDecoder>& wavDecoder, std::string outfilename);

		//Avoid accidedntal copy and share of MP3Encoder
		Mp3Encoder(const Mp3Encoder&) = delete;
		const Mp3Encoder& operator=(const Mp3Encoder&) = delete;
		Mp3Encoder(Mp3Encoder&&) = delete;
		const Mp3Encoder& operator=(Mp3Encoder&&) = delete;
	private:
		lame_global_flags* lameHandle;		
		Settings::WavToMp3EncodeSettings encoderSettings;		
	};

} // namespace Encoder


#endif //AMP3ENCODER_H
