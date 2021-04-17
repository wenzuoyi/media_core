#ifndef OUTPUT_MEDIA_SOURCE_TESTSUITE_H_
#define OUTPUT_MEDIA_SOURCE_TESTSUITE_H_
#include "audio_output_media_source.h"
namespace output {
	class OutputMediaSourceTestSuite : public AudioOutputMediaSourceEvent {
	public:
		OutputMediaSourceTestSuite() = default;
		virtual ~OutputMediaSourceTestSuite() = default;
		void Init();
		void Fini();
		void TestPlayAudioFile();
	protected:
		void OnAudioOutputException(unsigned error_code) override;
		void OnAudioTransmitSample(AudioSamplePtr audio_sample) override;
		void OnAudioSampleFrequency(unsigned long frequency) override;
	private:
		AudioOutputMediaSourcePtr audio_output_media_source_;
		static std::wstring CONSOLE_TITLE_NAME;
	};

}
#endif // OUTPUT_MEDIA_SOURCE_TESTSUITE_H_
