#ifndef __CONVERTERS_H__
#define __CONVERTERS_H__

#include <algorithm>
#include <libwap.h>
#include "../SharedDefines.h"
#include "../Actor/ActorTemplates.h"
#include "../GameApp/BaseGameApp.h"

//
//
// TODO TODO TODO: Somehow refactor this, delete unneeded ones and use everything from Actor/ActorTemplates.cpp
//
//

#define INSERT_POSITION_COMPONENT(x, y, rootElem) \
{ \
   TiXmlElement* elem = new TiXmlElement("PositionComponent"); \
   rootElem->LinkEndChild(elem); \
   XML_ADD_2_PARAM_ELEMENT("Position", "x", x, "y", y, elem); \
} \

#define INSERT_COLLISION_COMPONENT(width, height, rootElem) \
{ \
   TiXmlElement* elem = new TiXmlElement("CollisionComponent"); \
   rootElem->LinkEndChild(elem); \
   XML_ADD_2_PARAM_ELEMENT("CollisionSize", "width", width, "height", height, elem); \
} \

inline TiXmlElement* CreatePositionComponent(int x, int y)
{
    TiXmlElement* elem = new TiXmlElement("PositionComponent");
    XML_ADD_2_PARAM_ELEMENT("Position", "x", x, "y", y, elem);

    return elem;
}

inline TiXmlElement* CreateCollisionComponent(int width, int height)
{
    TiXmlElement* elem = new TiXmlElement("CollisionComponent");
    XML_ADD_2_PARAM_ELEMENT("CollisionSize", "width", width, "height", height, elem);

    return elem;
}

inline TiXmlElement* CreatePhysicsComponent(bool canClimb, bool canBounce, bool canJump, int maxJumpHeight, int width, int height, double gravityScale, double friction, double density)
{
    TiXmlElement* elem = new TiXmlElement("PhysicsComponent");

    XML_ADD_TEXT_ELEMENT("CanClimb", ToStr(canClimb).c_str(), elem);
    XML_ADD_TEXT_ELEMENT("CanBounce", ToStr(canBounce).c_str(), elem);
    XML_ADD_TEXT_ELEMENT("CanJump", ToStr(canJump).c_str(), elem);
    XML_ADD_TEXT_ELEMENT("JumpHeight", ToStr(maxJumpHeight).c_str(), elem);
    XML_ADD_2_PARAM_ELEMENT("CollisionSize", "width", width, "height", height, elem);
    XML_ADD_TEXT_ELEMENT("GravityScale", ToStr(gravityScale).c_str(), elem);
    XML_ADD_TEXT_ELEMENT("Friction", ToStr(friction).c_str(), elem);
    XML_ADD_TEXT_ELEMENT("Density", ToStr(density).c_str(), elem);

    return elem;
}

inline TiXmlElement* CreateControllableComponent(bool active)
{
    TiXmlElement* elem = new TiXmlElement("ClawControllableComponent");
    
    XML_ADD_TEXT_ELEMENT("IsActive", ToStr(active).c_str(), elem);

    return elem;
}

inline TiXmlElement* CreateAnimationComponent(const char* aniPath)
{
    TiXmlElement* elem = new TiXmlElement("AnimationComponent");

    XML_ADD_TEXT_ELEMENT("AnimationPath", aniPath, elem);

    return elem;
}

inline TiXmlElement* CreateActorRenderComponent(const char* imagesPath, int32 zCoord, bool visible = true)
{
    TiXmlElement* elem = new TiXmlElement("ActorRenderComponent");

    XML_ADD_TEXT_ELEMENT("ImagePath", imagesPath, elem);
    XML_ADD_TEXT_ELEMENT("ZCoord", ToStr(zCoord).c_str(), elem);

    return elem;
}

//=====================================================================================================================

inline TiXmlElement* CreateAnimationComponent(std::string aniPath)
{
    TiXmlElement* animationComponent = new TiXmlElement("AnimationComponent");
    XML_ADD_TEXT_ELEMENT("AnimationPath", aniPath.c_str(), animationComponent);
    return animationComponent;
}

inline TiXmlElement* CreateCycleAnimation(int animFrameTime)
{
    TiXmlElement* cycleElem = new TiXmlElement("Animation");
    std::string cycleStr = "cycle" + ToStr(animFrameTime);
    cycleElem->SetAttribute("type", cycleStr.c_str());
    //XML_ADD_1_PARAM_ELEMENT("Animation", "type", cycleStr.c_str(), cycleElem);

    return cycleElem;
}

//=====================================================================================================================


inline TiXmlElement* EyeCandyToXml(WwdObject* pWwdObject)
{
    if (std::string(pWwdObject->logic).find("Ani") != std::string::npos)
    {
        TiXmlElement* animElem = new TiXmlElement("AnimationComponent");
        XML_ADD_1_PARAM_ELEMENT("Animation", "type", "cycle150", animElem);

        return animElem;
    }

    return NULL;
}

inline TiXmlElement* TogglePegToXml(WwdObject* pWwdObject)
{
    TiXmlElement* pTogglePegAIElem = new TiXmlElement("TogglePegAIComponent");
    pTogglePegAIElem->SetAttribute("Type", pWwdObject->logic);
    pTogglePegAIElem->SetAttribute("resource", "created");

    std::string logic = pWwdObject->logic;
    std::string imageSet = pWwdObject->imageSet;

    uint32 delay = 0;
    if (logic == "TogglePeg")
    {
        delay = 0;
    }
    else if (logic == "TogglePeg2")
    {
        delay = 750;
    }
    else if (logic == "TogglePeg3")
    {
        delay = 1500;
    }
    else if (logic == "TogglePeg4")
    {
        delay = 2250;
    }
    else
    {
        LOG_WARNING("Unknown TogglePeg: " + logic)
    }

    if (pWwdObject->speed > 0)
    {
        delay = pWwdObject->speed;
    }

    XML_ADD_TEXT_ELEMENT("Delay", ToStr(delay).c_str(), pTogglePegAIElem);

    uint32 timeOn = 0;
    uint32 timeOff = 0;
    if (pWwdObject->speedX > 0)
    {
        timeOn = pWwdObject->speedX;
    }
    else
    {
        timeOn = 1500;
    }
    if (pWwdObject->speedY > 0)
    {
        timeOff = pWwdObject->speedY;
    }
    else
    {
        timeOff = 1500;
    }

    if (pWwdObject->smarts & 0x1)
    {
        XML_ADD_TEXT_ELEMENT("AlwaysOn", "true", pTogglePegAIElem);
    }
    else
    {
        XML_ADD_TEXT_ELEMENT("TimeOn", ToStr(timeOn).c_str(), pTogglePegAIElem);
        XML_ADD_TEXT_ELEMENT("TimeOff", ToStr(timeOff).c_str(), pTogglePegAIElem);
    }

    // Different image sets have different anim frames at which they toggle

    int toggleFrameIdx = -1;
    if (imageSet == "LEVEL_PEG") // LEVEL 1
    {
        toggleFrameIdx = 9;
    }
    else if (imageSet == "LEVEL_PEGSLIDER") // LEVEL 2
    {
        toggleFrameIdx = 2;
    }

    assert(toggleFrameIdx != -1);

    XML_ADD_TEXT_ELEMENT("ToggleFrameIdx", ToStr(toggleFrameIdx).c_str(), pTogglePegAIElem);

    return pTogglePegAIElem;
}

// @moveType: Left/Right/LeftTop/... or stop for a certain time only 1-9 are valid
// @value: pixel distance or milisecond stop
inline ElevatorStepDef GetElevatorStepDef(int moveType, int value)
{
    assert(moveType >= 1 && moveType <= 9);
    assert(value > 0);

    Direction dir = Direction_None;

    switch (moveType)
    {
        case 1: dir = Direction_Down_Left; break;
        case 2: dir = Direction_Down; break;
        case 3: dir = Direction_Down_Right; break;
        case 4: dir = Direction_Left; break;
        case 5: dir = Direction_None; break;
        case 6: dir = Direction_Right; break;
        case 7: dir = Direction_Up_Left; break;
        case 8: dir = Direction_Up; break;
        case 9: dir = Direction_Up_Right; break;
        default: assert(false);
    }

    ElevatorStepDef def;

    // Stop for given amount of milis
    if (dir == Direction_None)
    {
        def.isWaiting = true;
        def.waitMsTime = value;
    }
    else
    {
        def.direction = dir;
        def.stepDeltaDistance = value;
    }
    
    return def; 
}

inline TiXmlElement* WwdObjectToXml(WwdObject* wwdObject, std::string& imagesRootPath, int levelNumber)
{
    TiXmlElement* pActorElem = new TiXmlElement("Actor");
    pActorElem->SetAttribute("Type", wwdObject->imageSet);

    std::string logic = wwdObject->logic;
    std::string imageSet = wwdObject->imageSet;

    // Common components for all actors

    // Position component
    INSERT_POSITION_COMPONENT(wwdObject->x, wwdObject->y, pActorElem);

    // ActorRenderComponent
    //----- [Level::Actors::ActorProperties::ActorRenderComponent]
    TiXmlElement* actorRenderComponent = new TiXmlElement("ActorRenderComponent");
    pActorElem->LinkEndChild(actorRenderComponent);

    // For debug
    XML_ADD_TEXT_ELEMENT("imgpath", wwdObject->imageSet, actorRenderComponent);

    XML_ADD_TEXT_ELEMENT("Visible", ToStr(true).c_str(), actorRenderComponent);
    XML_ADD_TEXT_ELEMENT("Mirrored", ToStr((wwdObject->drawFlags & WAP_OBJECT_DRAW_FLAG_MIRROR) != 0).c_str(), actorRenderComponent);
    XML_ADD_TEXT_ELEMENT("Inverted", ToStr((wwdObject->drawFlags & WAP_OBJECT_DRAW_FLAG_INVERT) != 0).c_str(), actorRenderComponent);
    XML_ADD_TEXT_ELEMENT("ZCoord", ToStr(wwdObject->z).c_str(), actorRenderComponent);

    std::string tmpImagesRootPath = imagesRootPath;
    std::string tmpImageSet = wwdObject->imageSet;
    bool imageSetValid = false;

    if (tmpImageSet.find("LEVEL_") == 0)
    {
        // Remove "LEVEL_" from tmpImageSet, e.g. "LEVEL_SOLDIER" -> "SOLDIER"
        tmpImageSet.erase(0, strlen("LEVEL_"));
        tmpImagesRootPath = "/LEVEL" + ToStr(levelNumber) + "/IMAGES/";
        imageSetValid = true;
    }
    else if (tmpImageSet.find("GAME_") == 0)
    {
        // Remove "GAME_" from tmpImageSet, e.g. "GAME_TREASURE_COINS" -> "TREASURE_COINS"
        tmpImageSet.erase(0, strlen("GAME_"));
        tmpImagesRootPath = std::string("/GAME/IMAGES/");
        std::replace(tmpImageSet.begin(), tmpImageSet.end(), '_', '/');
        imageSetValid = true;

    }
    else
    {
        LOG_WARNING("Unknown actor image path: " + std::string(wwdObject->imageSet));
    }
    

    if (imageSetValid)
    {
        //std::replace(tmpImageSet.begin(), tmpImageSet.end(), '_', '/');
        tmpImageSet += "/*";
        tmpImageSet = tmpImagesRootPath + tmpImageSet;
    }

    // DoNothing logic only has one single frame which has to be assigned
    // We cant use general wildcard here
    // Example: We want "/LEVEL1/IMAGES/ARCHESFRONT/*1.PID"
    if (logic == "DoNothing")
    {
        // Unfortunately, this hack IS NOT ENOUGH, index "i" == -1 means "i" should be 1
        // I really have no clue how could Claw's level parsing code should be sane by any
        // means. Ok, enough of my crying.
        if (wwdObject->i == -1)
        {
            wwdObject->i = 1;
        }
        tmpImageSet += ToStr(wwdObject->i) + ".PID";
    }

    XML_ADD_TEXT_ELEMENT("ImagePath", tmpImageSet.c_str(), actorRenderComponent);

    std::string aniPath;
    // We probably should have valid animation frames in order to create an animation :d
    if (imageSetValid)
    {
        aniPath = tmpImageSet;
        aniPath.replace(aniPath.find("/IMAGES/"), strlen("/IMAGES/"), "/ANIS/");
    }

    //=========================================================================
    // Specific logics to XML
    //=========================================================================

    //LOG("Logic: " + logic);
    if (logic.find("AmbientSound") != std::string::npos)
    {
        std::string sound = wwdObject->sound;
        int soundVolume = wwdObject->damage;
        if (soundVolume == 0)
        {
            soundVolume = 100;
        }

        if (logic == "GlobalAmbientSound")
        {
            int minTimeOff = wwdObject->moveRect.right;
            int maxTimeOff = wwdObject->moveRect.bottom;
            int minTimeOn = wwdObject->moveRect.left;
            int maxTimeOn = wwdObject->moveRect.top;
            bool isLooping = minTimeOn == 0;

            // Level 2/3/4 global sounds are SO DAMN LOUD
            if ((levelNumber == 2 || levelNumber == 3 || levelNumber == 4) && isLooping)
            {
                soundVolume /= 5;
            }
            // Also non-looping sounds require tweaks
            if ((levelNumber == 1 || levelNumber == 2 || levelNumber == 3 || levelNumber == 4) && !isLooping)
            {
                //soundVolume = (int)((float)soundVolume / 1.5f);
            }

            SAFE_DELETE(pActorElem);
            return ActorTemplates::CreateXmlData_GlobalAmbientSoundActor(
                sound,
                soundVolume,
                minTimeOff,
                maxTimeOff,
                minTimeOn,
                maxTimeOn,
                isLooping);
        }
        else if (logic == "SpotAmbientSound" || 
                 logic == "AmbientSound")
        {
            /*if (pWwdObject->maxX != 0 && pWwdObject->maxY != 0)
            {
                pAmbientSoundElem->LinkEndChild(CreatePlayAreaElement(pWwdObject->minX, pWwdObject->minY,
                    pWwdObject->maxX - pWwdObject->minX, pWwdObject->maxY - pWwdObject->minY));
            }*/

            /*assert(wwdObject->maxX != 0);
            assert(wwdObject->maxY != 0);*/

            // Claw guys seem to have some typos in image set names...
            if (sound == "LEVEL_AMBIENT_ANGVIL")
            {
                sound = "LEVEL_AMBIENT_ANVIL";
                // This had to be some kind of a mistake on their part
                soundVolume = 0;
            }

            /*Point center(
                (wwdObject->minX + wwdObject->maxX) / 2,
                (wwdObject->minY + wwdObject->maxY) / 2);*/

            Point center(wwdObject->x, wwdObject->y);
            Point size;

            if (wwdObject->maxX == 0 && wwdObject->maxY == 0)
            {
                assert(wwdObject->hitRect.left != 0);
                assert(wwdObject->hitRect.right != 0);
                assert(wwdObject->hitRect.top != 0);
                assert(wwdObject->hitRect.bottom != 0);

                center.Set(
                    (wwdObject->hitRect.right + wwdObject->hitRect.left) / 2,
                    (wwdObject->hitRect.bottom + wwdObject->hitRect.top) / 2);
                size.Set(
                    (wwdObject->hitRect.right - wwdObject->hitRect.left),
                    (wwdObject->hitRect.bottom - wwdObject->hitRect.top));
            }
            else
            {
                size.Set(
                    wwdObject->maxX - wwdObject->minX,
                    wwdObject->maxY - wwdObject->minY);
            }

            assert(!center.IsZeroXY());
            assert(!size.IsZeroXY());
            
            if (size.y > size.x)
            {
                size.y = size.x;
            }

            LocalAmbientSoundDef soundDef;
            soundDef.sound = ActorTemplates::GetSoundPathFromClawPath(sound);
            soundDef.volume = soundVolume;
            soundDef.soundAreaSize = size;

            SAFE_DELETE(pActorElem);
            return ActorTemplates::CreateXmlData_LocalAmbientSound(
                ActorPrototype_LocalAmbientSound,
                center,
                soundDef);
        }
        else
        {
            LOG("Not created ambient sound logic: " + logic);
            Point pos(wwdObject->x, wwdObject->y);
            LOG("Position: " + pos.ToString());
        }

        //pActorElem->LinkEndChild(AmbientSoundToXml(wwdObject));
    }
    else if (logic.find("SoundTrigger") != std::string::npos)
    {
        std::string sound = wwdObject->sound;
        if (true)//sound == "LEVEL_TRIGGER_PUDDLE2")
        {
            SDL_Rect predefinedPosition;
            predefinedPosition.x = wwdObject->minX;
            predefinedPosition.y = wwdObject->minY;
            predefinedPosition.w = wwdObject->maxX;
            predefinedPosition.h = wwdObject->maxY;

            int enterCount = wwdObject->smarts;
            // TODO: ...
            if (enterCount == 0)
            {
                enterCount = 1;
            }

            if ((levelNumber == 3) && (sound == "LEVEL_TRIGGER_BIRDCALL2"))
            {
                sound = "LEVEL_AMBIENT_BIRDCALL2";
            }
            if ((levelNumber == 3) && (sound == "LEVEL_TRIGGER_1013"))
            {
                sound = "/LEVEL10/SOUNDS/TRIGGER/1013.WAV";
            }

            SAFE_DELETE(pActorElem);
            return ActorTemplates::CreateXmlData_SoundTriggerActor(sound, logic, Point(wwdObject->x, wwdObject->y), predefinedPosition, enterCount);
        }
        //pActorElem->LinkEndChild(SoundTriggerToXml(wwdObject));
    }
    else if (logic.find("StackedCrates") != std::string::npos)
    {
        //pActorElem->LinkEndChild(StackedCratesToXml(wwdObject));
        
    }
    else if (logic.find("Crate") != std::string::npos)
    {
        std::vector<PickupType> loot;
        if (wwdObject->powerup > 0) { loot.push_back(PickupType(wwdObject->powerup)); }
        if (wwdObject->userRect1.left > 0) { loot.push_back(PickupType(wwdObject->userRect1.left)); }
        if (wwdObject->userRect1.right > 0) { loot.push_back(PickupType(wwdObject->userRect1.right)); }
        if (wwdObject->userRect1.bottom > 0) { loot.push_back(PickupType(wwdObject->userRect1.bottom)); }
        if (wwdObject->userRect1.top > 0) { loot.push_back(PickupType(wwdObject->userRect1.top)); }
        if (wwdObject->userRect2.left > 0) { loot.push_back(PickupType(wwdObject->userRect2.left)); }
        if (wwdObject->userRect2.right > 0) { loot.push_back(PickupType(wwdObject->userRect2.right)); }
        if (wwdObject->userRect2.bottom > 0) { loot.push_back(PickupType(wwdObject->userRect2.bottom)); }
        if (wwdObject->userRect2.top > 0) { loot.push_back(PickupType(wwdObject->userRect2.top)); }

        SAFE_DELETE(pActorElem);
        return ActorTemplates::CreateXmlData_CrateActor(tmpImageSet, Point(wwdObject->x, wwdObject->y), loot, 5, wwdObject->z);
    }
    else if (logic.find("Statue") != std::string::npos)
    {
        std::vector<PickupType> loot;
        if (wwdObject->powerup > 0) { loot.push_back(PickupType(wwdObject->powerup)); }
        if (wwdObject->userRect1.left > 0) { loot.push_back(PickupType(wwdObject->userRect1.left)); }
        if (wwdObject->userRect1.right > 0) { loot.push_back(PickupType(wwdObject->userRect1.right)); }
        if (wwdObject->userRect1.bottom > 0) { loot.push_back(PickupType(wwdObject->userRect1.bottom)); }
        if (wwdObject->userRect1.top > 0) { loot.push_back(PickupType(wwdObject->userRect1.top)); }
        if (wwdObject->userRect2.left > 0) { loot.push_back(PickupType(wwdObject->userRect2.left)); }
        if (wwdObject->userRect2.right > 0) { loot.push_back(PickupType(wwdObject->userRect2.right)); }
        if (wwdObject->userRect2.bottom > 0) { loot.push_back(PickupType(wwdObject->userRect2.bottom)); }
        if (wwdObject->userRect2.top > 0) { loot.push_back(PickupType(wwdObject->userRect2.top)); }

        assert(levelNumber == 5);

        SAFE_DELETE(pActorElem);
        return ActorTemplates::CreateXmlData_LootContainer(
            ActorPrototype_Level5_LootStatue, 
            Point(wwdObject->x, wwdObject->y), 
            loot, 
            wwdObject->z);
    }
    else if (logic.find("Candy") != std::string::npos ||
             logic == "AniCycle")
    {
        TiXmlElement* elem = EyeCandyToXml(wwdObject);
        if (elem)
        {
            pActorElem->LinkEndChild(elem);
        }
    }
    else if (logic.find("CrumblingPeg") != std::string::npos)
    {
        SAFE_DELETE(pActorElem);

        Point position = Point(wwdObject->x, wwdObject->y);
        ActorPrototype proto = ActorPrototype_None;

        switch (levelNumber)
        {
            case 1: proto = ActorPrototype_Level1_CrumblingPeg; break;
            case 3:
            {
                if (imageSet == "LEVEL_CRUMBLINPEG1")
                {
                    proto = ActorPrototype_Level3_CrumblingPeg_1;
                }
                else if (imageSet == "LEVEL_CRUMBLINPEG2")
                {
                    proto = ActorPrototype_Level3_CrumblingPeg_2;
                }
                break;
            }
            case 4: proto = ActorPrototype_Level4_CrumblingPeg; break;
            case 5: proto = ActorPrototype_Level5_CrumblingPeg; break;
            case 6: proto = ActorPrototype_Level6_CrumblingPeg; break;
            case 7: proto = ActorPrototype_Level7_CrumblingPeg; break;
            default: return NULL;
        }

        if (proto == ActorPrototype_None)
        {
            return NULL;
        }

        // assert(proto != ActorPrototype_Start && "Unsupported level ?");

        return ActorTemplates::CreateXmlData_CrumblingPeg(proto, position, 0);
    }
    else if (logic.find("Elevator") != std::string::npos &&
             logic != "PathElevator")
    {
        SAFE_DELETE(pActorElem);

        std::string logic = wwdObject->logic;

        Point speed(0, 0);
        bool hasHorizontalMovement = false;
        bool hasVerticalMovement = false;
        if (wwdObject->minX > 0 && wwdObject->maxX > 0)
        {
            hasHorizontalMovement = true;
        }
        if (wwdObject->minY > 0 && wwdObject->maxY > 0)
        {
            hasVerticalMovement = true;
        }

        if (hasHorizontalMovement)
        {
            if (wwdObject->speedX > 0)
            {
                speed.x = wwdObject->speedX;
            }
            else
            {
                speed.x = 125;
            }
        }
        if (hasVerticalMovement)
        {
            if (wwdObject->speedY > 0)
            {
                speed.y = wwdObject->speedY;
            }
            else if (wwdObject->speedX > 0)
            {
                speed.y = wwdObject->speedX;
            }
            else
            {
                speed.y = 125;
            }
        }

        ElevatorDef elevatorDef;
        elevatorDef.speed = speed;
        elevatorDef.minPosition.Set(wwdObject->minX, wwdObject->minY);
        elevatorDef.maxPosition.Set(wwdObject->maxX, wwdObject->maxY);
        elevatorDef.hasTriggerBehaviour = logic.find("Trigger") != std::string::npos;
        elevatorDef.hasStartBehaviour = logic.find("Start") != std::string::npos;
        elevatorDef.hasStopBehaviour = logic.find("Stop") != std::string::npos;
        elevatorDef.hasOneWayBehaviour = logic.find("OneWay") != std::string::npos;

        Point position(wwdObject->x, wwdObject->y);

        ActorPrototype elevatorProto = ActorPrototype_None;
        if (levelNumber == 1)
        {
            elevatorProto = ActorPrototype_Level1_Elevator;
        }
        else if (levelNumber == 2)
        {
            elevatorProto = ActorPrototype_Level2_Elevator;
        }
        else if (levelNumber == 3)
        {
            if (imageSet == "LEVEL_ELEVATOR1")
            {
                elevatorProto = ActorPrototype_Level3_Elevator_1;
            }
            else
            {
                elevatorProto = ActorPrototype_Level3_Elevator_2;
            }
        }
        else if (levelNumber == 4)
        {
            elevatorProto = ActorPrototype_Level4_Elevator;
        }
        else if (levelNumber == 5)
        {
            elevatorProto = ActorPrototype_Level5_Elevator;
        }
        else if (levelNumber == 6)
        {
            if (imageSet == "LEVEL_GRILLELEVATOR")
            {
                elevatorProto = ActorPrototype_Level6_Elevator_1;
            }
            else
            {
                elevatorProto = ActorPrototype_Level6_Elevator_2;
            }
        }
        else if (levelNumber == 7)
        {
            elevatorProto = ActorPrototype_Level7_Elevator;
        }

        if (elevatorProto == ActorPrototype_None)
        {
            return NULL;
        }

        // assert(elevatorProto != ActorPrototype_Start && "Unsupported level ?");

        return ActorTemplates::CreateXmlData_ElevatorActor(elevatorProto, position, elevatorDef);
    }
    else if (logic.find("TogglePeg") != std::string::npos)
    {
        SAFE_DELETE(pActorElem);

        std::string logic = wwdObject->logic;
        std::string imageSet = wwdObject->imageSet;

        TogglePegDef togglePegDef;

        uint32 delay = 0;
        if (logic == "TogglePeg")
        {
            togglePegDef.delay = 0;
        }
        else if (logic == "TogglePeg2")
        {
            togglePegDef.delay = 750;
        }
        else if (logic == "TogglePeg3")
        {
            togglePegDef.delay = 1500;
        }
        else if (logic == "TogglePeg4")
        {
            togglePegDef.delay = 2250;
        }
        else
        {
            LOG_WARNING("Unknown TogglePeg: " + logic)
        }

        if (wwdObject->speed > 0)
        {
            togglePegDef.delay = wwdObject->speed;
        }

        uint32 timeOn = 0;
        uint32 timeOff = 0;
        if (wwdObject->speedX > 0)
        {
            timeOn = wwdObject->speedX;
        }
        else
        {
            timeOn = 1500;
        }
        if (wwdObject->speedY > 0)
        {
            timeOff = wwdObject->speedY;
        }
        else
        {
            timeOff = 1500;
        }

        if (wwdObject->smarts & 0x1)
        {
            togglePegDef.isAlwaysOn = true;
        }
        else
        {
            togglePegDef.timeOn = timeOn;
            togglePegDef.timeOff = timeOff;
        }

        Point position(wwdObject->x, wwdObject->y);

        ActorPrototype proto = ActorPrototype_None;
        if (levelNumber == 1) proto = ActorPrototype_Level1_TogglePeg;
        else if (levelNumber == 2) proto = ActorPrototype_Level2_TogglePeg;
        else if (levelNumber == 5) proto = ActorPrototype_Level5_TogglePeg;

        if (proto == ActorPrototype_None)
        {
            return NULL;
        }

        // assert(proto != ActorPrototype_Start);

        return ActorTemplates::CreateXmlData_TogglePegActor(
            proto,
            position,
            togglePegDef);

        /*pActorElem->LinkEndChild(TogglePegToXml(wwdObject));

        TiXmlElement* animElem = new TiXmlElement("AnimationComponent");
        XML_ADD_1_PARAM_ELEMENT("Animation", "type", "cycle75", animElem);
        pActorElem->LinkEndChild(animElem);*/
    }
    else if (logic.find("Checkpoint") != std::string::npos)
    {
        bool isSaveCheckpoint = false;
        uint32 saveCheckpointNumber = 0;
        if (logic == "FirstSuperCheckpoint")
        {
            isSaveCheckpoint = true;
            saveCheckpointNumber = 1;
        }
        else if (logic == "SecondSuperCheckpoint")
        {
            isSaveCheckpoint = true;
            saveCheckpointNumber = 2;
        }

        SAFE_DELETE(pActorElem);
        return ActorTemplates::CreateXmlData_CheckpointActor(
            wwdObject->imageSet, 
            Point(wwdObject->x, wwdObject->y), 
            wwdObject->z, 
            Point(wwdObject->x, wwdObject->y), 
            isSaveCheckpoint, 
            saveCheckpointNumber);
    }
    else if (logic.find("PowderKeg") != std::string::npos)
    {
        //pActorElem->LinkEndChild(PowderKegToXml(wwdObject));
        SAFE_DELETE(pActorElem);
        return ActorTemplates::CreateXmlData_PowderKegActor(tmpImageSet, Point(wwdObject->x, wwdObject->y), 50, wwdObject->z);
    }
    else if (logic == "Officer" ||
             logic == "Soldier" ||
             logic == "Rat" ||
             logic == "PunkRat" ||
             logic == "CutThroat" ||
             logic == "RobberThief" ||
             logic == "TownGuard1" ||
             logic == "TownGuard2" ||
             logic == "Seagull" ||
             logic == "RedTailPirate" ||
             logic == "BearSailor")
    {
        std::vector<PickupType> loot;
        if (wwdObject->powerup > 0) { loot.push_back(PickupType(wwdObject->powerup)); }
        if (wwdObject->userRect1.left > 0) { loot.push_back(PickupType(wwdObject->userRect1.left)); }
        if (wwdObject->userRect1.right > 0) { loot.push_back(PickupType(wwdObject->userRect1.right)); }
        if (wwdObject->userRect1.bottom > 0) { loot.push_back(PickupType(wwdObject->userRect1.bottom)); }
        if (wwdObject->userRect1.top > 0) { loot.push_back(PickupType(wwdObject->userRect1.top)); }

        SAFE_DELETE(pActorElem);

        ActorPrototype actorProto = ActorPrototype_None;
        if (levelNumber == 1)
        {
            if (logic == "Rat")
            {
                actorProto = ActorPrototype_Level1_Rat;
            }
            else if (logic == "Soldier")
            {
                actorProto = ActorPrototype_Level1_Soldier;
            }
            else if (logic == "Officer")
            {
                actorProto = ActorPrototype_Level1_Officer;
            }
        }
        else if (levelNumber == 2)
        {
            if (logic == "Soldier")
            {
                actorProto = ActorPrototype_Level2_Soldier;
            }
            else if (logic == "Officer")
            {
                actorProto = ActorPrototype_Level2_Officer;
            }
            else if (logic == "PunkRat")
            {
                actorProto = ActorPrototype_Level2_PunkRat;
            }
        }
        else if (levelNumber == 3)
        {
            if (logic == "Rat")
            {
                actorProto = ActorPrototype_Level3_Rat;
            }
            else if (logic == "CutThroat")
            {
                actorProto = ActorPrototype_Level3_CutThroat;
            }
            else if (logic == "RobberThief")
            {
                actorProto = ActorPrototype_Level3_RobberThief;
            }
        }
        else if (levelNumber == 4)
        {
            if (logic == "Rat")
            {
                actorProto = ActorPrototype_Level4_Rat;
            }
            else if (logic == "CutThroat")
            {
                actorProto = ActorPrototype_Level4_CutThroat;
            }
            else if (logic == "RobberThief")
            {
                actorProto = ActorPrototype_Level4_RobberThief;
            }
        }
        else if (levelNumber == 5)
        {
            if (logic == "TownGuard1")
            {
                actorProto = ActorPrototype_Level5_TownGuard1;

                // Jumping is not yet implemented. Make them patrol instead
                if ((wwdObject->maxX - wwdObject->minX) <= 100)
                {
                    wwdObject->minX -= 80;
                    wwdObject->maxX += 80;
                }
            }
            else if (logic == "TownGuard2")
            {
                actorProto = ActorPrototype_Level5_TownGuard2;
            }
            else if (logic == "Seagull")
            {
                actorProto = ActorPrototype_Level5_Seagull;
            }
        }
        else if (levelNumber == 6)
        {
            if (logic == "TownGuard1")
            {
                actorProto = ActorPrototype_Level6_TownGuard1;
            }
            else if (logic == "TownGuard2")
            {
                actorProto = ActorPrototype_Level6_TownGuard2;
            }
            else if (logic == "Seagull")
            {
                actorProto = ActorPrototype_Level6_Seagull;
            }
            else if (logic == "Rat")
            {
                actorProto = ActorPrototype_Level6_Rat;
            }
        }
        else if (levelNumber == 7)
        {
            if (logic == "Seagull")
            {
                actorProto = ActorPrototype_Level7_Seagull;
            }
            if (logic == "RedTailPirate")
            {
                actorProto = ActorPrototype_Level7_RedTailPirate;
            }
            if (logic == "BearSailor")
            {
                actorProto = ActorPrototype_Level7_BearSailor;
            }
        }

        if (actorProto == ActorPrototype_None)
        {
            //LOG_ERROR("Failed to find ActorPrototype for logic: " + logic + " in level: " + ToStr(levelNumber));
            return NULL;
            // assert(false && "Unsupported level ?");
        }

        return ActorTemplates::CreateXmlData_EnemyAIActor(
            actorProto,
            Point(wwdObject->x, wwdObject->y), 
            loot, 
            wwdObject->minX, 
            wwdObject->maxX,
            wwdObject->userValue1);
    }
    else if (logic.find("Rat") != std::string::npos)
    {
        //pActorElem->LinkEndChild(RatToXml(wwdObject));
    }
    //=============================================================================================
    // PICKUPS
    //=============================================================================================
    else if (logic == "AmmoPowerup" ||
             logic == "SpecialPowerup" ||
             logic == "TreasurePowerup" ||
             logic == "GlitterlessPowerup" ||
             logic == "HealthPowerup" ||
             logic == "BossWarp" ||
             logic == "EndOfLevelPowerup" ||
             logic == "MagicPowerup")
    {
        SAFE_DELETE(pActorElem);

        ParamMap paramMap;
        paramMap["IsMirrored"] = ToStr((wwdObject->drawFlags & WAP_OBJECT_DRAW_FLAG_MIRROR) != 0);
        paramMap["IsInverted"] = ToStr((wwdObject->drawFlags & WAP_OBJECT_DRAW_FLAG_INVERT) != 0);

        if (imageSet == "GAME_WARP" || imageSet == "GAME_VERTWARP" || imageSet == "GAME_BOSSWARP")
        {
            paramMap["IsGlitter"] = "false";
            paramMap["DestinationX"] = ToStr(wwdObject->speedX);
            paramMap["DestinationY"] = ToStr(wwdObject->speedY);
        }

        if (imageSet == "GAME_TREASURE_COINS")
        {
            paramMap["IsGlitter"] = "false";
        }

        if ((imageSet.find("_CATNIP") != std::string::npos) ||
            imageSet.find("_POWERUPS") != std::string::npos)
        {
            if (wwdObject->smarts > 0)
            {
                paramMap["PowerupDuration"] = ToStr(wwdObject->smarts);
            }
        }

        PickupType pickupType = ActorTemplates::StringToEnum_ImageSetToPickupType(wwdObject->imageSet);

        return ActorTemplates::CreateXmlData_PickupActor(pickupType, Point(wwdObject->x, wwdObject->y), true, paramMap);
    }
    else if (logic == "TowerCannonLeft")
    {
        SAFE_DELETE(pActorElem);

        assert(levelNumber == 2 && "Expected only level 2");

        Point position(wwdObject->x, wwdObject->y);
        return ActorTemplates::CreateXmlData_Actor(ActorPrototype_Level2_TowerCannonLeft, position);
    }
    else if (logic == "TowerCannonRight")
    {
        SAFE_DELETE(pActorElem);

        assert(levelNumber == 2 && "Expected only level 2");

        Point position(wwdObject->x, wwdObject->y);
        return ActorTemplates::CreateXmlData_Actor(ActorPrototype_Level2_TowerCannonRight, position);
    }
    else if (logic == "BossStager")
    {
        SAFE_DELETE(pActorElem);

        //assert(levelNumber == 2 && "Expected only level 2");
        ActorPrototype proto = ActorPrototype_None;
        if (levelNumber == 2)
        {
            proto = ActorPrototype_Level2_BossStager;
        }
        else if (levelNumber == 4)
        {
            proto = ActorPrototype_Level4_BossStager;
        }
        else if (levelNumber == 6)
        {
            proto = ActorPrototype_Level6_BossStager;
        }

        if (proto == ActorPrototype_None)
        {
            return NULL;
        }

        // assert(proto != ActorPrototype_None);

        Point position(wwdObject->x, wwdObject->y);
        return ActorTemplates::CreateXmlData_Actor(proto, position);
    }
    else if (logic == "Raux")
    {
        SAFE_DELETE(pActorElem);

        return ActorTemplates::CreateXmlData_EnemyAIActor(
            ActorPrototype_Level2_LaRaux,
            Point(wwdObject->x, wwdObject->y),
            {},
            wwdObject->minX,
            wwdObject->maxX,
            false);
    }
    else if (logic == "Katherine")
    {
        SAFE_DELETE(pActorElem);

        return ActorTemplates::CreateXmlData_EnemyAIActor(
            ActorPrototype_Level4_Katherine,
            Point(wwdObject->x, wwdObject->y),
            {},
            wwdObject->minX,
            wwdObject->maxX,
            false);
    }
    else if (logic == "Wolvington")
    {
        SAFE_DELETE(pActorElem);

        return ActorTemplates::CreateXmlData_EnemyAIActor(
            ActorPrototype_Level6_Wolvington,
            Point(wwdObject->x, wwdObject->y),
            {},
            wwdObject->minX,
            wwdObject->maxX,
            false);
    }
    else if (logic == "PathElevator")
    {
        /*static bool wasHere = false;
        if (wasHere) return pActorElem;
        wasHere = true;*/

        ActorPrototype proto = ActorPrototype_BasePathElevator;
        if (levelNumber == 7)
        {
            proto = ActorPrototype_Level7_PathElevator;
        }

        PathElevatorDef pathElevatorDef;

        pathElevatorDef.speed = wwdObject->speed;
        if (wwdObject->moveRect.left != 0)
        {
            pathElevatorDef.elevatorPath.push_back(GetElevatorStepDef(wwdObject->moveRect.left, wwdObject->moveRect.top));
        }
        if (wwdObject->moveRect.right != 0)
        {
            pathElevatorDef.elevatorPath.push_back(GetElevatorStepDef(wwdObject->moveRect.right, wwdObject->moveRect.bottom));
        }

        if (wwdObject->hitRect.left != 0)
        {
            pathElevatorDef.elevatorPath.push_back(GetElevatorStepDef(wwdObject->hitRect.left, wwdObject->hitRect.top));
        }
        if (wwdObject->hitRect.right != 0)
        {
            pathElevatorDef.elevatorPath.push_back(GetElevatorStepDef(wwdObject->hitRect.right, wwdObject->hitRect.bottom));
        }

        if (wwdObject->attackRect.left != 0)
        {
            pathElevatorDef.elevatorPath.push_back(GetElevatorStepDef(wwdObject->attackRect.left, wwdObject->attackRect.top));
        }
        if (wwdObject->attackRect.right != 0)
        {
            pathElevatorDef.elevatorPath.push_back(GetElevatorStepDef(wwdObject->attackRect.right, wwdObject->attackRect.bottom));
        }

        if (wwdObject->clipRect.left != 0)
        {
            pathElevatorDef.elevatorPath.push_back(GetElevatorStepDef(wwdObject->clipRect.left, wwdObject->clipRect.top));
        }
        if (wwdObject->clipRect.right != 0)
        {
            pathElevatorDef.elevatorPath.push_back(GetElevatorStepDef(wwdObject->clipRect.right, wwdObject->clipRect.bottom));
        }

        // Default value. First occurance in level 7
        if (pathElevatorDef.speed == 0)
        {
            pathElevatorDef.speed = 125;
        }

        assert(pathElevatorDef.elevatorPath.size() > 2);

        Point position(wwdObject->x, wwdObject->y);

        SAFE_DELETE(pActorElem);

        return ActorTemplates::CreateXmlData_PathElevator(
            proto,
            position,
            tmpImageSet,
            pathElevatorDef);
    }
    else if (logic.find("FloorSpike") != std::string::npos)
    {
        int delay = 0;
        if (logic == "FloorSpike")
        {
            delay = 0;
        }
        else if (logic == "FloorSpike2")
        {
            delay = 750;
        }
        else if (logic == "FloorSpike3")
        {
            delay = 1500;
        }
        else if (logic == "FloorSpike4")
        {
            delay = 2250;
        }
        else
        {
            LOG("Offending floor spike: " + logic);
            assert(false && "Unknown floor spike");
        }

        ActorPrototype proto = ActorPrototype_BaseFloorSpike;
        Point position(wwdObject->x, wwdObject->y);

        // TODO: Make specific actor prototypes in next level(s)
        FloorSpikeDef def;
        if (levelNumber == 3)
        {
            def.activeFrameIdx = 5;
            def.activateSound = "/LEVEL3/SOUNDS/FLOORSPIKEUP.WAV";
            def.deactivateSound = "/LEVEL3/SOUNDS/FLOORSPIKEDOWN.WAV";
        }
        else if (levelNumber == 4)
        {
            def.activeFrameIdx = 4;
            def.activateSound = "/LEVEL4/SOUNDS/FLOORSPIKEUP.WAV";
            def.deactivateSound = "/LEVEL4/SOUNDS/FLOORSPIKEDOWN.WAV";
        }
        def.startDelay = delay;
        def.damage = 5;
        def.cycleDuration = 75;
        def.damagePulseInterval = 1000;
        def.timeOn = 1000;

        SAFE_DELETE(pActorElem);
        return ActorTemplates::CreateXmlData_FloorSpike(
            proto,
            position,
            tmpImageSet,
            def);
    }
    else if (logic == "GooVent")
    {
        Point position(wwdObject->x, wwdObject->y);

        SAFE_DELETE(pActorElem);
        return ActorTemplates::CreateXmlData_Actor(ActorPrototype_Level6_GooVent, position);
    }
    else if (logic == "AniRope")
    {
        ActorPrototype proto = ActorPrototype_BaseRope;
        Point position(wwdObject->x, wwdObject->y);

        RopeDef def;
        def.timeToFlayBackAndForth = wwdObject->speedX;
        if (def.timeToFlayBackAndForth == 0)
        {
            def.timeToFlayBackAndForth = 1500;
        }
        assert(def.timeToFlayBackAndForth != 0);

        // Specified value in WWD is only the time to flay in 1 direction
        def.timeToFlayBackAndForth *= 2;

        SAFE_DELETE(pActorElem);

        return ActorTemplates::CreateXmlData_Rope(
            proto,
            position,
            tmpImageSet,
            def);
    }
    else if (logic.find("SteppingStone") != std::string::npos)
    {
        SAFE_DELETE(pActorElem);

        ActorPrototype proto = ActorPrototype_None;

        if (levelNumber == 4)
        {
            proto = ActorPrototype_Level4_SteppingGround;
        }
        else if (levelNumber == 6)
        {
            proto = ActorPrototype_Level6_SteppingGround;
        }
        else if (levelNumber == 7)
        {
            proto = ActorPrototype_Level7_SteppingGround;
        }

        if (proto == ActorPrototype_None)
        {
            return NULL;
        }

        // assert(proto != ActorPrototype_None && "Unsupported level ?");

        Point position(wwdObject->x, wwdObject->y);

        // I dont know where this is defined
        SteppingGroundDef def;
        def.timeOn = 150;
        def.timeOff = 500;

        return ActorTemplates::CreateXmlData_SteppingGround(
            proto,
            position,
            def);
    }
    else if (logic == "SpringBoard" ||
             logic == "GroundBlower")
    {
        ActorPrototype proto = ActorPrototype_None;

        if (levelNumber == 4)
        {
            proto = ActorPrototype_Level4_SpringBoard;
        }
        else if (levelNumber == 6)
        {
            proto = ActorPrototype_Level6_GroundBlower;
        }
        else if (levelNumber == 7)
        {
            proto = ActorPrototype_Level7_SpringBoard;
        }

        if (proto == ActorPrototype_None)
        {
            return NULL;
        }

        // assert(proto != ActorPrototype_None && "Unsupported level ?");

        Point position(wwdObject->x, wwdObject->y);

        SpringBoardDef def;
        def.springHeight = wwdObject->maxY;
        //LOG("Position: " + position.ToString() + ", SpringHeight: " + ToStr(def.springHeight));
        if (def.springHeight == 0)
        {
            def.springHeight = 450;
        }

        SAFE_DELETE(pActorElem);

        // Everything should be in XML here
        return ActorTemplates::CreateXmlData_SpringBoard(
            proto,
            position,
            def);
    }
    else
    {
        static std::vector<std::string> s_ReportedUnknownLogicsList;

        bool isAlreadyReported = false;
        for (std::string unkLogic : s_ReportedUnknownLogicsList)
        {
            if (unkLogic == logic)
            {
                isAlreadyReported = true;
                break;
            }
        }

        if (!isAlreadyReported)
        {
            s_ReportedUnknownLogicsList.push_back(logic);
            LOG_WARNING("Unknown logic: " + logic);
        }
    }

    return pActorElem;
}

//=====================================================================================================================
// Claw to Xml
//=====================================================================================================================

inline TiXmlElement* CreateClawActor(WapWwd* pWapWwd)
{
    TiXmlElement* pClawActor = new TiXmlElement("Actor");
    pClawActor->SetAttribute("Type", "Claw");
    pClawActor->SetAttribute("resource", "created");

    /*ActorBodyDef clawBodyDef;
    clawBodyDef.addFootSensor = true;
    clawBodyDef.bodyType = b2_dynamicBody;
    clawBodyDef.collisionFlag = CollisionFlag_Controller;
    clawBodyDef.collisionMask = 0x1;
    clawBodyDef.makeSensor = false;
    clawBodyDef.makeCapsule = true;
    clawBodyDef.size = Point(40, 130);
    clawBodyDef.gravityScale = 4.0;
    clawBodyDef.fixtureType = FixtureType_Controller;
    pClawActor->LinkEndChild(ActorTemplates::CreatePhysicsComponent(&clawBodyDef));*/

    pClawActor->LinkEndChild(CreatePositionComponent(pWapWwd->properties.startX, pWapWwd->properties.startY));
    //pClawActor->LinkEndChild(CreatePositionComponent(6250, 4350));
    //pClawActor->LinkEndChild(CreateCollisionComponent(40, 100));
    pClawActor->LinkEndChild(CreatePhysicsComponent(true, false, true, g_pApp->GetGlobalOptions()->maxJumpHeight, 40, 90, 4.0, 0.0, 0.5));
    pClawActor->LinkEndChild(CreateControllableComponent(true));
    pClawActor->LinkEndChild(CreateAnimationComponent("/CLAW/ANIS/*"));
    pClawActor->LinkEndChild(CreateActorRenderComponent("/CLAW/IMAGES/*", 4000));
    pClawActor->LinkEndChild(ActorTemplates::CreateFollowableComponent(Point(-5, -80), "/GAME/IMAGES/EXCLAMATION/*", ""));

    TiXmlElement* pScoreComponent = new TiXmlElement("ScoreComponent");
    XML_ADD_TEXT_ELEMENT("Score", "0", pScoreComponent);
    pClawActor->LinkEndChild(pScoreComponent);

    TiXmlElement* pLifeComponent = new TiXmlElement("LifeComponent");
    XML_ADD_TEXT_ELEMENT("Lives", "0", pLifeComponent);
    pClawActor->LinkEndChild(pLifeComponent);

    TiXmlElement* pHealthComponent = new TiXmlElement("HealthComponent");
    XML_ADD_TEXT_ELEMENT("Health", "0", pHealthComponent);
    XML_ADD_TEXT_ELEMENT("MaxHealth", "100", pHealthComponent);
    pClawActor->LinkEndChild(pHealthComponent);

    TiXmlElement* pAmmoComponent = new TiXmlElement("AmmoComponent");
    XML_ADD_TEXT_ELEMENT("Pistol", "0", pAmmoComponent);
    XML_ADD_TEXT_ELEMENT("Magic", "0", pAmmoComponent);
    XML_ADD_TEXT_ELEMENT("Dynamite", "0", pAmmoComponent);
    pClawActor->LinkEndChild(pAmmoComponent);

    TiXmlElement* pPowerupComponent = new TiXmlElement("PowerupComponent");
    pClawActor->LinkEndChild(pPowerupComponent);

    return pClawActor;
}

//=====================================================================================================================
// HUD to Xml
//=====================================================================================================================

inline TiXmlElement* CreateHUDElement(std::string pathToImages, int animFrameDuration, std::string animPath, Point position, bool anchorRight, bool anchorBottom, std::string key, bool visible = true)
{
    TiXmlElement* pHUDElement = new TiXmlElement("Actor");
    pHUDElement->SetAttribute("Type", pathToImages.c_str());
    pHUDElement->SetAttribute("resource", "created");

    pHUDElement->LinkEndChild(CreatePositionComponent(position.x, position.y));

    if (!animPath.empty())
    {
        pHUDElement->LinkEndChild(CreateAnimationComponent(animPath));
    }
    else if (animFrameDuration > 0)
    {
        TiXmlElement* pAnimCompElem = new TiXmlElement("AnimationComponent");
        pAnimCompElem->LinkEndChild(CreateCycleAnimation(animFrameDuration));
        pHUDElement->LinkEndChild(pAnimCompElem);
    }

    TiXmlElement* pHUDRenderComponentElem = new TiXmlElement("HUDRenderComponent");
    XML_ADD_TEXT_ELEMENT("ImagePath", pathToImages.c_str(), pHUDRenderComponentElem);
    if (anchorRight)
    {
        XML_ADD_TEXT_ELEMENT("AnchorRight", "true", pHUDRenderComponentElem);
    }
    if (anchorBottom)
    {
        XML_ADD_TEXT_ELEMENT("AnchorBottom", "true", pHUDRenderComponentElem);
    }
    if (visible)
    {
        XML_ADD_TEXT_ELEMENT("Visible", "true", pHUDRenderComponentElem);
    }
    else
    {
        XML_ADD_TEXT_ELEMENT("Visible", "false", pHUDRenderComponentElem);
    }
    if (!key.empty())
    {
        XML_ADD_TEXT_ELEMENT("HUDElementKey", key.c_str(), pHUDRenderComponentElem);
    }
    pHUDElement->LinkEndChild(pHUDRenderComponentElem);

    return pHUDElement;
}

struct HUDElementDef
{
    HUDElementDef()
    {
        isPositionProtortional = false;
        isAnchoredRight = false;
        isAnchoredBottom = false;
        isVisible = true;
    }

    std::string imageSet;
    AnimationDef animDef;
    bool isPositionProtortional;
    Point positionProportion;
    Point position;
    bool isAnchoredRight;
    bool isAnchoredBottom;
    bool isVisible;
    std::string HUDElemKey;
};

inline TiXmlElement* CreateHUDElement(const HUDElementDef& def)
{
    Point pos = def.position;
    if (def.isPositionProtortional)
    {
        Point windowSize = g_pApp->GetWindowSize();
        Point windowScale = g_pApp->GetScale();

        pos.Set(
            (windowSize.x * def.positionProportion.x) / windowScale.x, 
            (windowSize.y * def.positionProportion.y) / windowScale.y);
    }

    return CreateHUDElement(
        def.imageSet,
        def.animDef.cycleAnimationDuration,
        def.animDef.animationPath,
        pos,
        def.isAnchoredRight,
        def.isAnchoredBottom,
        def.HUDElemKey,
        def.isVisible);
}

TiXmlElement* WwdToXml(WapWwd* wapWwd, int levelNumber);


#endif
