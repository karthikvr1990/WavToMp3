**C++ command line application to encodes all WAV files present in a directory to MP3**


**Build**:
      
      Navigate to folder where this file is located.

	linux
		cmake -S .
		make	

	windows
		cmake -S .
		open VS solution created in above and build (If VS compiler) or else use any other build system found when above cmake was run


**Run**:

    "application name" "directory containing WAV-files to be encoded to MP3"

	example
	Linux: ./WavToMp3 test_data/
	Windows: WavToMp3.exe ..\test_data\
	(Application name after built is WavToMp3 in linux and WavToMp3.exe in windows(when built in VS))


**Notes**:
Only 8 and 16 bit PCMs are currently supported( mono and stereo)


**Sample Wav files**:
	test_data folder has sample Wav files (mono and stereo 8 and 16 bit)
