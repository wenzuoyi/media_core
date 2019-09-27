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
		void OnAudioOutputMediaExceptionEvent(unsigned error_code) override;
		void OnTransmitDataEvent(AudioSamplePtr audio_sample) override;
		void OnSampleFrequency(unsigned long frequency) override;
	private:
		AudioOutputMediaSourcePtr audio_output_media_source_;
		static std::wstring CONSOLE_TITLE_NAME;
	};

}
#endif // OUTPUT_MEDIA_SOURCE_TESTSUITE_H_
