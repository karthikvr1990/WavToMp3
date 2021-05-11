#include "Mp3Encoder.h"

#include <ostream>
#include <iostream>


using namespace std;
using namespace Encoder;
using namespace Settings;
using namespace Decoder;


Mp3Encoder::Mp3Encoder(WavToMp3EncodeSettings settings)
    :encoderSettings(settings)
{
    lameHandle = lame_init();
    if (!lameHandle)
    {
        throw "lame_init() failed";
    }
}

Mp3Encoder::~Mp3Encoder()
{
    lame_close(lameHandle);
}


void Mp3Encoder::Encode(shared_ptr<WavDecoder>& wavDecoder, std::string outfilename)
{
    std::ofstream outFile(outfilename, ios::binary);

    if (!outFile)
    {
        throw "Invalid output file stream";
    }

    // initialize lame
    lame_set_num_channels(lameHandle, wavDecoder->GetHeader().channels);
    lame_set_in_samplerate(lameHandle, wavDecoder->GetHeader().samplesPerSec);
    lame_set_quality(lameHandle, encoderSettings.quality);

    if (lame_init_params(lameHandle) < 0)
    {
        throw "lame initialization failed";
    }

    int outBufferSize = 1.25 * 1 + 7200;  // worst-case estimate from lame/API
    unsigned char* outBuffer = new unsigned char[outBufferSize];


    while (false == wavDecoder->ReadComplete()) 
    {        
        const std::vector<int16_t> inputBuffer = wavDecoder->ReadSample();
        

        int numberOfOutputBytes;
        if (wavDecoder->GetHeader().channels > 1)
        {
            numberOfOutputBytes = lame_encode_buffer_interleaved(lameHandle,
               (short int*) &inputBuffer[0], 1, &outBuffer[0], outBufferSize);
        }
        else
        {
            numberOfOutputBytes = lame_encode_buffer(lameHandle,
                (short int*)&inputBuffer[0], nullptr,1, &outBuffer[0], outBufferSize);
        }

        if (numberOfOutputBytes < 0)
        {
            cerr << "lame_encode_buffer  error: " << numberOfOutputBytes << endl;
            throw "lame_encode_buffer  error!";            
        }
        
        if (!outFile.write((const char *)outBuffer, numberOfOutputBytes))
        {
            cerr << "outFile write  error: "  << endl;
            throw "Writing to output failed!";
        }
    }

    //Finalize

    int numberOfOutputBytes = lame_encode_flush(lameHandle, &outBuffer[0], outBufferSize);
    if (numberOfOutputBytes < 0)
    {
        std::cerr << "lame_encode_flush  error: " << numberOfOutputBytes <<std::endl;
        throw "lame_encode_flush  error!";
    }

    if (numberOfOutputBytes > 0)
    {
        if (!outFile.write((const char*)outBuffer, numberOfOutputBytes))
        {
            std::cerr << "outFile write on flush  error: " << std::endl;
            throw "Writing to output on flush failed!";
        }        
    }

    outFile.close();
}
