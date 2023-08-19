#include <fmtlog/fmtlog.h>

#include "app/HelloTriangle.h"
#include "app/ColorTriangle.h"
#include "app/TexturedTriangle.h"
#include "app/TexturedQuad.h"
#include "app/TransformedQuad.h"
#include "app/HelloPerspective.h"
#include "app/CubePerspective.h"
#include "app/RotatingCubes.h"
#include "app/HelloCamera.h"
#include "app/SimpleMoveCamera.h"
#include "app/LookAroundCamera.h"

enum class AppKind
{
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
    LookAroundCamera
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
    }
    throw std::runtime_error("Unsupported app kind");
}

int main(int argc, char** argv)
{
    fmtlog::startPollingThread();
    try
    {
        std::vector<AppKind> kinds;
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

        for (auto kind : kinds)
        {
            createApp(kind)->run();
        }
    }
    catch (const std::exception& e)
    {
        loge(e.what());
    }
    return 0;
}