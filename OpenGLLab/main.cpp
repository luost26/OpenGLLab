#include "Playground/playground.hpp"

int main() {
    try {
        return playground::AdvancedLighting::main();
    } catch (ShaderCompilationException e) {
        std::cerr << e.what() << std::endl;
        return -1;
    } catch (ProgramLinkException e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
