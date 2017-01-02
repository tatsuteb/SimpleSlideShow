#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "ImageWidget.h"
#include "Playlist.h"
#include "SlideshowSetting.h"

#include <QFileDialog>
#include <QJsonDocument>
#include <QTime>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_slideshowSetting(QSharedPointer<SlideshowSetting>::create()),
	m_playlist(QSharedPointer<Playlist>::create()),
	m_imageWidget(new ImageWidget),
	m_timer(new QTimer),
	m_openedFilename("")
{
	ui->setupUi(this);

	setupDataOnUi();
	setupConnection();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::setupDataOnUi()
{
	if (m_playlist.isNull()) return;

	ui->lineEdit_playlistTitle->setText(m_playlist->playlistName());
	ui->listWidget_playlist->clear();
	ui->listWidget_playlist->addItems(m_playlist->filenames());

	if (m_slideshowSetting.isNull()) return;

	ui->doubleSpinBox_playSpeed->setValue(m_slideshowSetting->getPlaySpeed());
	ui->checkBox_random->setChecked(m_slideshowSetting->isRandom());
	ui->checkBox_repeat->setChecked(m_slideshowSetting->isRepeat());
	ui->checkBox_fullscreen->setChecked(m_slideshowSetting->isFullscreen());
}

void MainWindow::setupConnection()
{
	connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
	connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
	connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::saveAs);
	connect(ui->actionQuit, &QAction::triggered, qApp, &QApplication::quit);

	connect(m_timer, &QTimer::timeout, this, &MainWindow::slideshowTimerEvent);
	connect(m_imageWidget, &ImageWidget::escapeKeyPressed, this, &MainWindow::stopSlideshow);
	connect(m_imageWidget, &ImageWidget::windowClosed, this, &MainWindow::stopSlideshow);
}

void MainWindow::setPlayOptionLocked(const bool &isLocked)
{
	bool isEnabled = !isLocked;

	ui->doubleSpinBox_playSpeed->setEnabled(isEnabled);
	ui->checkBox_random->setEnabled(isEnabled);
	ui->checkBox_fullscreen->setEnabled(isEnabled);
	ui->checkBox_repeat->setEnabled(isEnabled);

	ui->pushButton_addImagePath->setEnabled(isEnabled);
	ui->pushButton_removeImagePath->setEnabled(isEnabled);
}

int MainWindow::getNextImageIndex()
{
	if (m_slideIndexHistory.count() >= m_playlist->fileCount()) return -1;

	int index = 0;

	if (!m_slideIndexHistory.isEmpty())
	{
		index = m_slideIndexHistory.last();
		index++;
	}

	m_slideIndexHistory.push_back(index);

	return index;
}

int MainWindow::getRandomImageIndex()
{
	if (m_slideIndexHistory.count() >= m_playlist->fileCount()) return -1;

	int i = qrand() % m_playlist->fileCount();
	while (m_slideIndexHistory.contains(i))
	{
		i = qrand() % m_playlist->fileCount();
	}

	m_slideIndexHistory.push_back(i);

	return i;
}

void MainWindow::startSlideshow()
{
	if (ui->listWidget_playlist->count() == 0)
	{
		stopSlideshow();
		return;
	}

	if (m_slideshowSetting->isFullscreen())
	{
		m_imageWidget->showFullScreen();
	}
	else
	{
		m_imageWidget->show();
	}

	qsrand(QTime::currentTime().msec());
	setNextImage();	// 最初の一回

	m_timer->start(m_slideshowSetting->getPlaySpeedAsMsec());

	setPlayOptionLocked(true);

	ui->pushButton_Play_Pose->setChecked(true);
	ui->pushButton_Play_Pose->setText("停止");
}

void MainWindow::stopSlideshow()
{
	m_imageWidget->setWindowState(Qt::WindowNoState);
	m_imageWidget->close();
	m_timer->stop();

	setPlayOptionLocked(false);

	ui->pushButton_Play_Pose->setChecked(false);
	ui->pushButton_Play_Pose->setText("再生");
}

void MainWindow::open()
{
	QString filename = QFileDialog::getOpenFileName(this, "開く", "/home", "JSON (*.json);;Binary Data (*.dat)");

	if (filename.isEmpty()) return;

	// ファイルオブジェクトを生成して、開く
	QFile openFile(filename);
	openFile.open(QIODevice::ReadOnly);

	// データをバイト列として読み込む
	QByteArray data = openFile.readAll();

	// 読み込んだバイト列から、QJsonDocumentオブジェクトを生成
	QFileInfo fileInfo(filename);
	QJsonDocument jsonDoc(fileInfo.suffix() == "json" ?
							  QJsonDocument::fromJson(data) :
							  QJsonDocument::fromBinaryData(data));

	// QJsonDocumentのobject()関数を使って、各クラスにQJsonObjectオブジェクト渡す。
	// QJsonObjectオブジェクトを受け取ったクラスは、データをメンバ変数に展開
	m_slideshowSetting->readFromJson(jsonDoc.object());
	m_playlist->readFromJson(jsonDoc.object());

	openFile.close();

	// ウィジェットを更新
	setupDataOnUi();

	m_openedFilename = filename;
}

void MainWindow::save()
{
	if (m_openedFilename == "")
	{
		saveAs();
		return;
	}

	// ファイルオブジェクトを生成して、開く
	QFile saveFile(m_openedFilename);
	saveFile.open(QIODevice::WriteOnly);

	// QJsonObjectオブジェクトを生成し、保存したいデータを持っているクラスに渡す。
	// 各クラスに作成したwriteToJson関数の中で、保存したいデータを記録する
	QJsonObject jsonObj;
	m_slideshowSetting->writeToJson(jsonObj);
	m_playlist->writeToJson(jsonObj);

	// QJsonObjectオブジェクトから、QJsonDocumentオブジェクトを生成
	QJsonDocument jsonDoc(jsonObj);

	// QJsonDocumentオブジェクトから、ファイルフォーマットに応じたバイト列を取得
	QFileInfo fileInfo(m_openedFilename);
	QByteArray data(fileInfo.suffix() == "json" ?
						jsonDoc.toJson() :
						jsonDoc.toBinaryData());

	// ファイルオブジェクトにバイトデータを書き込んで、閉じる
	saveFile.write(data);
	saveFile.close();
}

void MainWindow::saveAs()
{
	QString filename = QFileDialog::getSaveFileName(this, "名前をつけて保存", "/home", "JSON (*.json);;Binary Data (*.dat)");

	if (filename.isEmpty()) return;

	m_openedFilename = filename;

	save();
}

void MainWindow::on_pushButton_addImagePath_clicked()
{
	static QString openDir = "/home";

	QStringList filenameList = QFileDialog::getOpenFileNames(this, "ファイルの追加", openDir, "Image (*.png *.jpg *.jpeg)");

	if (filenameList.isEmpty()) return;

	// リストデータ更新
	m_playlist->addFiles(filenameList);

	// データに合わせて、ウィジェットを更新
	ui->listWidget_playlist->clear();
	ui->listWidget_playlist->addItems(m_playlist->filenames());

	openDir = m_playlist->pathAt(m_playlist->fileCount() - 1);
}

void MainWindow::on_pushButton_removeImagePath_clicked()
{
	QItemSelectionModel *selectionModel = ui->listWidget_playlist->selectionModel();
	QModelIndexList modelIndexList = selectionModel->selectedRows();

	if (modelIndexList.count() == 0) return;

	// 大きいインデックスから消すので、降順にソート
	qSort(modelIndexList.begin(), modelIndexList.end(), [&](const QModelIndex &i1, const QModelIndex &i2)
	{
		return i1.row() > i2.row();
	});

	// リストデータ更新
	for (auto modelIndex : modelIndexList)
	{
		m_playlist->removeFileAt(modelIndex.row());
	}

	// データに合わせて、ウィジェットを更新
	ui->listWidget_playlist->clear();
	ui->listWidget_playlist->addItems(m_playlist->filenames());

}

void MainWindow::on_pushButton_Play_Pose_clicked(bool checked)
{
	if (checked)
	{
		startSlideshow();

		return;
	}

	stopSlideshow();
}

bool MainWindow::setNextImage()
{
	int i;

	if (m_slideshowSetting->isRandom())
	{
		i = getRandomImageIndex();
	}
	else
	{
		i = getNextImageIndex();
	}

	if (i == -1)
	{
		m_slideIndexHistory.clear();

		return false;
	}

	m_imageWidget->setImage(m_playlist->filePathAt(i));

	return true;
}

void MainWindow::slideshowTimerEvent()
{
	bool hasNextImage = setNextImage();

	if (!hasNextImage)
	{
		m_slideIndexHistory.clear();

		if (!m_slideshowSetting->isRepeat())
		{
			stopSlideshow();
			return;
		}

		setNextImage();
	}
}

void MainWindow::on_lineEdit_playlistTitle_editingFinished()
{
	m_playlist->setPlaylistName(ui->lineEdit_playlistTitle->text());
}

void MainWindow::on_doubleSpinBox_playSpeed_valueChanged(double arg1)
{
	m_slideshowSetting->changePlaySpeed(arg1);
}

void MainWindow::on_checkBox_repeat_toggled(bool checked)
{
	m_slideshowSetting->setRepeatState(checked);
}

void MainWindow::on_checkBox_random_toggled(bool checked)
{
	m_slideshowSetting->setRandomState(checked);
}

void MainWindow::on_checkBox_fullscreen_toggled(bool checked)
{
	m_slideshowSetting->setFullscreenState(checked);
}

void MainWindow::closeEvent(QCloseEvent *)
{
	qApp->quit();
}
