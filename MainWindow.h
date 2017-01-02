#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFileInfoList>

namespace Ui {
	class MainWindow;
}

class ImageWidget;
class SlideshowSetting;
class Playlist;

class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	private:
		Ui::MainWindow *ui;
		QSharedPointer<SlideshowSetting> m_slideshowSetting;
		QSharedPointer<Playlist> m_playlist;
		ImageWidget *m_imageWidget;
		QTimer *m_timer;
		QString m_openedFilename;
		QList<int> m_slideIndexHistory;

	private:
		void setupDataOnUi();
		void setupConnection();
		void setPlayOptionLocked(const bool &isLocked);
		int getNextImageIndex();
		int getRandomImageIndex();

	public slots:
		void startSlideshow();
		void stopSlideshow();

	private slots:
		void open();	// JSON形式か、バイナリ形式で書き出したデータを読み込む
		void save();	// JSON形式か、バイナリ形式でプレイリストとスライドショーの設定を保存
		void saveAs();
		void quitApp();

		bool setNextImage();
		void slideshowTimerEvent();

		void on_pushButton_addImagePath_clicked();
		void on_pushButton_removeImagePath_clicked();
		void on_pushButton_Play_Pose_clicked(bool checked);
		void on_lineEdit_playlistTitle_editingFinished();
		void on_doubleSpinBox_playSpeed_valueChanged(double arg1);
		void on_checkBox_repeat_toggled(bool checked);
		void on_checkBox_random_toggled(bool checked);
		void on_checkBox_fullscreen_toggled(bool checked);
};

#endif // MAINWINDOW_H
