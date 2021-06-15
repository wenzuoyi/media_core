#ifndef SNAPSHOT_HANDLER_IMPL_H_
#define SNAPSHOT_HANDLER_IMPL_H_
#include "include/snapshot_handler.h"
#include <atomic>
#include "base_saver.h"
namespace handler {
	class SnapshotHandlerImpl :	public SnapshotHandler {
	public:
		SnapshotHandlerImpl();
		virtual ~SnapshotHandlerImpl();
	protected:
    void Start() override;
    void Stop() override;
    void InputVideoFrame(VideoFramePtr video_frame) override;
    void SetEvent(SnapshotHandlerEvent* event) override;
    bool Save(const std::string& path) override;
	private:
		SnapshotHandlerEvent* event_{ nullptr };
		std::atomic_bool flag_{ false };
		BaseSaverPtr saver_{ nullptr };
	};
}
#endif // SNAPSHOT_HANDLER_IMPL_H_
