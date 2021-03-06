// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <algorithm>
#include <thread>
#include "Engine.h"
#include "CompileConfig.h"
#include "Cache.h"
#include "Localization.h"

#if defined(OUZEL_PLATFORM_OSX)
#include "osx/WindowOSX.h"
#elif defined(OUZEL_PLATFORM_IOS)
#include "ios/WindowIOS.h"
#elif defined(OUZEL_PLATFORM_TVOS)
#include "tvos/WindowTVOS.h"
#elif defined(OUZEL_PLATFORM_ANDROID)
#include "android/WindowAndroid.h"
#elif defined(OUZEL_PLATFORM_LINUX)
#include "linux/WindowLinux.h"
#elif defined(OUZEL_PLATFORM_WINDOWS)
#include "win/WindowWin.h"
#endif

#if defined(OUZEL_SUPPORTS_OPENGL) || defined(OUZEL_SUPPORTS_OPENGLES)
#include "opengl/RendererOGL.h"
#endif

#if defined(OUZEL_SUPPORTS_DIRECT3D11)
#include "direct3d11/RendererD3D11.h"
#endif

#if defined(OUZEL_SUPPORTS_METAL)
#include "metal/RendererMetal.h"
#endif

#include "Utils.h"
#include "Renderer.h"
#include "FileSystem.h"

#if defined(OUZEL_PLATFORM_OSX) || defined(OUZEL_PLATFORM_IOS) || defined(OUZEL_PLATFORM_TVOS)
#include "apple/InputApple.h"
#elif defined(OUZEL_PLATFORM_WINDOWS)
#include "win/InputWin.h"
#endif

namespace ouzel
{
    ouzel::Engine* sharedEngine = nullptr;

    Engine::Engine()
    {
        sharedEngine = this;
    }

    Engine::~Engine()
    {
        if (sceneManager)
        {
            sceneManager->setScene(nullptr);
        }
    }

    std::set<graphics::Renderer::Driver> Engine::getAvailableDrivers()
    {
        std::set<graphics::Renderer::Driver> availableDrivers;

#if defined(OUZEL_SUPPORTS_OPENGL) || defined(OUZEL_SUPPORTS_OPENGLES)
        availableDrivers.insert(graphics::Renderer::Driver::OPENGL);
#endif

#if defined(OUZEL_SUPPORTS_DIRECT3D11)
        availableDrivers.insert(graphics::Renderer::Driver::DIRECT3D11);
#endif

#if defined(OUZEL_SUPPORTS_METAL)
        if (graphics::RendererMetal::available())
        {
            availableDrivers.insert(graphics::Renderer::Driver::METAL);
        }
#endif

        return availableDrivers;
    }

    bool Engine::init(Settings& newSettings)
    {
        settings = newSettings;

        if (!settings.verticalSync && settings.targetFPS > 0.0f)
        {
            targetFPS = settings.targetFPS;
            targetFrameInterval = static_cast<uint64_t>(1000000L * 1.0f / targetFPS);
        }
        else
        {
            targetFPS = 0.0f;
            targetFrameInterval = 0L;
        }

        if (settings.driver == graphics::Renderer::Driver::DEFAULT)
        {
#if defined(OUZEL_SUPPORTS_METAL)
            if (graphics::RendererMetal::available())
            {
                settings.driver = graphics::Renderer::Driver::METAL;
            }
    #if defined(OUZEL_SUPPORTS_OPENGL) || defined(OUZEL_SUPPORTS_OPENGLES)
            else
            {
                settings.driver = graphics::Renderer::Driver::OPENGL;
            }
    #endif
#elif defined(OUZEL_SUPPORTS_DIRECT3D11)
            settings.driver = graphics::Renderer::Driver::DIRECT3D11;
#elif defined(OUZEL_SUPPORTS_OPENGL) || defined(OUZEL_SUPPORTS_OPENGLES)
            settings.driver = graphics::Renderer::Driver::OPENGL;
#endif
        }

        if (settings.driver == graphics::Renderer::Driver::DEFAULT)
        {
            log("Failed to select render driver");
            return false;
        }

#if defined(OUZEL_PLATFORM_OSX)
        window.reset(new WindowOSX(settings.size, settings.resizable, settings.fullscreen, settings.title));
#elif defined(OUZEL_PLATFORM_IOS)
        window.reset(new WindowIOS(settings.size, settings.resizable, settings.fullscreen, settings.title));
#elif defined(OUZEL_PLATFORM_TVOS)
        window.reset(new WindowTVOS(settings.size, settings.resizable, settings.fullscreen, settings.title));
#elif defined(OUZEL_PLATFORM_ANDROID)
        window.reset(new WindowAndroid(settings.size, settings.resizable, settings.fullscreen, settings.title));
#elif defined(OUZEL_PLATFORM_LINUX)
        window.reset(new WindowLinux(settings.size, settings.resizable, settings.fullscreen, settings.title));
#elif defined(OUZEL_PLATFORM_WINDOWS)
        window.reset(new WindowWin(settings.size, settings.resizable, settings.fullscreen, settings.title));
#endif

        eventDispatcher.reset(new EventDispatcher());
        cache.reset(new Cache());
        fileSystem.reset(new FileSystem());
        sceneManager.reset(new scene::SceneManager());

#if defined(OUZEL_PLATFORM_OSX) || defined(OUZEL_PLATFORM_IOS) || defined(OUZEL_PLATFORM_TVOS)
        input.reset(new input::InputApple());
#elif defined(OUZEL_PLATFORM_WINDOWS)
        input.reset(new input::InputWin());
#else
        input.reset(new input::Input());
#endif

        localization.reset(new Localization());

        switch (settings.driver)
        {
#if defined(OUZEL_SUPPORTS_OPENGL) || defined(OUZEL_SUPPORTS_OPENGLES)
            case graphics::Renderer::Driver::OPENGL:
                log("Using OpenGL render driver");
                renderer.reset(new graphics::RendererOGL());
                break;
#endif
#if defined(OUZEL_SUPPORTS_DIRECT3D11)
            case graphics::Renderer::Driver::DIRECT3D11:
                log("Using Direct3D 11 render driver");
                renderer.reset(new graphics::RendererD3D11());
                break;
#endif
#if defined(OUZEL_SUPPORTS_METAL)
            case graphics::Renderer::Driver::METAL:
                log("Using Metal render driver");
                renderer.reset(new graphics::RendererMetal());
                break;
#endif
            default:
                log("Unsupported render driver");
                return false;
        }

        if (!window->init())
        {
            return false;
        }

        if (!sharedEngine->getRenderer()->init(window,
                                               settings.sampleCount,
                                               settings.textureFiltering,
                                               settings.targetFPS,
                                               settings.verticalSync))
        {
            return false;
        }

        previousFrameTime = getCurrentMicroSeconds();

        return true;
    }

    void Engine::exit()
    {
        active = false;
    }

    void Engine::begin()
    {
        running = true;
    }

    void Engine::end()
    {

    }

    bool Engine::run()
    {
        uint64_t currentTime = getCurrentMicroSeconds();
        float delta = static_cast<float>((currentTime - previousFrameTime)) / 1000000.0f;
        previousFrameTime = currentTime;

        currentFPS = 1.0f / delta;

        input->update();
        eventDispatcher->update();

        auto updateCallbacksCopy = updateCallbacks;

        for (const UpdateCallbackPtr& updateCallback : updateCallbacksCopy)
        {
            if (updateCallback->callback)
            {
                updateCallback->callback(delta);
            }
        }

        renderer->clear();
        sceneManager->draw();
        renderer->present();

        if (targetFrameInterval > 0)
        {
            uint64_t diff = getCurrentMicroSeconds() - currentTime;

            if (targetFrameInterval > diff)
            {
                std::this_thread::sleep_for(std::chrono::microseconds(targetFrameInterval - diff));
            }
        }

        return active;
    }

    void Engine::scheduleUpdate(const UpdateCallbackPtr& callback)
    {
        std::vector<UpdateCallbackPtr>::iterator i = std::find(updateCallbacks.begin(), updateCallbacks.end(), callback);

        if (i == updateCallbacks.end())
        {
            updateCallbacks.push_back(callback);
        }
    }

    void Engine::unscheduleUpdate(const UpdateCallbackPtr& callback)
    {
        std::vector<UpdateCallbackPtr>::iterator i = std::find(updateCallbacks.begin(), updateCallbacks.end(), callback);

        if (i != updateCallbacks.end())
        {
            updateCallbacks.erase(i);
        }
    }
}
