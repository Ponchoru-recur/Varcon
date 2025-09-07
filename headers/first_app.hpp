#ifndef FIRST_APP_HPP
#define FIRST_APP_HPP

#include "window.hpp"
#include "pipeline.hpp"

namespace lve {

class FirstApp {
   public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    void run();
    void events() {};
    void exit();

   private:
    LveWindow lveWindow{WIDTH, HEIGHT, "Hello world"};
    LvePipeline lvePipeline{"../shaders/shader.vert.spv", "../shaders/shader.frag.spv"};
};

}  // namespace lve

#endif  // FIRST_APP_HPP
