#include "first_app.hpp"

namespace lve {
void FirstApp::run() {

};

void FirstApp::exit() {
    lveWindow.~LveWindow();
}
}  // namespace lve
