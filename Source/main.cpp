#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <vector>

#include "WavDecoder.h"
#include "Mp3Encoder.h"
#include "Utility.h"
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

using namespace std;
using namespace Utils;
using namespace Settings;
using namespace Decoder;
using namespace Encoder;


const int NumberOfCommandLineArguments = 2;
const string WavFileExtension = ".wav";

void* EncodeWavToMp3(void* arguments);

/*
     Based on LAME
     internal algorithm selection.  True quality is determined by the bitrate
     but this variable will effect quality by selecting expensive or cheap algorithms.
     quality=0..9.  0=best (very slow).  9=worst.
     recommended:  2     near-best quality, not too slow
                   5     good quality, fast
                   7     ok quality, really fast
*/
const int WavToMp3EncodeQuality = 2; //


int main(int argc, char* argv[])
{
    if (argc < NumberOfCommandLineArguments) {
        cerr << "Missing directory " << endl;
        return -1;
    }
    
    if (argc > NumberOfCommandLineArguments) {
        cerr << "Too many command line arguments." << endl;
        cerr << "rovide only directory path containing wav files as argument" << endl;
        return -1;
    }


    //Get WAV files

    std::string directory(argv[1]);
    std::vector<std::string> files= Utility::GetFilesInDirectory(directory);

    if (0 == files.size())
    {
        cout << "No files found. Check directory path"<<endl;
        return 0;
    }

    std::vector<std::string> wavFiles = Utility::GetFilesWithMatchingExtension(files, WavFileExtension);
    if (0 == wavFiles.size())
    {
        cout << "No .wav files found." << endl;
        return 0;
    }
        

    //Encode wav to MP3. Each file in a separate thread
    const int numWavFiles = wavFiles.size();
    std::vector<pthread_t> threads(numWavFiles);

    int i = 0;
    for (auto& wavFile : wavFiles)
    {
        pthread_t thread;
        int rc = pthread_create(&(threads[i]), NULL, &EncodeWavToMp3, (void *) &wavFile);        
        if (rc) {
            cerr << "pthread_create error :" << rc <<" index: "<<i<<endl;
            return -1;
        }
        cout << "thread for encoding File:" << wavFile<< " created" <<endl;

        i++;
    }

    // wait for all threads to finish
    for (auto& thread : threads)
    {
        pthread_join(thread, nullptr); 
    }

    cout << "Successfully converted  WAV files to mp3." << endl;

    return 0;
}



void* EncodeWavToMp3(void* arguments)
{
    std::string wavFile = *((std::string *) arguments);

    std::string outfilename(wavFile);
    outfilename.replace(outfilename.size() - WavFileExtension.size(), WavFileExtension.size(), ".mp3");


    shared_ptr<WavDecoder> wav(new WavDecoder(wavFile));  

    WavToMp3EncodeSettings settings = WavToMp3EncodeSettings(WavToMp3EncodeQuality);
    Mp3Encoder mp3Encoder(settings);

    
    mp3Encoder.Encode(wav, outfilename);

    return nullptr;

}


