#include "SlideshowSetting.h"

SlideshowSetting::SlideshowSetting():
	m_playSpeed(1.0),
	m_isRandom(false),
	m_isRepeat(false),
	m_isFullscreen(false)
{

}

void SlideshowSetting::readFromJson(const QJsonObject &jsonObj)
{
	// メンバ変数 = QJsonObjectオブジェクト[キー].to〜()
	m_playSpeed = jsonObj["playSpeed"].toDouble();
	m_isRandom = jsonObj["isRandom"].toBool();
	m_isRepeat = jsonObj["isRepeat"].toBool();
	m_isFullscreen = jsonObj["isFullscreen"].toBool();
}

void SlideshowSetting::writeToJson(QJsonObject &jsonObj)
{
	// QJsonObjectオブジェクト[キー] = 値
	jsonObj["playSpeed"] = m_playSpeed;
	jsonObj["isRandom"] = m_isRandom;
	jsonObj["isRepeat"] = m_isRepeat;
	jsonObj["isFullscreen"] = m_isFullscreen;
}

const double &SlideshowSetting::getPlaySpeed()
{
	return m_playSpeed;
}

double SlideshowSetting::getPlaySpeedAsMsec()
{
	return m_playSpeed * 1000;
}

const bool &SlideshowSetting::isRandom()
{
	return m_isRandom;
}

const bool &SlideshowSetting::isRepeat()
{
	return m_isRepeat;
}

const bool &SlideshowSetting::isFullscreen()
{
	return m_isFullscreen;
}

void SlideshowSetting::changePlaySpeed(const double &sec)
{
	m_playSpeed = sec;
}

void SlideshowSetting::setRandomState(const bool &isRandom)
{
	m_isRandom = isRandom;
}

void SlideshowSetting::setRepeatState(const bool &isRepeat)
{
	m_isRepeat = isRepeat;
}

void SlideshowSetting::setFullscreenState(const bool &isFullscreen)
{
	m_isFullscreen = isFullscreen;
}
