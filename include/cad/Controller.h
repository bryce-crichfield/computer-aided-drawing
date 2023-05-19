#pragma once

#include <core/Controller.h>
#include <cad/object/ObjectRegistry.h>
#include <cad/Viewfinder.h>

namespace Cad {

struct Controller : public Core::Controller {
    Core::Controller& controller;
    ObjectRegistry& registry;
    Viewfinder& viewfinder;

    Controller(Core::Controller& controller, ObjectRegistry& registry, Viewfinder& viewfinder) : 
        controller(controller), registry(registry), viewfinder(viewfinder) {}

    Core::Graphics& GetGraphics() override {
        return controller.GetGraphics();
    }

    Core::Input& GetInput() override {
        return controller.GetInput();
    }

    Core::Output& GetOutput() override {
        return controller.GetOutput();
    }

    Core::Chronometer& GetChronometer() override {
        return controller.GetChronometer();
    }

    Core::FontManager& GetFontManager() override {
        return controller.GetFontManager();
    }

    Core::Gui::StyleManager& GetStyleManager() override {
        return controller.GetStyleManager();
    }

    virtual Cad::ObjectRegistry& GetRegistry() { 
        return registry;
    }

    virtual Cad::Viewfinder& GetViewfinder() {
        return viewfinder;
    }
};
}