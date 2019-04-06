#include "Playground/playground.hpp"

using namespace playground;

int main() {
    try {
        return Framebuffer::main();
    } catch (ShaderCompilationException e) {
        std::cerr << e.what() << std::endl;
        return -1;
    } catch (ProgramLinkException e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
