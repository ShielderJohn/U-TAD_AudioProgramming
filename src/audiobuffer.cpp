#include "../include/audiobuffer.h"
#include "../include/file.h"
#include "../openal/AL/al.h"

#define STB_VORBIS_HEADER_ONLY
#include "../lib/stb_vorbis.c"

AudioBuffer::AudioBuffer(const String& filename) {
	alBuffer = 0;

	File file(filename, FILE_READ);

	char chunkName[5];
	char subChunkName[5];
	char format[5];
	char header[5];

	int fmtChunkSize;
	int audioFormat;
	int channels;
	int sampleRate;
	int byteRate;
	int blockAlign;
	int bitsPerSample;
	int extraParams;

	int dataSize;
	char* data;

	chunkName[4] = '\0';
	subChunkName[4] = '\0';
	format[4] = '\0';
	header[4] = '\0';

	String extension = filename.ExtractExt();

	// WAV File
	if (extension == "wav") {

		file.ReadBytes(chunkName, 4);

		if (String(chunkName) == "RIFF") {
			file.Seek(file.Pos() + 4);
			file.ReadBytes(format, 4);

			if (String(format) == "WAVE") {
				file.ReadBytes(subChunkName, 4);

				if (String(subChunkName) == "fmt ") {
					fmtChunkSize = file.ReadInt();
					audioFormat = file.ReadInt16();
					channels = file.ReadInt16();
					sampleRate = file.ReadInt();
					byteRate = file.ReadInt();
					blockAlign = file.ReadInt16();
					bitsPerSample = file.ReadInt16();

					if (fmtChunkSize > 16) {
						extraParams = file.ReadInt16();
						file.Seek(file.Pos() + extraParams);
					}

					file.ReadBytes(header, 4);

					while (strcmp(header, "data")) {
						dataSize = file.ReadInt();
						file.Seek(file.Pos() + dataSize);
						file.ReadBytes(header, 4);
					}

					dataSize = file.ReadInt();
					data = new char[dataSize];
					file.ReadBytes(data, dataSize);

					alGenBuffers(1, &alBuffer);

					if (bitsPerSample == 8) {
						if (channels == 1) {
							alBufferData(alBuffer, AL_FORMAT_MONO8, data, dataSize, sampleRate);
						}
						else {
							alBufferData(alBuffer, AL_FORMAT_STEREO8, data, dataSize, sampleRate);
						}
					}
					else {
						if (channels == 1) {
							alBufferData(alBuffer, AL_FORMAT_MONO16, data, dataSize, sampleRate);
						}
						else {
							alBufferData(alBuffer, AL_FORMAT_STEREO16, data, dataSize, sampleRate);
						}
					}
				}
			}
		}
	}

	// OGG File
	else if (extension == "ogg") {
		stb_vorbis* vorbisPointer = stb_vorbis_open_filename(filename.ToCString(), nullptr, nullptr);

		if (vorbisPointer != nullptr) {
			stb_vorbis_info vorbisInfo = stb_vorbis_get_info(vorbisPointer);
			int streamLength = stb_vorbis_stream_length_in_samples(vorbisPointer) * vorbisInfo.channels;
			short* buffer = new short[streamLength];

			stb_vorbis_get_samples_short_interleaved(vorbisPointer, vorbisInfo.channels, buffer, streamLength);

			alGenBuffers(1, &alBuffer);

			if (vorbisInfo.channels == 1) {
				alBufferData(alBuffer, AL_FORMAT_MONO16, buffer, streamLength, vorbisInfo.sample_rate);
			}
			else {
				alBufferData(alBuffer, AL_FORMAT_MONO16, buffer, streamLength, vorbisInfo.sample_rate);
			}

			delete buffer;
		}
	}
}

AudioBuffer::~AudioBuffer(void) {
	alDeleteBuffers(1, &alBuffer);
}