#ifndef WAV_TO_MP3_ENCODE_SETTINGS_H_
#define WAV_TO_MP3_ENCODE_SETTINGS_H_




namespace Settings {


	struct WavToMp3EncodeSettings
	{
		/*
			 internal algorithm selection.  True quality is determined by the bitrate
			 but this variable will effect quality by selecting expensive or cheap algorithms.
			 quality=0..9.  0=best (very slow).  9=worst.
			 recommended:  2     near-best quality, not too slow
						   5     good quality, fast
						   7     ok quality, really fast
		*/
		int quality;

		WavToMp3EncodeSettings(int a_Quality)
		{
			quality = a_Quality;
		}
	};


}; // namespace WAV_TO_MP3_ENCODE_SETTINGS_H_

#endif //WAV_TO_MP3_ENCODE_SETTINGS_H_
