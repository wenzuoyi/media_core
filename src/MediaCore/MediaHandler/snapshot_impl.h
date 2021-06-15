#ifndef SNAPSHOT_IMPL_H_
#define SNAPSHOT_IMPL_H_
#include "./include/snapshot.h"
namespace handler {
	class SnapshotImpl :	public Snapshot {
	public:
		SnapshotImpl();
		virtual ~SnapshotImpl();
	protected:
    void Start() override;
    void Stop() override;
    void InputVideoFrame(VideoFramePtr video_frame) override;
    void SetEvent(SnapshotEvent* event) override;
    bool Save(const std::string& path) override;
	private:
		SnapshotEvent* event_{ nullptr };
	};
}
#endif // SNAPSHOT_IMPL_H_
