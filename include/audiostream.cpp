#include "../include/audiostream.h"
#include "../include/audiosource.h"
#include "../openal/AL/al.h"

#define STREAM_BLOCK_SIZE 32768

Array<AudioStream*> AudioStream::streams;

AudioStream::AudioStream(const String& filename, AudioSource* source) {
	stream = stb_vorbis_open_filename(filename.ToCString(), nullptr, nullptr);

	if (stream != nullptr && source != nullptr) {
		this->source = source;
		info = stb_vorbis_get_info(stream);
		samplesLeft = stb_vorbis_stream_length_in_samples(stream) * info.channels;

		alGenBuffers(2, buffers);

		if (Stream(buffers[0]) && Stream(buffers[1])) {
			alSourceQueueBuffers(source->GetSource(), 2, buffers);
			AudioStream::streams.Add(this);
		}
	}
}

AudioStream::~AudioStream() {
	AudioStream* audioStream = this;
	AudioStream::streams.Remove(audioStream);
	alDeleteBuffers(2, buffers);
}

void AudioStream::UpdateAll() {
	unsigned int size = AudioStream::streams.Size();

	for (int i = 0; i < size; ++i) {
		AudioStream::streams[i]->Update();
	}
}

bool AudioStream::Update() {
	int processedBuffers;
	bool returnValue = true;

	alGetSourcei(source->GetSource(), AL_BUFFERS_PROCESSED, &processedBuffers);

	if (processedBuffers > 0) {
		for (int i = 0; i < processedBuffers; ++i) {
			alSourceUnqueueBuffers(source->GetSource(), 1, &buffers[i]);

			if (Stream(buffers[i])) {
				alSourceQueueBuffers(source->GetSource(), 1, &buffers[i]);
			}
			else if (shouldLoop) {
				stb_vorbis_seek_start(stream);
				samplesLeft = stb_vorbis_stream_length_in_samples(stream) * info.channels;

				if (Stream(buffers[i])) {
					alSourceQueueBuffers(source->GetSource(), 1, &buffers[i]);
				}
				else {
					returnValue = false;
				}
			}
		}
	}

	return returnValue;
}

bool AudioStream::Stream(unsigned int buffer) {
	short samplesShort[STREAM_BLOCK_SIZE];

	int size = stb_vorbis_get_samples_short_interleaved(stream, info.channels, samplesShort, STREAM_BLOCK_SIZE);

	if (size == 0) {
		return false;
	}

	if (info.channels == 1) {
		alBufferData(buffer, AL_FORMAT_MONO16, samplesShort, size * info.channels * sizeof(short), info.sample_rate);
	}
	else {
		alBufferData(buffer, AL_FORMAT_STEREO16, samplesShort, size * info.channels * sizeof(short), info.sample_rate);
	}

	samplesLeft -= size;

	return true;
}