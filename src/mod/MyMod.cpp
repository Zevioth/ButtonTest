#include "mod/MyMod.h"

#include <filesystem>
#include <pl/Mod.hpp>
#include <pl/ModMenu.hpp>

namespace clange_me {

ClangeMeMod &ClangeMeMod::instance() {
    static ClangeMeMod instance;
    return instance;
}

ClangeMeMod::ClangeMeMod()
    : mSelf(*ll::mod::NativeMod::current()) {}

bool ClangeMeMod::load() {
    auto &self = getSelf();

    self.getLogger().info("ButtonTest loading!");

    std::error_code ec;

    std::filesystem::create_directories(self.getDataDir(), ec);
    if (ec) {
        self.getLogger().error(
            "Failed to create data directory: {}",
            ec.message()
        );
        return false;
    }

    std::filesystem::create_directories(self.getConfigDir(), ec);
    if (ec) {
        self.getLogger().error(
            "Failed to create config directory: {}",
            ec.message()
        );
        return false;
    }

    return true;
}

bool ClangeMeMod::enable() {
    auto &self = getSelf();

    self.getLogger().info("ButtonTest enabling!");

    // Create the floating button.
    const bool buttonRegistered =
        pl::modmenu::ButtonBuilder(
            "buttontest.button",
            "Button Test"
        )
        .modId(self.getId())
        .label("TEST")
        .androidKeyCode(0)
        .behavior(pl::modmenu::ButtonBehavior::Click)
        .onEvent(
            [&self](
                std::string_view buttonId,
                pl::modmenu::ButtonEvent event,
                float value
            ) {
                (void)buttonId;
                (void)value;

                if (event == pl::modmenu::ButtonEvent::Click) {
                    self.getLogger().info(
                        "!!! BUTTON TEST CLICKED !!!"
                    );
                }
            }
        )
        .registerButton();

    if (!buttonRegistered) {
        self.getLogger().error(
            "Failed to register Button Test button."
        );
        return false;
    }

    self.getLogger().info(
        "Button Test button registered successfully!"
    );

    return true;
}

bool ClangeMeMod::disable() {
    getSelf().getLogger().info("ButtonTest disabling...");

    pl::modmenu::unregisterButton(
        "buttontest.button"
    );

    return true;
}

bool ClangeMeMod::unload() {
    getSelf().getLogger().info("ButtonTest unloaded.");

    return true;
}

} // namespace clange_me
