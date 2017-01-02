#ifndef SLIDESHOWSETTING_H
#define SLIDESHOWSETTING_H

#include <QJsonObject>

class SlideshowSetting
{

	public:
		SlideshowSetting();

	public:
		void readFromJson(const QJsonObject &jsonObj);
		void writeToJson(QJsonObject &jsonObj);

		const double &getPlaySpeed();
		double getPlaySpeedAsMsec();
		const bool &isRandom();
		const bool &isRepeat();
		const bool &isFullscreen();

		void changePlaySpeed(const double &sec);
		void setRandomState(const bool &isRandom);
		void setRepeatState(const bool &isRepeat);
		void setFullscreenState(const bool &isFullscreen);

	private:
		double m_playSpeed;
		bool m_isRandom;
		bool m_isRepeat;
		bool m_isFullscreen;

};

#endif // SLIDESHOWSETTING_H
