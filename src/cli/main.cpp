#include <CPPVersus.hpp>

#include <CLI/CLI.hpp>

int main(int argc, char** argv) {
#ifdef DEBUG
    spdlog::set_level(spdlog::level::debug);
#endif

    CLI::App app("Multiversus CLI Test Utility", "CPPVersus CLI");
    argv = app.ensure_utf8(argv);

    std::string apiKey = "";
    app.add_option("-k,--key", apiKey, "Steam key for authentication (REQUIRED).");

    CLI11_PARSE(app, argc, argv);
    if(apiKey == "") {
        spdlog::critical("No Steam key specified.");
        return 1;
    }

    std::unique_ptr<CPPVersus::DokkenAPI> dokkenAPI;
    try {
        dokkenAPI = std::unique_ptr<CPPVersus::DokkenAPI>(new CPPVersus::DokkenAPI(apiKey));
    }
    catch(std::runtime_error const&) {
        return 1;
    }

    dokkenAPI->getPlayerInfo("6289bfe8213b6d574354d9fc");

    return 0;
}