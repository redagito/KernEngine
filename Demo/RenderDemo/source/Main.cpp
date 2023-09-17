#include <fmtlog/fmtlog.h>

// Getting started
#include "app/ColorTriangle.h"
#include "app/CubePerspective.h"
#include "app/HelloCamera.h"
#include "app/HelloPerspective.h"
#include "app/HelloTriangle.h"
#include "app/LookAroundCamera.h"
#include "app/RotatingCubes.h"
#include "app/SimpleMoveCamera.h"
#include "app/TexturedQuad.h"
#include "app/TexturedTriangle.h"
#include "app/TransformedQuad.h"

// Lighting
#include "app/BasicLighting.h"
#include "app/LightCube.h"
#include "app/SimpleMaterial.h"
#include "app/LightingMaps.h"
#include "app/DirectionalLightCaster.h"
#include "app/PointLightCaster.h"
#include "app/SpotLightCaster.h"

// Model loading
#include "app/ModelLoad.h"

// Advanced OpenGL

// Advanced lighting

// PBR

// In practice
#include "app/RenderText.h"

// Other
#include "app/ImguiDemo.h"

enum class AppKind
{
    // Getting started
    HelloTriangle,
    ColorTriangle,
    TexturedTriangle,
    TexturedQuad,
    TransformedQuad,
    HelloPerspective,
    CubePerspective,
    RotatingCubes,
    HelloCamera,
    SimpleMoveCamera,
    LookAroundCamera,
    LightCube,
    // Lighting
    BasicLighting,
    SimpleMaterial,
    LightingMaps,
    // Lighting - light casters
    DirectionalLightCaster,
    PointLightCaster,
    SpotLightCaster,

    // Model loading
    ModelLoad,

    // In practice
    RenderText,

    // Other
    ImguiDemo
};

std::unique_ptr<RenderApplication> createApp(AppKind kind)
{
    switch (kind)
    {
    case AppKind::HelloTriangle:
        return std::make_unique<HelloTriangle>();
    case AppKind::ColorTriangle:
        return std::make_unique<ColorTriangle>();
    case AppKind::TexturedTriangle:
        return std::make_unique<TexturedTriangle>();
    case AppKind::TexturedQuad:
        return std::make_unique<TexturedQuad>();
    case AppKind::TransformedQuad:
        return std::make_unique<TransformedQuad>();
    case AppKind::HelloPerspective:
        return std::make_unique<HelloPerspective>();
    case AppKind::CubePerspective:
        return std::make_unique<CubePerspective>();
    case AppKind::RotatingCubes:
        return std::make_unique<RotatingCubes>();
    case AppKind::HelloCamera:
        return std::make_unique<HelloCamera>();
    case AppKind::SimpleMoveCamera:
        return std::make_unique<SimpleMoveCamera>();
    case AppKind::LookAroundCamera:
        return std::make_unique<LookAroundCamera>();
    case AppKind::LightCube:
        return std::make_unique<LightCube>();
    case AppKind::BasicLighting:
        return std::make_unique<BasicLighting>();
    case AppKind::SimpleMaterial:
        return std::make_unique<SimpleMaterial>();
    case AppKind::LightingMaps:
        return std::make_unique<LightingMaps>();
    case AppKind::DirectionalLightCaster:
        return std::make_unique<DirectionalLightCaster>();
    case AppKind::PointLightCaster:
        return std::make_unique<PointLightCaster>();
    case AppKind::SpotLightCaster:
        return std::make_unique<SpotLightCaster>();
    case AppKind::RenderText:
        return std::make_unique<RenderText>();
    case AppKind::ModelLoad:
        return std::make_unique<ModelLoad>();
    case AppKind::ImguiDemo:
        return std::make_unique<ImguiDemo>();
    }
    throw std::runtime_error("Unsupported app kind");
}

int main(int argc, char** argv)
{
#ifndef NDEBUG
    fmtlog::setLogLevel(fmtlog::DBG);
#endif
    fmtlog::startPollingThread();

    try
    {
        std::vector<AppKind> kinds;
        
        // Getting started
        /*
        kinds.push_back(AppKind::HelloTriangle);
        kinds.push_back(AppKind::ColorTriangle);
        kinds.push_back(AppKind::TexturedTriangle);
        kinds.push_back(AppKind::TexturedQuad);
        kinds.push_back(AppKind::TransformedQuad);
        kinds.push_back(AppKind::HelloPerspective);
        kinds.push_back(AppKind::CubePerspective);
        kinds.push_back(AppKind::RotatingCubes);
        kinds.push_back(AppKind::HelloCamera);
        kinds.push_back(AppKind::SimpleMoveCamera);
        kinds.push_back(AppKind::LookAroundCamera);
        */

        // Lighting
        /*
        kinds.push_back(AppKind::LightCube);
        kinds.push_back(AppKind::BasicLighting);
        kinds.push_back(AppKind::SimpleMaterial);
        kinds.push_back(AppKind::LightingMaps);
        kinds.push_back(AppKind::DirectionalLightCaster);
        kinds.push_back(AppKind::PointLightCaster);
        */
        kinds.push_back(AppKind::SpotLightCaster);

        // Model loading
        // TODO Not working
        // kinds.push_back(AppKind::ModelLoad);
        
        // In practice
        kinds.push_back(AppKind::RenderText);
        // Other
        kinds.push_back(AppKind::ImguiDemo);
        
        for (auto kind : kinds)
        {
            createApp(kind)->run();
        }
    }
    catch (const std::exception& e)
    {
        loge("{}", e.what());
    }
    return 0;
}