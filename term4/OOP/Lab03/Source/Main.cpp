#include "Application.hpp"

#include "Algorithm.hpp"

Ziben::Application* Ziben::CreateApplication(int argc, char** argv) {
    (void)argc;
    (void)argv;

    return new Lab03::Application;
}