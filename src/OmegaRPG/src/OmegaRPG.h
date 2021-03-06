#pragma once

namespace Omega
{
class OmegaRPG
{
public:
    OmegaRPG(void);
    OmegaRPG(char* filename);

    ~OmegaRPG(void);

    void Initialize();

    char* SaveFilename();
    void SaveFilename(char* filename);

    bool Continuing();
    void Continuing(bool value);

    bool ScoresOnly();
    void ScoresOnly(bool value);

private:
    char* _saveFilename;
    bool _continuing;
    bool _scoresOnly;
};
}