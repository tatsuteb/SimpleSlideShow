#include "Playlist.h"

#include <QJsonArray>
#include <QJsonObject>

Playlist::Playlist():
	m_playlistName("")
{

}

void Playlist::readFromJson(const QJsonObject &jsonObj)
{
	m_playlistName = jsonObj["playlistName"].toString();

	QJsonArray jsonArr = jsonObj["filePathList"].toArray();

	QStringList filePathList;
	for (auto filePath : jsonArr)
	{
		filePathList << filePath.toString();
	}

	addFiles(filePathList, true);
}

void Playlist::writeToJson(QJsonObject &jsonObj) const
{
	jsonObj["playlistName"] = m_playlistName;

	QJsonArray jsonArr;

	for (auto fileInfo : m_fileInfoList)
	{
		jsonArr.append(fileInfo.filePath());
	}

	jsonObj["filePathList"] = jsonArr;
}

void Playlist::setPlaylistName(const QString &playlistName)
{
	m_playlistName = playlistName;
}

void Playlist::addFiles(const QStringList &filenames, bool isPlaylistClear)
{
	if (filenames.isEmpty()) return;

	if (isPlaylistClear)
	{
		m_fileInfoList.clear();
	}

	// リストデータ更新
	for (auto filename : filenames)
	{
		m_fileInfoList << QFileInfo(filename);
	}
}

void Playlist::removeFileAt(const int &index)
{
	if (index < 0 || index >= m_fileInfoList.count()) return;

	m_fileInfoList.removeAt(index);
}

const QString &Playlist::playlistName() const
{
	return m_playlistName;
}

QStringList Playlist::filenames() const
{
	QStringList filenames;

	for (auto fileInfo : m_fileInfoList)
	{
		filenames << fileInfo.fileName();
	}

	return filenames;
}

QString Playlist::filePathAt(const int &index) const
{
	QString filePath;

	if (index < 0 || index >= m_fileInfoList.count())
	{
		return filePath;
	}

	QFileInfo fileInfo = m_fileInfoList.at(index);
	filePath = fileInfo.filePath();

	return filePath;
}

QString Playlist::pathAt(const int &index) const
{
	QString path;

	if (index < 0 || index >= m_fileInfoList.count())
	{
		return path;
	}

	QFileInfo fileInfo = m_fileInfoList.at(index);
	path = fileInfo.path();

	return path;
}

int Playlist::fileCount() const
{
	return m_fileInfoList.count();
}
