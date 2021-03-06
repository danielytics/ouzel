// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "CheckBox.h"
#include "Engine.h"
#include "Sprite.h"
#include "Utils.h"

namespace ouzel
{
    namespace gui
    {
        CheckBoxPtr CheckBox::create(const std::string& normalImage, const std::string& selectedImage, const std::string& pressedImage, const std::string& disabledImage, const std::string& tickImage)
        {
            CheckBoxPtr result = std::make_shared<CheckBox>();
            result->init(normalImage, selectedImage, pressedImage, disabledImage, tickImage);

            return result;
        }

        CheckBox::CheckBox()
        {

        }

        CheckBox::~CheckBox()
        {
            sharedEngine->getEventDispatcher()->removeEventHandler(eventHandler);
        }

        bool CheckBox::init(const std::string& normalImage, const std::string& selectedImage, const std::string& pressedImage, const std::string& disabledImage, const std::string& tickImage)
        {
            eventHandler = std::make_shared<EventHandler>(EventHandler::PRIORITY_MAX + 1);

            eventHandler->gamepadHandler = std::bind(&CheckBox::handleGamepad, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
            eventHandler->uiHandler = std::bind(&CheckBox::handleUI, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

            sharedEngine->getEventDispatcher()->addEventHandler(eventHandler);

            if (!normalImage.empty())
            {
                normalSprite = std::make_shared<scene::Sprite>();
                if (normalSprite->initFromFile(normalImage, false))
                {
                    addDrawable(normalSprite);
                }
            }

            if (!selectedImage.empty())
            {
                selectedSprite = std::make_shared<scene::Sprite>();
                if (selectedSprite->initFromFile(selectedImage, false))
                {
                    addDrawable(selectedSprite);
                }
            }

            if (!pressedImage.empty())
            {
                pressedSprite = std::make_shared<scene::Sprite>();
                if (pressedSprite->initFromFile(pressedImage, false))
                {
                    addDrawable(pressedSprite);
                }
            }

            if (!disabledImage.empty())
            {
                disabledSprite = std::make_shared<scene::Sprite>();
                if (disabledSprite->initFromFile(disabledImage, false))
                {
                    addDrawable(disabledSprite);
                }
            }

            if (!tickImage.empty())
            {
                tickSprite = std::make_shared<scene::Sprite>();
                if (tickSprite->initFromFile(tickImage, false))
                {
                    addDrawable(tickSprite);
                }
            }

            return true;
        }

        void CheckBox::setEnabled(bool enabled)
        {
            Widget::setEnabled(enabled);
        }

        void CheckBox::setChecked(bool newChecked)
        {
            checked = newChecked;
        }

        bool CheckBox::handleGamepad(Event::Type type, const GamepadEvent& event, const VoidPtr& sender)
        {
            OUZEL_UNUSED(type);
            OUZEL_UNUSED(event);
            OUZEL_UNUSED(sender);

            return true;
        }

        bool CheckBox::handleUI(Event::Type type, const UIEvent& event, const VoidPtr& sender)
        {
            OUZEL_UNUSED(type);
            OUZEL_UNUSED(event);
            OUZEL_UNUSED(sender);

            return true;
        }

        void CheckBox::updateSprite()
        {
        }
    } // namespace gui
} // namespace ouzel
