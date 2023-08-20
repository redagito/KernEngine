#include "graphics/resource/LoadShader.h"

#include <foundation/debug/Log.h>
#include <foundation/io/CIniFile.h>
#include <foundation/io/JsonDeserialize.h>
#include <foundation/io/JsonUtil.h>

#include "graphics/resource/CResourceManager.h"

bool load(const std::string &file, CResourceManager &manager, SShader &shader)
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
        LOG_ERROR("Unknown or invalid shader file format.");
        return false;
    }

    // Requires vertex and fragment shader files
    if (vertex.empty())
    {
        LOG_ERROR(
            "The shader program file %s is missing a vertex shader source "
            "file specifier.",
            file.c_str());
        return false;
    }

    if (fragment.empty())
    {
        LOG_ERROR(
            "The shader program file %s is missing a vertex shader source "
            "file specifier.",
            file.c_str());
        return false;
    }

    ResourceId vertexId = manager.loadString(vertex, true);
    if (vertexId == invalidResource)
    {
        LOG_ERROR(
            "The vertex shader source file %s, specified in the shader "
            "program file %s could not "
            "be loaded.",
            vertex.c_str(), file.c_str());
        return false;
    }

    ResourceId fragmentId = manager.loadString(fragment, true);
    if (fragmentId == invalidResource)
    {
        LOG_ERROR(
            "The fragment shader source file %s, specified in the shader "
            "program file %s could not "
            "be loaded.",
            fragment.c_str(), file.c_str());
        return false;
    }

    ResourceId tessCtrlId = invalidResource;
    if (!tessControl.empty())
    {
        tessCtrlId = manager.loadString(tessControl, true);
        if (tessCtrlId == invalidResource)
        {
            LOG_ERROR(
                "The tessellation control shader source file %s, specified in "
                "the shader program "
                "file %s could not be loaded.",
                tessControl.c_str(), file.c_str());
            return false;
        }
    }

    ResourceId tessEvalId = invalidResource;
    if (!tessEval.empty())
    {
        tessEvalId = manager.loadString(tessEval, true);
        if (tessEvalId == invalidResource)
        {
            LOG_ERROR(
                "The tessellation evaluation shader source file %s, specified "
                "in the shader "
                "program file %s could not be loaded.",
                tessEval.c_str(), file.c_str());
            return false;
        }
    }

    ResourceId geometryId = invalidResource;
    if (!geometry.empty())
    {
        geometryId = manager.loadString(geometry, true);
        if (geometryId == invalidResource)
        {
            LOG_ERROR(
                "The geometry shader source file %s, specified in the shader "
                "program file %s could "
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

bool loadShaderFromIni(const std::string &file, std::string &vertex, std::string &tessControl,
                       std::string &tessEval, std::string &geometry, std::string &fragment)
{
    // Load shader ini
    CIniFile ini;
    if (!ini.load(file))
    {
        LOG_ERROR("Failed to load shader program file %s", file.c_str());
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

bool loadShaderFromJson(const std::string &file, std::string &vertex, std::string &tessControl,
                        std::string &tessEval, std::string &geometry, std::string &fragment)
{
    LOG_DEBUG("Not implemented");
    return false;
}