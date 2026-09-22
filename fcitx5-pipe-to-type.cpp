#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdlib>
#include <string>

#include <fcitx/addonfactory.h>
#include <fcitx/addoninstance.h>
#include <fcitx/addonmanager.h>
#include <fcitx/inputcontext.h>
#include <fcitx/instance.h>
#include <fcitx-utils/event.h>

class MyAddonInstance : public fcitx::AddonInstance {
public:
	MyAddonInstance(fcitx::Instance *instance) : m_instance(instance) {
		const char *runtimeDir = getenv("XDG_RUNTIME_DIR");
		if (!runtimeDir || !*runtimeDir) return;
		std::string path = runtimeDir + std::string("/fcitx5-pipe-to-type");

		mkfifo(path.c_str(), 0600);
		// O_RDWR keeps the pipe open indefinitely without receiving EOF on writer close
		m_fd = open(path.c_str(), O_RDWR | O_NONBLOCK | O_CLOEXEC | O_NOFOLLOW);
		if (m_fd < 0) return;
		m_event = m_instance->eventLoop().addIOEvent(
			m_fd, fcitx::IOEventFlag::In,
			[this](fcitx::EventSourceIO *, int fd, fcitx::IOEventFlags) {
				auto *ic = m_instance->lastFocusedInputContext();
				if (!ic) return false;
				char buf[4096];
				ssize_t n;
				while ((n = read(fd, buf, sizeof(buf))) > 0) {
					ic->commitString(std::string(buf, n));
				}
				return true;
			}
		);
	}

	~MyAddonInstance() override {
		m_event.reset();
		if (m_fd >= 0) close(m_fd);
	}

private:
	fcitx::Instance *m_instance;
	int m_fd = -1;
	std::unique_ptr<fcitx::EventSourceIO> m_event;
};

class MyAddonFactory : public fcitx::AddonFactory {
public:
	fcitx::AddonInstance *create(fcitx::AddonManager *manager) override {
		return new MyAddonInstance(manager->instance());
	}
};

FCITX_ADDON_FACTORY(MyAddonFactory)
