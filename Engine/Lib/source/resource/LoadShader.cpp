#include "kern/resource/LoadShader.h"

#include <fmtlog/fmtlog.h>

#include "kern/foundation/IniFile.h"
#include "kern/foundation/JsonDeserialize.h"
#include "kern/foundation/JsonUtil.h"

bool load(const std::string &file, ResourceManager &manager, SShader &shader)
{
    std::string vertex;
    std::string tessControl;
    std::string tessEval;
    std::string geometry;
    std::string fragment;

    if (file.find(".ini") != std::string::npos)
    {
        if (!loadShaderFromIni(file, vertex, tessControl, tessEval, geometry, fragment))
        {
            return false;
        }
    }
    else if (file.find(".json") != std::string::npos)
    {
        if (!loadShaderFromJson(file, vertex, tessControl, tessEval, geometry, fragment))
        {
            return false;
        }
    }
    else
    {
        loge("Unknown or invalid shader file format.");
        return false;
    }

    // Requires vertex and fragment shader files
    if (vertex.empty())
    {
        loge(
            "The shader program file {} is missing a vertex shader source "
            "file specifier.",
            file.c_str());
        return false;
    }

    if (fragment.empty())
    {
        loge(
            "The shader program file {} is missing a vertex shader source "
            "file specifier.",
            file.c_str());
        return false;
    }

    ResourceId vertexId = manager.loadString(vertex);
    if (vertexId == InvalidResource)
    {
        loge(
            "The vertex shader source file {}, specified in the shader "
            "program file %s could not "
            "be loaded.",
            vertex.c_str(), file.c_str());
        return false;
    }

    ResourceId fragmentId = manager.loadString(fragment);
    if (fragmentId == InvalidResource)
    {
        loge(
            "The fragment shader source file {}, specified in the shader "
            "program file {} could not "
            "be loaded.",
            fragment.c_str(), file.c_str());
        return false;
    }

    ResourceId tessCtrlId = InvalidResource;
    if (!tessControl.empty())
    {
        tessCtrlId = manager.loadString(tessControl);
        if (tessCtrlId == InvalidResource)
        {
            loge(
                "The tessellation control shader source file {}, specified in "
                "the shader program "
                "file {} could not be loaded.",
                tessControl.c_str(), file.c_str());
            return false;
        }
    }

    ResourceId tessEvalId = InvalidResource;
    if (!tessEval.empty())
    {
        tessEvalId = manager.loadString(tessEval);
        if (tessEvalId == InvalidResource)
        {
            loge(
                "The tessellation evaluation shader source file %s, specified "
                "in the shader "
                "program file {} could not be loaded.",
                tessEval.c_str(), file.c_str());
            return false;
        }
    }

    ResourceId geometryId = InvalidResource;
    if (!geometry.empty())
    {
        geometryId = manager.loadString(geometry);
        if (geometryId == InvalidResource)
        {
            loge(
                "The geometry shader source file {}, specified in the shader "
                "program file {} could "
                "not be loaded.",
                geometry.c_str(), file.c_str());
            return false;
        }
    }

    shader.m_vertex = vertexId;
    shader.m_tessCtrl = tessCtrlId;
    shader.m_tessEval = tessEvalId;
    shader.m_geometry = geometryId;
    shader.m_fragment = fragmentId;
    return true;
}

bool loadShaderFromIni(const std::string &file, std::string &vertex, std::string &tessControl, std::string &tessEval,
                       std::string &geometry, std::string &fragment)
{
    // Load shader ini
    IniFile ini;
    if (!ini.load(file))
    {
        loge("Failed to load shader program file {}", file.c_str());
        return false;
    }

    // Load from ini
    vertex = ini.getValue("vertex", "file", "");
    tessControl = ini.getValue("tess_control", "file", "");
    tessEval = ini.getValue("tess_evaluation", "file", "");
    geometry = ini.getValue("geometry", "file", "");
    fragment = ini.getValue("fragment", "file", "");
    return true;
}

bool loadShaderFromJson(const std::string &file, std::string &vertex, std::string &tessControl, std::string &tessEval,
                        std::string &geometry, std::string &fragment)
{
    logd("Not implemented");
    return false;
}