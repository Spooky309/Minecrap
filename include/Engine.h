#ifndef _H_ENGINE_
#define _H_ENGINE_
#include "Graphics.h"
#include "Input.h"
#include "World.h"
#include "Player.h"
#include "FileSystem.h"
#include "EngineVitals.h"
#include "FontManager.h"
class Engine
{
public:
    static Engine& Instance()
    {
        static Engine _inst;
        return _inst;
    }
    void Go(const std::vector<std::string> argv);
    inline Graphics& GetGraphics()
    {
        return *m_graphics;
    }
    inline Player& GetPlayer()
    {
        return *m_player;
    }
    inline World& GetWorld()
    {
        return *m_world;
    }
    inline BlockData& GetBlockData()
    {
        return *m_bdata;
    }
    inline TextureDictionary& GetTextureDictionary()
    {
        return *m_tdict;
    }
    inline Input& GetInput()
    {
        return *m_input;
    }
    inline FileSystem& GetFileSystem()
    {
        return *m_fs;
    }
    inline FontManager& GetFontManager()
    {
        return *m_fm;
    }
private:
    bool Tick();
    Graphics* m_graphics;
    Player* m_player;
    World* m_world;
    BlockData* m_bdata;
    TextureDictionary* m_tdict;
    Input* m_input;
    FileSystem* m_fs;
    EngineVitals* m_vitals;
    FontManager* m_fm;
    double m_oTime;
    Engine();
    ~Engine() = default;
    Engine(const Engine&) = delete;
};
#endif