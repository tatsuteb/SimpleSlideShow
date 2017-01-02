#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QFileInfoList>
#include <QString>

class Playlist
{
	public:
		Playlist();

	public:
		void readFromJson(const QJsonObject &jsonObj);
		void writeToJson(QJsonObject &jsonObj) const;

		const QString &playlistName() const;
		QStringList filenames() const;
		QString filePathAt(const int &index) const;
		QString pathAt(const int &index) const;
		int fileCount() const;

		void setPlaylistName(const QString &playlistName);
		void addFiles(const QStringList &filenames, bool isPlaylistClear = false);
		void removeFileAt(const int &index);

	private:
		QFileInfoList m_fileInfoList;
		QString m_playlistName;
};

#endif // PLAYLIST_H
