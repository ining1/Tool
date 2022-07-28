#pragma once
#include "Base.h"

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

BEGIN(Engine)

class CSound_Manager final : public CBase {
DECLARE_SINGLETON(CSound_Manager)
public:
	enum CHANNELID { BGM, MAXCHANNEL };
private:
	CSound_Manager();
	virtual ~CSound_Manager() = default;
public:
	HRESULT NativeConstruct(const _tchar* pSoundFilePath);
public:
	int VolumeUp(CHANNELID eID, _float _vol);
	int VolumeDown(CHANNELID eID, _float _vol);
	int BGMVolumeUp(_float _vol);
	int BGMVolumeDown(_float _vol);
	int Pause(CHANNELID eID);
	void Play_Sound(TCHAR* pSoundKey, CHANNELID eID, _float _vol);
	void PlayBGM(TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();
private:
	float m_volume = SOUND_DEFAULT;
	float m_BGMvolume = SOUND_DEFAULT;
	FMOD_BOOL m_bool;
private:
	void LoadSoundFile(const _tchar* pSoundFilePath);
private:
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	FMOD_SYSTEM* m_pSystem;
	_bool m_bPause = false;
public:
	virtual void Free() override;
};

END